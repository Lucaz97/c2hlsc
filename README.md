# c2hlsc: A framework for automatically refactoring C code into synthesizable C code

### Requirements
The current version of the framework uses Catapult HLS. 
The framework uses gcc, g++ and gdb.

Python dependencies:

    python3.11 -m pip install pycparser openai anthropic pyyaml

An API key for the model in use is needed.
It can be set as an environment variable (ANTHROPIC_API_KEY or OPENAI_API_KEY)

### Input Structure
The C/C++ code should be split into 3 files:

 - includes file: should contain the include libraries  and global variables that should not be refactored. We explicitly prompr the LLM to assume that the content of this file will be provided and should not be produced by the LLM.
 - functions file: should contain all and only  the functions that the LLM should refactor.
 - test file: should contain a main function with one or more tests, the results should be printed to stdout.

One yaml file contains information about top function, file paths, type of interface (streaming or nor) and whether the code is hierarchical or not. 

###  Using c2hlsc:
To run one of our benchmarks:

    python3.11 src/run.py inputs/${benchmark}/config_${benchmark}.yaml --model <model name> --opt_target <area/latency>

To get a list of the possible models and other options: 

    python3.11 src/run.py -h 

