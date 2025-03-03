
# Loop constraints
directive set /filter/core/core:rlp CSTEPS_FROM {{. == 0}}
directive set /filter/core/core:rlp/main CSTEPS_FROM {{. == 2} {.. == 0}}

# IO operation constraints
directive set /filter/core/core:rlp/main/input:io_read(input:rsc.@) CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/fir:io_write(fir_output:rsc.@) CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/iir:io_write(iir_output:rsc.@) CSTEPS_FROM {{.. == 1}}

# Sync operation constraints

# Real operation constraints
directive set /filter/core/core:rlp/main/fir:acc:acc#5 CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/fir:acc:acc#4 CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/fir:acc:acc#7 CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/fir:acc:acc#6 CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/fir:acc:acc CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/iir:feedforward:acc#6 CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/iir:feedforward:acc#5 CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/iir:feedforward:acc#8 CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/iir:feedforward:acc#4 CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/iir:feedforward:acc#7 CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/iir:feedforward:acc CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/iir:iir:acc#1 CSTEPS_FROM {{.. == 1}}

# Probe constraints
