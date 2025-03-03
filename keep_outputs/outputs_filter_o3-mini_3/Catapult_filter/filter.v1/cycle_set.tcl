
# Loop constraints
directive set /filter/core/core:rlp CSTEPS_FROM {{. == 0}}
directive set /filter/core/core:rlp/main CSTEPS_FROM {{. == 3} {.. == 0}}
directive set /filter/core/core:rlp/main/fir:for CSTEPS_FROM {{. == 1} {.. == 1}}
directive set /filter/core/core:rlp/main/fir:for#1 CSTEPS_FROM {{. == 1} {.. == 1}}

# IO operation constraints
directive set /filter/core/core:rlp/main/input:io_read(input:rsc.@) CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/fir:io_write(fir_output:rsc.@) CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/iir:io_write(iir_output:rsc.@) CSTEPS_FROM {{.. == 2}}

# Sync operation constraints

# Real operation constraints
directive set /filter/core/core:rlp/main/fir:for/fir:for:mux#4 CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/fir:for/fir:for:mux CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/fir:for/fir:for:mux#1 CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/fir:for/fir:for:mux#2 CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/fir:for/fir:for:mux#3 CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/fir:for/fir:for:acc#3 CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/fir:for/fir:for:acc CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/fir:for#1/fir:for#1:mux CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/fir:for#1/fir:for#1:mux#1 CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/fir:for#1/fir:for#1:mul CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/fir:for#1/fir:for#1:acc#1 CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/fir:for#1/fir:for#1:acc#2 CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/fir:for#1/fir:for#1:acc CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/iir:feedforward:acc#6 CSTEPS_FROM {{.. == 2}}
directive set /filter/core/core:rlp/main/iir:feedforward:acc#5 CSTEPS_FROM {{.. == 2}}
directive set /filter/core/core:rlp/main/iir:feedforward:acc#8 CSTEPS_FROM {{.. == 2}}
directive set /filter/core/core:rlp/main/iir:feedforward:acc#4 CSTEPS_FROM {{.. == 2}}
directive set /filter/core/core:rlp/main/iir:feedforward:acc#7 CSTEPS_FROM {{.. == 2}}
directive set /filter/core/core:rlp/main/iir:feedforward:acc CSTEPS_FROM {{.. == 2}}
directive set /filter/core/core:rlp/main/iir:iir:acc#1 CSTEPS_FROM {{.. == 2}}

# Probe constraints
