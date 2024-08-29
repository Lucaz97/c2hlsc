from openai import OpenAI
import argparse
import subprocess
import yaml

parser = argparse.ArgumentParser(description='C2HLSC script, yaml config required')

parser.add_argument('config', type=str, help='yaml config file with the following fields: orig_code, test_code, includes, tcl, top_function')
# model name optional argument from a list of models
models = ["gpt-4o-mini","gpt-4-turbo-2024-04-09", "gpt-3.5-turbo-0125"]
modes = ["standard", "streaming"]
parser.add_argument('--model', type=str, default="gpt-4o-mini", choices=models, help='model name to use, default is gpt-3.5-turbo-0125')
args = parser.parse_args()
client = OpenAI()

model_name = args.model

system_content = """You are a C expert programmer and High Level Synthesis expert, 
assist in coding tasks targetet to produce synthesizable HLS code. 
Your response must contain one C code snippet contaning all necesary parts needed for successful compilation. 
Do not add any pragmas or directives to the code unless asked to,
the code must be written in a way that the HLS tool can infer the correct behavior."""



# parse yaml file with orig code test code includes and tcl; and top function
with open(args.config, "r") as f:
    config = yaml.safe_load(f)

if "mode" in config:
    mode = config["mode"] if config["mode"] in modes else "standard"
else:
    mode = "standard"

print("Running in mode: ", mode)

# read orig code from config
with open(config["orig_code"], "r") as f:
    orig_code = f.read()
# read test code
with open(config["test_code"], "r") as f:
    test_code = f.read()
#read includes
with open(config["includes"], "r") as f:
    inlcudes = f.read()
#read tcl
with open(config["tcl"], "r") as f:
    tcl = f.read()

top_function = config["top_function"]
# out folder
out_folder = "outputs/"

# write initial c
file_name = f"temp_dut.c"
with open(file_name, "w") as f:
    f.write(inlcudes)
    f.write(orig_code)

# create a file with the formatted tcl
tcl_file = out_folder + "initial.tcl"
with open(tcl_file, "w") as f:
    f.write(tcl.format(top_function=top_function, c_file=file_name))

if mode == "standard":
    # run catapult and get the error
    print("Running catapult")
    subprocess.run(["catapult", "-shell", "-file", tcl_file], capture_output=True)
 

    # parse log
    with open("catapult.log", "r") as f:
        log = f.read()
        error = log.split("# Error:")[1]
        print(error)

    std_prompt = f"""Help me rewrite this function to be compatible with HLS: \n  {orig_code} \n The current problem is:" {error}
    \n\n also include a main function that tests the code in the same way of the reference code: {test_code}"""

    initial_prompt = std_prompt

else :
    error = 1
    # read straming example
    with open("inputs/streaming_example.c", "r") as f:
        streaming_example = f.read()
    interface_prompt = f"""Rewrite the {top_function} function to be compatible for HLS. The first task it to rewrite it such that it will get inferred as a streaming function,
    to do so, I need to get rid of the global array and have the function take a parameter
    to accept an element at each function call. The following is an example on how this can be done: {streaming_example}
    The function is \n{inlcudes}\n{orig_code}\n
    also include a main function that tests the code in the same way of the reference code: \n{test_code}"""
    initial_prompt = interface_prompt

# initial message list
message_list=[
        {"role": "system", "content": system_content},
        {"role": "user", "content": initial_prompt}
    ]
# counter
total_gpt_runs =0
j= 0
while error != None:
    # functionality check loop
    i = 0
    if j == 10:
        exit(2)
    while error != None:
        error = None
        print("iteration ", i)
        if i ==10:
            exit(1)
        i+=1
        # prompt
        print("Prompt: ", message_list[-1]["content"])
        completion = client.chat.completions.create(
            model=model_name,
            messages = message_list
        )
        total_gpt_runs+=1
        print("LLM RESPONSE:")
        print( completion.choices[0].message.content)

        # get c copde and create a file with it
        c_code_dut = completion.choices[0].message.content.split("```c")[1].split("```")[0]

        # update message_list
        message_list.append(completion.choices[0].message)
        #message_list.append({"role": "user", "content": "Help me rewrite the main function that tests the code to be compatible with your changes: \n" + test_code})

        # new file
        file_name = f"temp_dut.c"
        with open(file_name, "w") as f:
            # f.write(inlcudes)
            f.write(c_code_dut)
            # f.write(test_code_dut)

        # compile the file with gcc
        print("Compiling the code")
        log = subprocess.run(["gcc", file_name, "-o", f"temp_dut"], capture_output=True)
        if "error" in log.stderr.decode():
            error = log.stderr.decode()
            #only keep the first 3 lines
            error = "\n".join(error.split("\n")[:3])
            # update message_list
            print("There is an error in the code: ", error)
            message_list.append({"role": "user", "content": "There is an error in the code: " + error + ", please try again"})
            continue

        # make file for reference output
        file_name = f"temp_ref.c"
        with open(file_name, "w") as f:
            f.write(inlcudes)
            f.write(orig_code)
            f.write(test_code)

        # compile the reference file with gcc
        subprocess.run(["gcc", file_name, "-o", f"temp_ref"])

        # run the compiled files and check the outputs match
        out_dut = subprocess.run(["./temp_dut"], capture_output=True)
        out_ref = subprocess.run(["./temp_ref"], capture_output=True)

        # check the outputs match, ignore whitespaces and new lines
        if out_dut.stdout.decode().replace(" ", "").replace("\n", "") == out_ref.stdout.decode().replace(" ", "").replace("\n", ""):
            print("The code is correct")
            error = None
        else:
            print("The code is incorrect")
            # retry with same error
            error = True
            message_list.append({"role": "user", "content": "There is an error in the code, the result should be " +  out_ref.stdout.decode()  + " \n the output was instead: "+ out_dut.stdout.decode()+", please try again"})

        print(out_dut.stdout)
        print(out_ref.stdout)

    print("The code is functionally correct, number of iterations:" , i)
    
    # check with catapult
    # create a file with the formatted tcl
    tcl_file = out_folder + "initial.tcl"
    with open(tcl_file, "w") as f:
        f.write(tcl.format(top_function=top_function, c_file="temp_dut.c"))

    print("Running catapult")
    subprocess.run(["catapult", "-shell", "-file", tcl_file], capture_output=True)
    j += 1
    # parse log
    with open("catapult.log", "r") as f:
        log = f.read()
        if "# Error:" in log:
            error = log.split("# Error:")[1]
            print(error)
            message_list = [{"role": "system", "content": system_content},
            {"role": "user", "content": "Help me rewrite this function to be compatible with HLS: \n" + c_code_dut + "\n The current problem is:" + error}]
            continue
        else:
            error = None
            print("The code is correct")
            break

print("The code is correct, number of gpt runs: ", total_gpt_runs)