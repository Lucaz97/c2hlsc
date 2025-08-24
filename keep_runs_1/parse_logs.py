import os
import glob
from pprint import pprint

# get all files that end with .log
log_files = glob.glob('*.log')
class BenchOptData:
    def __init__(self, bench_name, func_name, model_name, area, latency):
        self.bench_name = bench_name
        self.func_name = func_name
        self.model_name = model_name
        self.area = area
        self.latency = latency
    
    def __eq__(self, value):
        return self.bench_name == value.bench_name and self.func_name == value.func_name and self.model_name == value.model_name and self.area == value.area and self.latency == value.latency

    def __repr__(self):
        return f'{self.bench_name}, {self.func_name}, {self.model_name}, {self.area}, {self.latency}'
        
    def __hash__(self):
        return hash((self.bench_name, self.func_name, self.model_name, self.area, self.latency))
    
data_points = {}

for log_file in log_files:
    with open(log_file, 'r') as f:
        lines = f.readlines()
        start = 0

        for line in lines:
            if not start and 'The application has the following functions with their respective options:' in line:
                start = 1
                continue
            if start:
                if not line.startswith("Option"):
                    start = 0
                    break
                model = log_file.split("-")[0].split("_")[-1] +"-" +log_file.split("-")[1].split("_")[0]
                benchmark = log_file.split(model)[0][4:-1]
                function = line.split(' ')[2]
                if function.endswith("_hls"): function = function[:-4]
                area = line.split(',')[0].split(' ')[-1]
                latency = line.split(',')[1].split(' ')[-1]
                data = BenchOptData(benchmark, function, model, area, latency)
                #print(data)
                if benchmark not in data_points:
                    data_points[benchmark] = []
                data_points[benchmark].append(data)
    
for key, value in data_points.items():
    data_points[key] = list(set(value))

#pprint(data_points)

# for every benchmark, plot a scaterplot of area vs latency, different color for different functions, different shape for different models
import matplotlib.pyplot as plt
from matplotlib.lines import Line2D
import numpy as np
plt.style.use("seaborn-v0_8-talk")

# array with many colors as we dont know how many functions we have
colors = ['red', 'blue', 'green', 'purple', 'orange', 'pink', 'cyan', 'magenta']

shapes = ['o', 'P', '^']
for key, value in data_points.items():
    fig, ax = plt.subplots(figsize=(16, 7))

    # get list of functions for this benchmark
    functions = []
    for data in value:
        if data.func_name not in functions:
            functions.append(data.func_name)
    # create legend for functions
    legend_elements = []
    for i, func in enumerate(functions):
        legend_elements.append(Line2D([0], [0], marker="s", color='w', label=func, markerfacecolor=colors[i], markersize=25))

    # creat legend for models
    legend_elements.append(Line2D([0], [0], marker=shapes[0], color='w', label='o3-mini', markerfacecolor='k', markersize=25))
    legend_elements.append(Line2D([0], [0], marker=shapes[1], color='w', label='deepseek-chat', markerfacecolor='k', markersize=25))
    legend_elements.append(Line2D([0], [0], marker=shapes[2], color='w', label='deepseek-reasoner', markerfacecolor='k', markersize=25))

    for data in value:
        # color taken from colors array based on function index
        color = colors[functions.index(data.func_name)]
        if data.model_name == 'o3-mini':
            ax.scatter(float(data.area), float(data.latency), c=color, marker=shapes[0], s =180, edgecolors='k')
        elif data.model_name == 'deepseek-chat':
            ax.scatter(float(data.area), float(data.latency), c=color, marker=shapes[1], s =180, edgecolors='k')
        elif data.model_name == 'deepseek-reasoner' or data.model_name == 'hyperbolic-reasoner':
            ax.scatter(float(data.area), float(data.latency), c=color, marker=shapes[2], s =180, edgecolors='k')
    
    ax.legend(handles=legend_elements, loc='upper right', ncols=2, fontsize=25, labelspacing=0.1)

    ax.set_xlabel('Area', fontsize=32)
    ax.set_ylabel('Latency', fontsize=32)
    plt.yticks(fontsize=32)
    plt.xticks(fontsize=32)
    plt.tight_layout()
    #save plot as pdf
    plt.savefig(key + ".pdf")