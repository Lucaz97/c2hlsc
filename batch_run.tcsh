#! /bin/tcsh
# aes sha256 present des ascon monobit overlapping quicksort runs block cusums
# ["claude-3-5-sonnet-20240620",, "claude-3-5-haiku-20241022","gpt-4o-mini","gpt-4-turbo-2024-04-09", "gpt-3.5-turbo-0125", "gpt-4o", "adaptive", "o3-mini", "deepseek-chat", "deepseek-reasoner"]
set num_runs = 5
set models = ( hyperbolic-reasoner ) #deepseek-reasoner )
set benchmarks = ( two_parallel four_sequential four_parallel repeated_four_p repeated_four_s two_sequential kmp aes sha256 present )

set print_only = 0  # Use numeric 1/0 instead of "true"/"false"
set use_checkpoint = 1
set nonomatch

foreach benchmark ($benchmarks)
    foreach model ($models)
        foreach run (`seq 1 $num_runs`)
            set log_file = "run_${benchmark}_${model}_${run}.log"
            set files = ( tmp*/${benchmark}_${model}_cfg.pkl )

            # Check if the pattern expanded to actual files
            if ( $#files == 1 && "$files[1]" == "tmp*/${benchmark}_${model}_cfg.pkl" ) then
                echo "File does not exist."
                set found = 0
            else
                # Extract the first matched path (assuming only one exists)
                set checkpoint = "$files[1]"
                echo "File found: $checkpoint"
                set found = 1
            endif
            
            # Checkpoint condition
            if ($use_checkpoint && $found) then
                set command = "python3.11 src/c2hlsc.py inputs/${benchmark}/config_${benchmark}.yaml --from_saved ${checkpoint} --model ${model} > ${log_file}"
            else
                set command = "python3.11 src/c2hlsc.py inputs/${benchmark}/config_${benchmark}.yaml --model ${model} > ${log_file}"
            endif

            echo "Running: $command"
            
            # Execution condition
            if (! $print_only) then
                eval "$command"
            endif
        end
    end
end