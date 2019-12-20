//
// Copyright 2019 Tobias Strauch, Munich, Bavaria
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//

module sdp_ram_512x32x4 (clka,
                         wea,
                         addra,
                         dina,
                         clkb,
                         addrb,
                         doutb);

input clka;
input [3:0] wea;
input [8:0] addra;
input [31:0] dina;
input clkb;
input [8:0] addrb;
output [31:0] doutb;

reg [31:0] mem [511:0];
integer i;
  
always @ (posedge clka) begin
if (wea[0])
   mem[addra][ 7: 0] <= dina[ 7: 0];
if (wea[1])
   mem[addra][15: 8] <= dina[15: 8];
if (wea[2])
   mem[addra][23:16] <= dina[23:16];
if (wea[3])
   mem[addra][31:24] <= dina[31:24];
end

reg [8:0] addrb_reg;

always @(posedge clkb)
   addrb_reg <= addrb;

assign doutb = mem[addrb_reg];

endmodule
