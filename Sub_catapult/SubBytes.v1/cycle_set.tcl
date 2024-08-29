
# Loop constraints
directive set /SubBytes/core/core:rlp CSTEPS_FROM {{. == 0}}
directive set /SubBytes/core/core:rlp/main CSTEPS_FROM {{. == 2} {.. == 0}}
directive set /SubBytes/core/core:rlp/main/for CSTEPS_FROM {{. == 1} {.. == 1}}
directive set /SubBytes/core/core:rlp/main/for/for:for CSTEPS_FROM {{. == 2} {.. == 0}}

# IO operation constraints
directive set /SubBytes/core/core:rlp/main/for/for:for/for:for:io_read(state:rsc.@) CSTEPS_FROM {{.. == 1}}
directive set /SubBytes/core/core:rlp/main/for/for:for/for:for:io_write(state:rsc.@) CSTEPS_FROM {{.. == 1}}

# Sync operation constraints

# Real operation constraints
directive set /SubBytes/core/core:rlp/main/for/for:for/for:for:mux CSTEPS_FROM {{.. == 1}}
directive set /SubBytes/core/core:rlp/main/for/for:for/U:ROM_1i8_1o8_ec965ae3708891513d2041cf4eab49d42f CSTEPS_FROM {{.. == 1}}
directive set /SubBytes/core/core:rlp/main/for/for:for/for:for:for:for:mux CSTEPS_FROM {{.. == 1}}
directive set /SubBytes/core/core:rlp/main/for/for:for/for:for:for:for:mux#1 CSTEPS_FROM {{.. == 1}}
directive set /SubBytes/core/core:rlp/main/for/for:for/for:for:for:for:mux#2 CSTEPS_FROM {{.. == 1}}
directive set /SubBytes/core/core:rlp/main/for/for:for/for:for:for:for:mux#3 CSTEPS_FROM {{.. == 1}}
directive set /SubBytes/core/core:rlp/main/for/for:for/for:for:for:for:mux#4 CSTEPS_FROM {{.. == 1}}
directive set /SubBytes/core/core:rlp/main/for/for:for/for:for:for:for:mux#5 CSTEPS_FROM {{.. == 1}}
directive set /SubBytes/core/core:rlp/main/for/for:for/for:for:for:for:mux#6 CSTEPS_FROM {{.. == 1}}
directive set /SubBytes/core/core:rlp/main/for/for:for/for:for:for:for:mux#7 CSTEPS_FROM {{.. == 1}}
directive set /SubBytes/core/core:rlp/main/for/for:for/for:for:for:for:mux#8 CSTEPS_FROM {{.. == 1}}
directive set /SubBytes/core/core:rlp/main/for/for:for/for:for:for:for:mux#9 CSTEPS_FROM {{.. == 1}}
directive set /SubBytes/core/core:rlp/main/for/for:for/for:for:for:for:mux#10 CSTEPS_FROM {{.. == 1}}
directive set /SubBytes/core/core:rlp/main/for/for:for/for:for:for:for:mux#11 CSTEPS_FROM {{.. == 1}}
directive set /SubBytes/core/core:rlp/main/for/for:for/for:for:for:for:mux#12 CSTEPS_FROM {{.. == 1}}
directive set /SubBytes/core/core:rlp/main/for/for:for/for:for:for:for:mux#13 CSTEPS_FROM {{.. == 1}}
directive set /SubBytes/core/core:rlp/main/for/for:for/for:for:for:for:mux#14 CSTEPS_FROM {{.. == 1}}
directive set /SubBytes/core/core:rlp/main/for/for:for/for:for:for:for:mux#15 CSTEPS_FROM {{.. == 1}}
directive set /SubBytes/core/core:rlp/main/for/for:for/for:for:acc#3 CSTEPS_FROM {{.. == 2}}
directive set /SubBytes/core/core:rlp/main/for/for:acc#1 CSTEPS_FROM {{.. == 1}}

# Probe constraints
