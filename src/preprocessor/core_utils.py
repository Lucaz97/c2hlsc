# code_utils.py
from openai import OpenAI
import subprocess
from pathlib import Path

client = OpenAI(api_key="YOUR_API_KEY_HERE")

def detect_one_init_function_with_llm(full_code: str):
    prompt = f"""
You are an expert in C code analysis for HLS.
Given the C code below (headers + source), identify the one function that:
- Initializes global or static data like arrays with precomputed values
- Is only called once and has no runtime-dependent input

Output exactly two things:
1. The function name (on the first line)
2. The full function definition (starting from the line with the return type, ending at the closing brace)

------------------
{full_code}
------------------
"""
    response = client.chat.completions.create(
        model="o3-mini",
        messages=[{"role": "user", "content": prompt}]
    )

    content = response.choices[0].message.content
    lines = content.splitlines()
    func_name = lines[0].strip()
    func_code = "\n".join(lines[1:]).strip()
    return func_name, func_code

def generate_driver_from_func_with_llm(full_code: str, init_func: str) -> str:
    prompt = f"""
You are a C code generator.
Given the C code below and the name of an initialization function `{init_func}`:
Generate a standalone C program that:
- Includes the initialization function
- Runs it in main()
- Prints the initialized global/static array in the same type and size format as defined in the code
- Use printf to dump the values as a C declaration

------------------
{full_code}
------------------
"""
    response = client.chat.completions.create(
        model="o3-mini",
        messages=[{"role": "user", "content": prompt}]
    )
    return response.choices[0].message.content

def remove_init_func_and_call_with_llm(code: str, func_name: str) -> str:
    prompt = f"""
You are a C code refactoring assistant.
Given the C code below:
- Remove the full definition of the function `{func_name}`
- Remove all calls to `{func_name}()`
- Preserve the validity and structure of the code

Return only the modified C code.

------------------
{code}
------------------
"""
    response = client.chat.completions.create(
        model="o3-mini",
        messages=[{"role": "user", "content": prompt}]
    )
    return response.choices[0].message.content

def compile_and_run_main(c_path: str, out_path: str):
    exe = c_path.replace(".c", "")
    subprocess.run(["gcc", c_path, "-o", exe, "-lm"], check=True)
    with open(out_path, "w") as f:
        subprocess.run([f"./{exe}"], stdout=f, check=True)


# config.yaml
# includes: "fft_inc.txt"
# orig_code: "fft_original.c"
# test_code: "fft_test.txt"
