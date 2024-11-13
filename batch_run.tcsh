#! /bin/tcsh
# aes sha256 present des ascon monobit overlapping quicksort runs block cusums

set num_runs = 1
set models = ( adaptive claude-3-5-sonnet-20240620 gpt-3.5-turbo-0125)
set benchmarks = ( monobit aes )

set benchmarks2 = (  aes )
set models2 = ( claude-3-5-sonnet-20240620 )

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

