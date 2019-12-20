//
// Copyright 2019 Tobias Strauch, Munich, Bavaria
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//

//
//	receiving part not really self-checking
//

module tb_top;

reg clk;
reg rstn;

wire [111:0] gpio_out;
reg [111:0] gpio_in;
reg uart_rx_in;
wire uart_tx_out;

reg [7:0] program [100000:0];
integer ptr_offset = 32'h00010000;

task uart_send;
input [7:0] send_data;
integer i;
begin
   uart_rx_in <= 1'b0;
   #200; //8680;
   for (i = 0; i < 8; i = i + 1) begin
      uart_rx_in <= send_data[i];
      #200; //8680;
   end
   uart_rx_in <= 1'b1;
   #200; //8680;
   #200; //8680;
end
endtask

integer i, ptr;
initial begin
   cycle_cnt <= 0;
   clk <= 1'b0;
   rstn <= 1'b0;
   uart_rx_in <= 1'b1;
   for (ptr = 0; ptr < 100000; ptr = ptr + 4) begin
      program[(ptr) + 0] = 8'h13;
      program[(ptr) + 1] = 8'h00;
      program[(ptr) + 2] = 8'h00;
      program[(ptr) + 3] = 8'h00;
   end

   $readmemh("../../../../../../tests/system/system_top/work/main_0.hex", program);
   #500;
   rstn <= 1'b1;

   ///////////////////////////////////////////////////// clear reset
   $display("Clear reset.");
   uart_send(8'h10);               // 0x11 clear reset

   #500;
   rstn <= 1'b0; 
   #500;
   rstn <= 1'b1; 
   #500;

   $display("Set reset.");
   uart_send(8'h1f);               // 0x11 set reset

   ///////////////////////////////////////////////////// loopback
   $display("loopback.");
   uart_send(8'h20);               // 0x20 loopback
   uart_send(8'hA5);

   wait (uart_tx_out == 1'b0);
   wait (uart_tx_out == 1'b1);
   wait (uart_tx_out == 1'b0);
   wait (uart_tx_out == 1'b1);
   wait (uart_tx_out == 1'b0);
   wait (uart_tx_out == 1'b1);

   ///////////////////////////////////////////////////// 8 byte c1 data write 
   $display("Memory write data 8 byte c1.");
   uart_send(8'h30);               // 0x30 memory programming
   uart_send(8'h03);
   uart_send(8'h00);
   uart_send(8'h00);               // address 0 of core 0
   uart_send(8'h00);
   uart_send(8'h07);               // length 8
   for (i = 0; i < 8; i = i + 1) begin
      uart_send(i);
   end

   ///////////////////////////////////////////////////// 8 byte data read
   $display("Memory read data 8 byte c1.");
   uart_send(8'h40);               // 0x40 memory read
   uart_send(8'h03);
   uart_send(8'h00);
   uart_send(8'h00);               // address 0 of core 0
   uart_send(8'h00);
   uart_send(8'h07);               // length 8

   wait (uart_tx_out == 1'b1);
   for (i = 0; i < 15; i = i + 1) begin
      wait (uart_tx_out == 1'b0);
      wait (uart_tx_out == 1'b1);
   end

   ///////////////////////////////////////////////////// 8 byte c3 prog write 
   $display("Memory write prog 8 byte c3.");
   uart_send(8'h30);               // 0x30 memory programming
   uart_send(8'h06);
   uart_send(8'h00);
   uart_send(8'h00);               // address 0 of core 3
   uart_send(8'h00);
   uart_send(8'h07);               // length 8
   for (i = 0; i < 8; i = i + 1) begin
      uart_send(16 + i);
   end

   ///////////////////////////////////////////////////// 8 byte c3 prog read
   $display("Memory read prog 8 byte c3.");
   uart_send(8'h40);                // 0x40 memory read
   uart_send(8'h06);
   uart_send(8'h00);
   uart_send(8'h00);               // address 0 of core 3
   uart_send(8'h00);
   uart_send(8'h07);         	   // length 8

   wait (uart_tx_out == 1'b1);
   for (i = 0; i < 23; i = i + 1) begin
      wait (uart_tx_out == 1'b0);
      wait (uart_tx_out == 1'b1);
   end

   ///////////////////////////////////////////////////// final program
   $display("Final programming.");
   uart_send(8'h30);               // 0x30 memory programming
   uart_send(8'h00);
   uart_send(8'h00);
   uart_send(8'h00);               // address 0 of core 0
   uart_send(8'h00);
   uart_send(8'h7f);               // length 128
   for (i = 0; i < 128; i = i + 1) begin
      uart_send(program[ptr_offset + i]);
   end

   ///////////////////////////////////////////////////// 4 byte program read {13, 00, 00, 00}
   $display("4 byte read program.");
   uart_send(8'h40);               // 0x40 memory read
   uart_send(8'h00);
   uart_send(8'h00);
   uart_send(8'h00);               // address 0 of core 0
   uart_send(8'h00);
   uart_send(8'h03);               // length 4

   wait (uart_tx_out == 1'b1);
   for (i = 0; i < 5; i = i + 1) begin
      wait (uart_tx_out == 1'b0);
      wait (uart_tx_out == 1'b1);
   end

   ///////////////////////////////////////////////////// clear reset
   $display("Clear reset.");
   uart_send(8'h10);               // 0x11 clear reset

   ///////////////////////////////////////////////////// communication
   $display("UART communication.");
   uart_send(8'h50);               // 0x50 communication
   uart_send(8'h00);
   uart_send(8'h01);               // length 2
   uart_send(8'hA5);
   uart_send(8'h5A);

   for (i = 0; i < 4; i = i + 1) begin
      wait (uart_tx_out == 1'b0);
      wait (uart_tx_out == 1'b1);
   end

   #20000;

   $display("Test top passed.");

$finish;
end

always begin       // ~ 180MHz in average
   #2;
   clk <= ~clk;
   #3;
   clk <= ~clk;
   #3;
   clk <= ~clk;
   #3;
   clk <= ~clk;
end

integer cycle_cnt;
always @ (posedge clk)
   if (cycle_cnt == 13000000) begin
      $display("Test runaway.\n");
      $finish;
   end else
      cycle_cnt <= cycle_cnt + 1;


fpga_func duti (clk,
                rstn,
                gpio_out,
                gpio_in,
                uart_rx_in,
                uart_tx_out);


endmodule
