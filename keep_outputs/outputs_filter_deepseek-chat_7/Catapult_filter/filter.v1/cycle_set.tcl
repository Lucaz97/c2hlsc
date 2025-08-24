
# Loop constraints
directive set /filter/core/core:rlp CSTEPS_FROM {{. == 0}}
directive set /filter/core/core:rlp/main CSTEPS_FROM {{. == 4} {.. == 0}}
directive set /filter/core/core:rlp/main/iir:for#1 CSTEPS_FROM {{. == 1} {.. == 2}}

# IO operation constraints
directive set /filter/core/core:rlp/main/input:io_read(input:rsc.@) CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/fir:for#1:io_write(fir_output:rsc.@) CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/iir:io_write(iir_output:rsc.@) CSTEPS_FROM {{.. == 3}}

# Sync operation constraints

# Real operation constraints
directive set /filter/core/core:rlp/main/fir:for#1:acc#4 CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/fir:for#1:acc#6 CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/fir:for#1:acc CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/fir:for#1:acc#5 CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/fir:for#1-5:acc#1 CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/iir:for#1/iir:for#1:mux CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/iir:for#1/iir:for#1:mux#1 CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/iir:for#1/iir:for#1:mul CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/iir:for#1/iir:for#1:acc#1 CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/iir:for#1/iir:for#1:acc#2 CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/iir:for#1/iir:for#2:mux CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/iir:for#1/iir:for#2:and CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/iir:for#1/iir:for#2:or CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/iir:for#1/iir:for#2:mul CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/iir:for#1/iir:for#2:acc#1 CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/iir:for#1/iir:for#2:acc#2 CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/iir:for#1/iir:for#1:acc CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/iir:for#1/iir:for#2:acc CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/iir:iir:acc#1 CSTEPS_FROM {{.. == 3}}

# Probe constraints
