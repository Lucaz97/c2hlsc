import os

benchmarks = ["Cipher", "sha256_update", "Runs", "quickSort", "present80_encryptBlock", "Overlapping", "Frequency", "des_crypt", "CumulativeSums", "BlockFrequency"]
models = ["adaptive", "claude-3-5-sonnet-20240620"]

# get folders in this folder
folders = [f for f in os.listdir('.') if os.path.isdir(f)]

class Benchmark:
    def __init__(self, name):
        self.adaptive_success = 0
        self.claude_success = 0
        self.name = name
        self.input_tokens_4o = 0
        self.min_input_tokens_4o = 99999
        self.max_input_tokens_4o = 0
        self.input_tokens_mini = 0
        self.min_input_tokens_mini = 99999
        self.max_input_tokens_mini = 0
        self.input_tokens_claude = 0
        self.min_input_tokens_claude = 99999
        self.max_input_tokens_claude = 0
        self.output_tokens_4o = 0
        self.min_output_tokens_4o = 99999
        self.max_output_tokens_4o = 0
        self.output_tokens_mini = 0
        self.min_output_tokens_mini = 99999
        self.max_output_tokens_mini = 0
        self.output_tokens_claude = 0
        self.min_output_tokens_claude = 99999
        self.max_output_tokens_claude = 0
        self._4o_runs = 0
        self.min_4o_runs = 99999
        self.max_4o_runs = 0
        self.mini_runs = 0
        self.min_mini_runs = 99999
        self.max_mini_runs = 0
        self.claude_runs = 0
        self.min_claude_runs = 99999
        self.max_claude_runs = 0
        self.adaptive_hls = 0
        self.min_adaptive_hls = 99999
        self.max_adaptive_hls = 0
        self.adaptive_compile = 0
        self.min_adaptive_compile = 99999
        self.max_adaptive_compile = 0
        self.adaptive_area = 0
        self.min_adaptive_area = 99999
        self.max_adaptive_area = 0
        self.adaptive_latency = 0
        self.min_adaptive_latency = 99999
        self.max_adaptive_latency = 0
        self.claude_hls = 0
        self.min_claude_hls = 99999
        self.max_claude_hls = 0
        self.claude_compile = 0
        self.min_claude_compile = 99999
        self.max_claude_compile = 0
        self.claude_area = 0
        self.min_claude_area = 99999
        self.max_claude_area = 0
        self.claude_latency = 0
        self.min_claude_latency = 99999
        self.max_claude_latency = 0
        self.claude_cost = 0
        self.min_claude_cost = 99999
        self.max_claude_cost = 0
        self.adaptive_cost = 0
        self.min_adaptive_cost = 99999
        self.max_adaptive_cost = 0

b_dict = {b: Benchmark(b) for b in benchmarks}

for folder in folders:
    #print(folder)
    for b in benchmarks:
        if b in folder:
            # check log exists
            log = folder+f"/{b}.log"
            #print(log)
            # check for a .c file in the folder
            c_file = folder+f"/{b}_llm_opt.c"
            if not os.path.exists(c_file):
                continue
            if os.path.exists(log):
                with open(log, "r") as f:
                    for line in f:
                        if "gpt-4o-mini runs:" in line:
                            runs = int(line.split(":")[1].strip())
                            b_dict[b].mini_runs += runs
                            b_dict[b].min_mini_runs = min(b_dict[b].min_mini_runs, runs)
                            b_dict[b].max_mini_runs = max(b_dict[b].max_mini_runs, runs)
                        if "gpt-4o runs:" in line:
                            runs = int(line.split(":")[1].strip())
                            b_dict[b]._4o_runs += runs
                            b_dict[b].min_4o_runs = min(b_dict[b].min_4o_runs, runs)
                            b_dict[b].max_4o_runs = max(b_dict[b].max_4o_runs, runs)
                        if "claude-3-5-sonnet-20240620 runs:" in line:
                            runs = int(line.split(":")[1].strip())
                            b_dict[b].claude_runs += runs
                            b_dict[b].min_claude_runs = min(b_dict[b].min_claude_runs, runs)
                            b_dict[b].max_claude_runs = max(b_dict[b].max_claude_runs, runs)
                        if "HLS runs:" in line:
                            runs = int(line.split(":")[1].strip())
                            if "claude" in folder:
                                b_dict[b].claude_hls += runs
                                b_dict[b].min_claude_hls = min(b_dict[b].min_claude_hls, runs)
                                b_dict[b].max_claude_hls = max(b_dict[b].max_claude_hls, runs)
                            else: 
                                b_dict[b].adaptive_hls += runs
                                b_dict[b].min_adaptive_hls = min(b_dict[b].min_adaptive_hls, runs)
                                b_dict[b].max_adaptive_hls = max(b_dict[b].max_adaptive_hls, runs)
                        if "Compile runs:" in line:
                            runs = int(line.split(":")[1].strip())
                            if "claude" in folder:
                                b_dict[b].claude_compile += runs
                                b_dict[b].min_claude_compile = min(b_dict[b].min_claude_compile, runs)
                                b_dict[b].max_claude_compile = max(b_dict[b].max_claude_compile, runs)
                            else:
                                b_dict[b].adaptive_compile += runs
                                b_dict[b].min_adaptive_compile = min(b_dict[b].min_adaptive_compile, runs)
                                b_dict[b].max_adaptive_compile = max(b_dict[b].max_adaptive_compile, runs)
                        if "gpt-4o-mini input tokens:" in line: 
                            tokens = int(line.split(":")[1].strip())
                            b_dict[b].input_tokens_mini += tokens
                            b_dict[b].min_input_tokens_mini = min(b_dict[b].min_input_tokens_mini, tokens)
                            b_dict[b].max_input_tokens_mini = max(b_dict[b].max_input_tokens_mini, tokens)
                        if "gpt-4o input tokens:" in line:
                            tokens = int(line.split(":")[1].strip())
                            b_dict[b].input_tokens_4o += tokens
                            b_dict[b].min_input_tokens_4o = min(b_dict[b].min_input_tokens_4o, tokens)
                            b_dict[b].max_input_tokens_4o = max(b_dict[b].max_input_tokens_4o, tokens)
                        if "claude-3-5-sonnet-20240620 input tokens:" in line:
                            tokens = int(line.split(":")[1].strip())
                            b_dict[b].input_tokens_claude += tokens
                            b_dict[b].min_input_tokens_claude = min(b_dict[b].min_input_tokens_claude, tokens)
                            b_dict[b].max_input_tokens_claude = max(b_dict[b].max_input_tokens_claude, tokens)
                        if "gpt-4o-mini output tokens:" in line:
                            tokens = int(line.split(":")[1].strip())
                            b_dict[b].output_tokens_mini += tokens
                            b_dict[b].min_output_tokens_mini = min(b_dict[b].min_output_tokens_mini, tokens)
                            b_dict[b].max_output_tokens_mini = max(b_dict[b].max_output_tokens_mini, tokens)
                        if "gpt-4o output tokens:" in line:
                            tokens = int(line.split(":")[1].strip())
                            b_dict[b].output_tokens_4o += tokens
                            b_dict[b].min_output_tokens_4o = min(b_dict[b].min_output_tokens_4o, tokens)
                            b_dict[b].max_output_tokens_4o = max(b_dict[b].max_output_tokens_4o, tokens)
                        if "claude-3-5-sonnet-20240620 output tokens:" in line:
                            tokens = int(line.split(":")[1].strip())
                            b_dict[b].output_tokens_claude += tokens
                            b_dict[b].min_output_tokens_claude = min(b_dict[b].min_output_tokens_claude, tokens)
                            b_dict[b].max_output_tokens_claude = max(b_dict[b].max_output_tokens_claude, tokens)
                        if "Total Area Score:" in line:
                            area = float(line.split(":")[1].strip().split(" ")[-1])
                            if "claude" in folder:
                                b_dict[b].claude_success += 1
                                b_dict[b].claude_area += area
                                b_dict[b].min_claude_area = min(b_dict[b].min_claude_area, area)
                                b_dict[b].max_claude_area = max(b_dict[b].max_claude_area, area)
                            else:
                                b_dict[b].adaptive_success += 1
                                b_dict[b].adaptive_area += area
                                b_dict[b].min_adaptive_area = min(b_dict[b].min_adaptive_area, area)
                                b_dict[b].max_adaptive_area = max(b_dict[b].max_adaptive_area, area)
                        if "Design Total:" in line:
                            latency = int(" ".join(line.split(":")[1].strip().split(" ")[1:]).strip().split(" ")[0])
                            if "claude" in folder:
                                b_dict[b].claude_latency += latency
                                b_dict[b].min_claude_latency = min(b_dict[b].min_claude_latency, latency)
                                b_dict[b].max_claude_latency = max(b_dict[b].max_claude_latency, latency)
                            else:
                                b_dict[b].adaptive_latency += latency
                                b_dict[b].min_adaptive_latency = min(b_dict[b].min_adaptive_latency, latency)
                                b_dict[b].max_adaptive_latency = max(b_dict[b].max_adaptive_latency, latency)

for b in benchmarks:

    # calc averages based on success
    if b_dict[b].claude_success > 0:
        b_dict[b].claude_runs /= b_dict[b].claude_success
        b_dict[b].claude_hls /= b_dict[b].claude_success
        b_dict[b].claude_compile /= b_dict[b].claude_success
        b_dict[b].input_tokens_claude /= b_dict[b].claude_success
        b_dict[b].output_tokens_claude /= b_dict[b].claude_success
        b_dict[b].claude_area /= b_dict[b].claude_success
        b_dict[b].claude_latency /= b_dict[b].claude_success
    if b_dict[b].adaptive_success > 0:
        b_dict[b].mini_runs /= b_dict[b].adaptive_success
        b_dict[b]._4o_runs /= b_dict[b].adaptive_success
        b_dict[b].adaptive_hls /= b_dict[b].adaptive_success
        b_dict[b].adaptive_compile /= b_dict[b].adaptive_success
        b_dict[b].input_tokens_mini /= b_dict[b].adaptive_success
        b_dict[b].input_tokens_4o /= b_dict[b].adaptive_success
        b_dict[b].output_tokens_mini /= b_dict[b].adaptive_success
        b_dict[b].output_tokens_4o /= b_dict[b].adaptive_success
        b_dict[b].adaptive_area /= b_dict[b].adaptive_success
        b_dict[b].adaptive_latency /= b_dict[b].adaptive_success

    # calc cost
    claude_cost_per_million_input_tokens = 3
    claude_cost_per_million_output_tokens = 15
    mini_cost_per_million_input_tokens = 0.150
    mini_cost_per_million_output_tokens = 0.60
    _4o_cost_per_million_input_tokens = 2.5
    _4o_cost_per_million_output_tokens = 10
    
    if b_dict[b].min_input_tokens_claude != 99999:
        b_dict[b].claude_cost = (b_dict[b].input_tokens_claude * claude_cost_per_million_input_tokens + b_dict[b].output_tokens_claude * claude_cost_per_million_output_tokens) / 1000000
        b_dict[b].min_claude_cost = (b_dict[b].min_input_tokens_claude * claude_cost_per_million_input_tokens + b_dict[b].min_output_tokens_claude * claude_cost_per_million_output_tokens) / 1000000
        b_dict[b].max_claude_cost = (b_dict[b].max_input_tokens_claude * claude_cost_per_million_input_tokens + b_dict[b].max_output_tokens_claude * claude_cost_per_million_output_tokens) / 1000000

    if b_dict[b].min_input_tokens_mini != 99999:
        b_dict[b].adaptive_cost = (b_dict[b].input_tokens_mini * mini_cost_per_million_input_tokens + b_dict[b].output_tokens_mini * mini_cost_per_million_output_tokens + b_dict[b].input_tokens_4o * _4o_cost_per_million_input_tokens + b_dict[b].output_tokens_4o * _4o_cost_per_million_output_tokens) / 1000000
        b_dict[b].min_adaptive_cost = (b_dict[b].min_input_tokens_mini * mini_cost_per_million_input_tokens + b_dict[b].min_output_tokens_mini * mini_cost_per_million_output_tokens + b_dict[b].min_input_tokens_4o * _4o_cost_per_million_input_tokens + b_dict[b].min_output_tokens_4o * _4o_cost_per_million_output_tokens) / 1000000
        b_dict[b].max_adaptive_cost = (b_dict[b].max_input_tokens_mini * mini_cost_per_million_input_tokens + b_dict[b].max_output_tokens_mini * mini_cost_per_million_output_tokens + b_dict[b].max_input_tokens_4o * _4o_cost_per_million_input_tokens + b_dict[b].max_output_tokens_4o * _4o_cost_per_million_output_tokens) / 1000000
# dump in a csv file
import csv
with open("parsed.csv", "w") as f:
    # header
    writer = csv.DictWriter(f, fieldnames=b_dict["Cipher"].__dict__.keys())
    writer.writeheader()
    for b in benchmarks:
        writer.writerow(b_dict[b].__dict__)

benchmarks = ["Cipher", "present80_encryptBlock", "sha256_update", "CumulativeSums", "Frequency",  "BlockFrequency", "Overlapping", "Runs", "quickSort", "des_crypt"]
pretty_names = ["AES", "Present","SHA256", "CuSums", "Monobit",  "Block", "Overlap", "Runs", "QS", "DES"]
# plots
import matplotlib.pyplot as plt
import numpy as np

# Calculate success rates
success_rates = []
for b in benchmarks:
    adaptive_success_rate = (b_dict[b].adaptive_success / 10) * 100
    claude_success_rate = (b_dict[b].claude_success / 10) * 100
    success_rates.append((adaptive_success_rate, claude_success_rate))

# Define the number of benchmarks and the width of the bars
x = np.arange(len(benchmarks))  # the label locations
width = 0.35  # the width of the bars
# Colors for each group
colors = ['#ffba49', '#20a39e']
hatches = ['/', '\\']
# Create the figure and axis
fig, ax = plt.subplots(figsize=(20, 5))
 # Set style
plt.style.use('seaborn-v0_8-talk')
# Plot bars side by side
rects1 = ax.bar(x - width/2, [rate[0] for rate in success_rates], width, edgecolor='black', hatch=hatches[0], color= colors[0], label='Adaptive')
rects2 = ax.bar(x + width/2, [rate[1] for rate in success_rates], width, edgecolor='black', hatch=hatches[1], color= colors[1], label='Claude')

# Add labels, title, and legend
ax.set_ylabel('Success Rate (%)',fontsize=24)
ax.set_xlabel('Benchmarks', fontsize=28)
ax.set_xticks(x)
plt.yticks(fontsize=24)
ax.set_xticklabels(pretty_names,fontsize=28)
ax.legend(fontsize=24)
plt.tight_layout()

# Save the plot as pdf
plt.savefig('success_rate_comparison.pdf')

# Calculate average latency
average_latencies = []
latency_errors = []

for b in benchmarks:
    adaptive_avg_latency = b_dict[b].adaptive_latency if b_dict[b].adaptive_latency != 99999 else 0
    claude_avg_latency = b_dict[b].claude_latency if b_dict[b].claude_latency != 99999 else 0
    adaptive_min_latency = b_dict[b].min_adaptive_latency if b_dict[b].min_adaptive_latency != 99999 else 0
    adaptive_max_latency = b_dict[b].max_adaptive_latency if b_dict[b].max_adaptive_latency != 99999 else 0
    claude_min_latency = b_dict[b].min_claude_latency if b_dict[b].min_claude_latency != 99999 else 0
    claude_max_latency = b_dict[b].max_claude_latency if b_dict[b].max_claude_latency != 99999 else 0

    # Append average latencies
    average_latencies.append((adaptive_avg_latency, claude_avg_latency))
    
    # Calculate the errors (difference between min/max and the average)
    adaptive_error = [adaptive_avg_latency - adaptive_min_latency, adaptive_max_latency - adaptive_avg_latency]
    claude_error = [claude_avg_latency - claude_min_latency, claude_max_latency - claude_avg_latency]
    
    # Append error values
    latency_errors.append((adaptive_error, claude_error))

average_latencies = np.array(average_latencies)
latency_errors = np.array(latency_errors)

# Create the figure and axis
fig, ax = plt.subplots(figsize=(20, 5))

# Plot bars side by side
rects3 = ax.bar(x - width/2, average_latencies[:, 0], width, edgecolor='black', hatch=hatches[0], color= colors[0], label='Adaptive')
rects4 = ax.bar(x + width/2, average_latencies[:, 1], width, edgecolor='black', hatch=hatches[1], color= colors[1], label='Claude')
ax.set_yscale('log')

# Add error bars for latency range (min-max)
for i in range(len(x)):
    # For Adaptive
    ax.errorbar(x[i] - width/2, average_latencies[i, 0], 
                yerr=[[latency_errors[i, 0][0]], [latency_errors[i, 0][1]]], 
                fmt='none', ecolor='black', elinewidth=2, capsize=15, markeredgewidth=2)
    
    # For Claude
    ax.errorbar(x[i] + width/2, average_latencies[i, 1], 
                yerr=[[latency_errors[i, 1][0]], [latency_errors[i, 1][1]]], 
                fmt='none', ecolor='black', elinewidth=2, capsize=15,markeredgewidth=2)

# Add labels, title, and legend
ax.set_ylabel('Latency [cycles]',fontsize=24)
ax.set_xlabel('Benchmarks', fontsize=28)
ax.set_xticks(x)
plt.yticks(fontsize=24)
ax.set_xticklabels(pretty_names,fontsize=28)
ax.legend(fontsize=24)
plt.tight_layout()

# Save the plot as pdf
plt.savefig('average_latency_comparison.pdf')

# Calculate average area
average_areas = []
area_errors = []

for b in benchmarks:
    adaptive_avg_area = b_dict[b].adaptive_area if b_dict[b].adaptive_area != 99999 else 0
    claude_avg_area = b_dict[b].claude_area if b_dict[b].claude_area != 99999 else 0
    adaptive_min_area = b_dict[b].min_adaptive_area if b_dict[b].min_adaptive_area != 99999 else 0
    adaptive_max_area = b_dict[b].max_adaptive_area if b_dict[b].max_adaptive_area != 99999 else 0
    claude_min_area = b_dict[b].min_claude_area if b_dict[b].min_claude_area != 99999 else 0
    claude_max_area = b_dict[b].max_claude_area if b_dict[b].max_claude_area != 99999 else 0

    # Append average areas
    average_areas.append((adaptive_avg_area, claude_avg_area))
    
    # Calculate the errors (difference between min/max and the average)
    adaptive_error = [adaptive_avg_area - adaptive_min_area if abs(adaptive_avg_area - adaptive_min_area) > 0.0000001 else 0, adaptive_max_area - adaptive_avg_area if abs(adaptive_max_area - adaptive_avg_area) > 0.0000001 else 0]
    claude_error = [claude_avg_area - claude_min_area if abs(claude_avg_area - claude_min_area) > 0.0000001 else 0, claude_max_area - claude_avg_area if abs(claude_max_area - claude_avg_area) > 0.0000001 else 0]
    
    # Append error values
    area_errors.append((adaptive_error, claude_error))

average_areas = np.array(average_areas)
area_errors = np.array(area_errors)

# Create the figure and axis
fig, ax = plt.subplots(figsize=(20, 5))

# Plot bars side by side
rects5 = ax.bar(x - width/2, average_areas[:, 0], width, edgecolor='black', hatch=hatches[0], color= colors[0], label='Adaptive')
rects6 = ax.bar(x + width/2, average_areas[:, 1], width, edgecolor='black', hatch=hatches[1], color= colors[1], label='Claude')
ax.set_yscale('log')

# Add error bars for area range (min-max)
for i in range(len(x)):
    # For Adaptive
    ax.errorbar(x[i] - width/2, average_areas[i, 0], 
                yerr=[[area_errors[i, 0][0]], [area_errors[i, 0][1]]], 
                fmt='none', ecolor='black', elinewidth=2, capsize=15, markeredgewidth=2)
    
    # For Claude
    ax.errorbar(x[i] + width/2, average_areas[i, 1], 
                yerr=[[area_errors[i, 1][0]], [area_errors[i, 1][1]]], 
                fmt='none', ecolor='black', elinewidth=2, capsize=15,markeredgewidth=2)

# Add labels, title, and legend
ax.set_ylabel('Area [um2]',fontsize=24)
ax.set_xlabel('Benchmarks', fontsize=28)
ax.set_xticks(x)
plt.yticks(fontsize=24)
ax.set_xticklabels(pretty_names,fontsize=28)
ax.legend(fontsize=24)
plt.tight_layout()

# Save the plot as pdf
plt.savefig('average_area_comparison.pdf')

# Calculate average cost
average_costs = []
cost_errors = []

for b in benchmarks:
    adaptive_avg_cost = b_dict[b].adaptive_cost if b_dict[b].adaptive_cost != 99999 else 0
    claude_avg_cost = b_dict[b].claude_cost if b_dict[b].claude_cost != 99999 else 0
    adaptive_min_cost = b_dict[b].min_adaptive_cost if b_dict[b].min_adaptive_cost != 99999 else 0
    adaptive_max_cost = b_dict[b].max_adaptive_cost if b_dict[b].max_adaptive_cost != 99999 else 0
    claude_min_cost = b_dict[b].min_claude_cost if b_dict[b].min_claude_cost != 99999 else 0
    claude_max_cost = b_dict[b].max_claude_cost if b_dict[b].max_claude_cost != 99999 else 0

    # Append average costs
    average_costs.append((adaptive_avg_cost, claude_avg_cost))
    
    # Calculate the errors (difference between min/max and the average)
    adaptive_error = [adaptive_avg_cost - adaptive_min_cost if abs(adaptive_avg_cost - adaptive_min_cost) > 0.0000001 else 0, adaptive_max_cost - adaptive_avg_cost if abs(adaptive_max_cost - adaptive_avg_cost) > 0.0000001 else 0]
    claude_error = [claude_avg_cost - claude_min_cost if abs(claude_avg_cost - claude_min_cost) > 0.0000001 else 0, claude_max_cost - claude_avg_cost if abs(claude_max_cost - claude_avg_cost) > 0.0000001 else 0]
    
    # Append error values
    cost_errors.append((adaptive_error, claude_error))

average_costs = np.array(average_costs)
cost_errors = np.array(cost_errors)

# Create the figure and axis
fig, ax = plt.subplots(figsize=(20, 5))

# Plot bars side by side
rects7 = ax.bar(x - width/2, average_costs[:, 0], width, edgecolor='black', hatch=hatches[0], color= colors[0], label='Adaptive')
rects8 = ax.bar(x + width/2, average_costs[:, 1], width, edgecolor='black', hatch=hatches[1], color= colors[1], label='Claude')
#ax.set_yscale('log')

# Add error bars for cost range (min-max)
for i in range(len(x)):
    # For Adaptive
    ax.errorbar(x[i] - width/2, average_costs[i, 0], 
                yerr=[[cost_errors[i, 0][0]], [cost_errors[i, 0][1]]], 
                fmt='none', ecolor='black', elinewidth=2, capsize=15, markeredgewidth=2)
    
    # For Claude
    ax.errorbar(x[i] + width/2, average_costs[i, 1], 
                yerr=[[cost_errors[i, 1][0]], [cost_errors[i, 1][1]]], 
                fmt='none', ecolor='black', elinewidth=2, capsize=15,markeredgewidth=2)

# Add labels, title, and legend
ax.set_ylabel('Cost [$]',fontsize=24)
ax.set_xlabel('Benchmarks', fontsize=28)
ax.set_xticks(x)
plt.yticks(fontsize=24)
ax.set_xticklabels(pretty_names,fontsize=28)
ax.legend(fontsize=24)
plt.tight_layout()

# Save the plot as pdf
plt.savefig('average_cost_comparison.pdf')