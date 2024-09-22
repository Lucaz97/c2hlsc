from pycparser import c_ast, parse_file, c_generator, c_parser

class SigVisitor(c_ast.NodeVisitor):
    def visit_FuncDecl(self, node):
        #print('%s at %s' % (node.decl.name, node.decl.coord))
        self.node = node
       
filename = "tmp/AddRoundKey_final.c"
with open(filename) as file:
    code = file.read()
    # find line that contains the function signature
    for line in code.splitlines():
        if filename.split("/")[1].split("_final")[0] in line:
            print(line+";")
            break 