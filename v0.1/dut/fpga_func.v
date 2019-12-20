//
// Copyright 2019 Tobias Strauch, Munich, Bavaria
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//

module fpga_func (CLK,
                  RSTN,
                  gpio_out,
                  gpio_in,
                  uart_rx_in,
                  uart_tx_out);

input CLK;
input RSTN;
output [111:0] gpio_out;
input [111:0] gpio_in;
input uart_rx_in;
output uart_tx_out;

reg [111:0] gpio_out;
wire [111:0] gpio_dir_reg;
wire [111:0] gpio_out_reg;

integer k;

always @(gpio_dir_reg or gpio_out_reg)
begin
   for (k = 0; k < 112; k = k + 1) begin
      if (gpio_dir_reg[k])
         gpio_out[k] = gpio_out_reg[k];
      else
         gpio_out[k] = 1'bZ;
   end
end

wire uart_prog_access;
wire [31:0] uart_prog_wea;
wire [12:0] uart_prog_addra;
wire [7:0] uart_prog_dina;
wire [255:0] uart_prog_douta;
wire [31:0] uart_prog_dina_32 = {uart_prog_dina, uart_prog_dina, uart_prog_dina, uart_prog_dina};
wire [3:0] uart_quad_rstn;
wire uart_rec_valid;
wire [7:0] uart_rec_data;
wire uart_send_ready;
wire uart_send_req;
wire [7:0] uart_send_data;

wire [3:0] uart_quad_rstn_tmp = uart_quad_rstn; //~(~uart_quad_rstn | {4{!RSTN}});

quad quadi (CLK,
            uart_quad_rstn_tmp,
            uart_prog_access,
            uart_prog_wea,
            uart_prog_addra,
            uart_prog_dina_32,
            uart_prog_douta,
            uart_rec_valid,
            uart_rec_data,
            uart_send_ready,
            uart_send_req,
            uart_send_data,
            gpio_dir_reg,
            gpio_out_reg,
            gpio_in);

uart uarti (CLK,
            RSTN,
            uart_rx_in,
            uart_tx_out,
            uart_quad_rstn,
            uart_rec_valid,
            uart_rec_data,
            uart_send_ready,
            uart_send_req,
            uart_send_data,
            uart_prog_access,
            uart_prog_wea,
            uart_prog_addra,
            uart_prog_dina,
            uart_prog_douta);

endmodule
