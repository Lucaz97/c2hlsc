import os

top_function = "Cipher"
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

print("".join(saved_lines))