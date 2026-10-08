# hls_init_pipeline.py
# Entry point for detecting and replacing a single init function with precomputed constants

import yaml
from pathlib import Path
import subprocess
from core_utils import *

def main():
    with open("inputs/falcon_fft/config_fft.yaml", "r") as f:
        cfg = yaml.safe_load(f)

    inc_path = Path(cfg["includes"])
    orig_path = Path(cfg["orig_code"])

    inc_code = inc_path.read_text()
    orig_code = orig_path.read_text()
    combined_code = inc_code + "\n\n" + orig_code

    # Step 1: Detect init function and extract definition
    print("[+] Querying LLM for the initialization function...")
    func_name, func_code = detect_one_init_function_with_llm(combined_code)
    print(f"[+] Detected function: {func_name}")

    # Step 2: Generate driver to run the function and print initialized array
    print("[+] Generating driver using LLM...")
    driver_code = generate_driver_from_func_with_llm(combined_code, func_name)

    runner_path = f"{func_name}_runner.c"
    output_txt = f"{func_name}_gen.txt"

    with open(runner_path, "w") as f:
        f.write(driver_code)

    compile_and_run_main(runner_path, output_txt)

    # Step 3: Remove function definition and its call from original code
    updated_code = remove_init_func_and_call_with_llm(orig_code, func_name)
    orig_path.write_text(updated_code)

    updated_code = remove_init_func_and_call_with_llm(inc_code, func_name)
    inc_path.write_text(updated_code)

    # Step 4: Append constants to header file
    new_values = Path(output_txt).read_text()
    with open(inc_path, "a") as f:
        f.write("\n// === Auto-generated constants ===\n")
        f.write(new_values)

    

if __name__ == "__main__":
    main()