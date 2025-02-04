from openai import OpenAI
import argparse
import subprocess
import yaml
import anthropic 
from prompts import *
from collections import OrderedDict

import os
from pycparser import c_ast, parse_file, c_generator, c_parser
from subprocess import Popen, PIPE, STDOUT

# make dir if does not exist
if not os.path.exists("tmp"):
    os.makedirs("tmp")

#======================================================================================#
#                                 GLOBAL CONSTANTS                                     #
#======================================================================================#
# supported models
models = ["claude-3-5-sonnet-20240620", "claude-3-5-haiku-20241022","gpt-4o-mini","gpt-4-turbo-2024-04-09", "gpt-3.5-turbo-0125", "gpt-4o", "adaptive", "o3-mini", "deepseek-chat", "deepseek-reasoner"]
# float and fixed libraries includes
libs = """
#include "../include/ac_float.h"
#include "../include/ac_fixed.h"
#include <stdint.h>
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



#--------------------------------------------------------------------------------------#
#                                     CFG CLASS                                        #
#--------------------------------------------------------------------------------------#
class CFG:
    def __init__(self, args):
        self.model = args.model
        if self.model != "adaptive":
            self.model_name = self.model
        if "claude" in self.model:
            self.model_name = self.model
            self.client = anthropic.Anthropic(
            # defaults to os.environ.get("ANTHROPIC_API_KEY")
            )
        elif "deepseek" in self.model:
            ds_key = os.environ.get("DEEPSEEK_API_KEY")
            if ds_key is None:
                print("deepseek model selected but DEEPSEEK_API_KEY not set")
                exit(1)
            self.client = OpenAI(base_url="https://api.deepseek.com", api_key=ds_key)
        else:
            self.client = OpenAI()

        self.opt_target = args.opt_target
        self.characterize = args.characterize
        print("Model: ", self.model)
        print("Optimization target: ", self.opt_target)

        # parse yaml file with orig code test code includes and tcl; and top function
        with open(args.config, "r") as f:
            config = yaml.safe_load(f)

        if "mode" in config:
            self.mode = config["mode"] if config["mode"] in modes else "standard"
        else:
            self.mode = "standard"

        if "hierarchical" in config:
            self.hierarchical = config["hierarchical"]
        else:
            self.hierarchical = False

        print("Running in mode: ", self.mode, "Hierarchical: ", self.hierarchical)

        # read orig code from config
        with open(config["orig_code"], "r") as f:
            self.orig_code = f.read()
        # read test code
        with open(config["test_code"], "r") as f:
            self.test_code = f.read()
        #read includes
        with open(config["includes"], "r") as f:
            self.includes = f.read()
        #read tcl
        with open(config["tcl"], "r") as f:
            self.tcl = f.read()

        self.top_function = config["top_function"]
        # out folder
        self.out_folder = f"outputs_{self.top_function}"
        idx = 1
        while os.path.exists(self.out_folder+"_"+self.model+"_"+str(idx)):
            idx +=1 
        self.out_folder = self.out_folder+"_"+self.model+"_"+str(idx) + "/"
        os.makedirs(self.out_folder)
        self.hierarchical_calls = []
        self.processed = []
        self.calls_table = {}
        self.params_table = {}
        self.nodes_table = {}
        self.params_pointers_table = {}
        self.structs_table = {}  
        self.llm_runs = {model: 0 for model in models}
        self.llm_input_tokens = {model: 0 for model in models}
        self.llm_output_tokens = {model: 0 for model in models}
        self.hls_runs = 0
        self.compile_runs = 0



#--------------------------------------------------------------------------------------#
#                          TOP FUNCTION VISITOR CLASS                                  #
#--------------------------------------------------------------------------------------#
class TopFunctionVisitor(c_ast.NodeVisitor):
     # this visitor needs to know the name of top module
    def __init__(self, top):
        self.top = top
        self.top_node = None

    def visit_FuncDef(self, node):
        if node.decl.name == self.top:
            self.top_node = node


#--------------------------------------------------------------------------------------#
#                             HIERARCHY VISITOR CLASS                                  #
#--------------------------------------------------------------------------------------#
class HierarchyVisitor(c_ast.NodeVisitor):
    def __init__(self, cfg):
        self.cfg = cfg

    def visit_FuncDef(self, node):
        #print('%s at %s' % (node.decl.name, node.decl.coord))
        self.cfg.nodes_table[node.decl.name] = node
        self.cfg.calls_table[node.decl.name] = []
        self.cfg.params_table[node.decl.name] = []
        if node.decl.type.args:
            self.cfg.params_table[node.decl.name] = [(param.type, param.name) for param in node.decl.type.args]
            self.cfg.params_pointers_table[node.decl.name] = 1 in [1 if isinstance(param.type, c_ast.PtrDecl) or isinstance(param.type, c_ast.ArrayDecl) else 0 for param in node.decl.type.args]
        v = FuncCallVisitor(node.decl.name, self.cfg)
        v.visit(node)



#--------------------------------------------------------------------------------------#
#                          FUNCTION CALL VISITOR CLASS                                 #
#--------------------------------------------------------------------------------------#
class FuncCallVisitor(c_ast.NodeVisitor):
    # this visitor needs to know the name of itself before
    def __init__(self, name, cfg):
        self.name = name
        self.cfg = cfg

    def visit_FuncCall(self, node):
        #print(self.name, " calls", node.name.name)
        cfg.calls_table[self.name].append(node.name.name)



#--------------------------------------------------------------------------------------#
#                               OPERATOR VISITOR CLASS                                 #
#--------------------------------------------------------------------------------------#
class OperatorVisitor(c_ast.NodeVisitor):
    def __init__(self):
        self.operators_count =0
    
    def visit_BinaryOp(self, node):
        self.operators_count += 1
        self.generic_visit(node)
    
    def visit_UnaryOp(self, node):
        self.operators_count += 1
        self.generic_visit(node)



#--------------------------------------------------------------------------------------#
#                                 POINTER DATA CLASS                                   #
#--------------------------------------------------------------------------------------#
class PointerData():
    def __init__(self):
        self.byte_offset = 0     # this is in bytes
        self.byte_size = 0  # this is in bytes
        self.base = 0
        self.type_size = 0 # this is in bytes for each element
        self.element_offset = 0  # this is byte_offset/type_size
        self.element_size = 0 # this is in ellements



########################################################################################
#                                      CALL LLM                                        #
########################################################################################
def call_llm(model, message_list, cfg):  # unified interface for calling different LLM API based on the model
    if "claude" in model:
        system_content = message_list[0]["content"]
        mlist = message_list[1:]
        print(mlist)
        try:
            message = cfg.client.messages.create(
                model=model,
                messages=mlist,
                system=system_content,
                temperature=0.2,
                top_p=0.2,
                max_tokens=4096
            )
        except anthropic.APIError as e:
            # if overloaded error, retry
            if "overloaded" in str(e):
                return call_llm(model, message_list, cfg)
            else: 
                print(e)
                exit(1)
        print("LLM RAW RESPONSE: ", message)
        message_list.append({"role": "assistant", "content": message.content[0].text})
        cfg.llm_input_tokens[model] += message.usage.input_tokens
        cfg.llm_output_tokens[model] += message.usage.output_tokens
        return message.content[0].text
    else: 
        completion = cfg.client.chat.completions.create(
            model=model,
            messages = message_list,
            #top_p=0.2,
            #temperature=0.25
        )
        print("LLM RAW RESPONSE: ", completion)
        message_list.append({"role": "assistant", "content": completion.choices[0].message.content})
        cfg.llm_input_tokens[model] += completion.usage.prompt_tokens
        cfg.llm_output_tokens[model] += completion.usage.completion_tokens
        return completion.choices[0].message.content



########################################################################################
#                                   EXPLORE CALLS                                      #
########################################################################################
def explore_calls(top, hierarchical_calls, cfg): # gets called with different top and hierarchical, hence cannot take those from cfg
    for func in cfg.calls_table[top]:
        if func in cfg.calls_table:
            explore_calls(func, hierarchical_calls, cfg)
        if func not in hierarchical_calls:
            hierarchical_calls.append(func)
    if top not in hierarchical_calls:
        hierarchical_calls.append(top)
    return hierarchical_calls



########################################################################################
#                                   FIX REPEATS                                        #
########################################################################################
def fix_repeat(string):
    # string be like [0x0, 0x1, 0x2, 0x3, 0x4, 0x5 <repeats 11 times>, 0x10, 0x11, 0x12, 0x13]
    # we need to expand the repeat
    if "repeat" not in string:
        return string
    item_to_fix = ""
    fixed = ""
    for element in string.split(","):
        if "repeats" in element:
            item_to_fix = element
            fixed += "{expanded}"
        else:
            fixed += element
            fixed += ","
    fixed = fixed[:-1]
    to_expand = item_to_fix.split("<repeats")[0] + ","
    times = int(item_to_fix.split("repeats")[1].split("times>")[0])
    expanded = to_expand * times
    # print(fixed.format(expanded=expanded))
    return fixed.format(expanded=expanded)


########################################################################################
#                                  BUILD UNIT TEST                                     #
#######################################################################################
def build_unit_test(func, filename, cfg):
    print("Building unit test for ", func)

    # compile the file with gcc
    print(" ".join(["clang","-ggdb", "-m32", "-g3", "-O0", "-fsanitize=address",filename, "-o", f"tmp/to_debug"]))
    p= Popen(["clang","-ggdb", "-g3", "-O0", "-fsanitize=address",filename, "-o", f"tmp/to_debug"])
    p.wait()
    # get param values
    # build gdb script
    pointers_table = OrderedDict() # associates a pointer to its characteristics 

    if cfg.params_pointers_table[func]:
        # we need to run gdb twice, first time to get the addresses and sizes of memory locations and second time to print those out
        # get addresses
        with open("tmp/" + func + "_gdb.py", "w") as f:
            print("import gdb", file =f)
            print("""gdb.execute("file tmp/to_debug")""", file=f)
            print(f"""gdb.execute("break {func}")""", file =f)
            print("""gdb.execute("run")""", file =f)

            generator = c_generator.CGenerator()
            for param in cfg.params_table[func]:
                if isinstance(param[0], c_ast.PtrDecl) or isinstance(param[0], c_ast.ArrayDecl):
                    print(f"""gdb.execute("p (void) __asan_describe_address({param[-1]})")""", file =f)
                    ptr_type = generator.visit(param[0].type)
                    print(f"""gdb.execute('printf "sizeof {param[-1]} %d\\\\n", sizeof({ptr_type})')""", file =f)
                    pointers_table[param[-1]] = PointerData()
            print("""gdb.execute("quit")""", file =f)

        #run debug 
        p = Popen(["gdb"], stdout=PIPE, stdin=PIPE, stderr=PIPE, bufsize=0, text=True)
        stdout_data, stderr_data = p.communicate(input=f"\n\nsource tmp/{func}_gdb.py\n")
        
        with open("tmp/" + func + "_gdb_fsan.log", "w") as f:
            dbg_out = stdout_data.replace(", \n", ",")
            dbg_out += "STDERR\n"
            dbg_out += stderr_data.replace(", \n", ",")
            print(dbg_out, file=f) # this is just for debugging
    
        keys_list = list(pointers_table.keys())
        idx = 0 # tracks pointer number
        for line in dbg_out.split("\n"):
            
            if "sizeof" in line: 
                name = line.split(" ")[1]
                type_size = int(line.split(" ")[2])
                pointers_table[name].type_size = type_size
            # 3 cases for offset size and base: local array global array dynamic array
            elif "global variable" in line:
                #print(line)
                # 0x00000074bb80 is located 0 bytes inside of global variable 'array' defined in 'global.c' (0x74bb80) of size 80
                offset = int(line.split("located ")[1].split(" bytes")[0])
                size = int(line.split("of size ")[1])
                base = int(line.split("defined in '")[1].split("'")[1].split("(")[1].split(")")[0], 16)
                #print(offset, size, hex(base), idx)
                pointers_table[keys_list[idx]].byte_offset = offset
                pointers_table[keys_list[idx]].byte_size = size
                pointers_table[keys_list[idx]].base = base
                pointers_table[keys_list[idx]].element_offset = offset//pointers_table[keys_list[idx]].type_size
                idx +=1
            elif "region" in line:
                #print(line)
                # 0x507000000090 is located 0 bytes inside of 80-byte region [0x507000000090,0x5070000000e0)
                offset = int(line.spluint32_t("inside of ")[1].split("-byte")[0])
                base = int(line.split("[")[1].split(",")[0], 16)
                #print(offset, size, hex(base), idx)
                pointers_table[keys_list[idx]].byte_offset = offset
                pointers_table[keys_list[idx]].byte_size = size
                pointers_table[keys_list[idx]].base = base
                pointers_table[keys_list[idx]].element_offset = offset//pointers_table[keys_list[idx]].type_size
                idx +=1
            elif "Address " in line:
                #print(line)
                # Address 0x7ffff3f00048 is located in stack of thread T0 at offset 72 in frame
                base = int(line.split("Address ")[1].split(" is")[0], 16)
            elif "Memory access" in line: 
                #print(line)
                # [32, 112) 'array3' (line 19) <== Memory access at offset 72 is inside this variable
                # base is taken from elif above
                # offset is given from frame pointer, have to shift it to our base
                offset = (int(line.split("offset ")[1].split(" is")[0]) - int(line.split("[")[1].split(",")[0])) 
                size = int(line.split("[")[1].split(",")[1].split(")")[0]) - int(line.split("[")[1].split(",")[0])
                base = base - offset
                #print(offset, size, hex(base), idx)
                pointers_table[keys_list[idx]].byte_offset = offset
                pointers_table[keys_list[idx]].byte_size = size
                pointers_table[keys_list[idx]].base = base
                pointers_table[keys_list[idx]].element_offset = offset//pointers_table[keys_list[idx]].type_size
                idx +=1
            #else: print(line)
    # get values
    with open("tmp/" + func + "_gdb.py", "w") as f:
        print("import gdb", file =f)
        print("""gdb.execute("file tmp/to_debug")""", file=f)
        print(f"""gdb.execute("break {func}")""", file =f)
        print("""gdb.execute("run")""", file =f)
        
        for param in cfg.params_table[func]:
            if isinstance(param[0], c_ast.PtrDecl) or isinstance(param[0], c_ast.ArrayDecl):
                print(f"""gdb.execute("p/x *({pointers_table[param[-1]].base})@{pointers_table[param[-1]].byte_size//4}")""", file =f) ## print 4 bytes at a time
            else:
                print(f"""gdb.execute("p/x {param[-1]}")""", file =f)

        print("""gdb.execute("quit")""", file =f)
    # run debug
    p = Popen(["gdb"], stdout=PIPE, stdin=PIPE, stderr=PIPE, bufsize=0, text=True)
    stdout_data, stderr_data = p.communicate(input=f"\n\nsource tmp/{func}_gdb.py\n")
    
    with open("tmp/" + func + "_gdb.log", "w") as f:
        dbg_out = stdout_data.replace(", \n", ",")
        print(dbg_out, file=f) # this is just for debugging but info form fsan is in stderr

    # build the test functoin
    main_decl = c_ast.Decl("main", [], [], [], [], c_ast.FuncDecl(c_ast.ParamList([]), c_ast.TypeDecl("main", [], [], c_ast.IdentifierType(['int']))), None, None)
    main_def = c_ast.FuncDef(main_decl, None, c_ast.Compound([]))
    # add def of params
    n_params = len(cfg.params_table[func])
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
                    # pointers_table[params_table[func][i][1]] = (0,0)
                    init = c_ast.Constant(c_ast.IdentifierType(['int']), "0")
                    if "{" in line and line.count("=") == 1:
                        # is a single array
                        value = line.split(" = ")[1]
                        value = value.replace("{", "[").replace("}", "]")
                        if "repeat" in value:
                            value = fix_repeat(value)
                        value = eval(value)
                        pointers_table[cfg.params_table[func][i][1]].element_size = len(value) # array dimensions and sizes
                        init = c_ast.InitList([c_ast.Constant(c_ast.IdentifierType(['int']), str(val)) for val in value])
                    elif "{" in line and line.count("=") > 1:
                        # it's a struct
                        # needs to go from {size = 0x14, key = 0x37} to [0x14, key = 0x37]
                        value = "=".join(line.split(" = ")[1:])[1:-1]
                        elements = value.split(",")
                        cfg.structs_table[cfg.params_table[func][i][1]] = []
                        values = []
                        for element in elements:
                            split = element.split("=")
                            values.append(split[1].strip())
                            cfg.structs_table[cfg.params_table[func][i][1]].append(split[0].strip())
                        value = "[" + ", ".join(values) + "]"
                        value = fix_repeat(value)
                        value = eval(value)
                        init = c_ast.InitList([c_ast.Constant(c_ast.IdentifierType(['int']), str(val)) for val in value])
                    else:
                        value = line.split(" = ")[1]
                        init =c_ast.Constant(c_ast.IdentifierType(['int']), value)
                        #pointers_table[params_table[func][i][1]] = (0,0)
                else:
                    multiline = True
        if isinstance(cfg.params_table[func][i][0], c_ast.PtrDecl) or isinstance(cfg.params_table[func][i][0], c_ast.ArrayDecl):
            array_type = c_ast.TypeDecl(cfg.params_table[func][i][1], [], [], c_ast.IdentifierType(['unsigned','int']))
            main_def.body.block_items.append(c_ast.Decl(cfg.params_table[func][i][1], [], [], [], None, c_ast.ArrayDecl(array_type, None, None), init, None))
        elif isinstance(cfg.params_table[func][i][0], c_ast.TypeDecl): # struct
            main_def.body.block_items.append(c_ast.Decl(cfg.params_table[func][i][1], [], [], [], None, c_ast.TypeDecl(cfg.params_table[func][i][1], None, None, cfg.params_table[func][i][0]), init, None))
        else:
            main_def.body.block_items.append(c_ast.Decl(cfg.params_table[func][i][1], [], [], [], None, cfg.params_table[func][i][0].type, init, None))
    # build param list for call to function
    expr_list = []
    for param in cfg.params_table[func]:
        #if not isinstance(param[0], c_ast.PtrDecl) and not isinstance(param[0], c_ast.ArrayDecl):
            expr_list.append(c_ast.Cast(param[0],c_ast.ID(param[1])))
       
    # check function node for return type
    if cfg.nodes_table[func].decl.type.type.type.names[0] == "void":
        # add call to function
        main_def.body.block_items.append(c_ast.FuncCall(c_ast.ID(func), c_ast.ExprList(expr_list)))
    else:
        # need to add a variable to store the return value
        main_def.body.block_items.append(c_ast.Decl("ret", [], [], [], None, c_ast.TypeDecl("ret", [], [], c_ast.IdentifierType(cfg.nodes_table[func].decl.type.type.type.names)), None, None))
        # add call to function and assugn return value to ret
        main_def.body.block_items.append(c_ast.Assignment("=", c_ast.ID("ret"), c_ast.FuncCall(c_ast.ID(func), c_ast.ExprList(expr_list))))
        # add print of ret
        main_def.body.block_items.append(c_ast.FuncCall(c_ast.ID("printf"), c_ast.ExprList([c_ast.Constant(c_ast.IdentifierType(['char']), f'"%d\\n"'), c_ast.ID("ret")])))                           
    # add prints
    for i in range(n_params):
        if isinstance(cfg.params_table[func][i][0], c_ast.PtrDecl) or isinstance(cfg.params_table[func][i][0], c_ast.ArrayDecl):
            if cfg.params_table[func][i][1] in pointers_table:
                code_str = array_printer.format(name=cfg.params_table[func][i][1], size=pointers_table[cfg.params_table[func][i][1]].element_size)
            else:
                main_def.body.block_items.append(c_ast.FuncCall(c_ast.ID("printf"), c_ast.ExprList([c_ast.Constant(c_ast.IdentifierType(['char']), f'"%d\\n"'), c_ast.ID(cfg.params_table[func][i][1])])))
                continue
            for_ast = c_parser.CParser().parse(code_str).ext[0]
            main_def.body.block_items.extend(for_ast.body.block_items)
        elif isinstance(cfg.params_table[func][i][0], c_ast.TypeDecl) and isinstance(cfg.params_table[func][i][0].type, c_ast.Struct):
                for element in cfg.structs_table[cfg.params_table[func][i][1]]:
                    main_def.body.block_items.append(c_ast.FuncCall(c_ast.ID("printf"), c_ast.ExprList([c_ast.Constant(c_ast.IdentifierType(['char']), f'"{element} %d\\n"'), c_ast.ID(f"{cfg.params_table[func][i][1]}.{element}")])))
        else:
            main_def.body.block_items.append(c_ast.FuncCall(c_ast.ID("printf"), c_ast.ExprList([c_ast.Constant(c_ast.IdentifierType(['char']), f'"%d\\n"'), c_ast.ID(cfg.params_table[func][i][1])])))

    generator = c_generator.CGenerator()
    with open("tmp/" + func + ".c", "w") as f:
        children = []
        explore_calls(func, children, cfg)
        for child_func in children:
            print(generator.visit(cfg.nodes_table[child_func]), file=f)
    with open("tmp/" + func + "_test.c", "w") as f:
        print(generator.visit(main_def), file=f)




########################################################################################
#                                  GET SIGNATURES                                      #
########################################################################################
def getSignatures(func):
    # get all the signatures of the functions called by func
    sig_string = ""
    for filename in cfg.processed:
        with open(filename) as file:
            code = file.read()
            # find line that contains the function signature
            for line in code.splitlines():
                if filename.split("/")[1].split("_final")[0] in line:
                    sig_string += line+";\n"
                    break 
    return sig_string



########################################################################################
#                                 LOG FAILD RUNS                                       #
########################################################################################
def log_failed_runs(cfg):
    with open(f"{cfg.out_folder}{cfg.top_function}.log", "w") as f:
        for model in models:
            if cfg.llm_runs[model] == 0:
                continue
            print(f"{model} runs: {cfg.llm_runs[model]}", file=f)
            print(f"{model} input tokens: {cfg.llm_input_tokens[model]}", file=f)
            print(f"{model} output tokens: {cfg.llm_output_tokens[model]}", file=f)
            if cfg.hierarchical_calls:
                print(f"# of functions: {len(cfg.hierarchical_calls)}", file=f)
        print(f"HLS runs: {cfg.hls_runs}", file=f)
        print(f"Compile runs: {cfg.compile_runs}", file=f)



########################################################################################
#                                   FEEDBACK LOOP                                      #
########################################################################################
def feedback_loop(message_list, cfg, postfix): # message list should contain system prompt and first user prompt
    error = 1
    j=0
    print("System Prompt: ", message_list[0]["content"])
    while error != None:
        # functionality check loop
        i = 0
        if j == 10:
            log_failed_runs(cfg)
            print("Exiting due to too many iterations")
            exit(1)
        while error != None:
            error = None
            print("iteration ", i)
            if i ==10:
                log_failed_runs(cfg)
                print("Exiting due to too many iterations")
                exit(1)
            if cfg.model == "adaptive":
                model_name = "gpt-4o-mini" if i+j<4 else "gpt-4o"
            else: 
                model_name = cfg.model
            print("Model: ", model_name)
            
            i+=1
            # prompt
            print("Prompt: ", message_list[-1]["content"])
            response = call_llm(model_name, message_list, cfg)
            print("LLM RESPONSE:")
            print( response)
            cfg.llm_runs[model_name] += 1
            # get c copde and create a file with it
            if "```c" in response:
                c_code_dut = response.split("```c")[1].split("```")[0]
            else:
                c_code_dut = response.split("```")[1].split("```")[0]
            # remove "#include lines"
            c_code_dut = "\n".join([line for line in c_code_dut.split("\n") if not line.startswith("#include")])

            # new file
            llm_file = f"tmp/{cfg.top_function}_llm.c"
            with open(llm_file, "w") as f:
                f.write(libs)
                f.write(cfg.includes)
                for proc in cfg.processed:
                    with open(f"{proc}", "r") as p:
                        f.write(p.read())
                f.write(c_code_dut)
                f.write(cfg.test_code)

            # compile the file with gcc 
            print("Compiling the code")
            log = subprocess.run(["clang++", "--no-warnings", llm_file, "-o", llm_file[:-2]], capture_output=True)
            cfg.compile_runs += 1
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
            orig_file = f"tmp/{cfg.top_function}_testbench.c"
            with open(orig_file, "w") as f:
                f.write(cfg.includes)
                f.write(cfg.orig_code)
                f.write(cfg.test_code)

            # compile the reference file with gcc
            subprocess.run(["clang", "--no-warnings", orig_file, "-o", orig_file[:-2]])
            cfg.compile_runs += 1

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
        tcl_file = cfg.out_folder + "initial.tcl"
        with open(tcl_file, "w") as f:
            f.write(cfg.tcl.format(top_function=cfg.top_function+"_hls", c_file=llm_file))

        print("Running catapult")
        subprocess.run(["catapult", "-shell", "-file", tcl_file], capture_output=True)
        cfg.hls_runs += 1
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

                prompt += f"""include a function named {cfg.top_function} that mainains the original function signature and calls the new {cfg.top_function}_hls, this will serve as a wrapper. 
                I will call this function in an automated test and provide feedback on the outcome."""       
                message_list = [{"role": "system", "content": system_content_c2hlsc},
                {"role": "user", "content": prompt}]
                continue
            else:
                error = None
                print("The code is correct")
                # write file
                with open(f"tmp/{cfg.top_function}{postfix}.c", "w") as f:
                    # need to take out the main function
                    code= c_code_dut.split("int main()")[0]
                    f.write(code)
                return code
               


########################################################################################
#                                       C2HLSC                                         #
########################################################################################
def C2HLSC (cfg, optimize=False):
    # write initial c
    print("model: ", cfg.model)
    postfix = "_opt" if optimize else ""
    file_name = f"tmp/{cfg.top_function}_initial{postfix}.c"
    with open(file_name, "w") as f:
        f.write(cfg.includes)
        f.write(cfg.orig_code)

    # create a file with the formatted tcl
    tcl_file = cfg.out_folder + "initial.tcl"
    with open(tcl_file, "w") as f:
        f.write(cfg.tcl.format(top_function=cfg.top_function, c_file=file_name))


    # code to fix from orig code
    # get only the top function
    # parse orig code
    ast = parse_file(file_name, use_cpp=True,
                        cpp_args=r'-Iutils/fake_libc_include')
    v = TopFunctionVisitor(cfg.top_function)
    v.visit(ast)
    code_to_fix = c_generator.CGenerator().visit(v.top_node)

    if cfg.mode == "standard":
        # run catapult and get the error
        print("Running catapult")
        subprocess.run(["catapult", "-shell", "-file", tcl_file], capture_output=True)
        cfg.hls_runs += 1

        # parse log
        with open("catapult.log", "r") as f:
            log = f.read()
            if "# Error:" in log:
                error = log.split("# Error:")[1]
                print("Error: ", error)
            else:
                print(f"{cfg.top_function} is correct, does not need any changes")
                # write final file
                with open(f"tmp/{cfg.top_function}_final.c", "w") as f:
                    f.write(code_to_fix)
                return f"tmp/{cfg.top_function}_final.c"
               
        
        if "Floating-point"in error:
            error += floating_point_prompt
        elif "recursion" in error:
            error += recursion_prompt
        elif "pointer" in error:
            error += pointer_prompt

        signatures = getSignatures(cfg.top_function)

        std_prompt = f"""Help me rewrite the {cfg.top_function} function to be compatible with HLS, name the new function {cfg.top_function}_hls: \n```\n{code_to_fix}```\n 
        The following child functions and includes will be provided with the following signature, assume them present in the code:
        \n```{cfg.includes}\n{signatures}\n```\n
        The current problem is:" \n{error}
        \n\n include a function named {cfg.top_function} that mainains the original function signature and calls the new {cfg.top_function}_hls, this will serve as a wrapper. I will call this function in an automated test and provide feedback on the outcome."""

        initial_prompt = std_prompt

    else :
        error = 1
        # read straming example
        with open("inputs/streaming_example.c", "r") as f:
            streaming_example = f.read()
        interface_prompt = f"""Rewrite the {cfg.top_function} function to be compatible for HLS. The first task it to rewrite it such that it will get inferred as a streaming function,
        to do so, I need to get rid of the global array and have the function take a parameter
        to accept one element at each function call. The following is an example on how this can be done: \n```\n{streaming_example}\n```\n
        The following includes will be provided, assume them present in the code and do not repeat them:
        \n```{cfg.includes}\n```\n
        \nThe function is \n```\n{code_to_fix}\n```\n
        If there is more than one loop one will need multiple if statements to differentiate the outer loops actions.
        The final function must not contain loops.
        Include a main function that tests the code in the same way of the reference code: \n```\n{cfg.test_code}```\n
        Do not add any global variables or defines, if needed I will add them to your code. You should only modify the function you are being asked to, copy the rest of the code as is.
        """
        initial_prompt = interface_prompt

    # initial message list
    message_list=[
            {"role": "system", "content": system_content_c2hlsc},
            {"role": "user", "content": initial_prompt}
        ]
    
    code_to_optimize = feedback_loop(message_list, cfg, "_to_opt")

    return HLSC_optimizer(cfg, code_to_optimize)
                


########################################################################################
#                                  HLS OPTIMIZER                                       #
########################################################################################
def HLSC_optimizer (cfg, code_to_optimize):
    global compile_runs
    global hls_runs
    # write initial c
    file_name = f"tmp/{cfg.top_function}_initial_opt.c"
    with open(file_name, "w") as f:
        f.write(cfg.includes)
        f.write(cfg.orig_code)
    
    # get signatures
    signatures = getSignatures(cfg.top_function)
    initial_prompt = f"""Update the {cfg.top_function}_hls function to optimize it for HLS targetting {cfg.opt_target}.
        The function is \n```\n{code_to_optimize}\n```\n
        The following child functions and includes will be provided to with the following signature, assume them present in the code:
        \n```{cfg.includes}\n{signatures}\n```\n
        You should not change the function signature. Do not touch {cfg.top_function}, it is used for testing purposes only"""
    
    message_list=[
            {"role": "system", "content": system_content_optimizer},
            {"role": "user", "content": initial_prompt}
        ]
    
    feedback_loop(message_list, cfg, "_final")
    return f"tmp/{cfg.top_function}_final.c"



########################################################################################
#                               CHARACTERIZE BENCHMARK                                 #
########################################################################################
def characterize_benchmark():
    filename = "tmp/complete.c"
    with open(filename, "w") as f:
        f.write(cfg.includes)
        f.write(cfg.orig_code)
        f.write(cfg.test_code)

    ast = parse_file(filename, use_cpp=True,
                    cpp_args=r'-Iutils/fake_libc_include')
    v = HierarchyVisitor(cfg)
    v.visit(ast)
    
    # print(cfg.calls_table)
    explore_calls(cfg.top_function, cfg.hierarchical_calls) # inits hierarchical_calls

    total_lines=0
    min_lines = 99999999
    max_lines = 0
    tot_operators = 0
    min_operators = 99999999
    max_operators = 0

    for func in cfg.hierarchical_calls:
        generator = c_generator.CGenerator()
        func_lines = generator.visit(cfg.nodes_table[func]).count("\n")
        total_lines += func_lines
        if func_lines < min_lines:
            min_lines = func_lines
        if func_lines > max_lines:
            max_lines = func_lines
        
        v = OperatorVisitor()
        v.visit(cfg.nodes_table[func])
        tot_operators += v.operators_count
        if v.operators_count < min_operators:
            min_operators = v.operators_count
        if v.operators_count > max_operators:
            max_operators = v.operators_count

    print(f"Tot functions:\t{len(cfg.hierarchical_calls)}")
    print(f"Tot calls:\t{sum([len(cfg.calls_table[func]) for func in cfg.hierarchical_calls])}")
    print(f"Total line count:\t {total_lines}")
    print(f"Min lines:\t{min_lines}")
    print(f"Max lines:\t{max_lines}")
    print(f"Total operators:\t{tot_operators}")
    print(f"Min operators:\t{min_operators}")
    print(f"Max operators:\t{max_operators}")



########################################################################################
#                               HIERARCHICAL PROCESSING                                #
########################################################################################
def hierarchical_processing(cfg):
    filename = "tmp/complete.c"
    with open(filename, "w") as f:
        f.write(cfg.includes)
        f.write(cfg.orig_code)
        f.write(cfg.test_code)

    ast = parse_file(filename, use_cpp=True,
                    cpp_args=r'-Iutils/fake_libc_include')
    v = HierarchyVisitor(cfg)
    v.visit(ast)
    
    
    # print(calls_table)
    explore_calls(cfg.top_function, cfg.hierarchical_calls, cfg) # inits hierarchical_calls

    # print("Hierarchical calls: ", cfg.hierarchical_calls)
    for func in cfg.hierarchical_calls:
        build_unit_test(func, filename, cfg)
        with open(f"tmp/{func}.c", "r") as f:
            cfg.orig_code = f.read()
        with open(f"tmp/{func}_test.c", "r") as f:
            cfg.test_code = f.read()
        cfg.top_function = func
        cfg.processed.append(C2HLSC(cfg))
    
    # write final file
    with open(f"tmp/{cfg.top_function}_final.c", "w") as f:
        # new file
        f.write(libs)
        f.write(cfg.includes)
        for proc in cfg.processed:
            with open(f"{proc}", "r") as p:
                f.write(p.read())
        f.write(cfg.test_code)



########################################################################################
#                                    LOG RESULTS                                       #
########################################################################################
def log_results(cfg):
    catapult_dirs = []
    for dir in os.listdir("."):
        if os.path.isdir(dir):
            if dir.startswith("Catapult_"):
                catapult_dirs.append(int(dir.split("_")[1]))

    log = f"Catapult_{max(catapult_dirs)}/{cfg.top_function}_hls.v1/rtl.rpt"

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
    with open(f"{cfg.out_folder}{cfg.top_function}.log", "w") as f:
        for model in models:
            if cfg.llm_runs[model] == 0:
                continue
            print(f"{model} runs: {cfg.llm_runs[model]}", file=f)
            print(f"{model} input tokens: {cfg.llm_input_tokens[model]}", file=f)
            print(f"{model} output tokens: {cfg.llm_output_tokens[model]}", file=f)
            if cfg.hierarchical_calls:
                print(f"# of functions: {len(cfg.hierarchical_calls)}", file=f)
        print(f"HLS runs: {cfg.hls_runs}", file=f)
        print(f"Compile runs: {cfg.compile_runs}", file=f)
        print("")    
        print("".join(saved_lines), file=f)
        
    # copy important files
    subprocess.run(["cp", "-r", f"Catapult_{max(catapult_dirs)}", f"{cfg.out_folder}Catapult_{cfg.top_function}"])
    subprocess.run(["cp", f"tmp/{cfg.top_function}_final.c", f"{cfg.out_folder}"])



########################################################################################
#                                       MAIN                                           #
########################################################################################
if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='C2HLSC script, yaml config required')

    parser.add_argument('config', type=str, help='yaml config file with the following fields: orig_code, test_code, includes, tcl, top_function')
    # model name optional argument from a list of models
    modes = ["standard", "streaming"] # from yaml file claude-3-5-sonnet-20240620
    parser.add_argument('--model', type=str, default="adaptive", choices=models, help='model name to use, default is adaptive')
    opt_target = ["area", "latency"]
    parser.add_argument('--opt_target', type=str, default="area", choices=opt_target, help='optimization target, default is area')
    parser.add_argument('--characterize', type=bool, default=False,  help='using this option will run the benchmark characterization')
    args = parser.parse_args()
    cfg = CFG(args)
    if cfg.characterize:
        characterize_benchmark()
        exit(0)

    if not cfg.hierarchical:
        C2HLSC(cfg)
    else:
        hierarchical_processing(cfg)
    
    print("DONE!")

    ## log results
    log_results(cfg)    