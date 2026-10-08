import subprocess
import sys
from openai import OpenAI
from pathlib import Path

import yaml  # requires PyYAML

CONFIG_FILE = "config.yaml"

COMBINED_INITIAL_FILE = Path("combined_initial.c")
OUTPUT_BINARY_INITIAL = "output_binary_initial"

LLM_FIXED_FILE = Path("llm_fixed.c")

COMBINED_FIXED_FILE = Path("combined_fixed.c")
OUTPUT_BINARY_FIXED = "output_binary_fixed"

client = OpenAI(api_key="YOUR_API_KEY_HERE")

def load_config(path):
    try:
        with open(path, "r") as f:
            return yaml.safe_load(f)
    except FileNotFoundError:
        print(f"Error: config file '{path}' not found")
        sys.exit(1)
    except yaml.YAMLError as e:
        print(f"Error parsing YAML: {e}")
        sys.exit(1)

def concatenate_files(file_list, output_path):
    try:
        with output_path.open("w") as out:
            for i, file_path in enumerate(file_list):
                out.write(Path(file_path).read_text())
                if i < len(file_list) - 1:
                    out.write("\n\n")
    except FileNotFoundError as e:
        print(f"Error: {e}")
        sys.exit(1)

def run_gcc(source_file, output_binary):
    gcc_cmd = [
        "gcc",
        "-Wall",
        "-Wextra",
        "-Wpedantic",
        source_file,
        "-o",
        output_binary,
    ]

    result = subprocess.run(gcc_cmd, text=True, capture_output=True)

    if result.stdout:
        print(result.stdout)
    if result.stderr:
        print(result.stderr)

    return result

def run_binary(binary_path):
    result = subprocess.run([str(Path(binary_path).resolve())], text=True, capture_output=True)
    return result

def compile_and_run(source_file, output_binary):
    gcc_result = run_gcc(source_file, output_binary)
    if gcc_result.returncode != 0:
        print("Compilation failed. Binary will not be executed.")
        return gcc_result.returncode

    run_result = run_binary(output_binary)
    if run_result.returncode != 0:
        print("Binary execution failed.")
        return run_result.returncode
    
    return 0

def read_file_contents(path):
    """
    Reads a text file and returns its contents as a string.
    """
    try:
        return Path(path).read_text()
    except FileNotFoundError:
        raise FileNotFoundError(f"File not found: {path}")
    
def write_file_contents(path, content):
    """
    Writes content to a file, overwriting it if it exists.
    """
    Path(path).write_text(content)

def call_llm(prompt):
    response = client.chat.completions.create(
        model="o3-mini",
        messages=[{"role": "user", "content": prompt}]
    )
    return response.choices[0].message.content

def fix_gcc_issues(code, gcc_errors):
    prompt = f"""
            You are an expert in C code generation.
            The following C code produces the given GCC errors/warnings when compiled. 
            Please fix the code to eliminate all errors and warnings. Do not provide main functions or testbenches. Return only the modified C code. I dont want any code explanations.
            GCC Errors/Warnings:
            ```
            {gcc_errors}
            ```
            C Code:
            ```
            {code}
            ```
            """

    return call_llm(prompt)

def check_funtionality(initial_binary, fixed_binary):
    """this function checks the functionality of the initial and fixed binaries"""
    print("Checking functionality of the fixed binary against the initial binary...")

    initial_result = run_binary(initial_binary)
    fixed_result = run_binary(fixed_binary)

    if initial_result.stdout != fixed_result.stdout:
        print("Warning: Output of the fixed binary differs from the initial binary.")
        return False
    else:
        print("Functionality check passed: Outputs are identical.")
        return True

def fix_malloc_issues(code):
    prompt = f"""
            You are an expert in C code generation.
            The following C code contains dynamic memory allocation using malloc.
            Rewrite the code to eliminate all uses of malloc, using only static or stack allocation.
            If there are any memory frees or functions that call memory frees, remove them as well.
            Do not provide main functions or testbenches. Return only the modified C code. I dont want any code explanations.
            C Code:
            ```
            {code}
            ```
            """

    return call_llm(prompt)

def fix_struct_issues(code, top_function):
    prompt = f"""
            You are an expert in HLS C code generation.
            The following C code contains struct definitions.
            I want to make this code more suitable for HLS synthesis. {top_function} is the top function for synthesis.
            Rewrite the code to eliminate all uses of structs, using only primitive data types and arrays. Do not change the {top_function} function interface.
            Do not provide main function or testbenches. Return only the modified C code. I dont want any code explanations.
            ```
            {code}
            ```
            """

    return call_llm(prompt)

def extract_structs(code):
    prompt = f"""
            You are an expert in C code generator.
            The following C code contains struct definitions. I want you to extract only the struct definitions in the given order. I dont want any code explanations.
            ```
            {code}
            ```
            """

    return call_llm(prompt)


def main():
    config = load_config(CONFIG_FILE)

    implementation = config.get("implementation_code")
    test = config.get("test_code")
    top_function = config.get("top_function")

    if not implementation or not test:
        print("Error: config.yaml must define 'implementation_code' and 'test_code'", flush=True)
        sys.exit(1)

    # concatanate the implementation and test code
    concatenate_files([implementation, test], COMBINED_INITIAL_FILE)

    # check if the provided C code compiles and runs
    compile_and_run_exit = compile_and_run(str(COMBINED_INITIAL_FILE), OUTPUT_BINARY_INITIAL)
    if compile_and_run_exit != 0:
        print("Provided code failed to compile or run.", flush=True)
        sys.exit(compile_and_run_exit)  
    
    # read the combined code
    code = read_file_contents(COMBINED_INITIAL_FILE)


    print("**********************************************", flush=True)

    error_found = True

    while error_found:
        error_found = False
        struct_found = False

        if "malloc" in code:
            print("Warning: malloc detected in the code.", flush=True)
            fixed_code = fix_malloc_issues(code)

            # print(fixed_code)
            code = fixed_code
            write_file_contents(LLM_FIXED_FILE, code)
            error_found = True

        elif "struct" in code:
            print("Warning: struct detected in the code.", flush=True)
            struct_found = True

            structs = extract_structs(code)
            # print("Extracted Structs:")
            # print(structs)
            write_file_contents(Path("extracted_structs.c"), structs)

            fixed_code = fix_struct_issues(code, top_function)

            # print(fixed_code)
            code = fixed_code
            write_file_contents(LLM_FIXED_FILE, code)

            error_found = True

        print("**********************************************", flush=True)

        if struct_found:
            concatenate_files([LLM_FIXED_FILE, "extracted_structs.c", test], COMBINED_FIXED_FILE)
        else:
            concatenate_files([LLM_FIXED_FILE, test], COMBINED_FIXED_FILE)

        # try compile the code
        gcc_result = run_gcc(str(COMBINED_FIXED_FILE), OUTPUT_BINARY_FIXED)
        if gcc_result.returncode == 0:
            print("Code compiled successfully.", flush=True)
        else:
            print("Code compilation failed.", flush=True)
            fixed_code = fix_gcc_issues(code, gcc_result.stderr)
            # print(fixed_code)
            code = fixed_code
            write_file_contents(LLM_FIXED_FILE, code)

        if error_found == False and gcc_result.returncode == 0:
            if check_funtionality(OUTPUT_BINARY_INITIAL,OUTPUT_BINARY_FIXED):
                print("Fixed code is functionaly correct.", flush=True)
                break
            else:
                print("Fixed code is NOT functionaly correct. Continuing fixing process.", flush=True)


        print("**********************************************", flush=True)

        error_found = True

    print("Done!")
    print("**********************************************")
    return 0
        

if __name__ == "__main__":
    main()
