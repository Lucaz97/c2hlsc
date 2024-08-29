# written for flow package DesignCompiler 
set sdc_version 1.7 

set_operating_conditions typical
set_load 11.40290 [all_outputs]
## driver/slew constraints on inputs
set data_inputs [list {arr_rsc_zin[*]} {low_rsc_dat[*]} {high_rsc_dat[*]}]
set_driving_cell -no_design_rule -library NangateOpenCellLibrary -lib_cell DFF_X1 -pin Q $data_inputs
set_units -time ns
# time_factor: 1.0

create_clock -name clk -period 20.0 -waveform { 0.0 10.0 } [get_ports {clk}]
set_clock_uncertainty 0.0 [get_clocks {clk}]

create_clock -name virtual_io_clk -period 20.0
## IO TIMING CONSTRAINTS
set_input_delay -clock [get_clocks {clk}] 0.0 [get_ports {rst_n}]
set_max_delay 20.0 -from [all_inputs] -to [all_outputs]
set_output_delay -clock [get_clocks {clk}] 0.0 [get_ports {arr_rsc_zout[*]}]
set_output_delay -clock [get_clocks {clk}] 0.0 [get_ports {arr_rsc_lzout}]
set_input_delay -clock [get_clocks {clk}] 0.0 [get_ports {arr_rsc_zin[*]}]
set_output_delay -clock [get_clocks {clk}] 0.0 [get_ports {arr_triosy_lz}]
set_input_delay -clock [get_clocks {clk}] 0.0 [get_ports {low_rsc_dat[*]}]
set_output_delay -clock [get_clocks {clk}] 0.0 [get_ports {low_triosy_lz}]
set_input_delay -clock [get_clocks {clk}] 0.0 [get_ports {high_rsc_dat[*]}]
set_output_delay -clock [get_clocks {clk}] 0.0 [get_ports {high_triosy_lz}]

