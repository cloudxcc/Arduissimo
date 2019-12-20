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
                doutb
 );

input clka;
input [0:0] wea;
input [3:0] addra;
input [31:0] dina;
output [31:0] douta;
input clkb;
input [3:0] addrb;
output [31:0] doutb;

wire [31:0] doutb_int;

sdp_ram_16x32 regi (.clka(clka),
                    .wea(wea),
                    .addra(addra),
                    .dina(dina),
                    .clkb(clkb),
                    .addrb(addrb),
                    .doutb(doutb_int)
);
 
reg [31:0] bypass_data;      

always @ (posedge clkb)
   bypass_data <= dina;

reg bypass;

always @ (posedge clkb)
if (wea & (addra == addrb))
   bypass <= 1'b1;
else
   bypass <= 1'b0;

assign doutb = bypass ? bypass_data : doutb_int;

endmodule
