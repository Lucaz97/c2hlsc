#!/bin/tcsh
source ~/tcshrc_mentor_local

# ---- USER CONFIG ----
set CMD = "python3.11 src/c2hlsc.py inputs/sampler_falcon/config_samplerz.yaml --model gpt-4o > output_sampler_falcon_dc_area_4o"
set FILE = "output_sampler_falcon_dc_area_4o"
# --------------------

set i = 10
while ( $i <= 20 )

    echo "Iteration $i"

    # Run the command
    eval $CMD

    # Rename the file if it exists
    if ( -e $FILE ) then
        mv $FILE ${FILE}_iteration${i}.txt
    else
        echo "Warning: $FILE not found in iteration $i"
    endif

    @ i++
end
