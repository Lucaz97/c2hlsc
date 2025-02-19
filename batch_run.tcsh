#! /bin/tcsh
# aes sha256 present des ascon monobit overlapping quicksort runs block cusums
# ["claude-3-5-sonnet-20240620",, "claude-3-5-haiku-20241022","gpt-4o-mini","gpt-4-turbo-2024-04-09", "gpt-3.5-turbo-0125", "gpt-4o", "adaptive", "o3-mini", "deepseek-chat", "deepseek-reasoner"]
set num_runs = 1
set models = ( gpt-4o )
set benchmarks = ( ascon )

set benchmarks2 = (  aes )
set models2 = ( claude-3-5-sonnet-20240620 adaptive )

foreach benchmark ($benchmarks)
    foreach model ($models)
        foreach run (`seq 1 $num_runs`)
            set log_file = "run_${benchmark}_${model}_${run}.log"
            set command = "python3.11 src/c2hlsc.py inputs/${benchmark}/config_${benchmark}.yaml --model ${model} > ${log_file}"
            echo "Running: ${command}"
            eval ${command}
        end
    end
end

