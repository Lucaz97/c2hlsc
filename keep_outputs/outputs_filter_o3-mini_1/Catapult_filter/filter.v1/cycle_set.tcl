
# Loop constraints
directive set /filter/core/core:rlp CSTEPS_FROM {{. == 0}}
directive set /filter/core/core:rlp/main CSTEPS_FROM {{. == 5} {.. == 0}}
directive set /filter/core/core:rlp/main/shift:for CSTEPS_FROM {{. == 1} {.. == 2}}
directive set /filter/core/core:rlp/main/mac:for CSTEPS_FROM {{. == 1} {.. == 2}}
directive set /filter/core/core:rlp/main/shift#1:for CSTEPS_FROM {{. == 1} {.. == 4}}

# IO operation constraints
directive set /filter/core/core:rlp/main/input:io_read(input:rsc.@) CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/fir:io_write(fir_output:rsc.@) CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/iir:io_write(iir_output:rsc.@) CSTEPS_FROM {{.. == 3}}

# Sync operation constraints

# Real operation constraints
directive set /filter/core/core:rlp/main/fir:acc:acc#5 CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/fir:acc:acc#4 CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/fir:acc:acc#7 CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/fir:acc:acc#6 CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/fir:acc:acc CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/shift:for/shift:for:mux#4 CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/shift:for/shift:for:mux CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/shift:for/shift:for:mux#1 CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/shift:for/shift:for:mux#2 CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/shift:for/shift:for:mux#3 CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/shift:for/shift:for:acc#2 CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/shift:for/shift:for:acc CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/mac:for/mac:for:mux CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/mac:for/mac:for:mux#1 CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/mac:for/mac:for:mul CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/mac:for/mac:for:acc#1 CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/mac:for/mac:for:acc#2 CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/mac:for/mac#1:for:mux CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/mac:for/mac#1:for:and CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/mac:for/mac#1:for:or CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/mac:for/mac#1:for:mul CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/mac:for/mac#1:for:acc#1 CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/mac:for/mac#1:for:acc#2 CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/mac:for/mac:for:acc CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/mac:for/mac#1:for:acc CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/iir:iir:acc#1 CSTEPS_FROM {{.. == 3}}
directive set /filter/core/core:rlp/main/shift#1:for/shift#1:for:mux#4 CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/shift#1:for/shift#1:for:mux CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/shift#1:for/shift#1:for:mux#1 CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/shift#1:for/shift#1:for:mux#2 CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/shift#1:for/shift#1:for:mux#3 CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/shift#1:for/shift#1:for:acc#2 CSTEPS_FROM {{.. == 1}}
directive set /filter/core/core:rlp/main/shift#1:for/shift#1:for:acc CSTEPS_FROM {{.. == 1}}

# Probe constraints
