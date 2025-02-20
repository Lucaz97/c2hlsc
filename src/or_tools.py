from ortools.sat.python import cp_model

# Define the function options (all functions included)
options = {
    'AddRoundKey_hls': [
        {'area': 2131.9, 'latency': 7.0, 'throughput': 10.0},
        {'area': 2373.2, 'latency': 0.0, 'throughput': 2.0},
        {'area': 2135.7, 'latency': 4.0, 'throughput': 7.0},
        {'area': 2135.7, 'latency': 4.0, 'throughput': 7.0},
        {'area': 2373.2, 'latency': 0.0, 'throughput': 2.0}
    ],
    'SubBytes_hls': [
        {'area': 2404.3, 'latency': 18.0, 'throughput': 22.0},
        {'area': 1599.9, 'latency': 7.0, 'throughput': 11.0},
        {'area': 1179.0, 'latency': 15.0, 'throughput': 19.0},
        {'area': 1599.9, 'latency': 7.0, 'throughput': 11.0},
        {'area': 1179.0, 'latency': 15.0, 'throughput': 19.0}
    ],
    'ShiftRows_hls': [
        {'area': 23.9, 'latency': 0.0, 'throughput': 2.0},
        {'area': 23.9, 'latency': 0.0, 'throughput': 2.0},
        {'area': 23.9, 'latency': 0.0, 'throughput': 2.0},
        {'area': 23.9, 'latency': 0.0, 'throughput': 2.0},
        {'area': 23.9, 'latency': 0.0, 'throughput': 2.0}
    ],
    'xtime_hls': [
        {'area': 77.0, 'latency': 1.0, 'throughput': 2.0},
        {'area': 77.0, 'latency': 1.0, 'throughput': 2.0},
        {'area': 77.0, 'latency': 1.0, 'throughput': 2.0},
        {'area': 77.0, 'latency': 1.0, 'throughput': 2.0},
        {'area': 77.0, 'latency': 1.0, 'throughput': 2.0}
    ],
    'MixColumns_hls': [
        {'area': 866.6, 'latency': 0.0, 'throughput': 2.0},
        {'area': 639.9, 'latency': 3.0, 'throughput': 7.0},
        {'area': 866.6, 'latency': 0.0, 'throughput': 2.0},
        {'area': 866.6, 'latency': 0.0, 'throughput': 2.0},
        {'area': 866.6, 'latency': 0.0, 'throughput': 2.0}
    ],
    'Cipher_hls': [
        {'area': 17144.4, 'latency': 110.0, 'throughput': 112.0},
        {'area': 15202.3, 'latency': 130.0, 'throughput': 133.0},
        {'area': 15759.6, 'latency': 122.0, 'throughput': 124.0},
        {'area': 15759.6, 'latency': 122.0, 'throughput': 124.0},
        {'area': 15202.3, 'latency': 130.0, 'throughput': 133.0}
    ]
}

# Scale floating-point values to integers
SCALE_FACTOR = 10
for func, opts in options.items():
    for opt in opts:
        opt['area'] = int(opt['area'] * SCALE_FACTOR)
        opt['latency'] = int(opt['latency'] * SCALE_FACTOR)
        opt['throughput'] = int(opt['throughput'] * SCALE_FACTOR)

# Create the model
model = cp_model.CpModel()

# Create variables for each function option
selected_options = {}
for func, opts in options.items():
    selected_options[func] = model.NewIntVar(0, len(opts) - 1, f'{func}_option')

# Create variables for total area, latency, and throughput
total_area = model.NewIntVar(0, 20000 * SCALE_FACTOR, 'total_area')
total_latency = model.NewIntVar(0, 1000 * SCALE_FACTOR, 'total_latency')
total_throughput = model.NewIntVar(0, 1000 * SCALE_FACTOR, 'total_throughput')

# Add constraints for total area, latency, and throughput
area_contributions = []
latency_contributions = []
throughput_contributions = []

for func, opts in options.items():
    # Use Element constraint to dynamically index into the options
    area_contributions.append(model.NewIntVar(0, 20000 * SCALE_FACTOR, f'{func}_area'))
    latency_contributions.append(model.NewIntVar(0, 1000 * SCALE_FACTOR, f'{func}_latency'))
    throughput_contributions.append(model.NewIntVar(0, 1000 * SCALE_FACTOR, f'{func}_throughput'))

    # Add Element constraints
    model.AddElement(selected_options[func], [opt['area'] for opt in opts], area_contributions[-1])
    model.AddElement(selected_options[func], [opt['latency'] for opt in opts], latency_contributions[-1])
    model.AddElement(selected_options[func], [opt['throughput'] for opt in opts], throughput_contributions[-1])

# Sum up contributions
model.Add(total_area == sum(area_contributions))
model.Add(total_latency == sum(latency_contributions))
model.Add(total_throughput == sum(throughput_contributions))

# No hard constraints on area, latency, or throughput

# Minimize latency first, then area, and finally throughput
model.Minimize(total_latency * 1000000 + total_area * 1000 + total_throughput)

# Solve the model
solver = cp_model.CpSolver()
status = solver.Solve(model)

# Print the results
if status == cp_model.OPTIMAL or status == cp_model.FEASIBLE:
    print('Optimal combination found:')
    for func in options:
        option_index = solver.Value(selected_options[func])
        print(f'{func}: Option {option_index + 1}')
    print(f'Total Area: {solver.Value(total_area) / SCALE_FACTOR}')
    print(f'Total Latency: {solver.Value(total_latency) / SCALE_FACTOR}')
    print(f'Total Throughput: {solver.Value(total_throughput) / SCALE_FACTOR}')
else:
    print('No feasible solution found.')