//
// Copyright 2019 Tobias Strauch, Munich, Bavaria
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//

module fpga_top (CLK,
                 RSTN,
                 GPIO,
                 uart_rx_in,
                 uart_tx_out);

input CLK;
input RSTN;
inout [111:0] GPIO;
input uart_rx_in;
output uart_tx_out;

wire CLK_int;

clk_100M clk_100Mi (
   // Clock out ports
   .clk_out1(CLK_int),
   // Clock in ports
   .clk_in1(CLK));

wire [111:0] gpio_out;
assign GPIO = gpio_out;
wire [111:0] gpio_in = GPIO;

fpga_func funci (.CLK(CLK_int),
                 .RSTN(RSTN),
                 .gpio_out(gpio_out),
                 .gpio_in(gpio_in),
                 .uart_rx_in(uart_rx_in),
                 .uart_tx_out(uart_tx_out));

endmodule
