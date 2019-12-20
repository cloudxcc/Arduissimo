//
// Copyright 2019 Tobias Strauch, Munich, Bavaria
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//

module tb_quad;

integer test;

reg clk;
reg [3:0] rstn;

reg prog_access;
reg [31:0] prog_wea;
reg [12:0] prog_addra;
reg [31:0] prog_dina;
wire [127:0] prog_douta;
reg uart_rec_valid;
reg [7:0] uart_rec_data;
reg uart_send_ready;
wire uart_send_req;
wire [7:0] uart_send_data;
wire [111:0] gpio_dir_reg;
wire [111:0] gpio_out_reg;
reg [111:0] gpio_in;
reg gpio_i2c_feedback;
reg gpio_i2c_feedback_demo;
reg gpio_spi_feedback;
reg gpio_spi_feedback_demo;
reg gpio_rs232_feedback;
reg gpio_rs232_feedback_demo;
reg [111:0] gpio_pad;

reg [7:0] program [100000:0];

integer cycle_cnt, max_cycle_count;
integer ptr_offset = 32'h00010000;

integer i, tc, ptr, n;

//////////////////////////////////////////
// handling gpio_pad
//////////////////////////////////////////
always @ (negedge clk) begin
for (n = 0; n < 112; n = n + 1) begin
   if (gpio_dir_reg[n] == 1'b0) begin 
     gpio_pad[n] <= 1'b1;
   end else begin
     gpio_pad[n] <= gpio_out_reg[n];
   end
end
end

//////////////////////////////////////////
// handling gpio_in
//////////////////////////////////////////
initial
   gpio_in <= 0;

always @ (negedge clk) begin
if (gpio_i2c_feedback == 1'b1) // I2C SCL
   if (gpio_dir_reg[10] == 1'b0) begin 
     gpio_in[10] <= 1'b1;
     gpio_in[12] <= 1'b1;
   end else begin
     gpio_in[10] <= gpio_out_reg[10];
     gpio_in[12] <= gpio_out_reg[10];
   end
if (gpio_i2c_feedback == 1'b1) // I2C SDA (combined [11] and [13] bit)
   if (gpio_dir_reg[11] == 1'b1) begin 
     gpio_in[11] <= gpio_out_reg[11];
     gpio_in[13] <= gpio_out_reg[11];
   end else if (gpio_dir_reg[13] == 1'b1) begin 
     gpio_in[11] <= gpio_out_reg[13];
     gpio_in[13] <= gpio_out_reg[13];
   end else begin
     gpio_in[11] <= 1'b1;
     gpio_in[13] <= 1'b1;
   end
if (gpio_i2c_feedback_demo == 1'b1) // I2C SCL
for (i = 0; i < 8; i = i + 1)
   if (gpio_dir_reg[i * 4 + 0] == 1'b0) begin 
     gpio_in[i * 4 + 0] <= 1'b1;
     gpio_in[i * 4 + 2] <= 1'b1;
   end else begin
     gpio_in[i * 4 + 0] <= gpio_out_reg[i * 4 + 0];
     gpio_in[i * 4 + 2] <= gpio_out_reg[i * 4 + 0];
   end
if (gpio_i2c_feedback_demo == 1'b1) // I2C SDA (combined [11] and [13] bit)
for (i = 0; i < 8; i = i + 1)
   if (gpio_dir_reg[i * 4 + 1] == 1'b1) begin 
     gpio_in[i * 4 + 1] <= gpio_out_reg[i * 4 + 1];
     gpio_in[i * 4 + 3] <= gpio_out_reg[i * 4 + 1];
   end else if (gpio_dir_reg[i * 4 + 3] == 1'b1) begin 
     gpio_in[i * 4 + 1] <= gpio_out_reg[i * 4 + 3];
     gpio_in[i * 4 + 3] <= gpio_out_reg[i * 4 + 3];
   end else begin
     gpio_in[i * 4 + 1] <= 1'b1;
     gpio_in[i * 4 + 3] <= 1'b1;
   end
if (gpio_spi_feedback == 1'b1) begin
   if (gpio_dir_reg[8] == 1'b1) begin // SS
     gpio_in[12] <= gpio_out_reg[8];
   end else begin
     gpio_in[12] <= 1'b0;
   end
   if (gpio_dir_reg[9] == 1'b1) begin // SCLK
     gpio_in[13] <= gpio_out_reg[9];
   end else begin
     gpio_in[13] <= 1'b0;
   end
   if (gpio_dir_reg[10] == 1'b1) begin // MOSI
     gpio_in[14] <= gpio_out_reg[10];
   end else begin
     gpio_in[14] <= 1'b0;
   end
   if (gpio_dir_reg[15] == 1'b1) begin // MISO
     gpio_in[11] <= gpio_out_reg[15];
   end else begin
     gpio_in[11] <= 1'b0;
   end    
end
if (gpio_spi_feedback_demo == 1'b1) begin
   gpio_in[63:32] <= gpio_out_reg[31:0];
end
if (gpio_rs232_feedback == 1'b1) begin
   if (gpio_dir_reg[8] == 1'b1) begin 
     gpio_in[9] <= gpio_out_reg[8];
   end else begin
     gpio_in[9] <= 1'b0;
   end
end
if (gpio_rs232_feedback_demo == 1'b1) begin
   gpio_in[39:32] <= gpio_out_reg[39:32];
end
end

/////////////////////////////////////////////////
// count active threads c0, just to show off
/////////////////////////////////////////////////
integer active_threads_c0_tmp;
integer active_threads_c0;
integer th;
always @ (negedge clk) begin 
   active_threads_c0_tmp = 0;
   for (th = 0; th < 16; th = th + 1)  
      if (quadi.c0.tid_active[th])
         active_threads_c0_tmp = active_threads_c0_tmp + 1;
   active_threads_c0 = active_threads_c0_tmp; 
end

/*
always @ (negedge clk) begin 
   if (quadi.c0.dbus_rdwr_be_final)
      if (quadi.c0.dbus_wr_add_tmp[10:0] == 11'h468) begin
         $display("Data RAM write top 468: %x", quadi.c0.dbus_wr_data_final);
         if (quadi.c0.dbus_wr_data_final[3:0] == 4'h4)
            $finish;
      end
end      
*/

initial begin
clk <= 1'b0;
uart_rec_valid <= 0;
uart_rec_data <= 0;
uart_send_ready <= 1;
cycle_cnt <= 0; 
gpio_i2c_feedback <= 0;
gpio_i2c_feedback_demo <= 0;
gpio_spi_feedback <= 0;
gpio_spi_feedback_demo <= 0;
gpio_rs232_feedback <= 0;
gpio_rs232_feedback_demo <= 0;
prog_access <= 0;
for (tc = 0; tc < 15; tc = tc + 1) begin              // Test 0 - 10 are self testing, 
                                                       // Test 11 temporary
                                                       // Test 12 - 14 demos
   rstn <= 4'h0;
   /////////////////////////////////////////////////
   //   0: Test UART
   /////////////////////////////////////////////////
   if (tc == 0) begin
      cycle_cnt <= 0;
      max_cycle_count <= 8000;
      for (i = 0; i < 1; i = i + 1) begin
         prog_wea <= 4'h0;
         for (ptr = 0; ptr < 100000; ptr = ptr + 4) begin
            program[(ptr) + 0] = 8'h13;
            program[(ptr) + 1] = 8'h00;
            program[(ptr) + 2] = 8'h00;
            program[(ptr) + 3] = 8'h00;
         end
         if (i == 0) $readmemh("../../../../../../tests/system/system_0/work/main_0.hex", program);  
         #7;
         prog_wea <= 32'h000000f << (i * 8);
         prog_access <= 1'b1;
         for (ptr = 0; ptr < 4096 * 4; ptr = ptr + 4) begin
            prog_addra = ptr / 4;
            prog_dina = {program[ptr_offset + ptr + 3], program[ptr_offset + ptr + 2], program[ptr_offset + ptr + 1], program[ptr_offset + ptr]};
            @ (posedge clk);
            #1;
         end
         prog_wea <= 0;
         prog_access <= 1'b0;
      end    
      #7;
      rstn <= 4'hf;

      #8000;

      for (i = 0; i < 8; i = i + 1) begin
         #1000;
         @ (negedge clk);
         uart_rec_valid <= 1'b1;
         uart_rec_data <= i;   
         @ (negedge clk);
         uart_rec_valid <= 1'b0;
      end
      wait (uart_send_req == 1'b1);
      uart_send_ready <= 0;
      for (i = 0; i < 100; i = i + 1)
         @ (negedge clk);
      uart_send_ready <= 1;
      #2000;
      for (i = 0; i < 8; i = i + 1) begin
         #1000;
         @ (negedge clk);
         uart_rec_valid <= 1'b1;
         uart_rec_data <= i;   
         @ (negedge clk);
         uart_rec_valid <= 1'b0;
      end
      #2000;
      if (gpio_out_reg[31:8] == 24'hffffff)
         $display("Test %d passed.", tc); 
      else begin
         $display("Test %d failed.", tc); 
         $finish;
      end 
   end
   /////////////////////////////////////////////////
   //   1: thread scheduler
   /////////////////////////////////////////////////
   if (tc == 1) begin
      cycle_cnt <= 0;
      max_cycle_count <= 5000;
      for (i = 0; i < 1; i = i + 1) begin
         prog_wea <= 4'h0;
         for (ptr = 0; ptr < 100000; ptr = ptr + 4) begin
            program[(ptr) + 0] = 8'h13;
            program[(ptr) + 1] = 8'h00;
            program[(ptr) + 2] = 8'h00;
            program[(ptr) + 3] = 8'h00;
         end
         if (i == 0) $readmemh("../../../../../../tests/system/system_5/work/main_0.hex", program);  
         #7;
         prog_wea <= 32'h000000f << (i * 8);
         prog_access <= 1'b1;
         for (ptr = 0; ptr < 4096 * 4; ptr = ptr + 4) begin
            prog_addra = ptr / 4;
            prog_dina = {program[ptr_offset + ptr + 3], program[ptr_offset + ptr + 2], program[ptr_offset + ptr + 1], program[ptr_offset + ptr]};
            @ (posedge clk);
            #1;
         end
         prog_wea <= 0;
         prog_access <= 1'b0;
      end    
      #7;
      rstn <= 4'hf;

      gpio_in[31:8] <= 24'h7EA55A;

      wait (gpio_out_reg[31:0] == 32'h7EA55A07);
      #500;
      wait (gpio_out_reg[31:0] == 32'h7EA55A07);     // bit 3 must not be set
      #500;
      $display("Test %d passed.", tc); 
   end
   /////////////////////////////////////////////////
   //   2: Basic CA test
   //      Data passing via TC test
   //      TC fifo and TC full test
   /////////////////////////////////////////////////
   if (tc == 2) begin 
      cycle_cnt <= 0;
      max_cycle_count <= 55000;
      for (i = 0; i < 1; i = i + 1) begin
         prog_wea <= 4'h0;
         for (ptr = 0; ptr < 100000; ptr = ptr + 4) begin
            program[(ptr) + 0] = 8'h13;
            program[(ptr) + 1] = 8'h00;
            program[(ptr) + 2] = 8'h00;
            program[(ptr) + 3] = 8'h00;
         end
         if (i == 0) $readmemh("../../../../../../tests/system/system_1/work/main_0.hex", program);  
         #7;
         prog_wea <= 32'h000000f << (i * 8);
         prog_access <= 1'b1;
         for (ptr = 0; ptr < 4096 * 4; ptr = ptr + 4) begin
            prog_addra = ptr / 4;
            prog_dina = {program[ptr_offset + ptr + 3], program[ptr_offset + ptr + 2], program[ptr_offset + ptr + 1], program[ptr_offset + ptr]};
            @ (posedge clk);
            #1;
         end
         prog_wea <= 0;
         prog_access <= 1'b0;
      end
      #7;
      rstn <= 4'hf;

      wait (gpio_out_reg[16] == 1'b1); 
      wait (gpio_out_reg[16] == 1'b0);
      wait (gpio_out_reg[16] == 1'b1);
      wait (gpio_out_reg[16] == 1'b0);
      wait (gpio_out_reg[16] == 1'b1);
      wait (gpio_out_reg[16] == 1'b0);
      wait (gpio_out_reg[0] == 1'b1);
      wait (gpio_out_reg[1] == 1'b1);
      wait (gpio_out_reg[2] == 1'b1);
      wait (gpio_out_reg[3] == 1'b1);
      wait (gpio_out_reg[4] == 1'b1);
      wait (gpio_out_reg[5] == 1'b1);
      wait (gpio_out_reg[6] == 1'b1);
      wait (gpio_out_reg[7] == 1'b1);
      wait (gpio_out_reg[8] == 1'b1);
      wait (gpio_out_reg[9] == 1'b1);
      wait (gpio_out_reg[10] == 1'b1);
      wait (gpio_out_reg[11] == 1'b1);
      wait (gpio_out_reg[12] == 1'b1);
      wait (gpio_out_reg[13] == 1'b1);
      wait (gpio_out_reg[14] == 1'b1);
      wait (gpio_out_reg[15] == 1'b1);
      wait (gpio_out_reg[16:0] == 17'h10000);
      wait (gpio_out_reg[16:0] == 17'h00000);
      #20000;
      if (gpio_out_reg == 0)
         $display("Test %d passed.", tc); 
      else begin
         $display("Test %d failed.", tc); 
         $finish;
      end
   end
   /////////////////////////////////////////////////
   //   3: GPIO event passing
   /////////////////////////////////////////////////
   if (tc == 3) begin
      cycle_cnt <= 0;
      max_cycle_count <= 9000;
      for (i = 0; i < 1; i = i + 1) begin
         prog_wea <= 4'h0;
         for (ptr = 0; ptr < 100000; ptr = ptr + 4) begin
            program[(ptr) + 0] = 8'h13;
            program[(ptr) + 1] = 8'h00;
            program[(ptr) + 2] = 8'h00;
            program[(ptr) + 3] = 8'h00;
         end
         if (i == 0) $readmemh("../../../../../../tests/system/system_3/work/system_3_0.hex", program);  
         #7;
         prog_wea <= 32'h000000f << (i * 8);
         prog_access <= 1'b1;
         for (ptr = 0; ptr < 4096 * 4; ptr = ptr + 4) begin
            prog_addra = ptr / 4;
            prog_dina = {program[ptr_offset + ptr + 3], program[ptr_offset + ptr + 2], program[ptr_offset + ptr + 1], program[ptr_offset + ptr]};
            @ (posedge clk);
            #1;
         end
         prog_wea <= 0;
         prog_access <= 1'b0;
      end    
      #7;
      rstn <= 4'hf;
      #4000;

      gpio_in[30] <= {1{1'b1}};
      wait (gpio_out_reg[30] == 1'b1);
      gpio_in[47] <= {1{1'b1}};
      wait (gpio_out_reg[47] == 1'b1);

      gpio_in <= {112{1'b1}};
      #35000;
        if (gpio_out_reg[111:0] == 112'h0000000000ffffffffffffffffff)
         $display("Test %d passed.", tc); 
      else begin
         $display("Test %d failed.", tc); 
         $finish;  
      end 
   end
   /////////////////////////////////////////////////
   //   4: data memory access
   /////////////////////////////////////////////////
   if (tc == 4) begin
      cycle_cnt <= 0;
      max_cycle_count <= 8000;
      for (i = 0; i < 1; i = i + 1) begin
         prog_wea <= 4'h0;
         for (ptr = 0; ptr < 100000; ptr = ptr + 4) begin
            program[(ptr) + 0] = 8'h13;
            program[(ptr) + 1] = 8'h00;
            program[(ptr) + 2] = 8'h00;
            program[(ptr) + 3] = 8'h00;
         end
         if (i == 0) $readmemh("../../../../../../tests/system/system_4/work/main_0.hex", program);  
         #7;
         prog_wea <= 32'h000000f << (i * 8);
         prog_access <= 1'b1;
         for (ptr = 0; ptr < 4096 * 4; ptr = ptr + 4) begin
            prog_addra = ptr / 4;
            prog_dina = {program[ptr_offset + ptr + 3], program[ptr_offset + ptr + 2], program[ptr_offset + ptr + 1], program[ptr_offset + ptr]};
            @ (posedge clk);
            #1;
         end
         prog_wea <= 0;
         prog_access <= 1'b0;
      end    
      #7;
      rstn <= 4'hf;

      #20000;
      if (gpio_out_reg[55:0] == 56'hffffffffffffff)
         $display("Test %d passed.", tc); 
      else begin
         $display("Test %d failed.", tc); 
         $finish;  
      end 
   end
   /////////////////////////////////////////////////
   //   5: Message passing
   /////////////////////////////////////////////////
   if (tc == 5) begin
      cycle_cnt <= 0;
      max_cycle_count <= 19000;
      for (i = 0; i < 4; i = i + 1) begin
         prog_wea <= 4'h0;
         for (ptr = 0; ptr < 100000; ptr = ptr + 4) begin
            program[(ptr) + 0] = 8'h13;
            program[(ptr) + 1] = 8'h00;
            program[(ptr) + 2] = 8'h00;
            program[(ptr) + 3] = 8'h00;
         end
         if (i == 1) $readmemh("../../../../../../tests/system/system_2/work/main_1.hex", program);
         if (i == 2) $readmemh("../../../../../../tests/system/system_2/work/main_2.hex", program);
         if (i == 3) $readmemh("../../../../../../tests/system/system_2/work/main_3.hex", program);
         #7;
         prog_wea <= 32'h000000f << (i * 8);
         prog_access <= 1'b1;
         for (ptr = 0; ptr < 4096 * 4; ptr = ptr + 4) begin
            prog_addra = ptr / 4;
            prog_dina = {program[ptr_offset + ptr + 3], program[ptr_offset + ptr + 2], program[ptr_offset + ptr + 1], program[ptr_offset + ptr]};
            @ (posedge clk);
            #1;
         end
         prog_wea <= 0;
         prog_access <= 1'b0;
      end
      #100;
      rstn <= 4'hf;

      wait (gpio_out_reg[23:0] == 24'hffff00);
      if (gpio_out_reg[23:0] == 24'hffff00)
         $display("Test %d passed.", tc); 
      else begin
         $display("Test %d failed.", tc); 
         $finish;  
      end 
   end
   /////////////////////////////////////////////////
   //   6: drivers PWM
   /////////////////////////////////////////////////
   if (tc == 6) begin
      cycle_cnt <= 0;
      max_cycle_count <= 37000;
      for (i = 0; i < 1; i = i + 1) begin
         prog_wea <= 4'h0;
         for (ptr = 0; ptr < 100000; ptr = ptr + 4) begin
            program[(ptr) + 0] = 8'h13;
            program[(ptr) + 1] = 8'h00;
            program[(ptr) + 2] = 8'h00;
            program[(ptr) + 3] = 8'h00;
         end
         if (i == 0) $readmemh("../../../../../../tests/drivers/drivers_pwm/work/main_0.hex", program);
         #7;
         prog_wea <= 32'h000000f << (i * 8);
         prog_access <= 1'b1;
         for (ptr = 0; ptr < 4096 * 4; ptr = ptr + 4) begin
            prog_addra = ptr / 4;
            prog_dina = {program[ptr_offset + ptr + 3], program[ptr_offset + ptr + 2], program[ptr_offset + ptr + 1], program[ptr_offset + ptr]};
            @ (posedge clk);
            #1;
         end
         prog_wea <= 0;
         prog_access <= 1'b0;
      end    
      #7;
      rstn <= 4'hf;

      wait (gpio_out_reg[9] == 1'b0);
      wait (gpio_out_reg[9] == 1'b1);
      wait (gpio_out_reg[9] == 1'b0);
      wait (gpio_out_reg[9] == 1'b1);
      wait (gpio_out_reg[9] == 1'b0);
      wait (gpio_out_reg[9] == 1'b1);
      wait (gpio_out_reg[9] == 1'b0);
      wait (gpio_out_reg[9] == 1'b1);
      #5000;
      $display("Test %d passed.", tc); 
   end
   /////////////////////////////////////////////////
   //   7: drivers SPI
   /////////////////////////////////////////////////
   if (tc == 7) begin
      cycle_cnt <= 0;
      max_cycle_count <= 47000;
      for (i = 0; i < 2; i = i + 1) begin
         prog_wea <= 4'h0;
         for (ptr = 0; ptr < 100000; ptr = ptr + 4) begin
            program[(ptr) + 0] = 8'h13;
            program[(ptr) + 1] = 8'h00;
            program[(ptr) + 2] = 8'h00;
            program[(ptr) + 3] = 8'h00;
         end
         if (i == 0) $readmemh("../../../../../../tests/drivers/drivers_spi/work/main_0.hex", program);  
         if (i == 1) $readmemh("../../../../../../tests/drivers/drivers_spi/work/main_1.hex", program);  
         #7;
         prog_wea <= 32'h000000f << (i * 8);
         prog_access <= 1'b1;
         for (ptr = 0; ptr < 4096 * 4; ptr = ptr + 4) begin
            prog_addra = ptr / 4;
            prog_dina = {program[ptr_offset + ptr + 3], program[ptr_offset + ptr + 2], program[ptr_offset + ptr + 1], program[ptr_offset + ptr]};
            @ (posedge clk);
            #1;
         end
         prog_wea <= 0;
         prog_access <= 1'b0;
      end    
      #7;
      rstn <= 4'hf;

      gpio_spi_feedback <= 1'b1;

      wait (gpio_out_reg[23:0] == 24'h7e875a);
      #5000;
      $display("Test %d passed.", tc); 
      gpio_spi_feedback <= 1'b0;
   end
   /////////////////////////////////////////////////
   //   8: drivers RS232
   /////////////////////////////////////////////////
   if (tc == 8) begin
      cycle_cnt <= 0;
      max_cycle_count <= 45000;
      for (i = 0; i < 1; i = i + 1) begin
         prog_wea <= 4'h0;
         for (ptr = 0; ptr < 100000; ptr = ptr + 4) begin
            program[(ptr) + 0] = 8'h13;
            program[(ptr) + 1] = 8'h00;
            program[(ptr) + 2] = 8'h00;
            program[(ptr) + 3] = 8'h00;
         end
         if (i == 0) $readmemh("../../../../../../tests/drivers/drivers_rs232/work/main_0.hex", program);  
         #7;
         prog_wea <= 32'h000000f << (i * 8);
         prog_access <= 1'b1;
         for (ptr = 0; ptr < 4096 * 4; ptr = ptr + 4) begin
            prog_addra = ptr / 4;
            prog_dina = {program[ptr_offset + ptr + 3], program[ptr_offset + ptr + 2], program[ptr_offset + ptr + 1], program[ptr_offset + ptr]};
            @ (posedge clk);
            #1;
         end
         prog_wea <= 0;
         prog_access <= 1'b0;
      end    
      #7;
      rstn <= 4'h1; // only core_0 active

      gpio_rs232_feedback <= 1'b1;

      wait (gpio_out_reg[23:0] == 24'hff015a);
      #5000;
      $display("Test %d passed.", tc); 
      gpio_rs232_feedback <= 1'b0;
   end
   /////////////////////////////////////////////////
   //   9: blinky64
   /////////////////////////////////////////////////
   if (tc == 9) begin
      cycle_cnt <= 0;
      max_cycle_count <= 190000;
      for (i = 0; i < 4; i = i + 1) begin
         prog_wea <= 4'h0;
         for (ptr = 0; ptr < 100000; ptr = ptr + 4) begin
            program[(ptr) + 0] = 8'h13;
            program[(ptr) + 1] = 8'h00;
            program[(ptr) + 2] = 8'h00;
            program[(ptr) + 3] = 8'h00;
         end
         if (i == 0) $readmemh("../../../../../../tests/system/blinky64/work/main_0.hex", program);  
         if (i == 1) $readmemh("../../../../../../tests/system/blinky64/work/main_1.hex", program);  
         if (i == 2) $readmemh("../../../../../../tests/system/blinky64/work/main_2.hex", program);  
         if (i == 3) $readmemh("../../../../../../tests/system/blinky64/work/main_3.hex", program);  
         #7;
         prog_wea <= 32'h000000f << (i * 8);
         prog_access <= 1'b1;
         for (ptr = 0; ptr < 4096 * 4; ptr = ptr + 4) begin
            prog_addra = ptr / 4;
            prog_dina = {program[ptr_offset + ptr + 3], program[ptr_offset + ptr + 2], program[ptr_offset + ptr + 1], program[ptr_offset + ptr]};
            @ (posedge clk);
            #1;
         end
         prog_wea <= 0;
         prog_access <= 1'b0;
      end    
      #7;
      rstn <= 4'hf;

      wait (gpio_out_reg[111:0] == 112'h000000000000ffffffffffffffff);
      wait (gpio_out_reg[111:0] == 112'h0000000000000000000000000003);
      $display("Test %d passed.", tc); 
   end
   /////////////////////////////////////////////////
   //   10: drivers I2C
   /////////////////////////////////////////////////
   if (tc == 10) begin
      cycle_cnt <= 0;
      max_cycle_count <= 650000;
      for (i = 0; i < 2; i = i + 1) begin
         prog_wea <= 4'h0;
         for (ptr = 0; ptr < 100000; ptr = ptr + 4) begin
            program[(ptr) + 0] = 8'h13;
            program[(ptr) + 1] = 8'h00;
            program[(ptr) + 2] = 8'h00;
            program[(ptr) + 3] = 8'h00;
         end
         if (i == 0) $readmemh("../../../../../../tests/drivers/drivers_i2c/work/main_0.hex", program);  
         if (i == 1) $readmemh("../../../../../../tests/drivers/drivers_i2c/work/main_1.hex", program);  
         #7;
         prog_wea <= 32'h000000f << (i * 8);
         prog_access <= 1'b1;
         for (ptr = 0; ptr < 4096 * 4; ptr = ptr + 4) begin
            prog_addra = ptr / 4;
            prog_dina = {program[ptr_offset + ptr + 3], program[ptr_offset + ptr + 2], program[ptr_offset + ptr + 1], program[ptr_offset + ptr]};
            @ (posedge clk);
            #1;
         end
         prog_wea <= 0;
         prog_access <= 1'b0;
      end    
      #7;
      rstn <= 4'hf;

      gpio_i2c_feedback <= 1'b1;

      wait (gpio_out_reg[23:0] == 24'h8100a5);
      $display("Test %d passed.", tc); 
      $finish;
      #5000;

      gpio_i2c_feedback <= 1'b0;
   end
   /////////////////////////////////////////////////
   //   11: tmp
   /////////////////////////////////////////////////
   if (tc == 11) begin
      cycle_cnt <= 0;
      max_cycle_count <= 60000;
      for (i = 0; i < 1; i = i + 1) begin
         prog_wea <= 4'h0;
         for (ptr = 0; ptr < 100000; ptr = ptr + 4) begin
            program[(ptr) + 0] = 8'h13;
            program[(ptr) + 1] = 8'h00;
            program[(ptr) + 2] = 8'h00;
            program[(ptr) + 3] = 8'h00;
         end
         if (i == 0) $readmemh("../../../../../../tests/tmp/tmp_0/work/main_0.hex", program);  
         #7;
         prog_wea <= 32'h000000f << (i * 8);
         prog_access <= 1'b1;
         for (ptr = 0; ptr < 4096 * 4; ptr = ptr + 4) begin
            prog_addra = ptr / 4;
            prog_dina = {program[ptr_offset + ptr + 3], program[ptr_offset + ptr + 2], program[ptr_offset + ptr + 1], program[ptr_offset + ptr]};
            @ (posedge clk);
            #1;
         end
         prog_wea <= 0;
         prog_access <= 1'b0;
      end    
      #7;
      rstn <= 4'hf;

      wait (gpio_out_reg[9] == 1'b1);
      #5000;
      $display("Test %d passed.", tc); 
   end
   /////////////////////////////////////////////////
   //   12: demo drivers RS232
   /////////////////////////////////////////////////
   if (tc == 12) begin
      cycle_cnt <= 0;
      max_cycle_count <= 650000;
      for (i = 0; i < 1; i = i + 1) begin
         prog_wea <= 4'h0;
         for (ptr = 0; ptr < 100000; ptr = ptr + 4) begin
            program[(ptr) + 0] = 8'h13;
            program[(ptr) + 1] = 8'h00;
            program[(ptr) + 2] = 8'h00;
            program[(ptr) + 3] = 8'h00;
         end
         if (i == 0) $readmemh("../../../../../../demo/drivers_rs232/work/main_0.hex", program);  
         #7;
         prog_wea <= 32'h000000f << (i * 8);
         prog_access <= 1'b1;
         for (ptr = 0; ptr < 4096 * 4; ptr = ptr + 4) begin
            prog_addra = ptr / 4;
            prog_dina = {program[ptr_offset + ptr + 3], program[ptr_offset + ptr + 2], program[ptr_offset + ptr + 1], program[ptr_offset + ptr]};
            @ (posedge clk);
            #1;
         end
         prog_wea <= 0;
         prog_access <= 1'b0;
      end    
      #7;
      rstn <= 4'h1;

      gpio_rs232_feedback_demo <= 1'b1;

      wait (gpio_out_reg[32:0] == 33'h100ff005a);
      #5000;
      $display("Test %d passed.", tc); 
      gpio_rs232_feedback_demo <= 1'b0;
   end
   /////////////////////////////////////////////////
   //   13: demo drivers I2C
   /////////////////////////////////////////////////
   if (tc == 13) begin
      cycle_cnt <= 0;
      max_cycle_count <= 2300000;
      for (i = 0; i < 1; i = i + 1) begin
         prog_wea <= 4'h0;
         for (ptr = 0; ptr < 100000; ptr = ptr + 4) begin
            program[(ptr) + 0] = 8'h13;
            program[(ptr) + 1] = 8'h00;
            program[(ptr) + 2] = 8'h00;
            program[(ptr) + 3] = 8'h00;
         end
         if (i == 0) $readmemh("../../../../../../demo/drivers_i2c/work/main_0.hex", program);  
         #7;
         prog_wea <= 32'h000000f << (i * 8);
         prog_access <= 1'b1;
         for (ptr = 0; ptr < 4096 * 4; ptr = ptr + 4) begin
            prog_addra = ptr / 4;
            prog_dina = {program[ptr_offset + ptr + 3], program[ptr_offset + ptr + 2], program[ptr_offset + ptr + 1], program[ptr_offset + ptr]};
            @ (posedge clk);
            #1;
         end
         prog_wea <= 0;
         prog_access <= 1'b0;
      end    
      #7;
      rstn <= 4'h1;

      gpio_i2c_feedback_demo <= 1'b1;

      wait (gpio_out_reg[32:0] == 33'h100ff005a);
      #5000;
      $display("Test %d passed.", tc); 
      gpio_i2c_feedback_demo <= 1'b0;
   end
   /////////////////////////////////////////////////
   //   14: demo blinky64
   /////////////////////////////////////////////////
   if (tc == 14) begin
      cycle_cnt <= 0;
      max_cycle_count <= 190000;
      for (i = 0; i < 4; i = i + 1) begin
         prog_wea <= 4'h0;
         for (ptr = 0; ptr < 100000; ptr = ptr + 4) begin
            program[(ptr) + 0] = 8'h13;
            program[(ptr) + 1] = 8'h00;
            program[(ptr) + 2] = 8'h00;
            program[(ptr) + 3] = 8'h00;
         end
         if (i == 0) $readmemh("../../../../../../demo/blinky64/work/main_0.hex", program);  
         if (i == 1) $readmemh("../../../../../../demo/blinky64/work/main_1.hex", program);  
         if (i == 2) $readmemh("../../../../../../demo/blinky64/work/main_2.hex", program);  
         if (i == 3) $readmemh("../../../../../../demo/blinky64/work/main_3.hex", program);  
         #7;
         prog_wea <= 32'h000000f << (i * 8);
         prog_access <= 1'b1;
         for (ptr = 0; ptr < 4096 * 4; ptr = ptr + 4) begin
            prog_addra = ptr / 4;
            prog_dina = {program[ptr_offset + ptr + 3], program[ptr_offset + ptr + 2], program[ptr_offset + ptr + 1], program[ptr_offset + ptr]};
            @ (posedge clk);
            #1;
         end
         prog_wea <= 0;
         prog_access <= 1'b0;
      end    
      #7;
      rstn <= 4'hf;

      wait (gpio_out_reg[111:0] == 112'h000000000000ffffffffffffffff);
      wait (gpio_out_reg[111:0] == 112'h0000000000000000000000000003);
      $display("Test %d passed.", tc); 
   end
end
$finish;
end


always #5 clk <= ~clk;

always @ (posedge clk)
if (cycle_cnt == max_cycle_count) begin
   $display("Test %d runaway.\n", tc); 
   $finish;  
end else
   cycle_cnt <= cycle_cnt + 1;


quad quadi (clk,
            rstn,
            prog_access,
            prog_wea,
            prog_addra, 
            prog_dina,
            prog_douta,
            uart_rec_valid,
            uart_rec_data, 
            uart_send_ready,
            uart_send_req,
            uart_send_data,
            gpio_dir_reg,
            gpio_out_reg,
            gpio_in);


endmodule
