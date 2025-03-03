
# Loop constraints
directive set /filter/core/core:rlp CSTEPS_FROM {{. == 0}}
directive set /filter/core/core:rlp/main CSTEPS_FROM {{. == 3} {.. == 0}}
directive set /filter/core/core:rlp/main/mac:for CSTEPS_FROM {{. == 2} {.. == 1}}

# IO operation constraints
directive set /filter/core/core:rlp/main/input:io_read(input:rsc.@) CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/fir:io_write(fir_output:rsc.@) CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/iir:io_write(iir_output:rsc.@) CSTEPS_FROM {{.. == 2}}

# Sync operation constraints

# Real operation constraints
directive set /filter/core/core:rlp/main/mac:for/mac:for:if:mux CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/mac:for/mac:for:if:mux#1 CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/mac:for/mac:for:if:mul CSTEPS_FROM {{.. == 2}}
directive set /filter/core/core:rlp/main/mac:for/mac:for:if:acc CSTEPS_FROM {{.. == 2}}
directive set /filter/core/core:rlp/main/mac:for/mac:for:acc#2 CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/mac:for/mac#1:for:if:mux CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/mac:for/mac#1:for:if:mux#1 CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/mac:for/mac#1:for:if:mul CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/mac:for/mac#1:for:if:acc CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/mac:for/mac#1:for:acc#2 CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/mac:for/mac#2:for:if:mux CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/mac:for/mac#2:for:if:and CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/mac:for/mac#2:for:if:or CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/mac:for/mac#2:for:if:mul CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/mac:for/mac#2:for:if:acc CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/mac:for/mac#2:for:acc#2 CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/mac:for/mac:for:acc CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/mac:for/mac#1:for:acc CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/mac:for/mac#2:for:acc CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/iir:iir:acc#1 CSTEPS_FROM {{.. == 2}}

# Probe constraints
