from openai import OpenAI
import argparse
import subprocess
import yaml
import anthropic 
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
N = 16
models = ["claude-3-5-sonnet-20240620","llama-3.1-70b-versatile","llama3.1-70b","gpt-4o-mini","gpt-4-turbo-2024-04-09", "gpt-3.5-turbo-0125", "gpt-4o", "adaptive", "o1-mini", "llama3.1-405b", "llama3.1-8b"]
llm_runs = {model: 0 for model in models}
llm_input_tokens = {model: 0 for model in models}
llm_output_tokens = {model: 0 for model in models}
hls_runs = 0
compile_runs = 0
libs = """
#include "../include/ac_float.h"
#include "../include/ac_fixed.h"
"""

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
        if func in calls_table:
            explore_calls(func, hierarchical_calls)
        if func not in hierarchical_calls:
            hierarchical_calls.append(func)
    if top not in hierarchical_calls:
        hierarchical_calls.append(top)
    return hierarchical_calls
      
def call_llm(model, message_list):
    if "claude" in model:
        system_content = message_list[0]["content"]
        mlist = message_list[1:]
        print(mlist)
        message = client.messages.create(
            model=model,
            messages=mlist,
            system=system_content,
            temperature=0.2,
            top_p=0.2,
            max_tokens=4096
        )
        print("LLM RAW RESPONSE: ", message)
        message_list.append({"role": "assistant", "content": message.content[0].text})
        llm_input_tokens[model] += message.usage.input_tokens
        llm_output_tokens[model] += message.usage.output_tokens
        return message.content[0].text
    else: 
        completion = client.chat.completions.create(
            model=model,
            messages = message_list,
            top_p=0.2,
            temperature=0.25
        )
        print("LLM RAW RESPONSE: ", completion)
        message_list.append({"role": "assistant", "content": completion.choices[0].message.content})
        llm_input_tokens[model] += completion.usage.prompt_tokens
        llm_output_tokens[model] += completion.usage.completion_tokens
        return completion.choices[0].message.content


def build_unit_test(func):
    global compile_runs
    print("Building unit test for ", func)
    # get param values
    # build gdb script
    with open("tmp/" + func + "_gdb.py", "w") as f:
        print("import gdb", file =f)
        print("""gdb.execute("file tmp/to_debug")""", file=f)
        print(f"""gdb.execute("break {func}")""", file =f)
        print("""gdb.execute("run")""", file =f)
        
        for i in range(1):
            for param in params_table[func]:
                if isinstance(param[0], c_ast.PtrDecl):
                    print(f"""gdb.execute("p/x *{param[-1]}")""", file =f)
                else:
                    print(f"""gdb.execute("p/x {param[-1]}")""", file =f)
            print("""gdb.execute("continue")""", file =f)
            

    # compile the file with gcc
    p= Popen(["gcc","-ggdb", "-g3", filename, "-o", f"tmp/to_debug"], stdout=PIPE, stderr=PIPE, text=True)
    compile_runs += 1
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
        lines = dbg_out.split("\n")
        multiline = False
        for j in range(len(lines)):
            if not multiline:
                line = lines[j]
            else:
                line = line + lines[j]
            if line.startswith(f"${i+1} = "):
                if lines[j+1].startswith("$") or lines[j+1].startswith("Breakpoint") or "process" in lines[j+1] or line.count("{") == line.count("}"):    
                    multiline = False
                    # is a pointer
                    pointers_table[params_table[func][i][1]] = (0,0)
                    init = c_ast.Constant(c_ast.IdentifierType(['int']), "0")
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
                        pointers_table[params_table[func][i][1]] = (0,0)
                else:
                    multiline = True
        if isinstance(params_table[func][i][0], c_ast.PtrDecl):
            main_def.body.block_items.append(c_ast.Decl(params_table[func][i][1], [], [], [], None, params_table[func][i][0].type, init, None))
        else:
            main_def.body.block_items.append(c_ast.Decl(params_table[func][i][1], [], [], [], None, params_table[func][i][0], init, None))
    # add call to function
    main_def.body.block_items.append(c_ast.FuncCall(c_ast.ID(func), c_ast.ExprList([c_ast.ID(param[1]) if not isinstance(param[0], c_ast.PtrDecl) else c_ast.UnaryOp('&', c_ast.ID(param[1])) for param in params_table[func]])))

    # add prints
    print(pointers_table)
    for i in range(n_params):
        if isinstance(params_table[func][i][0], c_ast.PtrDecl):
            if pointers_table[params_table[func][i][1]][0] == 2:
                code_str = double_array_printer.format(name=params_table[func][i][1], size1=pointers_table[params_table[func][i][1]][1], size2=pointers_table[params_table[func][i][1]][2])
            elif pointers_table[params_table[func][i][1]][0] == 1:
                code_str = array_printer.format(name=params_table[func][i][1], size=pointers_table[params_table[func][i][1]][1])
            else:
                main_def.body.block_items.append(c_ast.FuncCall(c_ast.ID("printf"), c_ast.ExprList([c_ast.Constant(c_ast.IdentifierType(['char']), f'"%d\\n"'), c_ast.ID(params_table[func][i][1])])))
                continue
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

def getSignatures(func):
    # get all the signatures of the functions called by func
    sig_string = ""
    for filename in processed:
        with open(filename) as file:
            code = file.read()
            # find line that contains the function signature
            for line in code.splitlines():
                if filename.split("/")[1].split("_final")[0] in line:
                    sig_string += line+";\n"
                    break 
    return sig_string

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
    global compile_runs
    global hls_runs
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
        hls_runs += 1

        # parse log
        with open("catapult.log", "r") as f:
            log = f.read()
            if "# Error:" in log:
                error = log.split("# Error:")[1]
                print("Error: ", error)
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
        
        if "Floating-point"in error:
            error += floating_point_prompt
        elif "recursion" in error:
            error += recursion_prompt
        elif "pointer" in error:
            error += pointer_prompt

        signatures = getSignatures(top_function)

        std_prompt = f"""Help me rewrite the {top_function} function to be compatible with HLS: \n```\n{code_to_fix}```\n 
        The following child functions and includes will be provided to with the following signature, assume them present in the code:
        \n```{includes}\n{signatures}\n```\n
        The current problem is:" \n{error}
        \n\n also include a main function that tests the code in the same way of the reference code: \n```{test_code}\n```"""

        initial_prompt = std_prompt

    else :
        error = 1
        # read straming example
        with open("inputs/streaming_example.c", "r") as f:
            streaming_example = f.read()
        interface_prompt = f"""Rewrite the {top_function} function to be compatible for HLS. The first task it to rewrite it such that it will get inferred as a streaming function,
        to do so, I need to get rid of the global array and have the function take a parameter
        to accept one element at each function call. The following is an example on how this can be done: \n```\n{streaming_example}\n```\n
        The following includes will be provided, assume them present in the code and do not repeat them:
        \n```{includes}\n```\n
        \nThe function is \n```\n{code_to_fix}\n```\n
        If there is more than one loop one will need multiple if statements to differentiate the outer loops actions.
        The final function must not contain loops.
        Include a main function that tests the code in the same way of the reference code: \n```\n{test_code}```\n
        Do not add any global variables or defines, if needed I will add them to your code. You should only modify the function you are being asked to, copy the rest of the code as is.
        """
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
            with open(f"{out_folder}{top_function}.log", "w") as f:
                for model in models:
                    if llm_runs[model] == 0:
                        continue
                    print(f"{model} runs: {llm_runs[model]}", file=f)
                    print(f"{model} input tokens: {llm_input_tokens[model]}", file=f)
                    print(f"{model} output tokens: {llm_output_tokens[model]}", file=f)
                    if hierarchical_calls:
                        print(f"# of functions: {len(hierarchical_calls)}", file=f)
                    print(f"HLS runs: {hls_runs}", file=f)
                    print(f"Compile runs: {compile_runs}", file=f)
            print("Exiting due to too many iterations")
            exit(1)
        while error != None:
            error = None
            print("iteration ", i)
            if i ==10:
                with open(f"{out_folder}{top_function}.log", "w") as f:
                    for model in models:
                        if llm_runs[model] == 0:
                            continue
                        print(f"{model} runs: {llm_runs[model]}", file=f)
                        print(f"{model} input tokens: {llm_input_tokens[model]}", file=f)
                        print(f"{model} output tokens: {llm_output_tokens[model]}", file=f)
                        if hierarchical_calls:
                            print(f"# of functions: {len(hierarchical_calls)}", file=f)
                    print(f"HLS runs: {hls_runs}", file=f)
                    print(f"Compile runs: {compile_runs}", file=f)
                print("Exiting due to too many iterations")
                exit(1)
            if model == "adaptive":
                model_name = "gpt-4o-mini" if i+j<4 else "gpt-4o"
            else: 
                model_name = model
            print("Model: ", model_name)
            
            i+=1
            # prompt
            print("Prompt: ", message_list[-1]["content"])
            response = call_llm(model_name, message_list)
            total_gpt_runs+=1
            print("LLM RESPONSE:")
            print( response)
            llm_runs[model_name] += 1
            # get c copde and create a file with it
            c_code_dut = response.split("```c")[1].split("```")[0]

            # remove "#include lines"
            c_code_dut = "\n".join([line for line in c_code_dut.split("\n") if not line.startswith("#include")])

            # new file
            llm_file = f"tmp/{top_function}_llm.c"
            with open(llm_file, "w") as f:
                f.write(libs)
                f.write(includes)
                for proc in processed:
                    with open(f"{proc}", "r") as p:
                        f.write(p.read())
                f.write(c_code_dut)
                # f.write(test_code_dut)

            # compile the file with gcc 
            print("Compiling the code")
            log = subprocess.run(["g++", llm_file, "-o", llm_file[:-2]], capture_output=True)
            compile_runs += 1
            if "error" in log.stderr.decode():
                error = log.stderr.decode()
                #only keep the first 3 lines
                print("Error: ", error)

                error = "\n".join(error.split("\n")[:3])
                prompt = "There is an error in the code: \n" + error + ", please try again\n"
                if "redefinition" in error:
                    prompt += redefinition_prompt
                # update message_list
                #print("There is an error in the code: ", error)
                message_list.append({"role": "user", "content": prompt})
                continue

            # make file for reference output
            orig_file = f"tmp/{top_function}_testbench.c"
            with open(orig_file, "w") as f:
                f.write(includes)
                f.write(orig_code)
                f.write(test_code)

            # compile the reference file with gcc
            subprocess.run(["g++", orig_file, "-o", orig_file[:-2]])
            compile_runs += 1

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
        hls_runs += 1
        j += 1
        # parse log
        with open("catapult.log", "r") as f:
            log = f.read()
            if "# Error:" in log:
                error = log.split("# Error:")[1]
                print("Error: ", error)
                prompt = "Help me rewrite this function to be compatible with HLS: \n" + c_code_dut + "\n The current problem is:" + error
                if "Floating-point"in error:
                    prompt += floating_point_prompt
                elif "recursion" in error:
                    prompt += recursion_prompt
                elif "pointer" in error:
                    prompt += pointer_prompt

                prompt += f"""Include a main function that tests the code in the same way of the reference code: \n{test_code}
        Do not add any global variables or defines, if needed I will add them to your code. You should only modify the function you are being asked to, copy the rest of the code as is.
"""        
                message_list = [{"role": "system", "content": system_content_c2hlsc},
                {"role": "user", "content": prompt}]
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
                return HLSC_optimizer(includes, orig_code, code_to_optimize, test_code, tcl, top_function, out_folder, processed, model, mode, opt_target)
                # with open(f"tmp/{top_function}_final.c", "w") as f:
                #     # need to take out the main function
                #     code_to_save = c_code_dut.split("int main()")[0]
                #     f.write(code_to_save)
                # return f"tmp/{top_function}_final.c"

    print("The code is correct, number of gpt runs: ", total_gpt_runs)

def HLSC_optimizer (includes, orig_code, code_to_fix, test_code, tcl, top_function, out_folder, processed, model="adaptive", mode="standard", opt_target="area"):
    global compile_runs
    global hls_runs
    # write initial c
    file_name = f"tmp/{top_function}_initial_opt.c"
    with open(file_name, "w") as f:
        f.write(includes)
        f.write(orig_code)

    
    # get signatures
    signatures = getSignatures(top_function)
    initial_prompt = f"""Update the {top_function} function to optimize it for HLS targetting {opt_target}.
        The function is \n```\n{code_to_fix}\n```\n
        The following child functions and includes will be provided to with the following signature, assume them present in the code:
        \n```{includes}\n{signatures}\n```\n
        You should include a main function that tests the code in the same way of the reference code: \n```\n{test_code}\n```"""
    
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
            with open(f"{out_folder}{top_function}.log", "w") as f:
                for model in models:
                    if llm_runs[model] == 0:
                        continue
                    print(f"{model} runs: {llm_runs[model]}", file=f)
                    print(f"{model} input tokens: {llm_input_tokens[model]}", file=f)
                    print(f"{model} output tokens: {llm_output_tokens[model]}", file=f)
                    if hierarchical_calls:
                        print(f"# of functions: {len(hierarchical_calls)}", file=f)
                    print(f"HLS runs: {hls_runs}", file=f)
                    print(f"Compile runs: {compile_runs}", file=f)
            print("Exiting due to too many iterations")
            exit(2)
        while error != None:
            error = None
            if model == "adaptive":
                model_name =  "gpt-4o"
            else: 
                model_name = model
            if i ==10:
                with open(f"{out_folder}{top_function}.log", "w") as f:
                    for model in models:
                        if llm_runs[model] == 0:
                            continue
                        print(f"{model} runs: {llm_runs[model]}", file=f)
                        print(f"{model} input tokens: {llm_input_tokens[model]}", file=f)
                        print(f"{model} output tokens: {llm_output_tokens[model]}", file=f)
                        if hierarchical_calls:
                            print(f"# of functions: {len(hierarchical_calls)}", file=f)
                        print(f"HLS runs: {hls_runs}", file=f)
                        print(f"Compile runs: {compile_runs}", file=f)
                print("Exiting due to too many iterations")
                exit(1)
            i+=1
            # prompt
            print("Prompt: ", message_list[-1]["content"])
            response = call_llm(model_name, message_list)
            total_gpt_runs+=1
            print("LLM RESPONSE:")
            print(response)
            llm_runs[model_name] += 1
            # get c copde and create a file with it
            c_code_dut = response.split("```c")[1].split("```")[0]

            # remove "#include lines"
            c_code_dut = "\n".join([line for line in c_code_dut.split("\n") if not line.startswith("#include")])

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
            log = subprocess.run(["g++", llm_file, "-o", llm_file[:-2]], capture_output=True)
            compile_runs += 1
            if "error" in log.stderr.decode():
                error = log.stderr.decode()
                #only keep the first 3 lines
                print("Error: ", error)
                error = "\n".join(error.split("\n")[:3])
                # update message_list

                prompt = "There is an error in the code: \n" + error + ", try again\n"
                if "redefinition" in error:
                    prompt += redefinition_prompt
                print("There is an error in the code: ", error)
                message_list.append({"role": "user", "content": prompt})
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
        hls_runs += 1
        j += 1
        # parse log
        with open("catapult.log", "r") as f:
            log = f.read()
            if "# Error:" in log:
                error = log.split("# Error:")[1]
                print("Error: ", error)
                main_function = f"""
Include a main function that tests the code in the same way of the reference code: \n{test_code}
        Do not add any global variables or defines, if needed I will add them to your code. You should only modify the function you are being asked to, copy the rest of the code as is.
        """
                message_list = [{"role": "system", "content": system_content_optimizer},
                {"role": "user", "content": "Help me rewrite this function to be compatible with HLS: \n" + c_code_dut + "\n The current problem is:" + error + main_function}]
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
    modes = ["standard", "streaming"] # from yaml file claude-3-5-sonnet-20240620
    parser.add_argument('--model', type=str, default="claude-3-5-sonnet-20240620", choices=models, help='model name to use, default is gpt-3.5-turbo-0125')
    opt_target = ["area", "latency"]
    parser.add_argument('--opt_target', type=str, default="area", choices=opt_target, help='optimization target, default is area')
    args = parser.parse_args()
    model = args.model
    if model != "adaptive":
        model_name = model
    if "llama" in model:
        model_name = "llama-3.1-70b-versatile"
        model = model_name
        client = OpenAI(
        api_key = os.environ.get("LLAMA_API_KEY"),
        base_url = "https://api.groq.com/openai/v1"
        #base_url = "https://api.llama-api.com"
        #base_url = "https://api.aimlapi.com"
        )
    elif "claude" in model:
        model_name = model
        client = anthropic.Anthropic(
        # defaults to os.environ.get("ANTHROPIC_API_KEY")
        )
    else:
        client = OpenAI()

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
    out_folder = f"outputs_{top_function}"
    idx = 1
    while os.path.exists(out_folder+"_"+model+"_"+str(idx)):
        idx +=1 
    out_folder = out_folder+"_"+model+"_"+str(idx) + "/"
    os.makedirs(out_folder)
    hierarchical_calls = []
    processed = []
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
        
        
        print(calls_table)
        explore_calls(top_function, hierarchical_calls) # inits hierarchical_calls

        print("Hierarchical calls: ", hierarchical_calls)
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
    with open(f"{out_folder}{top_function}.log", "w") as f:
        for model in models:
            if llm_runs[model] == 0:
                continue
            print(f"{model} runs: {llm_runs[model]}", file=f)
            print(f"{model} input tokens: {llm_input_tokens[model]}", file=f)
            print(f"{model} output tokens: {llm_output_tokens[model]}", file=f)
            if hierarchical_calls:
                print(f"# of functions: {len(hierarchical_calls)}", file=f)
        print(f"HLS runs: {hls_runs}", file=f)
        print(f"Compile runs: {compile_runs}", file=f)
        print("")    
        print("".join(saved_lines), file=f)
        
    # copy important files
    subprocess.run(["cp", "-r", f"Catapult_{max(catapult_dirs)}", f"{out_folder}Catapult_{top_function}"])
    subprocess.run(["cp", f"tmp/{top_function}_llm_opt.c", f"{out_folder}"])