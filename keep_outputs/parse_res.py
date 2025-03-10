import os

benchmarks = ["compute1", "compute2", "compute3", "compute4", "compute5", "compute6", "Cipher", "sha256_update", "present80_encryptBlock", "kmp", "filter", "needwun"]

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

        # TIME FOR C2HLSC
        self.deepseek_chat_c2hlsc_time = 0
        self.min_deepseek_chat_c2hlsc_time = 999999
        self.max_deepseek_chat_c2hlsc_time = 0

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

        # MODEL COST
        self.deepseek_chat_cost = 0
        self.min_deepseek_chat_cost = 999999
        self.max_deepseek_chat_cost = 0


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

        # TIME FOR C2HLSC
        self.deepseek_reasoner_c2hlsc_time = 0
        self.min_deepseek_reasoner_c2hlsc_time = 999999
        self.max_deepseek_reasoner_c2hlsc_time = 0

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

        # MODEL COST
        self.deepseek_reasoner_cost = 0
        self.min_deepseek_reasoner_cost = 999999
        self.max_deepseek_reasoner_cost = 0


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

        # TIME FOR C2HLSC
        self.o3_mini_c2hlsc_time = 0
        self.min_o3_mini_c2hlsc_time = 999999
        self.max_o3_mini_c2hlsc_time = 0

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

        # MODEL COST
        self.o3_mini_cost = 0
        self.min_o3_mini_cost = 999999
        self.max_o3_mini_cost = 0


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


                        if "deepseek-ai/DeepSeek-R1 runs:" in line or "deepseek-reasoner runs:" in line:
                            runs = int(line.split(":")[1].strip())
                            benchmark_dict[benchmark].deepseek_reasoner_runs += runs
                            benchmark_dict[benchmark].min_deepseek_reasoner_runs = min(benchmark_dict[benchmark].min_deepseek_reasoner_runs, runs)
                            benchmark_dict[benchmark].max_deepseek_reasoner_runs = max(benchmark_dict[benchmark].max_deepseek_reasoner_runs, runs)

                        if "deepseek-ai/DeepSeek-R1 input tokens:" in line or "deepseek-reasoner input tokens:" in line:
                            input_tokens = int(line.split(":")[1].strip())
                            benchmark_dict[benchmark].deepseek_reasoner_input_tokens += input_tokens
                            benchmark_dict[benchmark].min_deepseek_reasoner_input_tokens = min(benchmark_dict[benchmark].min_deepseek_reasoner_input_tokens, input_tokens)
                            benchmark_dict[benchmark].max_deepseek_reasoner_input_tokens = max(benchmark_dict[benchmark].max_deepseek_reasoner_input_tokens, input_tokens)
                            # print(f"{folder}: {input_tokens}, {benchmark_dict[benchmark].deepseek_reasoner_input_tokens}, {benchmark_dict[benchmark].min_deepseek_reasoner_input_tokens}, {benchmark_dict[benchmark].max_deepseek_reasoner_input_tokens}")

                        if "deepseek-ai/DeepSeek-R1 output tokens:" in line or "deepseek-reasoner output tokens:" in line:
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
                            
                            if "hyperbolic-reasoner" in folder or "deepseek-reasoner" in folder:
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

                            if "hyperbolic-reasoner" in folder or "deepseek-reasoner" in folder:
                                benchmark_dict[benchmark].deepseek_reasoner_compile_runs += runs
                                benchmark_dict[benchmark].min_deepseek_reasoner_compile_runs = min(benchmark_dict[benchmark].min_deepseek_reasoner_compile_runs, runs)
                                benchmark_dict[benchmark].max_deepseek_reasoner_compile_runs = max(benchmark_dict[benchmark].max_deepseek_reasoner_compile_runs, runs)
                                # print(f"{folder}: {runs}, {benchmark_dict[benchmark].deepseek_reasoner_compile_runs}, {benchmark_dict[benchmark].min_deepseek_reasoner_compile_runs}, {benchmark_dict[benchmark].max_deepseek_reasoner_compile_runs}")
                            
                            if "o3-mini" in folder:
                                benchmark_dict[benchmark].o3_mini_compile_runs += runs
                                benchmark_dict[benchmark].min_o3_mini_compile_runs = min(benchmark_dict[benchmark].min_o3_mini_compile_runs, runs)
                                benchmark_dict[benchmark].max_o3_mini_compile_runs = max(benchmark_dict[benchmark].max_o3_mini_compile_runs, runs)
                                # print(f"{folder}: {runs}, {benchmark_dict[benchmark].o3_mini_compile_runs}, {benchmark_dict[benchmark].min_o3_mini_compile_runs}, {benchmark_dict[benchmark].max_o3_mini_compile_runs}")


                        if "Time for c2hlsc:" in line:
                            time = int(line.split(":")[1].strip()) / 1000   # ms --> s

                            if "deepseek-chat" in folder:
                                benchmark_dict[benchmark].deepseek_chat_c2hlsc_time += time
                                benchmark_dict[benchmark].min_deepseek_chat_c2hlsc_time = min(benchmark_dict[benchmark].min_deepseek_chat_c2hlsc_time, time)
                                benchmark_dict[benchmark].max_deepseek_chat_c2hlsc_time = max(benchmark_dict[benchmark].max_deepseek_chat_c2hlsc_time, time)
                                # print(f"{folder}: {time}, {benchmark_dict[benchmark].deepseek_chat_c2hlsc_time}, {benchmark_dict[benchmark].min_deepseek_chat_c2hlsc_time}, {benchmark_dict[benchmark].max_deepseek_chat_c2hlsc_time}")

                            if "hyperbolic-reasoner" in folder or "deepseek-reasoner" in folder:
                                benchmark_dict[benchmark].deepseek_reasoner_c2hlsc_time += time
                                benchmark_dict[benchmark].min_deepseek_reasoner_c2hlsc_time = min(benchmark_dict[benchmark].min_deepseek_reasoner_c2hlsc_time, time)
                                benchmark_dict[benchmark].max_deepseek_reasoner_c2hlsc_time = max(benchmark_dict[benchmark].max_deepseek_reasoner_c2hlsc_time, time)
                                # print(f"{folder}: {time}, {benchmark_dict[benchmark].deepseek_reasoner_c2hlsc_time}, {benchmark_dict[benchmark].min_deepseek_reasoner_c2hlsc_time}, {benchmark_dict[benchmark].max_deepseek_reasoner_c2hlsc_time}")

                            if "o3-mini" in folder:
                                benchmark_dict[benchmark].o3_mini_c2hlsc_time += time
                                benchmark_dict[benchmark].min_o3_mini_c2hlsc_time = min(benchmark_dict[benchmark].min_o3_mini_c2hlsc_time, time)
                                benchmark_dict[benchmark].max_o3_mini_c2hlsc_time = max(benchmark_dict[benchmark].max_o3_mini_c2hlsc_time, time)
                                # print(f"{folder}: {time}, {benchmark_dict[benchmark].o3_mini_c2hlsc_time}, {benchmark_dict[benchmark].min_o3_mini_c2hlsc_time}, {benchmark_dict[benchmark].max_o3_mini_c2hlsc_time}")


                        if "Time for agent:" in line:
                            time = int(line.split(":")[1].strip()) / 1000   # ms --> s
                            
                            if "deepseek-chat" in folder:
                                benchmark_dict[benchmark].deepseek_chat_agent_time += time
                                benchmark_dict[benchmark].min_deepseek_chat_agent_time = min(benchmark_dict[benchmark].min_deepseek_chat_agent_time, time)
                                benchmark_dict[benchmark].max_deepseek_chat_agent_time = max(benchmark_dict[benchmark].max_deepseek_chat_agent_time, time)
                                # print(f"{folder}: {time}, {benchmark_dict[benchmark].deepseek_chat_agent_time}, {benchmark_dict[benchmark].min_deepseek_chat_agent_time}, {benchmark_dict[benchmark].max_deepseek_chat_agent_time}")

                            if "hyperbolic-reasoner" in folder or "deepseek-reasoner" in folder:
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

                            if "hyperbolic-reasoner" in folder or "deepseek-reasoner" in folder:
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

                            if "hyperbolic-reasoner" in folder or "deepseek-reasoner" in folder:
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

                            if "hyperbolic-reasoner" in folder or "deepseek-reasoner" in folder:
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

                            if "hyperbolic-reasoner" in folder or "deepseek-reasoner" in folder:
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

                            if "hyperbolic-reasoner" in folder or "deepseek-reasoner" in folder:
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

                            if "hyperbolic-reasoner" in folder or "deepseek-reasoner" in folder:
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

                            if "hyperbolic-reasoner" in folder or "deepseek-reasoner" in folder:
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
        
        benchmark_dict[benchmark].deepseek_chat_c2hlsc_time /= benchmark_dict[benchmark].deepseek_chat_success
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
        
        benchmark_dict[benchmark].deepseek_reasoner_c2hlsc_time /= benchmark_dict[benchmark].deepseek_reasoner_success
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
        
        benchmark_dict[benchmark].o3_mini_c2hlsc_time /= benchmark_dict[benchmark].o3_mini_success
        benchmark_dict[benchmark].o3_mini_agent_time /= benchmark_dict[benchmark].o3_mini_success
        benchmark_dict[benchmark].o3_mini_api_downtime /= benchmark_dict[benchmark].o3_mini_success
        
        benchmark_dict[benchmark].o3_mini_latency /= benchmark_dict[benchmark].o3_mini_success
        benchmark_dict[benchmark].o3_mini_area /= benchmark_dict[benchmark].o3_mini_success


    # calculate cost
    deepseek_chat_cost_per_million_input_tokens = 0.27
    deepseek_chat_cost_per_million_output_tokens = 1.10

    deepseek_reasoner_cost_per_million_input_tokens = 0.55
    deepseek_reasoner_cost_per_million_output_tokens = 2.19

    o3_mini_cost_per_million_input_tokens = 1.10
    o3_mini_cost_per_million_output_tokens = 4.40

    if benchmark_dict[benchmark].min_deepseek_chat_input_tokens != 999999:
        benchmark_dict[benchmark].deepseek_chat_cost = (benchmark_dict[benchmark].deepseek_chat_input_tokens * deepseek_chat_cost_per_million_input_tokens + benchmark_dict[benchmark].deepseek_chat_output_tokens * deepseek_chat_cost_per_million_output_tokens) / 1000000
        benchmark_dict[benchmark].min_deepseek_chat_cost = (benchmark_dict[benchmark].min_deepseek_chat_input_tokens * deepseek_chat_cost_per_million_input_tokens + benchmark_dict[benchmark].min_deepseek_chat_output_tokens * deepseek_chat_cost_per_million_output_tokens) / 1000000
        benchmark_dict[benchmark].max_deepseek_chat_cost = (benchmark_dict[benchmark].max_deepseek_chat_input_tokens * deepseek_chat_cost_per_million_input_tokens + benchmark_dict[benchmark].max_deepseek_chat_output_tokens * deepseek_chat_cost_per_million_output_tokens) / 1000000

    if benchmark_dict[benchmark].min_deepseek_reasoner_input_tokens != 999999:
        benchmark_dict[benchmark].deepseek_reasoner_cost = (benchmark_dict[benchmark].deepseek_reasoner_input_tokens * deepseek_reasoner_cost_per_million_input_tokens + benchmark_dict[benchmark].deepseek_reasoner_output_tokens * deepseek_reasoner_cost_per_million_output_tokens) / 1000000
        benchmark_dict[benchmark].min_deepseek_reasoner_cost = (benchmark_dict[benchmark].min_deepseek_reasoner_input_tokens * deepseek_reasoner_cost_per_million_input_tokens + benchmark_dict[benchmark].min_deepseek_reasoner_output_tokens * deepseek_reasoner_cost_per_million_output_tokens) / 1000000
        benchmark_dict[benchmark].max_deepseek_reasoner_cost = (benchmark_dict[benchmark].max_deepseek_reasoner_input_tokens * deepseek_reasoner_cost_per_million_input_tokens + benchmark_dict[benchmark].max_deepseek_reasoner_output_tokens * deepseek_reasoner_cost_per_million_output_tokens) / 1000000

    if benchmark_dict[benchmark].min_o3_mini_input_tokens != 999999:
        benchmark_dict[benchmark].o3_mini_cost = (benchmark_dict[benchmark].o3_mini_input_tokens * o3_mini_cost_per_million_input_tokens + benchmark_dict[benchmark].o3_mini_output_tokens * o3_mini_cost_per_million_output_tokens) / 1000000
        benchmark_dict[benchmark].min_o3_mini_cost = (benchmark_dict[benchmark].min_o3_mini_input_tokens * o3_mini_cost_per_million_input_tokens + benchmark_dict[benchmark].min_o3_mini_output_tokens * o3_mini_cost_per_million_output_tokens) / 1000000
        benchmark_dict[benchmark].max_o3_mini_cost = (benchmark_dict[benchmark].max_o3_mini_input_tokens * o3_mini_cost_per_million_input_tokens + benchmark_dict[benchmark].max_o3_mini_output_tokens * o3_mini_cost_per_million_output_tokens) / 1000000


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
colors = ["#fdb462", "#8dd3c7", "#fb8072", "#80b1d3"]
hatches = ["//", "\\\\", "XX"]

# define number of benchmarks and width of bars
x = np.arange(len(benchmarks))
x = x*0.34
print(x)
width = 0.10

# benchmarks = ["compute1", "compute2", "compute3", "compute4", "compute5", "compute6", "Cipher", "sha256_update", "present80_encryptBlock", "kmp", "filter", "needwun"]
pretty_names = ["SYN 1", "SYN 2", "SYN 3", "SYN 4", "SYN 5", "SYN 6", "AES", "SHA", "PRESENT", "KMP", "FIR+IIR", "NW"]


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

# create the figure and axis
fig, ax = plt.subplots(figsize=(17, 5))
# set style
plt.style.use("seaborn-v0_8-talk")

# plot bars side by side
rects1 = ax.bar(x - width, [rate[0] for rate in success_rates], width, edgecolor="black", color= colors[0], hatch=hatches[0], label="DeepSeek-V3")
rects2 = ax.bar(x, [rate[1] for rate in success_rates], width, edgecolor="black", color= colors[1], hatch=hatches[1], label="DeepSeek-R1")
rects3 = ax.bar(x + width, [rate[2] for rate in success_rates], width, edgecolor="black", color= colors[2], hatch=hatches[2], label="o3-mini")
# ax.set_yscale("log")

# add labels, title, and legend
ax.set_ylabel("Success Rate (%)", fontsize=30)
ax.set_xlabel("Benchmarks", fontsize=30)
ax.set_xticks(x)
plt.yticks(fontsize=30)
ax.set_xlim([-0.18, 3.92])
ax.set_xticklabels(pretty_names, fontsize=25)
ax.legend(fontsize=30, ncol=3, loc="lower center")
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
fig, ax = plt.subplots(figsize=(17, 5))

# plot bars side by side
rects4 = ax.bar(x - width, average_latencies[:, 0], width, edgecolor="black", color= colors[0], hatch=hatches[0], label="DeepSeek-V3")
rects5 = ax.bar(x, average_latencies[:, 1], width, edgecolor="black", color= colors[1], hatch=hatches[1], label="DeepSeek-R1")
rects6 = ax.bar(x + width, average_latencies[:, 2], width, edgecolor="black", color= colors[2], hatch=hatches[2], label="o3-mini")
ax.set_yscale("log")

# # add error bars for latency range (min-max)
for i in range(len(x)):

    # deepseek-chat
    ax.errorbar(x[i] - width, average_latencies[i, 0], 
                yerr=[[latency_errors[i, 0][0]], [latency_errors[i, 0][1]]], 
                fmt="none", ecolor="black", elinewidth=1, capsize=8, markeredgewidth=1)
    
    # deepseek-reasoner
    ax.errorbar(x[i], average_latencies[i, 1], 
                yerr=[[latency_errors[i, 1][0]], [latency_errors[i, 1][1]]], 
                fmt="none", ecolor="black", elinewidth=1, capsize=8, markeredgewidth=1)

    # o3-mini
    ax.errorbar(x[i] + width, average_latencies[i, 2], 
                yerr=[[latency_errors[i, 2][0]], [latency_errors[i, 2][1]]], 
                fmt="none", ecolor="black", elinewidth=1, capsize=8, markeredgewidth=1)

# add labels, title, and legend
ax.set_ylabel("Latency [cycles]", fontsize=30)
ax.set_xlabel("Benchmarks", fontsize=30)
ax.set_xticks(x)
plt.yticks(fontsize=30)
ax.set_ylim([1,10000])

ax.set_xlim([-0.18, 3.92])
ax.set_xticklabels(pretty_names, fontsize=25)
ax.legend(fontsize=30)
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
    if abs(deepseek_chat_error[0]) < 0.0000001:
        deepseek_chat_error[0] = 0
    if abs(deepseek_chat_error[1]) < 0.0000001:
        deepseek_chat_error[1] = 0

    deepseek_reasoner_error = [deepseek_reasoner_avg_area - deepseek_reasoner_min_area, deepseek_reasoner_max_area - deepseek_reasoner_avg_area]
    if abs(deepseek_reasoner_error[0]) < 0.0000001:
        deepseek_reasoner_error[0] = 0
    if abs(deepseek_reasoner_error[1]) < 0.0000001:
        deepseek_reasoner_error[1] = 0

    o3_mini_error = [o3_mini_avg_area - o3_mini_min_area, o3_mini_max_area - o3_mini_avg_area]
    if abs(o3_mini_error[0]) < 0.0000001:
        o3_mini_error[0] = 0
    if abs(o3_mini_error[1]) < 0.0000001:
        o3_mini_error[1] = 0
        
    # append errors
    area_errors.append((deepseek_chat_error, deepseek_reasoner_error, o3_mini_error))

average_areas = np.array(average_areas)
area_errors = np.array(area_errors)

# create the figure and axis
fig, ax = plt.subplots(figsize=(17, 5))

# plot bars side by side
rects7 = ax.bar(x - width, average_areas[:, 0], width, edgecolor="black", color= colors[0], hatch=hatches[0], label="DeepSeek-V3")
rects8 = ax.bar(x, average_areas[:, 1], width, edgecolor="black", color= colors[1], hatch=hatches[1], label="DeepSeek-R1")
rects9 = ax.bar(x + width, average_areas[:, 2], width, edgecolor="black", color= colors[2], hatch=hatches[2], label="o3-mini")
ax.set_yscale("log")

# # add error bars for area range (min-max)
for i in range(len(x)):
    # print([area_errors[i, 0][1]])
    # print([area_errors[i, 0][0]])
    # deepseek-chat
    ax.errorbar(x[i] - width, average_areas[i, 0], 
                yerr=[[area_errors[i, 0][0]], [area_errors[i, 0][1]]], 
                fmt="none", ecolor="black", elinewidth=1, capsize=8, markeredgewidth=1)
    
    # deepseek-reasoner
    ax.errorbar(x[i], average_areas[i, 1], 
                yerr=[[area_errors[i, 1][0]], [area_errors[i, 1][1]]], 
                fmt="none", ecolor="black", elinewidth=1, capsize=8, markeredgewidth=1)

    # o3-mini
    ax.errorbar(x[i] + width, average_areas[i, 2], 
                yerr=[[area_errors[i, 2][0]], [area_errors[i, 2][1]]], 
                fmt="none", ecolor="black", elinewidth=1, capsize=8, markeredgewidth=1)

# add labels, title, and legend
ax.set_ylabel("Area [um2]", fontsize=30)
ax.set_xlabel("Benchmarks", fontsize=30)
ax.set_xticks(x)
plt.yticks(fontsize=30)
ax.set_ylim([1000,100000])
ax.set_xlim([-0.18, 3.92])
ax.set_xticklabels(pretty_names, fontsize=25)
#ax.legend(fontsize=30)
plt.tight_layout()

# save the plot as pdf
plt.savefig("average_area_comparison.pdf")


##########################
### PLOT AVERAGE COSTS ###
##########################

# calculate average cost
average_costs = []
cost_errors = []

for benchmark in benchmarks:

    deepseek_chat_avg_cost = benchmark_dict[benchmark].deepseek_chat_cost if benchmark_dict[benchmark].deepseek_chat_cost != 999999 else 0
    deepseek_chat_min_cost = benchmark_dict[benchmark].min_deepseek_chat_cost if benchmark_dict[benchmark].min_deepseek_chat_cost != 999999 else 0
    deepseek_chat_max_cost = benchmark_dict[benchmark].max_deepseek_chat_cost if benchmark_dict[benchmark].max_deepseek_chat_cost != 999999 else 0
    
    deepseek_reasoner_avg_cost = benchmark_dict[benchmark].deepseek_reasoner_cost if benchmark_dict[benchmark].deepseek_reasoner_cost != 999999 else 0
    deepseek_reasoner_min_cost = benchmark_dict[benchmark].min_deepseek_reasoner_cost if benchmark_dict[benchmark].min_deepseek_reasoner_cost != 999999 else 0
    deepseek_reasoner_max_cost = benchmark_dict[benchmark].max_deepseek_reasoner_cost if benchmark_dict[benchmark].max_deepseek_reasoner_cost != 999999 else 0
    
    o3_mini_avg_cost = benchmark_dict[benchmark].o3_mini_cost if benchmark_dict[benchmark].o3_mini_cost != 999999 else 0
    o3_mini_min_cost = benchmark_dict[benchmark].min_o3_mini_cost if benchmark_dict[benchmark].min_o3_mini_cost != 999999 else 0
    o3_mini_max_cost = benchmark_dict[benchmark].max_o3_mini_cost if benchmark_dict[benchmark].max_o3_mini_cost != 999999 else 0
    
    # append average costs
    average_costs.append((deepseek_chat_avg_cost, deepseek_reasoner_avg_cost, o3_mini_avg_cost))
    
    # calculate the errors (difference between min/max and the average)
    deepseek_chat_error = [deepseek_chat_avg_cost - deepseek_chat_min_cost if abs(deepseek_chat_avg_cost - deepseek_chat_min_cost) > 0.0000001 else 0, deepseek_chat_max_cost - deepseek_chat_avg_cost if abs(deepseek_chat_max_cost - deepseek_chat_avg_cost) > 0.0000001 else 0]
    deepseek_reasoner_error = [deepseek_reasoner_avg_cost - deepseek_reasoner_min_cost if abs(deepseek_reasoner_avg_cost - deepseek_reasoner_min_cost) > 0.0000001 else 0, deepseek_reasoner_max_cost - deepseek_reasoner_avg_cost if abs(deepseek_reasoner_max_cost - deepseek_reasoner_avg_cost) > 0.0000001 else 0]
    o3_mini_error = [o3_mini_avg_cost - o3_mini_min_cost if abs(o3_mini_avg_cost - o3_mini_min_cost) > 0.0000001 else 0, o3_mini_max_cost - o3_mini_avg_cost if abs(o3_mini_max_cost - o3_mini_avg_cost) > 0.0000001 else 0]

    # append error values
    cost_errors.append((deepseek_chat_error, deepseek_reasoner_error, o3_mini_error))

average_costs = np.array(average_costs)
cost_errors = np.array(cost_errors)

# create the figure and axis
fig, ax = plt.subplots(figsize=(17, 5))

# plot bars side by side
rects10 = ax.bar(x - width, average_costs[:, 0], width, edgecolor="black", color= colors[0], hatch=hatches[0], label="DeepSeek-V3")
rects11 = ax.bar(x, average_costs[:, 1], width, edgecolor="black", color= colors[1], hatch=hatches[1], label="DeepSeek-R1")
rects12 = ax.bar(x + width, average_costs[:, 2], width, edgecolor="black", color= colors[2], hatch=hatches[2], label="o3-mini")
# #ax.set_yscale("log")

# # add error bars for area range (min-max)
# for i in range(len(x)):

#     # deepseek-chat
#     ax.errorbar(x[i] - width, average_costs[i, 0], 
#                 yerr=[[cost_errors[i, 0][0]], [cost_errors[i, 0][1]]], 
#                 fmt="none", ecolor="black", elinewidth=1, capsize=8, markeredgewidth=1)
    
#     # deepseek-reasoner
#     ax.errorbar(x[i], average_costs[i, 1], 
#                 yerr=[[cost_errors[i, 1][0]], [cost_errors[i, 1][1]]], 
#                 fmt="none", ecolor="black", elinewidth=1, capsize=8, markeredgewidth=1)

#     # o3-mini
#     ax.errorbar(x[i] + width, average_costs[i, 2], 
#                 yerr=[[cost_errors[i, 2][0]], [cost_errors[i, 2][1]]], 
#                 fmt="none", ecolor="black", elinewidth=1, capsize=8, markeredgewidth=1)

# add labels, title, and legend
ax.set_ylabel("Cost [$]", fontsize=30)
ax.set_xlabel("Benchmarks", fontsize=30)
ax.set_xticks(x)
ax.set_xlim([-0.18, 3.92])
plt.yticks(fontsize=30)
ax.set_xticklabels(pretty_names, fontsize=25)
#ax.legend(fontsize=30)
plt.tight_layout()

# save the plot as pdf
plt.savefig("average_cost_comparison.pdf")


##########################
### PLOT AVERAGE TIMES ###
##########################

# calculate average time
average_times = []
time_errors = []

for benchmark in benchmarks:

    # average times
    deepseek_chat_avg_c2hlsc_time = benchmark_dict[benchmark].deepseek_chat_c2hlsc_time if benchmark_dict[benchmark].deepseek_chat_c2hlsc_time != 999999 else 0
    deepseek_chat_avg_agent_time = benchmark_dict[benchmark].deepseek_chat_agent_time if benchmark_dict[benchmark].deepseek_chat_agent_time != 999999 else 0
    deepseek_chat_avg_api_downtime = benchmark_dict[benchmark].deepseek_chat_api_downtime if benchmark_dict[benchmark].deepseek_chat_api_downtime != 999999 else 0

    deepseek_reasoner_avg_c2hlsc_time = benchmark_dict[benchmark].deepseek_reasoner_c2hlsc_time if benchmark_dict[benchmark].deepseek_reasoner_c2hlsc_time != 999999 else 0
    deepseek_reasoner_avg_agent_time = benchmark_dict[benchmark].deepseek_reasoner_agent_time if benchmark_dict[benchmark].deepseek_reasoner_agent_time != 999999 else 0
    deepseek_reasoner_avg_api_downtime = benchmark_dict[benchmark].deepseek_reasoner_api_downtime if benchmark_dict[benchmark].deepseek_reasoner_api_downtime != 999999 else 0

    o3_mini_avg_c2hlsc_time = benchmark_dict[benchmark].o3_mini_c2hlsc_time if benchmark_dict[benchmark].o3_mini_c2hlsc_time != 999999 else 0
    o3_mini_avg_agent_time = benchmark_dict[benchmark].o3_mini_agent_time if benchmark_dict[benchmark].o3_mini_agent_time != 999999 else 0
    o3_mini_avg_api_downtime = benchmark_dict[benchmark].o3_mini_api_downtime if benchmark_dict[benchmark].o3_mini_api_downtime != 999999 else 0

    deepseek_chat_avg_time = (deepseek_chat_avg_c2hlsc_time + deepseek_chat_avg_agent_time - deepseek_chat_avg_api_downtime) / 60
    deepseek_reasoner_avg_time = (deepseek_reasoner_avg_c2hlsc_time + deepseek_reasoner_avg_agent_time - deepseek_reasoner_avg_api_downtime) / 60
    o3_mini_avg_time = (o3_mini_avg_c2hlsc_time + o3_mini_avg_agent_time - o3_mini_avg_api_downtime) / 60

    average_times.append((deepseek_chat_avg_time, deepseek_reasoner_avg_time, o3_mini_avg_time))

    # # min times
    # deepseek_chat_min_c2hlsc_time = benchmark_dict[benchmark].min_deepseek_chat_c2hlsc_time if benchmark_dict[benchmark].min_deepseek_chat_c2hlsc_time != 999999 else 0
    # deepseek_chat_min_agent_time = benchmark_dict[benchmark].min_deepseek_chat_agent_time if benchmark_dict[benchmark].min_deepseek_chat_agent_time != 999999 else 0
    # deepseek_chat_min_api_downtime = benchmark_dict[benchmark].min_deepseek_chat_api_downtime if benchmark_dict[benchmark].min_deepseek_chat_api_downtime != 999999 else 0

    # deepseek_reasoner_min_c2hlsc_time = benchmark_dict[benchmark].min_deepseek_reasoner_c2hlsc_time if benchmark_dict[benchmark].min_deepseek_reasoner_c2hlsc_time != 999999 else 0
    # deepseek_reasoner_min_agent_time = benchmark_dict[benchmark].min_deepseek_reasoner_agent_time if benchmark_dict[benchmark].min_deepseek_reasoner_agent_time != 999999 else 0
    # deepseek_reasoner_min_api_downtime = benchmark_dict[benchmark].min_deepseek_reasoner_api_downtime if benchmark_dict[benchmark].min_deepseek_reasoner_api_downtime != 999999 else 0

    # o3_mini_min_c2hlsc_time = benchmark_dict[benchmark].min_o3_mini_c2hlsc_time if benchmark_dict[benchmark].min_o3_mini_c2hlsc_time != 999999 else 0
    # o3_mini_min_agent_time = benchmark_dict[benchmark].min_o3_mini_agent_time if benchmark_dict[benchmark].min_o3_mini_agent_time != 999999 else 0
    # o3_mini_min_api_downtime = benchmark_dict[benchmark].min_o3_mini_api_downtime if benchmark_dict[benchmark].min_o3_mini_api_downtime != 999999 else 0

    # deepseek_chat_min_time = (deepseek_chat_min_c2hlsc_time + deepseek_chat_min_agent_time - deepseek_chat_min_api_downtime) / 60
    # deepseek_reasoner_min_time = (deepseek_reasoner_min_c2hlsc_time + deepseek_reasoner_min_agent_time - deepseek_reasoner_min_api_downtime) / 60
    # o3_mini_min_time = (o3_mini_min_c2hlsc_time + o3_mini_min_agent_time - o3_mini_min_api_downtime) / 60

    # # max times
    # deepseek_chat_max_c2hlsc_time = benchmark_dict[benchmark].max_deepseek_chat_c2hlsc_time if benchmark_dict[benchmark].max_deepseek_chat_c2hlsc_time != 999999 else 0
    # deepseek_chat_max_agent_time = benchmark_dict[benchmark].max_deepseek_chat_agent_time if benchmark_dict[benchmark].max_deepseek_chat_agent_time != 999999 else 0
    # deepseek_chat_max_api_downtime = benchmark_dict[benchmark].max_deepseek_chat_api_downtime if benchmark_dict[benchmark].max_deepseek_chat_api_downtime != 999999 else 0

    # deepseek_reasoner_max_c2hlsc_time = benchmark_dict[benchmark].max_deepseek_reasoner_c2hlsc_time if benchmark_dict[benchmark].max_deepseek_reasoner_c2hlsc_time != 999999 else 0
    # deepseek_reasoner_max_agent_time = benchmark_dict[benchmark].max_deepseek_reasoner_agent_time if benchmark_dict[benchmark].max_deepseek_reasoner_agent_time != 999999 else 0
    # deepseek_reasoner_max_api_downtime = benchmark_dict[benchmark].max_deepseek_reasoner_api_downtime if benchmark_dict[benchmark].max_deepseek_reasoner_api_downtime != 999999 else 0

    # o3_mini_max_c2hlsc_time = benchmark_dict[benchmark].max_o3_mini_c2hlsc_time if benchmark_dict[benchmark].max_o3_mini_c2hlsc_time != 999999 else 0
    # o3_mini_max_agent_time = benchmark_dict[benchmark].max_o3_mini_agent_time if benchmark_dict[benchmark].max_o3_mini_agent_time != 999999 else 0
    # o3_mini_max_api_downtime = benchmark_dict[benchmark].max_o3_mini_api_downtime if benchmark_dict[benchmark].max_o3_mini_api_downtime != 999999 else 0

    # deepseek_chat_max_time = (deepseek_chat_max_c2hlsc_time + deepseek_chat_max_agent_time - deepseek_chat_max_api_downtime) / 60
    # deepseek_reasoner_max_time = (deepseek_reasoner_max_c2hlsc_time + deepseek_reasoner_max_agent_time - deepseek_reasoner_max_api_downtime) / 60
    # o3_mini_max_time = (o3_mini_max_c2hlsc_time + o3_mini_max_agent_time - o3_mini_max_api_downtime) / 60
    
    # # calculate the errors (difference between min/max and the average)
    # deepseek_chat_error = [deepseek_chat_avg_time - deepseek_chat_min_time, deepseek_chat_max_time - deepseek_chat_avg_time]
    # deepseek_reasoner_error = [deepseek_reasoner_avg_time - deepseek_reasoner_min_time, deepseek_reasoner_max_time - deepseek_reasoner_avg_time]
    # o3_mini_error = [o3_mini_avg_time - o3_mini_min_time, o3_mini_max_time - o3_mini_avg_time]

    # # append error values
    # time_errors.append((deepseek_chat_error, deepseek_reasoner_error, o3_mini_error))

average_times = np.array(average_times)
# time_errors = np.array(time_errors)

# create the figure and axis
fig, ax = plt.subplots(figsize=(17, 5))

# plot bars side by side
rects13 = ax.bar(x - width, average_times[:, 0], width, edgecolor="black", color= colors[0], hatch=hatches[0], label="DeepSeek-V3")
rects14 = ax.bar(x, average_times[:, 1], width, edgecolor="black", color= colors[1], hatch=hatches[1], label="DeepSeek-R1")
rects15 = ax.bar(x + width, average_times[:, 2], width, edgecolor="black", color= colors[2], hatch=hatches[2], label="o3-mini")
# #ax.set_yscale("log")

# add error bars for area range (min-max)
# for i in range(len(x)):

#     # deepseek-chat
#     ax.errorbar(x[i] - width, average_times[i, 0], 
#                 yerr=[[time_errors[i, 0][0]], [time_errors[i, 0][1]]], 
#                 fmt="none", ecolor="black", elinewidth=1, capsize=8, markeredgewidth=1)
    
#     # deepseek-reasoner
#     ax.errorbar(x[i], average_times[i, 1], 
#                 yerr=[[time_errors[i, 1][0]], [time_errors[i, 1][1]]], 
#                 fmt="none", ecolor="black", elinewidth=1, capsize=8, markeredgewidth=1)

#     # o3-mini
#     ax.errorbar(x[i] + width, average_times[i, 2], 
#                 yerr=[[time_errors[i, 2][0]], [time_errors[i, 2][1]]], 
#                 fmt="none", ecolor="black", elinewidth=1, capsize=8, markeredgewidth=1)

# add labels, title, and legend
ax.set_ylabel("Time [min]", fontsize=30)
ax.set_xlabel("Benchmarks", fontsize=30)
ax.set_xticks(x)
ax.set_xlim([-0.18, 3.92])
plt.yticks(fontsize=30)
ax.set_xticklabels(pretty_names, fontsize=25)
#ax.legend(fontsize=30)
plt.tight_layout()

# save the plot as pdf
plt.savefig("average_time_comparison.pdf")


####################
### PLOT ACTIONS ###
####################

model_names = ["DeepSeek-V3", "DeepSeek-R1", "o3-mini"]
x = np.arange(len(model_names))
x = x*0.4
print(x)
deepseek_chat_synthesis_calls = 0
deepseek_reasoner_synthesis_calls = 0
o3_mini_synthesis_calls = 0

deepseek_chat_python_calls = 0
deepseek_reasoner_python_calls = 0
o3_mini_python_calls = 0

deepseek_chat_profile_calls = 0
deepseek_reasoner_profile_calls = 0
o3_mini_profile_calls = 0

deepseek_chat_inspect_calls = 0
deepseek_reasoner_inspect_calls = 0
o3_mini_inspect_calls = 0

for benchmark in benchmarks:
    
    deepseek_chat_synthesis_calls += benchmark_dict[benchmark].deepseek_chat_synthesis_calls
    deepseek_reasoner_synthesis_calls += benchmark_dict[benchmark].deepseek_reasoner_synthesis_calls
    o3_mini_synthesis_calls += benchmark_dict[benchmark].o3_mini_synthesis_calls

    deepseek_chat_python_calls += benchmark_dict[benchmark].deepseek_chat_python_calls
    deepseek_reasoner_python_calls += benchmark_dict[benchmark].deepseek_reasoner_python_calls
    o3_mini_python_calls += benchmark_dict[benchmark].o3_mini_python_calls

    deepseek_chat_profile_calls += benchmark_dict[benchmark].deepseek_chat_profile_calls
    deepseek_reasoner_profile_calls += benchmark_dict[benchmark].deepseek_reasoner_profile_calls
    o3_mini_profile_calls += benchmark_dict[benchmark].o3_mini_profile_calls

    deepseek_chat_inspect_calls += benchmark_dict[benchmark].deepseek_chat_inspect_calls
    deepseek_reasoner_inspect_calls += benchmark_dict[benchmark].deepseek_reasoner_inspect_calls
    o3_mini_inspect_calls += benchmark_dict[benchmark].o3_mini_inspect_calls

deepseek_chat_actions = [deepseek_chat_synthesis_calls, deepseek_chat_python_calls, deepseek_chat_profile_calls, deepseek_chat_inspect_calls]
deepseek_reasoner_actions = [deepseek_reasoner_synthesis_calls, deepseek_reasoner_python_calls, deepseek_reasoner_profile_calls, deepseek_reasoner_inspect_calls]
o3_mini_actions = [o3_mini_synthesis_calls, o3_mini_python_calls, o3_mini_profile_calls, o3_mini_inspect_calls]

actions = np.array([deepseek_chat_actions, deepseek_reasoner_actions, o3_mini_actions])

# create the figure and axis
fig, ax = plt.subplots(figsize=(17, 5))

# plot bars side by side
rects16 = ax.bar(x - 3 * width / 2, actions[:, 0], width, edgecolor="black", color= colors[0], hatch=hatches[0], label="Synthesis")
rects17 = ax.bar(x - width / 2, actions[:, 1], width, edgecolor="black", color= colors[1], hatch=hatches[1], label="Python")
rects18 = ax.bar(x + width / 2, actions[:, 3], width, edgecolor="black", color= colors[2], hatch=hatches[2], label="Inspect")
rects19 = ax.bar(x + 3 * width / 2, actions[:, 2], width, edgecolor="black", color= colors[3], label="Profile")
# #ax.set_yscale("log")

# add error bars for area range (min-max)
# for i in range(len(x)):

#     # deepseek-chat
#     ax.errorbar(x[i] - width, average_times[i, 0], 
#                 yerr=[[time_errors[i, 0][0]], [time_errors[i, 0][1]]], 
#                 fmt="none", ecolor="black", elinewidth=1, capsize=8, markeredgewidth=1)
    
#     # deepseek-reasoner
#     ax.errorbar(x[i], average_times[i, 1], 
#                 yerr=[[time_errors[i, 1][0]], [time_errors[i, 1][1]]], 
#                 fmt="none", ecolor="black", elinewidth=1, capsize=8, markeredgewidth=1)

#     # o3-mini
#     ax.errorbar(x[i] + width, average_times[i, 2], 
#                 yerr=[[time_errors[i, 2][0]], [time_errors[i, 2][1]]], 
#                 fmt="none", ecolor="black", elinewidth=1, capsize=8, markeredgewidth=1)

# add labels, title, and legend
ax.set_ylabel("Calls", fontsize=30)
ax.set_xlabel("Models", fontsize=30)
ax.set_xticks(x)
plt.yticks(fontsize=30)
ax.set_xlim([-0.21, 0.92])
ax.set_xticklabels(model_names, fontsize=30)
ax.legend(fontsize=30, ncol=2, loc="upper center")
plt.tight_layout()

# save the plot as pdf
plt.savefig("actions_comparison.pdf")