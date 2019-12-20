//
// Copyright 2019 Tobias Strauch, Munich, Bavaria
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//

module Reg_D16 (clka,
                wea,
                addra,
                dina,
                douta,
                clkb,
                addrb,
                doutb);

input clka;
input [0:0] wea;
input [3:0] addra;
input [31:0] dina;
output [31:0] douta;
input clkb;
input [3:0] addrb;
output [31:0] doutb;

reg [31:0] mem [15:0];

always @ (posedge clka)
if (wea)
   mem[addra] <= dina;

reg [31:0] doutb;
always @ (posedge clkb)
if (wea & (addra == addrb))
   doutb <= dina;
else
   doutb <= mem[addrb];

endmodule
