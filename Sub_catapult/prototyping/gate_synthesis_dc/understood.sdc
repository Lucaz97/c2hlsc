#
# Created by 
#   ../bin/Linux-x86_64-O/oasys 21.1-s001 on Tue Apr  9 23:33:03 2024
# (C) Mentor Graphics Corporation
#
set_units -time ns -capacitance ff -resistance kohm -power nW -voltage V -current mA
create_clock -period 20 -waveform {0 10} -name virtual_io_clk 
set_max_delay -from [list [get_ports {I_1[0]}] [get_ports {I_1[1]}] [get_ports {I_1[2]}] [get_ports {I_1[3]}] [get_ports {I_1[4]}] [get_ports {I_1[5]}] [get_ports {I_1[6]}] [get_ports {I_1[7]}]]  -to [list [get_ports {O_1[0]}] [get_ports {O_1[1]}] [get_ports {O_1[2]}] [get_ports {O_1[3]}] [get_ports {O_1[4]}] [get_ports {O_1[5]}] [get_ports {O_1[6]}] [get_ports {O_1[7]}]] 20.0000
set_max_delay -from [list [get_ports {I_1[0]}] [get_ports {I_1[1]}] [get_ports {I_1[2]}] [get_ports {I_1[3]}] [get_ports {I_1[4]}] [get_ports {I_1[5]}] [get_ports {I_1[6]}] [get_ports {I_1[7]}]]  -to [list [get_ports {O_1[0]}] [get_ports {O_1[1]}] [get_ports {O_1[2]}] [get_ports {O_1[3]}] [get_ports {O_1[4]}] [get_ports {O_1[5]}] [get_ports {O_1[6]}] [get_ports {O_1[7]}]] 20.0000
set_load 11.4029 [get_ports {O_1[7]}]
set_load 11.4029 [get_ports {O_1[6]}]
set_load 11.4029 [get_ports {O_1[5]}]
set_load 11.4029 [get_ports {O_1[4]}]
set_load 11.4029 [get_ports {O_1[3]}]
set_load 11.4029 [get_ports {O_1[2]}]
set_load 11.4029 [get_ports {O_1[1]}]
set_load 11.4029 [get_ports {O_1[0]}]
set_driving_cell -lib_cell DFF_X1 -from_pin CK -pin Q -library NangateOpenCellLibrary [get_ports {I_1[7]}]
set_driving_cell -lib_cell DFF_X1 -from_pin CK -pin Q -library NangateOpenCellLibrary [get_ports {I_1[6]}]
set_driving_cell -lib_cell DFF_X1 -from_pin CK -pin Q -library NangateOpenCellLibrary [get_ports {I_1[5]}]
set_driving_cell -lib_cell DFF_X1 -from_pin CK -pin Q -library NangateOpenCellLibrary [get_ports {I_1[4]}]
set_driving_cell -lib_cell DFF_X1 -from_pin CK -pin Q -library NangateOpenCellLibrary [get_ports {I_1[3]}]
set_driving_cell -lib_cell DFF_X1 -from_pin CK -pin Q -library NangateOpenCellLibrary [get_ports {I_1[2]}]
set_driving_cell -lib_cell DFF_X1 -from_pin CK -pin Q -library NangateOpenCellLibrary [get_ports {I_1[1]}]
set_driving_cell -lib_cell DFF_X1 -from_pin CK -pin Q -library NangateOpenCellLibrary [get_ports {I_1[0]}]
set_input_delay 0 -clock virtual_io_clk [get_ports {I_1[7]}]
set_input_delay 0 -clock virtual_io_clk [get_ports {I_1[6]}]
set_input_delay 0 -clock virtual_io_clk [get_ports {I_1[5]}]
set_input_delay 0 -clock virtual_io_clk [get_ports {I_1[4]}]
set_input_delay 0 -clock virtual_io_clk [get_ports {I_1[3]}]
set_input_delay 0 -clock virtual_io_clk [get_ports {I_1[2]}]
set_input_delay 0 -clock virtual_io_clk [get_ports {I_1[1]}]
set_input_delay 0 -clock virtual_io_clk [get_ports {I_1[0]}]
set_output_delay 0 -clock virtual_io_clk [get_ports {O_1[7]}]
set_output_delay 0 -clock virtual_io_clk [get_ports {O_1[6]}]
set_output_delay 0 -clock virtual_io_clk [get_ports {O_1[5]}]
set_output_delay 0 -clock virtual_io_clk [get_ports {O_1[4]}]
set_output_delay 0 -clock virtual_io_clk [get_ports {O_1[3]}]
set_output_delay 0 -clock virtual_io_clk [get_ports {O_1[2]}]
set_output_delay 0 -clock virtual_io_clk [get_ports {O_1[1]}]
set_output_delay 0 -clock virtual_io_clk [get_ports {O_1[0]}]
set_operating_conditions  typical
