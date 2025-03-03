
# Loop constraints
directive set /filter/core/core:rlp CSTEPS_FROM {{. == 0}}
directive set /filter/core/core:rlp/main CSTEPS_FROM {{. == 5} {.. == 0}}
directive set /filter/core/core:rlp/main/iir:for CSTEPS_FROM {{. == 1} {.. == 2}}
directive set /filter/core/core:rlp/main/iir:for#1 CSTEPS_FROM {{. == 1} {.. == 2}}
directive set /filter/core/core:rlp/main/iir:for#3 CSTEPS_FROM {{. == 1} {.. == 4}}

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
directive set /filter/core/core:rlp/main/iir:for/iir:for-1:iir:for:slc(iir:iir:for:mux CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/iir:for/iir:for:mux CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/iir:for/iir:for:mux#2 CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/iir:for/iir:for-2:iir:for:slc(iir:iir:for:mux CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/iir:for/iir:for:mux#3 CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/iir:for/iir:for:mux#4 CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/iir:for#1/iir:for#1-1:slc(iir:iir:for#1:mux CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/iir:for#1/iir:for#1-1:mul CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/iir:for#1/iir:for#1-1:acc#1 CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/iir:for#1/iir:for#1-2:slc(iir:iir:for#1:mux CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/iir:for#1/iir:for#1:acc CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/iir:for#1/iir:for#1:slc(iir:for#1-2:iir:for#1:acc CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/iir:for#1/iir:for#1:i:mux#1 CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/iir:for#1/iir:for#1:i:mux#2 CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/iir:for#1/iir:for#2-1:slc(iir:iir:for#2:mux CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/iir:for#1/iir:for#2:acc CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/iir:for#1/iir:for#2-1:iir:for#2:and CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/iir:for#1/iir:for#2-1:acc#1 CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/iir:for#1/iir:for#2-2:slc(iir:iir:for#2:mux CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/iir:for#1/iir:for#2-2:iir:for#2:and CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/iir:for#1/iir:for#2-2:acc#1 CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/iir:for#1/iir:for#2:slc(iir:for#2-2:iir:for#2:acc CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/iir:for#1/iir:for#2:i:mux CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/iir:for#1/iir:for#1:i:mux CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/iir:for#1/iir:for#2:i:mux#1 CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/iir:iir:acc#1 CSTEPS_FROM {{.. == 3}}
directive set /filter/core/core:rlp/main/iir:for#3/iir:for#3-1:iir:for#3:slc(iir:iir:for#3:mux CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/iir:for#3/iir:for#3:mux CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/iir:for#3/iir:for#3:mux#2 CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/iir:for#3/iir:for#3-2:iir:for#3:slc(iir:iir:for#3:mux CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/iir:for#3/iir:for#3:mux#3 CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/iir:for#3/iir:for#3:mux#4 CSTEPS_FROM {{.. == 1}}

# Probe constraints
