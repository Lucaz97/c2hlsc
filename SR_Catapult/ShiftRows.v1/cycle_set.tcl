
# Loop constraints
directive set /ShiftRows/core/core:rlp CSTEPS_FROM {{. == 1}}
directive set /ShiftRows/core/core:rlp/main CSTEPS_FROM {{. == 2} {.. == 1}}

# IO operation constraints
directive set /ShiftRows/core/core:rlp/main/io_read(state:rsc.@) CSTEPS_FROM {{.. == 1}}
directive set /ShiftRows/core/core:rlp/main/io_write(state:rsc.@)#11 CSTEPS_FROM {{.. == 1}}

# Sync operation constraints

# Real operation constraints

# Probe constraints
