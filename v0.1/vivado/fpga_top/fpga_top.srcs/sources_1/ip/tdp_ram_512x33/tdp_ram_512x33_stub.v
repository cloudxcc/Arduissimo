// Copyright 1986-2019 Xilinx, Inc. All Rights Reserved.
// --------------------------------------------------------------------------------
// Tool Version: Vivado v.2019.2 (win64) Build 2708876 Wed Nov  6 21:40:23 MST 2019
// Date        : Sat Nov 30 20:20:05 2019
// Host        : asuspc running 64-bit major release  (build 9200)
// Command     : write_verilog -force -mode synth_stub -rename_top tdp_ram_512x33 -prefix
//               tdp_ram_512x33_ tdp_ram_512x33_stub.v
// Design      : tdp_ram_512x33
// Purpose     : Stub declaration of top-level module interface
// Device      : xc7a35ticsg324-1L
// --------------------------------------------------------------------------------

// This empty module with port declaration file causes synthesis tools to infer a black box for IP.
// The synthesis directives are for Synopsys Synplify support to prevent IO buffer insertion.
// Please paste the declaration into a Verilog source file or add the file as an additional source.
(* x_core_info = "blk_mem_gen_v8_4_4,Vivado 2019.2" *)
module tdp_ram_512x33(clka, wea, addra, dina, douta, clkb, web, addrb, dinb, 
  doutb)
/* synthesis syn_black_box black_box_pad_pin="clka,wea[0:0],addra[8:0],dina[32:0],douta[32:0],clkb,web[0:0],addrb[8:0],dinb[32:0],doutb[32:0]" */;
  input clka;
  input [0:0]wea;
  input [8:0]addra;
  input [32:0]dina;
  output [32:0]douta;
  input clkb;
  input [0:0]web;
  input [8:0]addrb;
  input [32:0]dinb;
  output [32:0]doutb;
endmodule
