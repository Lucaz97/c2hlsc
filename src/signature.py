import re
with open("tmp_needwun/test.c", "r") as opt:
    #print(opt.readlines())
    for line in opt.readlines():
        pattern = r'^.*\s*\([^);]*\)(?:\s*\{\s*(//.*)?)?\n$'       
        if re.fullmatch(pattern, line):
            print(line.split("{")[0][:-1] + ";\n")
            
