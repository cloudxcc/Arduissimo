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

// for debugging:
reg [31:0] rf [511:0];
integer i;

always @ (posedge clka)
if (~rstn)
   for (i = 0; i < 512; i = i + 1) rf[i] <= 0;
else
if (wea)
   rf[addra] <= dina;

tdp_ram_512x32 RFi (.clka(clka),
                    .wea(wea),
                    .addra(addra),
                    .dina(dina),
                    .douta(douta),
                    .clkb(clkb),
                    .web(1'b0),
                    .addrb(addrb),
                    .dinb(0),
                    .doutb(doutb));


endmodule
