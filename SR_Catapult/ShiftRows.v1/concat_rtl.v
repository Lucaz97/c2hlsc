
//------> /eda/mentor/Siemens_EDA/Catapult_Synthesis_2023.1_2/Mgc_home/pkgs/siflibs/mgc_inout_prereg_en_v1.v 
//------------------------------------------------------------------------------
// Catapult Synthesis - Sample I/O Port Library
//
// Copyright (c) 2003-2019 Mentor Graphics Corp.
//       All Rights Reserved
//
// This document may be used and distributed without restriction provided that
// this copyright statement is not removed from the file and that any derivative
// work contains this copyright notice.
//
// The design information contained in this file is intended to be an example
// of the functionality which the end user may study in preparation for creating
// their own custom interfaces. This design does not necessarily present a 
// complete implementation of the named protocol or standard.
//
//------------------------------------------------------------------------------


module mgc_inout_prereg_en_v1 (din, ldout, dout, zin, lzout, zout);

    parameter integer rscid = 1;
    parameter integer width = 8;

    output [width-1:0] din;
    input              ldout;
    input  [width-1:0] dout;
    input  [width-1:0] zin;
    output             lzout;
    output [width-1:0] zout;

    wire   [width-1:0] din;
    wire               lzin;
    wire               lzout;
    wire   [width-1:0] z;

    assign lzout = ldout;
    assign din = zin;
    assign zout = dout;

endmodule



//------> /eda/mentor/Siemens_EDA/Catapult_Synthesis_2023.1_2/Mgc_home/pkgs/siflibs/mgc_io_sync_v2.v 
//------------------------------------------------------------------------------
// Catapult Synthesis - Sample I/O Port Library
//
// Copyright (c) 2003-2017 Mentor Graphics Corp.
//       All Rights Reserved
//
// This document may be used and distributed without restriction provided that
// this copyright statement is not removed from the file and that any derivative
// work contains this copyright notice.
//
// The design information contained in this file is intended to be an example
// of the functionality which the end user may study in preparation for creating
// their own custom interfaces. This design does not necessarily present a 
// complete implementation of the named protocol or standard.
//
//------------------------------------------------------------------------------


module mgc_io_sync_v2 (ld, lz);
    parameter valid = 0;

    input  ld;
    output lz;

    wire   lz;

    assign lz = ld;

endmodule


//------> ./rtl.v 
// ----------------------------------------------------------------------
//  HLS HDL:        Verilog Netlister
//  HLS Version:    2023.1_2/1049935 Production Release
//  HLS Date:       Sat Jun 10 10:53:51 PDT 2023
// 
//  Generated by:   lc4976@hansolo.poly.edu
//  Generated date: Tue Apr  9 23:09:14 2024
// ----------------------------------------------------------------------

// 
// ------------------------------------------------------------------
//  Design Unit:    ShiftRows_core_core_fsm
//  FSM Module
// ------------------------------------------------------------------


module ShiftRows_core_core_fsm (
  clk, rst_n, fsm_output
);
  input clk;
  input rst_n;
  output [2:0] fsm_output;
  reg [2:0] fsm_output;


  // FSM State Type Declaration for ShiftRows_core_core_fsm_1
  parameter
    core_rlp_C_0 = 2'd0,
    main_C_0 = 2'd1,
    main_C_1 = 2'd2;

  reg [1:0] state_var;
  reg [1:0] state_var_NS;


  // Interconnect Declarations for Component Instantiations 
  always @(*)
  begin : ShiftRows_core_core_fsm_1
    case (state_var)
      main_C_0 : begin
        fsm_output = 3'b010;
        state_var_NS = main_C_1;
      end
      main_C_1 : begin
        fsm_output = 3'b100;
        state_var_NS = main_C_0;
      end
      // core_rlp_C_0
      default : begin
        fsm_output = 3'b001;
        state_var_NS = main_C_0;
      end
    endcase
  end

  always @(posedge clk) begin
    if ( ~ rst_n ) begin
      state_var <= core_rlp_C_0;
    end
    else begin
      state_var <= state_var_NS;
    end
  end

endmodule

// ------------------------------------------------------------------
//  Design Unit:    ShiftRows_core
// ------------------------------------------------------------------


module ShiftRows_core (
  clk, rst_n, state_rsc_zout, state_rsc_lzout, state_rsc_zin, state_triosy_lz
);
  input clk;
  input rst_n;
  output [127:0] state_rsc_zout;
  output state_rsc_lzout;
  input [127:0] state_rsc_zin;
  output state_triosy_lz;


  // Interconnect Declarations
  wire [127:0] state_rsci_din;
  wire [2:0] fsm_output;
  reg reg_state_triosy_obj_ld_cse;


  // Interconnect Declarations for Component Instantiations 
  wire  nl_state_rsci_ldout;
  assign nl_state_rsci_ldout = fsm_output[1];
  wire [127:0] nl_state_rsci_dout;
  assign nl_state_rsci_dout = {(state_rsci_din[95:88]) , (state_rsci_din[55:48])
      , (state_rsci_din[15:8]) , (state_rsci_din[103:96]) , (state_rsci_din[63:56])
      , (state_rsci_din[23:16]) , (state_rsci_din[111:104]) , (state_rsci_din[71:64])
      , (state_rsci_din[31:24]) , (state_rsci_din[119:112]) , (state_rsci_din[79:72])
      , (state_rsci_din[39:32]) , (state_rsci_din[127:120]) , (state_rsci_din[87:80])
      , (state_rsci_din[47:40]) , (state_rsci_din[7:0])};
  mgc_inout_prereg_en_v1 #(.rscid(32'sd1),
  .width(32'sd128)) state_rsci (
      .din(state_rsci_din),
      .ldout(nl_state_rsci_ldout),
      .dout(nl_state_rsci_dout[127:0]),
      .zin(state_rsc_zin),
      .lzout(state_rsc_lzout),
      .zout(state_rsc_zout)
    );
  mgc_io_sync_v2 #(.valid(32'sd0)) state_triosy_obj (
      .ld(reg_state_triosy_obj_ld_cse),
      .lz(state_triosy_lz)
    );
  ShiftRows_core_core_fsm ShiftRows_core_core_fsm_inst (
      .clk(clk),
      .rst_n(rst_n),
      .fsm_output(fsm_output)
    );
  always @(posedge clk) begin
    if ( ~ rst_n ) begin
      reg_state_triosy_obj_ld_cse <= 1'b0;
    end
    else begin
      reg_state_triosy_obj_ld_cse <= fsm_output[1];
    end
  end
endmodule

// ------------------------------------------------------------------
//  Design Unit:    ShiftRows
// ------------------------------------------------------------------


module ShiftRows (
  clk, rst_n, state_rsc_zout, state_rsc_lzout, state_rsc_zin, state_triosy_lz
);
  input clk;
  input rst_n;
  output [127:0] state_rsc_zout;
  output state_rsc_lzout;
  input [127:0] state_rsc_zin;
  output state_triosy_lz;



  // Interconnect Declarations for Component Instantiations 
  ShiftRows_core ShiftRows_core_inst (
      .clk(clk),
      .rst_n(rst_n),
      .state_rsc_zout(state_rsc_zout),
      .state_rsc_lzout(state_rsc_lzout),
      .state_rsc_zin(state_rsc_zin),
      .state_triosy_lz(state_triosy_lz)
    );
endmodule


