//
// Copyright 2019 Tobias Strauch, Munich, Bavaria
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//

module RF_D16 (clka,
               rstn,
               wea,
               addra,
               dina,
               douta,
               clkb,
               addrb,
               doutb);

input clka;
input rstn;
input [0:0] wea;
input [8:0] addra;
input [31:0] dina;
output [31:0] douta;
input clkb;
input [8:0] addrb;
output [31:0] doutb;

reg [31:0] mem [511:0];
integer i;

always @ (posedge clka)
if (~rstn)
   for (i = 0; i < 512; i = i + 1) mem[i] <= 0;
else
if (wea)
   mem[addra] <= dina;

// douta unconnected

reg [31:0] doutb;

always @ (posedge clkb)
   doutb <= mem[addrb];

endmodule
