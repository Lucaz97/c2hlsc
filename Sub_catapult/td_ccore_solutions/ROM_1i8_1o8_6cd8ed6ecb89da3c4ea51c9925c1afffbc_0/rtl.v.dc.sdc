# written for flow package DesignCompiler 
set sdc_version 1.7 

set_operating_conditions typical
set_load 11.40290 [all_outputs]
## driver/slew constraints on inputs
set data_inputs [list {I_1[*]}]
set_driving_cell -no_design_rule -library NangateOpenCellLibrary -lib_cell DFF_X1 -pin Q $data_inputs
set_units -time ns
# time_factor: 1.0

create_clock -name virtual_io_clk -period 20.0
## IO TIMING CONSTRAINTS
set_max_delay 20.0 -from [all_inputs] -to [all_outputs]
set_input_delay 0.0 -clock virtual_io_clk [get_ports {I_1[*]}]
set_output_delay 0.0 -clock virtual_io_clk [get_ports {O_1[*]}]
set_max_delay 20.0 -from [all_inputs] -to [all_outputs]

