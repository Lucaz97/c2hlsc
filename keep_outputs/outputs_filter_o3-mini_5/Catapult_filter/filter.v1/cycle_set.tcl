
# Loop constraints
directive set /filter/core/core:rlp CSTEPS_FROM {{. == 0}}
directive set /filter/core/core:rlp/main CSTEPS_FROM {{. == 2} {.. == 0}}

# IO operation constraints
directive set /filter/core/core:rlp/main/input:io_read(input:rsc.@) CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/fir:io_write(fir_output:rsc.@) CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/iir:io_write(iir_output:rsc.@) CSTEPS_FROM {{.. == 1}}

# Sync operation constraints

# Real operation constraints
directive set /filter/core/core:rlp/main/fir:p2:acc#1 CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/fir:p2:acc#3 CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/fir:p2:acc CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/fir:p2:acc#2 CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/fir:sum:acc CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/mac:if:sum0:acc#4 CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/mac:if:sum0:acc#3 CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/mac:if:sum0:acc#6 CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/mac:if:sum0:acc#2 CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/mac:if:sum0:acc#1 CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/mac:if:sum0:acc#5 CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/mac:if:sum0:acc CSTEPS_FROM {{.. == 1}}

# Probe constraints
