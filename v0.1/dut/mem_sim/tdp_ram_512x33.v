//
// Copyright 2019 Tobias Strauch, Munich, Bavaria
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//

module tdp_ram_512x33 (clka,
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
input [0:0] wea;
input [8:0] addra;
input [32:0] dina;
output [32:0] douta;
input clkb;
input [0:0] web;
input [8:0] addrb;
input [32:0] dinb;
output [32:0] doutb;

reg [32:0] mem [511:0];
integer i;

always @(posedge clka) begin
if (wea)
   mem[addra] <= dina; 
end

always @(posedge clkb) begin
if (web)
   mem[addrb] <= dinb;
end

reg [8:0] addra_reg;

always @(posedge clka)
   addra_reg <= addra;

reg [8:0] addrb_reg;

always @(posedge clkb)
   addrb_reg <= addrb;

reg [32:0] douta;

always @(posedge clka)
   douta <= mem[addra_reg];

reg [32:0] doutb;

always @(posedge clkb)
   doutb <= mem[addrb_reg];

endmodule
