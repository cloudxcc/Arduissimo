//
// Copyright 2019 Tobias Strauch, Munich, Bavaria
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//

module tdp_ram_6144x16x2 (clka,
                          wea,
                          addra,
                          dina,
                          douta,
                          clkb,
                          web,
                          addrb,
                          dinb,
                          doutb);

input clka;
input [1:0] wea;
input [12:0] addra;
input [15:0] dina;
output [15:0] douta;
input clkb;
input [1:0] web;
input [12:0] addrb;
input [15:0] dinb;
output [15:0] doutb;

reg [15:0] mem [6143:0];
integer i;

always @ (posedge clka) begin
if (wea[0])
   mem[addra][7:0] <= dina[7:0];
if (wea[1])
   mem[addra][15:8] <= dina[15:8];
end

reg [12:0] addra_reg;

always @(posedge clka)
   addra_reg <= addra;

reg [12:0] addrb_reg;

always @(posedge clkb)
   addrb_reg <= addrb;


assign douta = mem[addra_reg];
assign doutb = mem[addrb_reg];

endmodule
