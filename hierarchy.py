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



def build_unit_test(func):
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
                    pointers_table[params_table[func][i][1]] = (1, len(value)) # array dimensions
                    value = value.replace("{", "[").replace("}", "]")
                    value = eval(value)
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
            main_def.body.block_items.append(c_ast.FuncCall(c_ast.ID("printf"), c_ast.ExprList([c_ast.Constant(c_ast.IdentifierType(['char']), f'"{params_table[func][i][1]}: %d\\n"'), c_ast.ID(params_table[func][i][1])])))

    generator = c_generator.CGenerator()
    with open("tmp/" + func + ".c", "w") as f:
        print(generator.visit( nodes_table[func]), file=f)
        print(generator.visit(main_def), file=f)

class FuncDefVisitor(c_ast.NodeVisitor):
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

def explore_calls(top):
    for func in calls_table[top]:
        if calls_table[func]:
            explore_calls(func)
        build_unit_test(func)

ast = parse_file(filename, use_cpp=True,
                    cpp_args=r'-Iutils/fake_libc_include')

v = FuncDefVisitor()
v.visit(ast)
#print(ast)
#exit(0)
#print(calls_table)
#print(params_table)
explore_calls(top)

