# c2hlsc for Post-Quantum Cryptography (LLM4PQC / LLM4SecurePQC)

This repository contains the code and benchmarks for the LLM4PQC and LLM4SecurePQC works.
It extends [c2hlsc](https://github.com/Lucaz97/c2hlsc), an LLM-driven framework that automatically
refactors C code into synthesizable C for High-Level Synthesis (HLS), to post-quantum cryptography
kernels: Kyber and Dilithium NTTs, the Falcon FFT and samplers, and SHA3/SHAKE256.

## How it works

`src/c2hlsc.py` runs a feedback loop between an LLM, a C compiler and Catapult HLS:

1. **Refactoring.** The original code is synthesized with Catapult. If synthesis fails, the error
   (with a targeted hint for floating point, recursion, pointers, structs or `void *` casts) is sent
   to the LLM, which returns a `<top>_hls` function plus a `<top>` wrapper with the original signature.
2. **Functional check.** The LLM's code is compiled with `clang++` together with the test file, and
   its stdout is compared with the reference (original) code. Compile errors or output mismatches are
   fed back to the LLM (up to 10 attempts per synthesis round, 10 synthesis rounds).
3. **Synthesis check.** The functionally correct code is synthesized again with Catapult; any new
   error starts another round.

## Requirements

- **Catapult HLS** (`catapult` on `PATH`; runs were done with Catapult 2023.1)
- **clang / clang++**
- **Python 3.11** with:

      python3.11 -m pip install pycparser openai anthropic pyyaml

- An API key for the selected model, set as an environment variable:

  | Model family | Variable |
  |---|---|
  | OpenAI (`gpt-*`, `o3-mini`, `adaptive`) | `OPENAI_API_KEY` |

The AC datatype headers used for synthesis (`ac_int`, `ac_fixed`, `ac_float`, ...) are included in
`include/`, and pycparser's fake libc headers are in `utils/fake_libc_include/`.

## Usage

Run from the repository root:

    python3.11 src/c2hlsc.py inputs/<benchmark>/<config>.yaml --model <model> --opt_target <latency|throughput>

For example, the Kyber NTT with o3-mini:

    python3.11 src/c2hlsc.py inputs/ntt_kyber/config_ntt.yaml --model o3-mini --opt_target latency

| Option | Default | Description |
|---|---|---|
| `--model` | `o3-mini` | LLM to use; see `python3.11 src/c2hlsc.py -h` for the full list |
| `--opt_target` | `latency` | Optimization target: `latency` or `throughput` |
| `--characterize` | off | Only print benchmark statistics (functions, calls, lines, operators) |

`script.sh` shows how the repeated runs in the logs were collected (it runs the same benchmark
several times and saves each stdout as `output_<name>_iteration<i>.txt`).

### Outputs

- `tmp_<top_function>/`: intermediate files (LLM code, testbenches, compiled binaries)
- `outputs_<top_function>_<model>_<n>/`: per-run log with LLM calls, token counts, HLS and compile
  runs, plus the TCL used
- `Catapult_<n>/`: Catapult project directories (one per synthesis run, in the working directory)

## Input format

Each benchmark is described by a YAML file and three source files:

- **includes file**: includes, defines and global data that must not be refactored. The LLM is told
  this content is provided and must not be reproduced.
- **functions file** (`orig_code`): all and only the functions the LLM should refactor.
- **test file**: a `main` function with one or more tests that print results to stdout. The output
  of the refactored code must match the output of the original code.

```yaml
tcl: "inputs/directives.tcl"          # Catapult script template
includes: "inputs/ntt_kyber/ntt_inc.txt"
orig_code: "inputs/ntt_kyber/ntt.txt"
test_code: "inputs/ntt_kyber/ntt_test.c"
top_function: "ntt"
mode: "streaming"                     # optional, "standard" (default) or "streaming"
```

### Synthesis targets

| TCL file | Target |
|---|---|
| `inputs/directives.tcl` | Nangate 45nm ASIC library, Design Compiler (`DESIGN_GOAL area`, 20 ns clock) |
| `inputs/directives_vivado.tcl` | Xilinx Artix-7 (`xc7a12tcsg325-1`), Vivado |

To switch targets, change the `tcl` entry in the benchmark YAML.

## Benchmarks

### Post-quantum cryptography

| Directory | Kernel |
|---|---|
| `ntt_kyber` | Kyber (ML-KEM) forward NTT |
| `ntt_kyber_area` | Kyber NTT, area-oriented run |
| `ntt_kyber_var1` | Side-channel-hardened Kyber NTT: unified butterfly unit |
| `ntt_kyber_var2` | Side-channel-hardened Kyber NTT: dummy operations and jitter |
| `ntt_kyber_var3` | Side-channel-hardened Kyber NTT: bounded butterfly |
| `ntt_inv_kyber` | Kyber inverse NTT |
| `montgomery_reduce_kyber` | Kyber Montgomery reduction |
| `ntt_dilithium` | Dilithium (ML-DSA) NTT |
| `fft_falcon` | Falcon FFT |
| `sampler_falcon`, `sampler_falcon_preprocessed` | Falcon sampler (original and preprocessed) |
| `samplerz_falcon_non_hierarcy`, `samplerz_gaussian_falcon` | Falcon SamplerZ variants |
| `sha3`, `sha3_new` | SHA3 / SHAKE256 (Keccak) |

Most PQC directories also contain the unmodified reference implementation (`*_original.c`).

### Other benchmarks (from the original c2hlsc work)

AES (`aes`, `add_round_key`, `sub_bytes`, `shift_rows`, `mix_columns`), DES, PRESENT, ASCON,
SHA-256, NIST statistical tests (`monobit`, `block`, `cusums`, `runs`, `overlapping`), `kmp`,
`nw`, `quicksort` and `filter`.

## Preprocessor

`src/preprocessor/` contains LLM-based passes that prepare PQC reference code before running c2hlsc
(they use `o3-mini` through the OpenAI API):

- `preprocessor.py`: removes dynamic allocation (`malloc`/`free`) and structs, then fixes compile
  errors until the code compiles cleanly and its output matches the original. It reads
  `config.yaml` from the working directory with the keys `implementation_code`, `test_code` and
  `top_function`.
- `hls_init_function_c2hlsc.py` (with `core_utils.py`): finds a function that only initializes
  global tables, runs it once, and replaces it with the precomputed constants. **It overwrites the
  benchmark's includes and code files in place**, so run it on a copy.

Set your OpenAI key in the `OpenAI(...)` client in these files before running them.

## Results and logs

- `output_*.txt` (repository root): stdout of the PQC runs (NTT, FFT, sampler, SHA3, NTT variants),
  named by benchmark, synthesis target (`dc`/`vivado`), optimization goal and iteration.
- `keep_outputs*/`, `keep_runs*/`, `results/`, `res_same_ckp*/`: results of the original c2hlsc
  experiments, with the `parse_res.py` scripts and plots used to summarize them.

## Repository layout

```
src/c2hlsc.py        main framework
src/prompts.py       system prompts and error-specific hints
src/preprocessor/    LLM preprocessing passes for PQC code
inputs/              benchmarks, YAML configs and Catapult TCL templates
include/             AC datatype headers
utils/               pycparser fake libc headers and utilities
script.sh            helper for repeated runs
rename.py            renumbers output folders (python3 rename.py <dir> <offset>)
```
