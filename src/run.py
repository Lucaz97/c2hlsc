from openai import OpenAI
import argparse
import subprocess
import yaml

from prompts import *

import os
from pycparser import c_ast, parse_file, c_generator, c_parser
from subprocess import Popen, PIPE, STDOUT
# A simple visitor for FuncDef nodes that prints the names and
# locations of function definitions.

# make dir if does not exist
if not os.path.exists("tmp"):
    os.makedirs("tmp")

calls_table = {}
params_table = {}
nodes_table = {}
top = "Cipher"
N = 16
filename = 'inputs/AES/AES.c'
models = ["gpt-4o-mini","gpt-4-turbo-2024-04-09", "gpt-3.5-turbo-0125", "gpt-4o", "adaptive", "o1-mini"]
llm_runs = {model: 0 for model in models}

# array printer template
array_printer = """
void func() {{
for(int _i = 0; _i < {size}; _i++) {{
    printf("%d ", {name}[_i]);
}}
printf("\\n");
}}
"""
double_array_printer = """
void func() {{
for(int _i = 0; _i < {size1}; _i++) {{
    for(int _j = 0; _j < {size2}; _j++) {{
        printf("%d ", {name}[_i][_j]);
    }}
    printf("\\n");
}}}}
"""

# for hierarchical 
def explore_calls(top, hierarchical_calls):
    for func in calls_table[top]:
        if calls_table[func]:
            explore_calls(func, hierarchical_calls)
        if func not in hierarchical_calls:
            hierarchical_calls.append(func)
    hierarchical_calls.append(top)
    return hierarchical_calls
      


def build_unit_test(func):
    print("Building unit test for ", func)
    # get param values
    # build gdb script
    with open("tmp/" + func + "_gdb.py", "w") as f:
        print("import gdb", file =f)
        print("""gdb.execute("file tmp/to_debug")""", file=f)
        print(f"""gdb.execute("break {func}")""", file =f)
        print("""gdb.execute("run")""", file =f)
        
        for i in range(20):
            for param in params_table[func]:
                if isinstance(param[0], c_ast.PtrDecl):
                    print(f"""gdb.execute("p/x *{param[-1]}")""", file =f)
                else:
                    print(f"""gdb.execute("p/x {param[-1]}")""", file =f)
            print("""gdb.execute("continue")""", file =f)
            

    # compile the file with gcc
    p= Popen(["gcc","-ggdb", "-g3", filename, "-o", f"tmp/to_debug"], stdout=PIPE, stderr=PIPE, text=True)
    #run debug 
    p = Popen(["gdb"], stdout=PIPE, stdin=PIPE, stderr=PIPE, bufsize=0, text=True)
    stdout_data, stderr_data = p.communicate(input=f"c\nsource tmp/{func}_gdb.py\n")
    
    with open("tmp/" + func + "_gdb.log", "w") as f:
        dbg_out = stdout_data.replace(", \n", ",")
        print(dbg_out, file=f) # this is just for debugging

    n_params = len(params_table[func])

    # build the test functoin
    main_decl = c_ast.Decl("main", [], [], [], [], c_ast.FuncDecl(c_ast.ParamList([]), c_ast.TypeDecl("main", [], [], c_ast.IdentifierType(['int']))), None, None)
    main_def = c_ast.FuncDef(main_decl, None, c_ast.Compound([]))
    # add def of params
    pointers_table = {} # associates a pointer with its dimensions and sizes
    for i in range(n_params):
        # find init values
        for line in dbg_out.split("\n"):
            if line.startswith(f"${i+1} = "):
                value = line.split(" = ")[1]
                if "{{" in value:
                    # is a double array
                    value = value.replace("{", "[").replace("}", "]")
                    value = eval(value)
                    pointers_table[params_table[func][i][1]] = (2, len(value), len(value[0])) # array dimensions and sizes
                    init = c_ast.InitList([c_ast.InitList([c_ast.Constant(c_ast.IdentifierType(['int']),str(val)) for val in outer_list]) for outer_list in value])
                elif "{" in value:
                    # is a single array
                    value = value.replace("{", "[").replace("}", "]")
                    value = eval(value)
                    pointers_table[params_table[func][i][1]] = (1, len(value)) # array dimensions and sizes
                    init = c_ast.InitList([c_ast.Constant(c_ast.IdentifierType(['int']), str(val)) for val in value])
                else:
                    init =c_ast.Constant(c_ast.IdentifierType(['int']), value)
        if isinstance(params_table[func][i][0], c_ast.PtrDecl):
            main_def.body.block_items.append(c_ast.Decl(params_table[func][i][1], [], [], [], None, params_table[func][i][0].type, init, None))
        else:
            main_def.body.block_items.append(c_ast.Decl(params_table[func][i][1], [], [], [], None, params_table[func][i][0], init, None))
    # add call to function
    main_def.body.block_items.append(c_ast.FuncCall(c_ast.ID(func), c_ast.ExprList([c_ast.ID(param[1]) if not isinstance(param[0], c_ast.PtrDecl) else c_ast.UnaryOp('&', c_ast.ID(param[1])) for param in params_table[func]])))

    # add prints
    for i in range(n_params):
        if isinstance(params_table[func][i][0], c_ast.PtrDecl):
            if pointers_table[params_table[func][i][1]][0] == 2:
                code_str = double_array_printer.format(name=params_table[func][i][1], size1=pointers_table[params_table[func][i][1]][1], size2=pointers_table[params_table[func][i][1]][2])
            else:
                code_str = array_printer.format(name=params_table[func][i][1], size=pointers_table[params_table[func][i][1]][1])
            for_ast = c_parser.CParser().parse(code_str).ext[0]
            main_def.body.block_items.extend(for_ast.body.block_items)
        else:
            main_def.body.block_items.append(c_ast.FuncCall(c_ast.ID("printf"), c_ast.ExprList([c_ast.Constant(c_ast.IdentifierType(['char']), f'"%d\\n"'), c_ast.ID(params_table[func][i][1])])))

    generator = c_generator.CGenerator()
    with open("tmp/" + func + ".c", "w") as f:
        children = []
        explore_calls(func, children)
        for child_func in children:
            print(generator.visit(nodes_table[child_func]), file=f)
    with open("tmp/" + func + "_test.c", "w") as f:
        print(generator.visit(main_def), file=f)

class TopFunctionVisitor(c_ast.NodeVisitor):
     # this visitor needs to know the name of top module
    def __init__(self, top):
        self.top = top
        self.top_node = None

    def visit_FuncDef(self, node):
        if node.decl.name == self.top:
            self.top_node = node

class HierarchyVisitor(c_ast.NodeVisitor):
    def visit_FuncDef(self, node):
        #print('%s at %s' % (node.decl.name, node.decl.coord))
        nodes_table[node.decl.name] = node
        calls_table[node.decl.name] = []
        params_table[node.decl.name] = []
        if node.decl.type.args:
            params_table[node.decl.name] = [(param.type, param.name) for param in node.decl.type.args]
        v = FuncCallVisitor(node.decl.name)
        v.visit(node)

class FuncCallVisitor(c_ast.NodeVisitor):
    # this visitor needs to know the name of itself before
    def __init__(self, name):
        self.name = name

    def visit_FuncCall(self, node):
        #print(self.name, " calls", node.name.name)
        calls_table[self.name].append(node.name.name)




def C2HLSC (includes, orig_code, test_code, tcl, top_function, out_folder, processed, model="adaptive", mode="standard", opt_target="area"):
    # write initial c
    print("model: ", model)
    file_name = f"tmp/{top_function}_initial.c"
    with open(file_name, "w") as f:
        f.write(includes)
        f.write(orig_code)

    # create a file with the formatted tcl
    tcl_file = out_folder + "initial.tcl"
    with open(tcl_file, "w") as f:
        f.write(tcl.format(top_function=top_function, c_file=file_name))


    # code to fix from orig code
    # get only the top function
    # parse orig code
    ast = parse_file(file_name, use_cpp=True,
                        cpp_args=r'-Iutils/fake_libc_include')
    v = TopFunctionVisitor(top_function)
    v.visit(ast)
    code_to_fix = c_generator.CGenerator().visit(v.top_node)

    if mode == "standard":
        # run catapult and get the error
        print("Running catapult")
        subprocess.run(["catapult", "-shell", "-file", tcl_file], capture_output=True)
    

        # parse log
        with open("catapult.log", "r") as f:
            log = f.read()
            if "# Error:" in log:
                error = log.split("# Error:")[1]
                print(error)
            else:
                print(f"{top_function} is correct, does not need any changes")
                # write final file
                with open(f"tmp/{top_function}_final.c", "w") as f:
                    f.write(code_to_fix)
                return f"tmp/{top_function}_final.c"
                # write final file
                # with open(f"tmp/{top_function}_to_opt.c", "w") as f:
                #     # need to take out the main function
                #     code_to_optimize = code_to_fix
                #     f.write(code_to_optimize)
                # # optimize this function
                # return HLSC_optimizer(includes, orig_code, test_code, tcl, top_function, out_folder, processed, model, mode, opt_target)
        
       

        std_prompt = f"""Help me rewrite the {top_function} function to be compatible with HLS: \n  {code_to_fix} \n The current problem is:" \n{error}
        \n\n also include a main function that tests the code in the same way of the reference code: \n {test_code}"""

        initial_prompt = std_prompt

    else :
        error = 1
        # read straming example
        with open("inputs/streaming_example.c", "r") as f:
            streaming_example = f.read()
        interface_prompt = f"""Rewrite the {top_function} function to be compatible for HLS. The first task it to rewrite it such that it will get inferred as a streaming function,
        to do so, I need to get rid of the global array and have the function take a parameter
        to accept an element at each function call. The following is an example on how this can be done: \n {streaming_example}
        \nThe function is \n{code_to_fix}\n
        also include a main function that tests the code in the same way of the reference code: \n{test_code}"""
        initial_prompt = interface_prompt

    # initial message list
    message_list=[
            {"role": "system", "content": system_content_c2hlsc},
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
                print("Exiting due to too many iterations")
                exit(1)
            if model == "adaptive":
                model_name = "gpt-4o-mini" if i+j<6 else "gpt-4o"
            else: 
                model_name = model
            print("Model: ", model_name)
            
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
            llm_runs[model_name] += 1
            # get c copde and create a file with it
            c_code_dut = completion.choices[0].message.content.split("```c")[1].split("```")[0]

            # update message_list
            message_list.append(completion.choices[0].message)
            #message_list.append({"role": "user", "content": "Help me rewrite the main function that tests the code to be compatible with your changes: \n" + test_code})

            # new file
            llm_file = f"tmp/{top_function}_llm.c"
            with open(llm_file, "w") as f:
                f.write(includes)
                for proc in processed:
                    with open(f"{proc}", "r") as p:
                        f.write(p.read())
                f.write(c_code_dut)
                # f.write(test_code_dut)

            # compile the file with gcc 
            print("Compiling the code")
            log = subprocess.run(["gcc", llm_file, "-o", llm_file[:-2]], capture_output=True)
            if "error" in log.stderr.decode():
                error = log.stderr.decode()
                #only keep the first 3 lines
                error = "\n".join(error.split("\n")[:3])
                # update message_list
                print("There is an error in the code: ", error)
                message_list.append({"role": "user", "content": "There is an error in the code: \n" + error + ", please try again"})
                continue

            # make file for reference output
            orig_file = f"tmp/{top_function}_testbench.c"
            with open(orig_file, "w") as f:
                f.write(includes)
                f.write(orig_code)
                f.write(test_code)

            # compile the reference file with gcc
            subprocess.run(["gcc", orig_file, "-o", orig_file[:-2]])

            # run the compiled files an    models = ["gpt-4o-mini","gpt-4-turbo-2024-04-09", "gpt-3.5-turbo-0125", "gpt-4o", "adaptive"]d check the outputs match
            out_dut = subprocess.run([f"./{llm_file[:-2]}"], capture_output=True)
            out_ref = subprocess.run([f"./{orig_file[:-2]}"], capture_output=True)

            # check the outputs match, ignore whitespaces and new lines
            if out_dut.stdout.decode().replace(" ", "").replace("\n", "") == out_ref.stdout.decode().replace(" ", "").replace("\n", ""):
                print("The code is correct")
                error = None
            else:
                print("The code is incorrect")
                # retry with same error
                error = True
                message_list.append({"role": "user", "content": "There is an error in the code, the result should be \n" +  out_ref.stdout.decode()  + " \n the output was instead: "+ out_dut.stdout.decode()+", please try again"})

            print(out_dut.stdout)
            print(out_ref.stdout)

        print("The code is functionally correct, number of iterations:" , i)
        
        # check with catapult
        # create a file with the formatted tcl
        tcl_file = out_folder + "initial.tcl"
        with open(tcl_file, "w") as f:
            f.write(tcl.format(top_function=top_function, c_file=llm_file))

        print("Running catapult")
        subprocess.run(["catapult", "-shell", "-file", tcl_file], capture_output=True)
        j += 1
        # parse log
        with open("catapult.log", "r") as f:
            log = f.read()
            if "# Error:" in log:
                error = log.split("# Error:")[1]
                print(error)
                message_list = [{"role": "system", "content": system_content_c2hlsc},
                {"role": "user", "content": "Help me rewrite this function to be compatible with HLS: \n" + c_code_dut + "\n The current problem is:" + error}]
                continue
            else:
                error = None
                print("The code is correct")
                # write final file
                with open(f"tmp/{top_function}_to_opt.c", "w") as f:
                    # need to take out the main function
                    code_to_optimize = c_code_dut.split("int main()")[0]
                    f.write(code_to_optimize)
                # optimize this function
                return HLSC_optimizer(includes, orig_code, test_code, tcl, top_function, out_folder, processed, model, mode, opt_target)
                # with open(f"tmp/{top_function}_final.c", "w") as f:
                #     # need to take out the main function
                #     code_to_save = c_code_dut.split("int main()")[0]
                #     f.write(code_to_save)
                # return f"tmp/{top_function}_final.c"

    print("The code is correct, number of gpt runs: ", total_gpt_runs)

def HLSC_optimizer (includes, orig_code, test_code, tcl, top_function, out_folder, processed, model="adaptive", mode="standard", opt_target="area"):

    # write initial c
    file_name = f"tmp/{top_function}_initial_opt.c"
    with open(file_name, "w") as f:
        f.write(includes)
        f.write(orig_code)

    # code to fix from orig code
    # get only the top function
    # parse orig code
    ast = parse_file(file_name, use_cpp=True,
                        cpp_args=r'-Iutils/fake_libc_include')
    v = TopFunctionVisitor(top_function)
    v.visit(ast)
    code_to_fix = c_generator.CGenerator().visit(v.top_node)

    initial_prompt = f"""Update the {top_function} function to optimize it for HLS targetting {opt_target}.
        The function is \n{code_to_fix}\n
        also include a main function that tests the code in the same way of the reference code: \n{test_code}"""
    
    message_list=[
            {"role": "system", "content": system_content_c2hlsc},
            {"role": "user", "content": initial_prompt}
        ]
    error = 1
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
            
            model_name = "gpt-4o"
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
            llm_runs[model_name] += 1
            # get c copde and create a file with it
            c_code_dut = completion.choices[0].message.content.split("```c")[1].split("```")[0]

            # update message_list
            message_list.append(completion.choices[0].message)
            #message_list.append({"role": "user", "content": "Help me rewrite the main function that tests the code to be compatible with your changes: \n" + test_code})

            # new file
            llm_file = f"tmp/{top_function}_llm_opt.c"
            with open(llm_file, "w") as f:
                f.write(includes)
                for proc in processed:
                    with open(f"{proc}", "r") as p:
                        f.write(p.read())
                f.write(c_code_dut)
                # f.write(test_code_dut)

            # compile the file with gcc 
            print("Compiling the code")
            log = subprocess.run(["gcc", llm_file, "-o", llm_file[:-2]], capture_output=True)
            if "error" in log.stderr.decode():
                error = log.stderr.decode()
                #only keep the first 3 lines
                error = "\n".join(error.split("\n")[:3])
                # update message_list
                print("There is an error in the code: ", error)
                message_list.append({"role": "user", "content": "There is an error in the code: \n" + error + ", please try again"})
                continue

            # make file for reference output
            orig_file = f"tmp/{top_function}_testbench_opt.c"
            with open(orig_file, "w") as f:
                f.write(includes)
                f.write(orig_code)
                f.write(test_code)

            # compile the reference file with gcc
            subprocess.run(["gcc", orig_file, "-o", orig_file[:-2]])

            # run the compiled files an    models = ["gpt-4o-mini","gpt-4-turbo-2024-04-09", "gpt-3.5-turbo-0125", "gpt-4o", "adaptive"]d check the outputs match
            out_dut = subprocess.run([f"./{llm_file[:-2]}"], capture_output=True)
            out_ref = subprocess.run([f"./{orig_file[:-2]}"], capture_output=True)

            # check the outputs match, ignore whitespaces and new lines
            if out_dut.stdout.decode().replace(" ", "").replace("\n", "") == out_ref.stdout.decode().replace(" ", "").replace("\n", ""):
                print("The code is correct")
                error = None
            else:
                print("The code is incorrect")
                # retry with same error
                error = True
                message_list.append({"role": "user", "content": "There is an error in the code, the result should be \n" +  out_ref.stdout.decode()  + " \n the output was instead: "+ out_dut.stdout.decode()+", please try again"})

            print(out_dut.stdout)
            print(out_ref.stdout)

        print("The code is functionally correct, number of iterations:" , i)
        
        # check with catapult
        # create a file with the formatted tcl
        tcl_file = out_folder + "initial_opt.tcl"
        with open(tcl_file, "w") as f:
            f.write(tcl.format(top_function=top_function, c_file=llm_file))

        print("Running catapult")
        subprocess.run(["catapult", "-shell", "-file", tcl_file], capture_output=True)
        j += 1
        # parse log
        with open("catapult.log", "r") as f:
            log = f.read()
            if "# Error:" in log:
                error = log.split("# Error:")[1]
                print(error)
                message_list = [{"role": "system", "content": system_content_optimizer},
                {"role": "user", "content": "Help me rewrite this function to be compatible with HLS: \n" + c_code_dut + "\n The current problem is:" + error}]
                continue
            else:
                error = None
                print("The code is correct")
                # write final file
                with open(f"tmp/{top_function}_final.c", "w") as f:
                    # need to take out the main function
                    code_to_save = c_code_dut.split("int main()")[0]
                    f.write(code_to_save)
                return f"tmp/{top_function}_final.c"

    print("The code is correct, number of gpt runs: ", total_gpt_runs)
 


########################################################################################
#                                       MAIN                                           #
########################################################################################
if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='C2HLSC script, yaml config required')

    parser.add_argument('config', type=str, help='yaml config file with the following fields: orig_code, test_code, includes, tcl, top_function')
    # model name optional argument from a list of models
    modes = ["standard", "streaming"] # from yaml file
    parser.add_argument('--model', type=str, default="adaptive", choices=models, help='model name to use, default is gpt-3.5-turbo-0125')
    opt_target = ["area", "latency"]
    parser.add_argument('--opt_target', type=str, default="area", choices=opt_target, help='optimization target, default is area')
    args = parser.parse_args()
    client = OpenAI()

    model = args.model
    if model != "adaptive":
        model_name = model
    target = args.opt_target
    print("Model: ", model)
    print("Optimization target: ", target)

    # parse yaml file with orig code test code includes and tcl; and top function
    with open(args.config, "r") as f:
        config = yaml.safe_load(f)

    if "mode" in config:
        mode = config["mode"] if config["mode"] in modes else "standard"
    else:
        mode = "standard"

    if "hierarchical" in config:
        hierarchical = config["hierarchical"]
    else:
        hierarchical = False

    print("Running in mode: ", mode, "Hierarchical: ", hierarchical)

    # read orig code from config
    with open(config["orig_code"], "r") as f:
        orig_code = f.read()
    # read test code
    with open(config["test_code"], "r") as f:
        test_code = f.read()
    #read includes
    with open(config["includes"], "r") as f:
        includes = f.read()
    #read tcl
    with open(config["tcl"], "r") as f:
        tcl = f.read()

    top_function = config["top_function"]
    # out folder
    out_folder = "outputs/"
    if not os.path.exists(out_folder):
        os.makedirs(out_folder)
    if not hierarchical:
        C2HLSC(includes, orig_code, test_code, tcl, top_function, out_folder, [], model=model, mode=mode, opt_target=target)
    else:
        filename = "tmp/complete.c"
        with open(filename, "w") as f:
            f.write(includes)
            f.write(orig_code)
            f.write(test_code)

        ast = parse_file(filename, use_cpp=True,
                        cpp_args=r'-Iutils/fake_libc_include')
        v = HierarchyVisitor()
        v.visit(ast)
        hierarchical_calls = []
        processed = []
        explore_calls(top, hierarchical_calls) # inits hierarchical_calls
        for func in hierarchical_calls:
            build_unit_test(func)
            with open(f"tmp/{func}.c", "r") as f:
                orig_code = f.read()
            with open(f"tmp/{func}_test.c", "r") as f:
                test_code = f.read()

            processed.append(C2HLSC(includes, orig_code, test_code, tcl, func, out_folder, processed,  model=model, mode=mode, opt_target=target))
    print("DONE!")

    ## log results
    # get Catapult dirs
    catapult_dirs = []
    for dir in os.listdir("."):
        if os.path.isdir(dir):
            if dir.startswith("Catapult_"):
                catapult_dirs.append(int(dir.split("_")[1]))

    log = f"Catapult_{max(catapult_dirs)}/{top_function}.v1/rtl.rpt"

    capture = False
    saved_lines = []
    with open(log, "r") as f:
        lines = f.readlines()
        for line in lines:
            if capture:
                saved_lines.append(line)
            if "Processes/Blocks" in line or "Area Scores" in line:
                capture = True
            if "Design Total" in line or "Total Reg" in line:
                capture = False
    with open(f"outputs/{top_function}.log", "w") as f:
        for model in models:
            if llm_runs[model] == 0:
                continue
            print(f"{model} runs: {llm_runs[model]}", file=f)
        print("")    
        print("".join(saved_lines), file=f)
        
    # copy important files
    subprocess.run(["cp", "-r", f"Catapult_{max(catapult_dirs)}", f"outputs/Catapult_{top_function}"])
    subprocess.run(["cp", f"tmp/{top_function}_llm.c", "outputs/"])