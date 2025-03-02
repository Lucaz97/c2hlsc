import os
import pprint

benchmarks = ["Cipher", "sha256_update", "present80_encryptBlock", "kmp", "filter", "needwun"]
model = "deepseek_chat"

class Benchmark:
    def __init__(self, name):

        self.name = name

        ###############################
        ### STATS FOR DEEPSEEK CHAT ###
        ###############################
        
        # SUCCESSFUL RUNS
        self.deepseek_chat_success = 0

        # MODEL RUNS
        self.deepseek_chat_runs = 0
        self.min_deepseek_chat_runs = 999999
        self.max_deepseek_chat_runs = 0

        # INPUT TOKENS
        self.deepseek_chat_input_tokens = 0
        self.min_deepseek_chat_input_tokens = 999999
        self.max_deepseek_chat_input_tokens = 0

        # OUTPUT TOKENS
        self.deepseek_chat_output_tokens = 0
        self.min_deepseek_chat_output_tokens = 999999
        self.max_deepseek_chat_output_tokens = 0

        # HLS RUNS
        self.deepseek_chat_hls_runs = 0
        self.min_deepseek_chat_hls_runs = 999999
        self.max_deepseek_chat_hls_runs = 0

        # COMPILE RUNS
        self.deepseek_chat_compile_runs = 0
        self.min_deepseek_chat_compile_runs = 999999
        self.max_deepseek_chat_compile_runs = 0

        # TIME FOR AGENT
        self.deepseek_chat_agent_time = 0
        self.min_deepseek_chat_agent_time = 999999
        self.max_deepseek_chat_agent_time = 0

        # AGENT SYNTHESIS CALLS
        self.deepseek_chat_synthesis_calls = 0
        self.min_deepseek_chat_synthesis_calls = 999999
        self.max_deepseek_chat_synthesis_calls = 0

        # AGENT PYTHON CALLS
        self.deepseek_chat_python_calls = 0
        self.min_deepseek_chat_python_calls = 999999
        self.max_deepseek_chat_python_calls = 0

        # AGENT PROFILE CALLS
        self.deepseek_chat_profile_calls = 0
        self.min_deepseek_chat_profile_calls = 999999
        self.max_deepseek_chat_profile_calls = 0

        # AGENT INSPECT CALLS
        self.deepseek_chat_inspect_calls = 0
        self.min_deepseek_chat_inspect_calls = 999999
        self.max_deepseek_chat_inspect_calls = 0

        # API DOWNTIME
        self.deepseek_chat_api_downtime = 0
        self.min_deepseek_chat_api_downtime = 999999
        self.max_deepseek_chat_api_downtime = 0

        # DESIGN LATENCY
        self.deepseek_chat_latency = 0
        self.min_deepseek_chat_latency = 999999
        self.max_deepseek_chat_latency = 0

        # DESIGN AREA
        self.deepseek_chat_area = 0
        self.min_deepseek_chat_area = 999999
        self.max_deepseek_chat_area = 0


        ###################################
        ### STATS FOR DEEPSEEK REASONER ###
        ###################################
        
        # SUCCESSFUL RUNS
        self.deepseek_reasoner_success = 0

        # MODEL RUNS
        self.deepseek_reasoner_runs = 0
        self.min_deepseek_reasoner_runs = 999999
        self.max_deepseek_reasoner_runs = 0

        # INPUT TOKENS
        self.deepseek_reasoner_input_tokens = 0
        self.min_deepseek_reasoner_input_tokens = 999999
        self.max_deepseek_reasoner_input_tokens = 0

        # OUTPUT TOKENS
        self.deepseek_reasoner_output_tokens = 0
        self.min_deepseek_reasoner_output_tokens = 999999
        self.max_deepseek_reasoner_output_tokens = 0

        # HLS RUNS
        self.deepseek_reasoner_hls_runs = 0
        self.min_deepseek_reasoner_hls_runs = 999999
        self.max_deepseek_reasoner_hls_runs = 0

        # COMPILE RUNS
        self.deepseek_reasoner_compile_runs = 0
        self.min_deepseek_reasoner_compile_runs = 999999
        self.max_deepseek_reasoner_compile_runs = 0

        # TIME FOR AGENT
        self.deepseek_reasoner_agent_time = 0
        self.min_deepseek_reasoner_agent_time = 999999
        self.max_deepseek_reasoner_agent_time = 0

        # AGENT SYNTHESIS CALLS
        self.deepseek_reasoner_synthesis_calls = 0
        self.min_deepseek_reasoner_synthesis_calls = 999999
        self.max_deepseek_reasoner_synthesis_calls = 0

        # AGENT PYTHON CALLS
        self.deepseek_reasoner_python_calls = 0
        self.min_deepseek_reasoner_python_calls = 999999
        self.max_deepseek_reasoner_python_calls = 0

        # AGENT PROFILE CALLS
        self.deepseek_reasoner_profile_calls = 0
        self.min_deepseek_reasoner_profile_calls = 999999
        self.max_deepseek_reasoner_profile_calls = 0

        # AGENT INSPECT CALLS
        self.deepseek_reasoner_inspect_calls = 0
        self.min_deepseek_reasoner_inspect_calls = 999999
        self.max_deepseek_reasoner_inspect_calls = 0

        # API DOWNTIME
        self.deepseek_reasoner_api_downtime = 0
        self.min_deepseek_reasoner_api_downtime = 999999
        self.max_deepseek_reasoner_api_downtime = 0

        # DESIGN LATENCY
        self.deepseek_reasoner_latency = 0
        self.min_deepseek_reasoner_latency = 999999
        self.max_deepseek_reasoner_latency = 0

        # DESIGN AREA
        self.deepseek_reasoner_area = 0
        self.min_deepseek_reasoner_area = 999999
        self.max_deepseek_reasoner_area = 0


        #########################
        ### STATS FOR O3-MINI ###
        #########################
        
        # SUCCESSFUL RUNS
        self.o3_mini_success = 0

        # MODEL RUNS
        self.o3_mini_runs = 0
        self.min_o3_mini_runs = 999999
        self.max_o3_mini_runs = 0

        # INPUT TOKENS
        self.o3_mini_input_tokens = 0
        self.min_o3_mini_input_tokens = 999999
        self.max_o3_mini_input_tokens = 0

        # OUTPUT TOKENS
        self.o3_mini_output_tokens = 0
        self.min_o3_mini_output_tokens = 999999
        self.max_o3_mini_output_tokens = 0

        # HLS RUNS
        self.o3_mini_hls_runs = 0
        self.min_o3_mini_hls_runs = 999999
        self.max_o3_mini_hls_runs = 0

        # COMPILE RUNS
        self.o3_mini_compile_runs = 0
        self.min_o3_mini_compile_runs = 999999
        self.max_o3_mini_compile_runs = 0

        # TIME FOR AGENT
        self.o3_mini_agent_time = 0
        self.min_o3_mini_agent_time = 999999
        self.max_o3_mini_agent_time = 0

        # AGENT SYNTHESIS CALLS
        self.o3_mini_synthesis_calls = 0
        self.min_o3_mini_synthesis_calls = 999999
        self.max_o3_mini_synthesis_calls = 0

        # AGENT PYTHON CALLS
        self.o3_mini_python_calls = 0
        self.min_o3_mini_python_calls = 999999
        self.max_o3_mini_python_calls = 0

        # AGENT PROFILE CALLS
        self.o3_mini_profile_calls = 0
        self.min_o3_mini_profile_calls = 999999
        self.max_o3_mini_profile_calls = 0

        # AGENT INSPECT CALLS
        self.o3_mini_inspect_calls = 0
        self.min_o3_mini_inspect_calls = 999999
        self.max_o3_mini_inspect_calls = 0

        # API DOWNTIME
        self.o3_mini_api_downtime = 0
        self.min_o3_mini_api_downtime = 999999
        self.max_o3_mini_api_downtime = 0

        # DESIGN LATENCY
        self.o3_mini_latency = 0
        self.min_o3_mini_latency = 999999
        self.max_o3_mini_latency = 0

        # DESIGN AREA
        self.o3_mini_area = 0
        self.min_o3_mini_area = 999999
        self.max_o3_mini_area = 0


# PARSE LOG FILES
folders = [folder for folder in os.listdir(".") if (os.path.isdir(folder) and "outputs_" in folder)]

benchmark_dict = {benchmark: Benchmark(benchmark) for benchmark in benchmarks}

for folder in folders:
    for benchmark in benchmarks:
        if benchmark in folder:
            
            log_name = folder + f"/{benchmark}.log"

            c_file = folder + f"/{benchmark}_result.c"
            if not os.path.exists(c_file):
                continue

            if os.path.exists(log_name):
                with open(log_name, "r") as log_file:
                    for line in log_file:

                        if "deepseek-chat runs:" in line:
                            runs = int(line.split(":")[1].strip())
                            benchmark_dict[benchmark].deepseek_chat_runs += runs
                            benchmark_dict[benchmark].min_deepseek_chat_runs = min(benchmark_dict[benchmark].min_deepseek_chat_runs, runs)
                            benchmark_dict[benchmark].max_deepseek_chat_runs = max(benchmark_dict[benchmark].max_deepseek_chat_runs, runs)

                        if "deepseek-chat input tokens:" in line:
                            input_tokens = int(line.split(":")[1].strip())
                            benchmark_dict[benchmark].deepseek_chat_input_tokens += input_tokens
                            benchmark_dict[benchmark].min_deepseek_chat_input_tokens = min(benchmark_dict[benchmark].min_deepseek_chat_input_tokens, input_tokens)
                            benchmark_dict[benchmark].max_deepseek_chat_input_tokens = max(benchmark_dict[benchmark].max_deepseek_chat_input_tokens, input_tokens)
                            # print(f"{folder}: {input_tokens}, {benchmark_dict[benchmark].deepseek_chat_input_tokens}, {benchmark_dict[benchmark].min_deepseek_chat_input_tokens}, {benchmark_dict[benchmark].max_deepseek_chat_input_tokens}")

                        if "deepseek-chat output tokens:" in line:
                            output_tokens = int(line.split(":")[1].strip())
                            benchmark_dict[benchmark].deepseek_chat_output_tokens += output_tokens
                            benchmark_dict[benchmark].min_deepseek_chat_output_tokens = min(benchmark_dict[benchmark].min_deepseek_chat_output_tokens, output_tokens)
                            benchmark_dict[benchmark].max_deepseek_chat_output_tokens = max(benchmark_dict[benchmark].max_deepseek_chat_output_tokens, output_tokens)
                            # print(f"{folder}: {output_tokens}, {benchmark_dict[benchmark].deepseek_chat_output_tokens}, {benchmark_dict[benchmark].min_deepseek_chat_output_tokens}, {benchmark_dict[benchmark].max_deepseek_chat_output_tokens}")

                        # if "# of functions:" in line:
                        #     num_functions = int(line.split(":")[1].strip())
                        #     print(num_functions)


                        if "deepseek-ai/DeepSeek-R1 runs:" in line:
                            runs = int(line.split(":")[1].strip())
                            benchmark_dict[benchmark].deepseek_reasoner_runs += runs
                            benchmark_dict[benchmark].min_deepseek_reasoner_runs = min(benchmark_dict[benchmark].min_deepseek_reasoner_runs, runs)
                            benchmark_dict[benchmark].max_deepseek_reasoner_runs = max(benchmark_dict[benchmark].max_deepseek_reasoner_runs, runs)

                        if "deepseek-ai/DeepSeek-R1 input tokens:" in line:
                            input_tokens = int(line.split(":")[1].strip())
                            benchmark_dict[benchmark].deepseek_reasoner_input_tokens += input_tokens
                            benchmark_dict[benchmark].min_deepseek_reasoner_input_tokens = min(benchmark_dict[benchmark].min_deepseek_reasoner_input_tokens, input_tokens)
                            benchmark_dict[benchmark].max_deepseek_reasoner_input_tokens = max(benchmark_dict[benchmark].max_deepseek_reasoner_input_tokens, input_tokens)
                            # print(f"{folder}: {input_tokens}, {benchmark_dict[benchmark].deepseek_reasoner_input_tokens}, {benchmark_dict[benchmark].min_deepseek_reasoner_input_tokens}, {benchmark_dict[benchmark].max_deepseek_reasoner_input_tokens}")

                        if "deepseek-ai/DeepSeek-R1 output tokens:" in line:
                            output_tokens = int(line.split(":")[1].strip())
                            benchmark_dict[benchmark].deepseek_reasoner_output_tokens += output_tokens
                            benchmark_dict[benchmark].min_deepseek_reasoner_output_tokens = min(benchmark_dict[benchmark].min_deepseek_reasoner_output_tokens, output_tokens)
                            benchmark_dict[benchmark].max_deepseek_reasoner_output_tokens = max(benchmark_dict[benchmark].max_deepseek_reasoner_output_tokens, output_tokens)
                            # print(f"{folder}: {output_tokens}, {benchmark_dict[benchmark].deepseek_reasoner_output_tokens}, {benchmark_dict[benchmark].min_deepseek_reasoner_output_tokens}, {benchmark_dict[benchmark].max_deepseek_reasoner_output_tokens}")


                        if "o3-mini runs:" in line:
                            runs = int(line.split(":")[1].strip())
                            benchmark_dict[benchmark].o3_mini_runs += runs
                            benchmark_dict[benchmark].min_o3_mini_runs = min(benchmark_dict[benchmark].min_o3_mini_runs, runs)
                            benchmark_dict[benchmark].max_o3_mini_runs = max(benchmark_dict[benchmark].max_o3_mini_runs, runs)

                        if "o3-mini input tokens:" in line:
                            input_tokens = int(line.split(":")[1].strip())
                            benchmark_dict[benchmark].o3_mini_input_tokens += input_tokens
                            benchmark_dict[benchmark].min_o3_mini_input_tokens = min(benchmark_dict[benchmark].min_o3_mini_input_tokens, input_tokens)
                            benchmark_dict[benchmark].max_o3_mini_input_tokens = max(benchmark_dict[benchmark].max_o3_mini_input_tokens, input_tokens)
                            # print(f"{folder}: {input_tokens}, {benchmark_dict[benchmark].o3_mini_input_tokens}, {benchmark_dict[benchmark].min_o3_mini_input_tokens}, {benchmark_dict[benchmark].max_o3_mini_input_tokens}")

                        if "o3-mini output tokens:" in line:
                            output_tokens = int(line.split(":")[1].strip())
                            benchmark_dict[benchmark].o3_mini_output_tokens += output_tokens
                            benchmark_dict[benchmark].min_o3_mini_output_tokens = min(benchmark_dict[benchmark].min_o3_mini_output_tokens, output_tokens)
                            benchmark_dict[benchmark].max_o3_mini_output_tokens = max(benchmark_dict[benchmark].max_o3_mini_output_tokens, output_tokens)
                            # print(f"{folder}: {output_tokens}, {benchmark_dict[benchmark].o3_mini_output_tokens}, {benchmark_dict[benchmark].min_o3_mini_output_tokens}, {benchmark_dict[benchmark].max_o3_mini_output_tokens}")


                        if "HLS runs:" in line:
                            runs = int(line.split(":")[1].strip())

                            if "deepseek-chat" in folder:
                                benchmark_dict[benchmark].deepseek_chat_hls_runs += runs
                                benchmark_dict[benchmark].min_deepseek_chat_hls_runs = min(benchmark_dict[benchmark].min_deepseek_chat_hls_runs, runs)
                                benchmark_dict[benchmark].max_deepseek_chat_hls_runs = max(benchmark_dict[benchmark].max_deepseek_chat_hls_runs, runs)
                                # print(f"{folder}: {runs}, {benchmark_dict[benchmark].deepseek_chat_hls_runs}, {benchmark_dict[benchmark].min_deepseek_chat_hls_runs}, {benchmark_dict[benchmark].max_deepseek_chat_hls_runs}")
                            
                            if "hyperbolic-reasoner" in folder:
                                benchmark_dict[benchmark].deepseek_reasoner_hls_runs += runs
                                benchmark_dict[benchmark].min_deepseek_reasoner_hls_runs = min(benchmark_dict[benchmark].min_deepseek_reasoner_hls_runs, runs)
                                benchmark_dict[benchmark].max_deepseek_reasoner_hls_runs = max(benchmark_dict[benchmark].max_deepseek_reasoner_hls_runs, runs)
                                # print(f"{folder}: {runs}, {benchmark_dict[benchmark].deepseek_reasoner_hls_runs}, {benchmark_dict[benchmark].min_deepseek_reasoner_hls_runs}, {benchmark_dict[benchmark].max_deepseek_reasoner_hls_runs}")

                            if "o3-mini" in folder:
                                benchmark_dict[benchmark].o3_mini_hls_runs += runs
                                benchmark_dict[benchmark].min_o3_mini_hls_runs = min(benchmark_dict[benchmark].min_o3_mini_hls_runs, runs)
                                benchmark_dict[benchmark].max_o3_mini_hls_runs = max(benchmark_dict[benchmark].max_o3_mini_hls_runs, runs)
                                # print(f"{folder}: {runs}, {benchmark_dict[benchmark].o3_mini_hls_runs}, {benchmark_dict[benchmark].min_o3_mini_hls_runs}, {benchmark_dict[benchmark].max_o3_mini_hls_runs}")


                        if "Compile runs:" in line:
                            runs = int(line.split(":")[1].strip())
                            
                            if "deepseek-chat" in folder:
                                benchmark_dict[benchmark].deepseek_chat_compile_runs += runs
                                benchmark_dict[benchmark].min_deepseek_chat_compile_runs = min(benchmark_dict[benchmark].min_deepseek_chat_compile_runs, runs)
                                benchmark_dict[benchmark].max_deepseek_chat_compile_runs = max(benchmark_dict[benchmark].max_deepseek_chat_compile_runs, runs)
                                # print(f"{folder}: {runs}, {benchmark_dict[benchmark].deepseek_chat_compile_runs}, {benchmark_dict[benchmark].min_deepseek_chat_compile_runs}, {benchmark_dict[benchmark].max_deepseek_chat_compile_runs}")

                            if "hyperbolic-reasoner" in folder:
                                benchmark_dict[benchmark].deepseek_reasoner_compile_runs += runs
                                benchmark_dict[benchmark].min_deepseek_reasoner_compile_runs = min(benchmark_dict[benchmark].min_deepseek_reasoner_compile_runs, runs)
                                benchmark_dict[benchmark].max_deepseek_reasoner_compile_runs = max(benchmark_dict[benchmark].max_deepseek_reasoner_compile_runs, runs)
                                # print(f"{folder}: {runs}, {benchmark_dict[benchmark].deepseek_reasoner_compile_runs}, {benchmark_dict[benchmark].min_deepseek_reasoner_compile_runs}, {benchmark_dict[benchmark].max_deepseek_reasoner_compile_runs}")
                            
                            if "o3-mini" in folder:
                                benchmark_dict[benchmark].o3_mini_compile_runs += runs
                                benchmark_dict[benchmark].min_o3_mini_compile_runs = min(benchmark_dict[benchmark].min_o3_mini_compile_runs, runs)
                                benchmark_dict[benchmark].max_o3_mini_compile_runs = max(benchmark_dict[benchmark].max_o3_mini_compile_runs, runs)
                                # print(f"{folder}: {runs}, {benchmark_dict[benchmark].o3_mini_compile_runs}, {benchmark_dict[benchmark].min_o3_mini_compile_runs}, {benchmark_dict[benchmark].max_o3_mini_compile_runs}")

                        # if "Time for c2hlsc:" in line:
                        #     time = int(line.split(":")[1].strip())
                        #     # print(time)


                        if "Time for agent:" in line:
                            time = int(line.split(":")[1].strip())
                            
                            if "deepseek-chat" in folder:
                                benchmark_dict[benchmark].deepseek_chat_agent_time += time
                                benchmark_dict[benchmark].min_deepseek_chat_agent_time = min(benchmark_dict[benchmark].min_deepseek_chat_agent_time, time)
                                benchmark_dict[benchmark].max_deepseek_chat_agent_time = max(benchmark_dict[benchmark].max_deepseek_chat_agent_time, time)
                                # print(f"{folder}: {time}, {benchmark_dict[benchmark].deepseek_chat_agent_time}, {benchmark_dict[benchmark].min_deepseek_chat_agent_time}, {benchmark_dict[benchmark].max_deepseek_chat_agent_time}")

                            if "hyperbolic-reasoner" in folder:
                                benchmark_dict[benchmark].deepseek_reasoner_agent_time += time
                                benchmark_dict[benchmark].min_deepseek_reasoner_agent_time = min(benchmark_dict[benchmark].min_deepseek_reasoner_agent_time, time)
                                benchmark_dict[benchmark].max_deepseek_reasoner_agent_time = max(benchmark_dict[benchmark].max_deepseek_reasoner_agent_time, time)
                                # print(f"{folder}: {time}, {benchmark_dict[benchmark].deepseek_reasoner_agent_time}, {benchmark_dict[benchmark].min_deepseek_reasoner_agent_time}, {benchmark_dict[benchmark].max_deepseek_reasoner_agent_time}")

                            if "o3-mini" in folder:
                                benchmark_dict[benchmark].o3_mini_agent_time += time
                                benchmark_dict[benchmark].min_o3_mini_agent_time = min(benchmark_dict[benchmark].min_o3_mini_agent_time, time)
                                benchmark_dict[benchmark].max_o3_mini_agent_time = max(benchmark_dict[benchmark].max_o3_mini_agent_time, time)
                                # print(f"{folder}: {time}, {benchmark_dict[benchmark].o3_mini_agent_time}, {benchmark_dict[benchmark].min_o3_mini_agent_time}, {benchmark_dict[benchmark].max_o3_mini_agent_time}")


                        if "Agent synthesis calls:" in line:
                            calls = int(line.split(":")[1].strip())
                            
                            if "deepseek-chat" in folder:
                                benchmark_dict[benchmark].deepseek_chat_synthesis_calls += calls
                                benchmark_dict[benchmark].min_deepseek_chat_synthesis_calls = min(benchmark_dict[benchmark].min_deepseek_chat_synthesis_calls, calls)
                                benchmark_dict[benchmark].max_deepseek_chat_synthesis_calls = max(benchmark_dict[benchmark].max_deepseek_chat_synthesis_calls, calls)
                                # print(f"{folder}: {calls}, {benchmark_dict[benchmark].deepseek_chat_synthesis_calls}, {benchmark_dict[benchmark].min_deepseek_chat_synthesis_calls}, {benchmark_dict[benchmark].max_deepseek_chat_synthesis_calls}")

                            if "hyperbolic-reasoner" in folder:
                                benchmark_dict[benchmark].deepseek_reasoner_synthesis_calls += calls
                                benchmark_dict[benchmark].min_deepseek_reasoner_synthesis_calls = min(benchmark_dict[benchmark].min_deepseek_reasoner_synthesis_calls, calls)
                                benchmark_dict[benchmark].max_deepseek_reasoner_synthesis_calls = max(benchmark_dict[benchmark].max_deepseek_reasoner_synthesis_calls, calls)
                                # print(f"{folder}: {calls}, {benchmark_dict[benchmark].deepseek_reasoner_synthesis_calls}, {benchmark_dict[benchmark].min_deepseek_reasoner_synthesis_calls}, {benchmark_dict[benchmark].max_deepseek_reasoner_synthesis_calls}")

                            if "o3-mini" in folder:
                                benchmark_dict[benchmark].o3_mini_synthesis_calls += calls
                                benchmark_dict[benchmark].min_o3_mini_synthesis_calls = min(benchmark_dict[benchmark].min_o3_mini_synthesis_calls, calls)
                                benchmark_dict[benchmark].max_o3_mini_synthesis_calls = max(benchmark_dict[benchmark].max_o3_mini_synthesis_calls, calls)
                                # print(f"{folder}: {calls}, {benchmark_dict[benchmark].o3_mini_synthesis_calls}, {benchmark_dict[benchmark].min_o3_mini_synthesis_calls}, {benchmark_dict[benchmark].max_o3_mini_synthesis_calls}")


                        if "Agent python calls:" in line:
                            calls = int(line.split(":")[1].strip())
                            
                            if "deepseek-chat" in folder:
                                benchmark_dict[benchmark].deepseek_chat_python_calls += calls
                                benchmark_dict[benchmark].min_deepseek_chat_python_calls = min(benchmark_dict[benchmark].min_deepseek_chat_python_calls, calls)
                                benchmark_dict[benchmark].max_deepseek_chat_python_calls = max(benchmark_dict[benchmark].max_deepseek_chat_python_calls, calls)
                                # print(f"{folder}: {calls}, {benchmark_dict[benchmark].deepseek_chat_python_calls}, {benchmark_dict[benchmark].min_deepseek_chat_python_calls}, {benchmark_dict[benchmark].max_deepseek_chat_python_calls}")

                            if "hyperbolic-reasoner" in folder:
                                benchmark_dict[benchmark].deepseek_reasoner_python_calls += calls
                                benchmark_dict[benchmark].min_deepseek_reasoner_python_calls = min(benchmark_dict[benchmark].min_deepseek_reasoner_python_calls, calls)
                                benchmark_dict[benchmark].max_deepseek_reasoner_python_calls = max(benchmark_dict[benchmark].max_deepseek_reasoner_python_calls, calls)
                                # print(f"{folder}: {calls}, {benchmark_dict[benchmark].deepseek_reasoner_python_calls}, {benchmark_dict[benchmark].min_deepseek_reasoner_python_calls}, {benchmark_dict[benchmark].max_deepseek_reasoner_python_calls}")

                            if "o3-mini" in folder:
                                benchmark_dict[benchmark].o3_mini_python_calls += calls
                                benchmark_dict[benchmark].min_o3_mini_python_calls = min(benchmark_dict[benchmark].min_o3_mini_python_calls, calls)
                                benchmark_dict[benchmark].max_o3_mini_python_calls = max(benchmark_dict[benchmark].max_o3_mini_python_calls, calls)
                                # print(f"{folder}: {calls}, {benchmark_dict[benchmark].o3_mini_python_calls}, {benchmark_dict[benchmark].min_o3_mini_python_calls}, {benchmark_dict[benchmark].max_o3_mini_python_calls}")


                        if "Agent profile calls:" in line:
                            calls = int(line.split(":")[1].strip())
                            
                            if "deepseek-chat" in folder:
                                benchmark_dict[benchmark].deepseek_chat_profile_calls += calls
                                benchmark_dict[benchmark].min_deepseek_chat_profile_calls = min(benchmark_dict[benchmark].min_deepseek_chat_profile_calls, calls)
                                benchmark_dict[benchmark].max_deepseek_chat_profile_calls = max(benchmark_dict[benchmark].max_deepseek_chat_profile_calls, calls)
                                # print(f"{folder}: {calls}, {benchmark_dict[benchmark].deepseek_chat_profile_calls}, {benchmark_dict[benchmark].min_deepseek_chat_profile_calls}, {benchmark_dict[benchmark].max_deepseek_chat_profile_calls}")

                            if "hyperbolic-reasoner" in folder:
                                benchmark_dict[benchmark].deepseek_reasoner_profile_calls += calls
                                benchmark_dict[benchmark].min_deepseek_reasoner_profile_calls = min(benchmark_dict[benchmark].min_deepseek_reasoner_profile_calls, calls)
                                benchmark_dict[benchmark].max_deepseek_reasoner_profile_calls = max(benchmark_dict[benchmark].max_deepseek_reasoner_profile_calls, calls)
                                # print(f"{folder}: {calls}, {benchmark_dict[benchmark].deepseek_reasoner_profile_calls}, {benchmark_dict[benchmark].min_deepseek_reasoner_profile_calls}, {benchmark_dict[benchmark].max_deepseek_reasoner_profile_calls}")
                            
                            if "o3-mini" in folder:
                                benchmark_dict[benchmark].o3_mini_profile_calls += calls
                                benchmark_dict[benchmark].min_o3_mini_profile_calls = min(benchmark_dict[benchmark].min_o3_mini_profile_calls, calls)
                                benchmark_dict[benchmark].max_o3_mini_profile_calls = max(benchmark_dict[benchmark].max_o3_mini_profile_calls, calls)
                                # print(f"{folder}: {calls}, {benchmark_dict[benchmark].o3_mini_profile_calls}, {benchmark_dict[benchmark].min_o3_mini_profile_calls}, {benchmark_dict[benchmark].max_o3_mini_profile_calls}")


                        if "Agent inspect calls:" in line:
                            calls = int(line.split(":")[1].strip())
                            
                            if "deepseek-chat" in folder:
                                benchmark_dict[benchmark].deepseek_chat_inspect_calls += calls
                                benchmark_dict[benchmark].min_deepseek_chat_inspect_calls = min(benchmark_dict[benchmark].min_deepseek_chat_inspect_calls, calls)
                                benchmark_dict[benchmark].max_deepseek_chat_inspect_calls = max(benchmark_dict[benchmark].max_deepseek_chat_inspect_calls, calls)
                                # print(f"{folder}: {calls}, {benchmark_dict[benchmark].deepseek_chat_inspect_calls}, {benchmark_dict[benchmark].min_deepseek_chat_inspect_calls}, {benchmark_dict[benchmark].max_deepseek_chat_inspect_calls}")

                            if "hyperbolic-reasoner" in folder:
                                benchmark_dict[benchmark].deepseek_reasoner_inspect_calls += calls
                                benchmark_dict[benchmark].min_deepseek_reasoner_inspect_calls = min(benchmark_dict[benchmark].min_deepseek_reasoner_inspect_calls, calls)
                                benchmark_dict[benchmark].max_deepseek_reasoner_inspect_calls = max(benchmark_dict[benchmark].max_deepseek_reasoner_inspect_calls, calls)
                                # print(f"{folder}: {calls}, {benchmark_dict[benchmark].deepseek_reasoner_inspect_calls}, {benchmark_dict[benchmark].min_deepseek_reasoner_inspect_calls}, {benchmark_dict[benchmark].max_deepseek_reasoner_inspect_calls}")

                            if "o3-mini" in folder:
                                benchmark_dict[benchmark].o3_mini_inspect_calls += calls
                                benchmark_dict[benchmark].min_o3_mini_inspect_calls = min(benchmark_dict[benchmark].min_o3_mini_inspect_calls, calls)
                                benchmark_dict[benchmark].max_o3_mini_inspect_calls = max(benchmark_dict[benchmark].max_o3_mini_inspect_calls, calls)
                                # print(f"{folder}: {calls}, {benchmark_dict[benchmark].o3_mini_inspect_calls}, {benchmark_dict[benchmark].min_o3_mini_inspect_calls}, {benchmark_dict[benchmark].max_o3_mini_inspect_calls}")

                        # if "Agent solution calls:" in line:
                        #     calls = int(line.split(":")[1].strip())
                        #     # print(calls)


                        if "Seconds lost due to API down:" in line:
                            time = int(line.split(":")[1].strip())

                            if "deepseek-chat" in folder:
                                benchmark_dict[benchmark].deepseek_chat_api_downtime += time
                                benchmark_dict[benchmark].min_deepseek_chat_api_downtime = min(benchmark_dict[benchmark].min_deepseek_chat_api_downtime, time)
                                benchmark_dict[benchmark].max_deepseek_chat_api_downtime = max(benchmark_dict[benchmark].max_deepseek_chat_api_downtime, time)
                                # print(f"{folder}: {time}, {benchmark_dict[benchmark].deepseek_chat_api_downtime}, {benchmark_dict[benchmark].min_deepseek_chat_api_downtime}, {benchmark_dict[benchmark].max_deepseek_chat_api_downtime}")

                            if "hyperbolic-reasoner" in folder:
                                benchmark_dict[benchmark].deepseek_reasoner_api_downtime += time
                                benchmark_dict[benchmark].min_deepseek_reasoner_api_downtime = min(benchmark_dict[benchmark].min_deepseek_reasoner_api_downtime, time)
                                benchmark_dict[benchmark].max_deepseek_reasoner_api_downtime = max(benchmark_dict[benchmark].max_deepseek_reasoner_api_downtime, time)
                                # print(f"{folder}: {time}, {benchmark_dict[benchmark].deepseek_reasoner_api_downtime}, {benchmark_dict[benchmark].min_deepseek_reasoner_api_downtime}, {benchmark_dict[benchmark].max_deepseek_reasoner_api_downtime}")

                            if "o3-mini" in folder:
                                benchmark_dict[benchmark].o3_mini_api_downtime += time
                                benchmark_dict[benchmark].min_o3_mini_api_downtime = min(benchmark_dict[benchmark].min_o3_mini_api_downtime, time)
                                benchmark_dict[benchmark].max_o3_mini_api_downtime = max(benchmark_dict[benchmark].max_o3_mini_api_downtime, time)
                                # print(f"{folder}: {time}, {benchmark_dict[benchmark].o3_mini_api_downtime}, {benchmark_dict[benchmark].min_o3_mini_api_downtime}, {benchmark_dict[benchmark].max_o3_mini_api_downtime}")


                        if "Design Total:" in line:
                            latency = int(line.split()[3].strip())
                            
                            if "deepseek-chat" in folder:
                                benchmark_dict[benchmark].deepseek_chat_success += 1
                                benchmark_dict[benchmark].deepseek_chat_latency += latency
                                benchmark_dict[benchmark].min_deepseek_chat_latency = min(benchmark_dict[benchmark].min_deepseek_chat_latency, latency)
                                benchmark_dict[benchmark].max_deepseek_chat_latency = max(benchmark_dict[benchmark].max_deepseek_chat_latency, latency)
                                # print(f"{folder}: {latency}, {benchmark_dict[benchmark].deepseek_chat_latency}, {benchmark_dict[benchmark].min_deepseek_chat_latency}, {benchmark_dict[benchmark].max_deepseek_chat_latency}")

                            if "hyperbolic-reasoner" in folder:
                                benchmark_dict[benchmark].deepseek_reasoner_success += 1
                                benchmark_dict[benchmark].deepseek_reasoner_latency += latency
                                benchmark_dict[benchmark].min_deepseek_reasoner_latency = min(benchmark_dict[benchmark].min_deepseek_reasoner_latency, latency)
                                benchmark_dict[benchmark].max_deepseek_reasoner_latency = max(benchmark_dict[benchmark].max_deepseek_reasoner_latency, latency)
                                # print(f"{folder}: {latency}, {benchmark_dict[benchmark].deepseek_reasoner_latency}, {benchmark_dict[benchmark].min_deepseek_reasoner_latency}, {benchmark_dict[benchmark].max_deepseek_reasoner_latency}")

                            if "o3-mini" in folder:
                                benchmark_dict[benchmark].o3_mini_success += 1
                                benchmark_dict[benchmark].o3_mini_latency += latency
                                benchmark_dict[benchmark].min_o3_mini_latency = min(benchmark_dict[benchmark].min_o3_mini_latency, latency)
                                benchmark_dict[benchmark].max_o3_mini_latency = max(benchmark_dict[benchmark].max_o3_mini_latency, latency)
                                # print(f"{folder}: {latency}, {benchmark_dict[benchmark].o3_mini_latency}, {benchmark_dict[benchmark].min_o3_mini_latency}, {benchmark_dict[benchmark].max_o3_mini_latency}")


                        if "Total Area Score:" in line:
                            area = float(line.split()[-1].strip())

                            if "deepseek-chat" in folder:
                                benchmark_dict[benchmark].deepseek_chat_area += area
                                benchmark_dict[benchmark].min_deepseek_chat_area = min(benchmark_dict[benchmark].min_deepseek_chat_area, area)
                                benchmark_dict[benchmark].max_deepseek_chat_area = max(benchmark_dict[benchmark].max_deepseek_chat_area, area)
                                # print(f"{folder}: {area}, {benchmark_dict[benchmark].deepseek_chat_area}, {benchmark_dict[benchmark].min_deepseek_chat_area}, {benchmark_dict[benchmark].max_deepseek_chat_area}")

                            if "hyperbolic-reasoner" in folder:
                                benchmark_dict[benchmark].deepseek_reasoner_area += area
                                benchmark_dict[benchmark].min_deepseek_reasoner_area = min(benchmark_dict[benchmark].min_deepseek_reasoner_area, area)
                                benchmark_dict[benchmark].max_deepseek_reasoner_area = max(benchmark_dict[benchmark].max_deepseek_reasoner_area, area)
                                # print(f"{folder}: {area}, {benchmark_dict[benchmark].deepseek_reasoner_area}, {benchmark_dict[benchmark].min_deepseek_reasoner_area}, {benchmark_dict[benchmark].max_deepseek_reasoner_area}")

                            if "o3-mini" in folder:
                                benchmark_dict[benchmark].o3_mini_area += area
                                benchmark_dict[benchmark].min_o3_mini_area = min(benchmark_dict[benchmark].min_o3_mini_area, area)
                                benchmark_dict[benchmark].max_o3_mini_area = max(benchmark_dict[benchmark].max_o3_mini_area, area)
                                # print(f"{folder}: {area}, {benchmark_dict[benchmark].o3_mini_area}, {benchmark_dict[benchmark].min_o3_mini_area}, {benchmark_dict[benchmark].max_o3_mini_area}")


# CALCULATE AVERAGES AND COSTS FOR EACH BENCHMARK  
for benchmark in benchmarks:

    #############################################
    ### BENCHMARK AVERAGES WITH DEEPSEEK CHAT ###
    #############################################

    if benchmark_dict[benchmark].deepseek_chat_success > 0:
        
        benchmark_dict[benchmark].deepseek_chat_runs /= benchmark_dict[benchmark].deepseek_chat_success
        benchmark_dict[benchmark].deepseek_chat_hls_runs /= benchmark_dict[benchmark].deepseek_chat_success
        benchmark_dict[benchmark].deepseek_chat_compile_runs /= benchmark_dict[benchmark].deepseek_chat_success
        
        benchmark_dict[benchmark].deepseek_chat_input_tokens /= benchmark_dict[benchmark].deepseek_chat_success
        benchmark_dict[benchmark].deepseek_chat_output_tokens /= benchmark_dict[benchmark].deepseek_chat_success
        
        benchmark_dict[benchmark].deepseek_chat_synthesis_calls /= benchmark_dict[benchmark].deepseek_chat_success
        benchmark_dict[benchmark].deepseek_chat_python_calls /= benchmark_dict[benchmark].deepseek_chat_success
        benchmark_dict[benchmark].deepseek_chat_profile_calls /= benchmark_dict[benchmark].deepseek_chat_success
        benchmark_dict[benchmark].deepseek_chat_inspect_calls /= benchmark_dict[benchmark].deepseek_chat_success
        
        benchmark_dict[benchmark].deepseek_chat_agent_time /= benchmark_dict[benchmark].deepseek_chat_success
        benchmark_dict[benchmark].deepseek_chat_api_downtime /= benchmark_dict[benchmark].deepseek_chat_success
        
        benchmark_dict[benchmark].deepseek_chat_latency /= benchmark_dict[benchmark].deepseek_chat_success
        benchmark_dict[benchmark].deepseek_chat_area /= benchmark_dict[benchmark].deepseek_chat_success
    

    #################################################
    ### BENCHMARK AVERAGES WITH DEEPSEEK REASONER ###
    #################################################

    if benchmark_dict[benchmark].deepseek_reasoner_success > 0:
        
        benchmark_dict[benchmark].deepseek_reasoner_runs /= benchmark_dict[benchmark].deepseek_reasoner_success
        benchmark_dict[benchmark].deepseek_reasoner_hls_runs /= benchmark_dict[benchmark].deepseek_reasoner_success
        benchmark_dict[benchmark].deepseek_reasoner_compile_runs /= benchmark_dict[benchmark].deepseek_reasoner_success
        
        benchmark_dict[benchmark].deepseek_reasoner_input_tokens /= benchmark_dict[benchmark].deepseek_reasoner_success
        benchmark_dict[benchmark].deepseek_reasoner_output_tokens /= benchmark_dict[benchmark].deepseek_reasoner_success
        
        benchmark_dict[benchmark].deepseek_reasoner_synthesis_calls /= benchmark_dict[benchmark].deepseek_reasoner_success
        benchmark_dict[benchmark].deepseek_reasoner_python_calls /= benchmark_dict[benchmark].deepseek_reasoner_success
        benchmark_dict[benchmark].deepseek_reasoner_profile_calls /= benchmark_dict[benchmark].deepseek_reasoner_success
        benchmark_dict[benchmark].deepseek_reasoner_inspect_calls /= benchmark_dict[benchmark].deepseek_reasoner_success
        
        benchmark_dict[benchmark].deepseek_reasoner_agent_time /= benchmark_dict[benchmark].deepseek_reasoner_success
        benchmark_dict[benchmark].deepseek_reasoner_api_downtime /= benchmark_dict[benchmark].deepseek_reasoner_success
        
        benchmark_dict[benchmark].deepseek_reasoner_latency /= benchmark_dict[benchmark].deepseek_reasoner_success
        benchmark_dict[benchmark].deepseek_reasoner_area /= benchmark_dict[benchmark].deepseek_reasoner_success


    #######################################
    ### BENCHMARK AVERAGES WITH O3-MINI ###
    #######################################

    if benchmark_dict[benchmark].o3_mini_success > 0:
        
        benchmark_dict[benchmark].o3_mini_runs /= benchmark_dict[benchmark].o3_mini_success
        benchmark_dict[benchmark].o3_mini_hls_runs /= benchmark_dict[benchmark].o3_mini_success
        benchmark_dict[benchmark].o3_mini_compile_runs /= benchmark_dict[benchmark].o3_mini_success
        
        benchmark_dict[benchmark].o3_mini_input_tokens /= benchmark_dict[benchmark].o3_mini_success
        benchmark_dict[benchmark].o3_mini_output_tokens /= benchmark_dict[benchmark].o3_mini_success
        
        benchmark_dict[benchmark].o3_mini_synthesis_calls /= benchmark_dict[benchmark].o3_mini_success
        benchmark_dict[benchmark].o3_mini_python_calls /= benchmark_dict[benchmark].o3_mini_success
        benchmark_dict[benchmark].o3_mini_profile_calls /= benchmark_dict[benchmark].o3_mini_success
        benchmark_dict[benchmark].o3_mini_inspect_calls /= benchmark_dict[benchmark].o3_mini_success
        
        benchmark_dict[benchmark].o3_mini_agent_time /= benchmark_dict[benchmark].o3_mini_success
        benchmark_dict[benchmark].o3_mini_api_downtime /= benchmark_dict[benchmark].o3_mini_success
        
        benchmark_dict[benchmark].o3_mini_latency /= benchmark_dict[benchmark].o3_mini_success
        benchmark_dict[benchmark].o3_mini_area /= benchmark_dict[benchmark].o3_mini_success


    # # calculate cost
    # deepseek_chat_cost_per_million_input_tokens = 0.27
    # deepseek_chat_cost_per_million_output_tokens = 1.10

    # deepseek_reasoner_cost_per_million_input_tokens = 0.55
    # deepseek_reasoner_cost_per_million_output_tokens = 2.19

    # o3_mini_cost_per_million_input_tokens = 
    # o3_mini_reasoner_cost_per_million_output_tokens = 
    
#     if b_dict[b].min_input_tokens_claude != 99999:
#         b_dict[b].claude_cost = (b_dict[b].input_tokens_claude * claude_cost_per_million_input_tokens + b_dict[b].output_tokens_claude * claude_cost_per_million_output_tokens) / 1000000
#         b_dict[b].min_claude_cost = (b_dict[b].min_input_tokens_claude * claude_cost_per_million_input_tokens + b_dict[b].min_output_tokens_claude * claude_cost_per_million_output_tokens) / 1000000
#         b_dict[b].max_claude_cost = (b_dict[b].max_input_tokens_claude * claude_cost_per_million_input_tokens + b_dict[b].max_output_tokens_claude * claude_cost_per_million_output_tokens) / 1000000

#     if b_dict[b].min_input_tokens_mini != 99999:
#         b_dict[b].adaptive_cost = (b_dict[b].input_tokens_mini * mini_cost_per_million_input_tokens + b_dict[b].output_tokens_mini * mini_cost_per_million_output_tokens + b_dict[b].input_tokens_4o * _4o_cost_per_million_input_tokens + b_dict[b].output_tokens_4o * _4o_cost_per_million_output_tokens) / 1000000
#         b_dict[b].min_adaptive_cost = (b_dict[b].min_input_tokens_mini * mini_cost_per_million_input_tokens + b_dict[b].min_output_tokens_mini * mini_cost_per_million_output_tokens + b_dict[b].min_input_tokens_4o * _4o_cost_per_million_input_tokens + b_dict[b].min_output_tokens_4o * _4o_cost_per_million_output_tokens) / 1000000
#         b_dict[b].max_adaptive_cost = (b_dict[b].max_input_tokens_mini * mini_cost_per_million_input_tokens + b_dict[b].max_output_tokens_mini * mini_cost_per_million_output_tokens + b_dict[b].max_input_tokens_4o * _4o_cost_per_million_input_tokens + b_dict[b].max_output_tokens_4o * _4o_cost_per_million_output_tokens) / 1000000
# # dump in a csv file
# import csv
# with open("parsed.csv", "w") as f:
#     # header
#     writer = csv.DictWriter(f, fieldnames=b_dict["Cipher"].__dict__.keys())
#     writer.writeheader()
#     for b in benchmarks:
#         writer.writerow(b_dict[b].__dict__)


# PLOTS
import matplotlib.pyplot as plt
import numpy as np

# colors for each model
colors = ["#fdb462", "#8dd3c7", "#fb8072"]
# hatches = ["/", "\\"]

# define number of benchmarks and width of bars
x = np.arange(len(benchmarks))
width = 0.35

# benchmarks = ["Cipher", "sha256_update", "present80_encryptBlock", "kmp", "filter", "needwun"]
pretty_names = ["AES", "SHA-256", "PRESENT", "KMP", "FIR+IIR", "NW"]


##########################
### PLOT SUCCESS RATES ###
##########################

# calculate success rates
success_rates = []
for benchmark in benchmarks:
    deepseek_chat_success_rate = (benchmark_dict[benchmark].deepseek_chat_success / 10) * 100
    deepseek_reasoner_success_rate = (benchmark_dict[benchmark].deepseek_reasoner_success / 10) * 100
    o3_mini_success_rate = (benchmark_dict[benchmark].o3_mini_success / 10) * 100
    success_rates.append((deepseek_chat_success_rate, deepseek_reasoner_success_rate, o3_mini_success_rate))

print(success_rates)

# create the figure and axis
fig, ax = plt.subplots(figsize=(20, 5))
# set style
plt.style.use("seaborn-v0_8-talk")

# plot bars side by side
rects1 = ax.bar(x - width, [rate[0] for rate in success_rates], width, edgecolor="black", color= colors[0], label="DeepSeek-V3")
rects2 = ax.bar(x, [rate[1] for rate in success_rates], width, edgecolor="black", color= colors[1], label="DeepSeek-R1")
rects3 = ax.bar(x + width, [rate[2] for rate in success_rates], width, edgecolor="black", color= colors[2], label="o3-mini")

# add labels, title, and legend
ax.set_ylabel("Success Rate (%)", fontsize=24)
ax.set_xlabel("Benchmarks", fontsize=24)
ax.set_xticks(x)
plt.yticks(fontsize=24)
ax.set_xticklabels(pretty_names, fontsize=24)
ax.legend(fontsize=24)
plt.tight_layout()

# save the plot as pdf
plt.savefig("success_rate_comparison.pdf")


##############################
### PLOT AVERAGE LATENCIES ###
##############################

average_latencies = []
latency_errors = []

for benchmark in benchmarks:
    
    deepseek_chat_avg_latency = benchmark_dict[benchmark].deepseek_chat_latency if benchmark_dict[benchmark].deepseek_chat_latency != 999999 else 0
    deepseek_chat_min_latency = benchmark_dict[benchmark].min_deepseek_chat_latency if benchmark_dict[benchmark].min_deepseek_chat_latency != 999999 else 0
    deepseek_chat_max_latency = benchmark_dict[benchmark].max_deepseek_chat_latency if benchmark_dict[benchmark].max_deepseek_chat_latency != 999999 else 0

    deepseek_reasoner_avg_latency = benchmark_dict[benchmark].deepseek_reasoner_latency if benchmark_dict[benchmark].deepseek_reasoner_latency != 999999 else 0
    deepseek_reasoner_min_latency = benchmark_dict[benchmark].min_deepseek_reasoner_latency if benchmark_dict[benchmark].min_deepseek_reasoner_latency != 999999 else 0
    deepseek_reasoner_max_latency = benchmark_dict[benchmark].max_deepseek_reasoner_latency if benchmark_dict[benchmark].max_deepseek_reasoner_latency != 999999 else 0

    o3_mini_avg_latency = benchmark_dict[benchmark].o3_mini_latency if benchmark_dict[benchmark].o3_mini_latency != 999999 else 0
    o3_mini_min_latency = benchmark_dict[benchmark].min_o3_mini_latency if benchmark_dict[benchmark].min_o3_mini_latency != 999999 else 0
    o3_mini_max_latency = benchmark_dict[benchmark].max_o3_mini_latency if benchmark_dict[benchmark].max_o3_mini_latency != 999999 else 0

    # append average latencies
    average_latencies.append((deepseek_chat_avg_latency, deepseek_reasoner_avg_latency, o3_mini_avg_latency))

    # calculate the errors (difference between min/max and the average)
    deepseek_chat_error = [deepseek_chat_avg_latency - deepseek_chat_min_latency, deepseek_chat_max_latency - deepseek_chat_avg_latency]
    deepseek_reasoner_error = [deepseek_reasoner_avg_latency - deepseek_reasoner_min_latency, deepseek_reasoner_max_latency - deepseek_reasoner_avg_latency]
    o3_mini_error = [o3_mini_avg_latency - o3_mini_min_latency, o3_mini_max_latency - o3_mini_avg_latency]

    # append errors
    latency_errors.append((deepseek_chat_error, deepseek_reasoner_error, o3_mini_error))
    
average_latencies = np.array(average_latencies)
latency_errors = np.array(latency_errors)

# create the figure and axis
fig, ax = plt.subplots(figsize=(20, 5))

# plot bars side by side
rects4 = ax.bar(x - width, average_latencies[:, 0], width, edgecolor="black", color= colors[0], label="DeepSeek-V3")
rects5 = ax.bar(x, average_latencies[:, 1], width, edgecolor="black", color= colors[1], label="DeepSeek-R1")
rects6 = ax.bar(x + width, average_latencies[:, 2], width, edgecolor="black", color= colors[2], label="o3-mini")
# ax.set_yscale("log")

# add error bars for latency range (min-max)
for i in range(len(x)):

    # deepseek-chat
    ax.errorbar(x[i] - width, average_latencies[i, 0], 
                yerr=[[latency_errors[i, 0][0]], [latency_errors[i, 0][1]]], 
                fmt="none", ecolor="black", elinewidth=2, capsize=15, markeredgewidth=2)
    
    # deepseek-reasoner
    ax.errorbar(x[i], average_latencies[i, 1], 
                yerr=[[latency_errors[i, 1][0]], [latency_errors[i, 1][1]]], 
                fmt="none", ecolor="black", elinewidth=2, capsize=15, markeredgewidth=2)

    # o3-mini
    ax.errorbar(x[i] + width, average_latencies[i, 2], 
                yerr=[[latency_errors[i, 2][0]], [latency_errors[i, 2][1]]], 
                fmt="none", ecolor="black", elinewidth=2, capsize=15, markeredgewidth=2)

# add labels, title, and legend
ax.set_ylabel("Latency [cycles]", fontsize=24)
ax.set_xlabel("Benchmarks", fontsize=24)
ax.set_xticks(x)
plt.yticks(fontsize=24)
ax.set_xticklabels(pretty_names, fontsize=24)
ax.legend(fontsize=24)
plt.tight_layout()

# save the plot as pdf
plt.savefig("average_latency_comparison.pdf")


##########################
### PLOT AVERAGE AREAS ###
##########################

average_areas = []
area_errors = []

for benchmark in benchmarks:
    deepseek_chat_avg_area = benchmark_dict[benchmark].deepseek_chat_area if benchmark_dict[benchmark].deepseek_chat_area != 999999 else 0
    deepseek_chat_min_area = benchmark_dict[benchmark].min_deepseek_chat_area if benchmark_dict[benchmark].min_deepseek_chat_area != 999999 else 0
    deepseek_chat_max_area = benchmark_dict[benchmark].max_deepseek_chat_area if benchmark_dict[benchmark].max_deepseek_chat_area != 999999 else 0

    deepseek_reasoner_avg_area = benchmark_dict[benchmark].deepseek_reasoner_area if benchmark_dict[benchmark].deepseek_reasoner_area != 999999 else 0
    deepseek_reasoner_min_area = benchmark_dict[benchmark].min_deepseek_reasoner_area if benchmark_dict[benchmark].min_deepseek_reasoner_area != 999999 else 0
    deepseek_reasoner_max_area = benchmark_dict[benchmark].max_deepseek_reasoner_area if benchmark_dict[benchmark].max_deepseek_reasoner_area != 999999 else 0

    o3_mini_avg_area = benchmark_dict[benchmark].o3_mini_area if benchmark_dict[benchmark].o3_mini_area != 999999 else 0
    o3_mini_min_area = benchmark_dict[benchmark].min_o3_mini_area if benchmark_dict[benchmark].min_o3_mini_area != 999999 else 0
    o3_mini_max_area = benchmark_dict[benchmark].max_o3_mini_area if benchmark_dict[benchmark].max_o3_mini_area != 999999 else 0

    # append average areas
    average_areas.append((deepseek_chat_avg_area, deepseek_reasoner_avg_area, o3_mini_avg_area))

    # calculate the errors (difference between min/max and the average)
    deepseek_chat_error = [deepseek_chat_avg_area - deepseek_chat_min_area, deepseek_chat_max_area - deepseek_chat_avg_area]
    deepseek_reasoner_error = [deepseek_reasoner_avg_area - deepseek_reasoner_min_area, deepseek_reasoner_max_area - deepseek_reasoner_avg_area]
    o3_mini_error = [o3_mini_avg_area - o3_mini_min_area, o3_mini_max_area - o3_mini_avg_area]

    # append errors
    area_errors.append((deepseek_chat_error, deepseek_reasoner_error, o3_mini_error))

average_areas = np.array(average_areas)
area_errors = np.array(area_errors)

# create the figure and axis
fig, ax = plt.subplots(figsize=(20, 5))

# plot bars side by side
rects7 = ax.bar(x - width, average_areas[:, 0], width, edgecolor="black", color= colors[0], label="DeepSeek-V3")
rects8 = ax.bar(x, average_areas[:, 1], width, edgecolor="black", color= colors[1], label="DeepSeek-R1")
rects9 = ax.bar(x + width, average_areas[:, 2], width, edgecolor="black", color= colors[2], label="o3-mini")
# ax.set_yscale("log")

# add error bars for area range (min-max)
for i in range(len(x)):

    # deepseek-chat
    ax.errorbar(x[i] - width, average_areas[i, 0], 
                yerr=[[area_errors[i, 0][0]], [area_errors[i, 0][1]]], 
                fmt="none", ecolor="black", elinewidth=2, capsize=15, markeredgewidth=2)
    
    # deepseek-reasoner
    ax.errorbar(x[i], average_areas[i, 1], 
                yerr=[[area_errors[i, 1][0]], [area_errors[i, 1][1]]], 
                fmt="none", ecolor="black", elinewidth=2, capsize=15, markeredgewidth=2)

    # o3-mini
    ax.errorbar(x[i] + width, average_areas[i, 2], 
                yerr=[[area_errors[i, 2][0]], [area_errors[i, 2][1]]], 
                fmt="none", ecolor="black", elinewidth=2, capsize=15, markeredgewidth=2)

# add labels, title, and legend
ax.set_ylabel("Area [um2]", fontsize=24)
ax.set_xlabel("Benchmarks", fontsize=24)
ax.set_xticks(x)
plt.yticks(fontsize=24)
ax.set_xticklabels(pretty_names, fontsize=24)
ax.legend(fontsize=24)
plt.tight_layout()

# save the plot as pdf
plt.savefig("average_area_comparison.pdf")


##########################
### PLOT AVERAGE COSTS ###
##########################

# # Calculate average cost
# average_costs = []
# cost_errors = []

# for b in benchmarks:
#     adaptive_avg_cost = b_dict[b].adaptive_cost if b_dict[b].adaptive_cost != 99999 else 0
#     claude_avg_cost = b_dict[b].claude_cost if b_dict[b].claude_cost != 99999 else 0
#     adaptive_min_cost = b_dict[b].min_adaptive_cost if b_dict[b].min_adaptive_cost != 99999 else 0
#     adaptive_max_cost = b_dict[b].max_adaptive_cost if b_dict[b].max_adaptive_cost != 99999 else 0
#     claude_min_cost = b_dict[b].min_claude_cost if b_dict[b].min_claude_cost != 99999 else 0
#     claude_max_cost = b_dict[b].max_claude_cost if b_dict[b].max_claude_cost != 99999 else 0

#     # Append average costs
#     average_costs.append((adaptive_avg_cost, claude_avg_cost))
    
#     # Calculate the errors (difference between min/max and the average)
#     adaptive_error = [adaptive_avg_cost - adaptive_min_cost if abs(adaptive_avg_cost - adaptive_min_cost) > 0.0000001 else 0, adaptive_max_cost - adaptive_avg_cost if abs(adaptive_max_cost - adaptive_avg_cost) > 0.0000001 else 0]
#     claude_error = [claude_avg_cost - claude_min_cost if abs(claude_avg_cost - claude_min_cost) > 0.0000001 else 0, claude_max_cost - claude_avg_cost if abs(claude_max_cost - claude_avg_cost) > 0.0000001 else 0]
    
#     # Append error values
#     cost_errors.append((adaptive_error, claude_error))

# average_costs = np.array(average_costs)
# cost_errors = np.array(cost_errors)

# # Create the figure and axis
# fig, ax = plt.subplots(figsize=(20, 5))

# # Plot bars side by side
# rects7 = ax.bar(x - width/2, average_costs[:, 0], width, edgecolor="black", hatch=hatches[0], color= colors[0], label="Adaptive")
# rects8 = ax.bar(x + width/2, average_costs[:, 1], width, edgecolor="black", hatch=hatches[1], color= colors[1], label="Claude")
# #ax.set_yscale("log")

# # Add error bars for cost range (min-max)
# for i in range(len(x)):
#     # For Adaptive
#     ax.errorbar(x[i] - width/2, average_costs[i, 0], 
#                 yerr=[[cost_errors[i, 0][0]], [cost_errors[i, 0][1]]], 
#                 fmt="none", ecolor="black", elinewidth=2, capsize=15, markeredgewidth=2)
    
#     # For Claude
#     ax.errorbar(x[i] + width/2, average_costs[i, 1], 
#                 yerr=[[cost_errors[i, 1][0]], [cost_errors[i, 1][1]]], 
#                 fmt="none", ecolor="black", elinewidth=2, capsize=15,markeredgewidth=2)

# # Add labels, title, and legend
# ax.set_ylabel("Cost [$]",fontsize=24)
# ax.set_xlabel("Benchmarks", fontsize=28)
# ax.set_xticks(x)
# plt.yticks(fontsize=24)
# ax.set_xticklabels(pretty_names,fontsize=28)
# ax.legend(fontsize=24)
# plt.tight_layout()

# # Save the plot as pdf
# plt.savefig("average_cost_comparison.pdf")