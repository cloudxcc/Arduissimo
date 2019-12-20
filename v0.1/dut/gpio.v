//
// Copyright 2019 Tobias Strauch, Munich, Bavaria
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//

module gpio (clk,
             rstn,
             gpio_dir_reg,
             gpio_out_reg,
             gpio_in,
             dbus_rdwr_gpio_base,
             dbus_rd_add_bus,
             dbus_wr_add_bus,
             dbus_rdwr_be_in,
             dbus_wr_en,
             dbus_wr_data_in,
             dbus_rd_req,
             dbus_rd_data_out,
             ca_match,
             ca_command_in,
             gpio_event,
             gpio_event_ack,
             gpio_command_out);

`include "MPSOC_parameters.v"

input clk;
input rstn;
output [111:0] gpio_dir_reg;
output [111:0] gpio_out_reg;
input [111:0] gpio_in;
input [3:0] dbus_rdwr_gpio_base;
input [127:0] dbus_rd_add_bus;
input [127:0] dbus_wr_add_bus;
input [7:0] dbus_rdwr_be_in;
input [3:0] dbus_wr_en;
input [127:0] dbus_wr_data_in;
input [3:0] dbus_rd_req;
output [127:0] dbus_rd_data_out;
input [3:0] ca_match;
input [127:0] ca_command_in;
output [3:0] gpio_event;
input [3:0] gpio_event_ack;
output [127:0] gpio_command_out;

reg [3:0] dbus_rdwr_gpio_base_c3;
wire [31:0] dbus_rdwr_add [3:0];
reg [31:0] dbus_rdwr_add_c3 [3:0];
assign dbus_rdwr_add[0] = dbus_rd_add_bus[ 31: 0];
assign dbus_rdwr_add[1] = dbus_rd_add_bus[ 63:32];
assign dbus_rdwr_add[2] = dbus_rd_add_bus[ 95:64];
assign dbus_rdwr_add[3] = dbus_rd_add_bus[127:96];

wire [31:0] dbus_wr_add [3:0];
reg [31:0] dbus_wr_add_c2 [3:0];
reg [31:0] dbus_wr_add_c3 [3:0];
assign dbus_wr_add[0] = dbus_wr_add_bus[ 31: 0];
assign dbus_wr_add[1] = dbus_wr_add_bus[ 63:32];
assign dbus_wr_add[2] = dbus_wr_add_bus[ 95:64];
assign dbus_wr_add[3] = dbus_wr_add_bus[127:96];

reg [3:0] dbus_rd_req_c3;
reg [3:0] dbus_wr_en_c3;

wire [31:0] dbus_wr_data [3:0];
reg [31:0] dbus_wr_data_c2 [3:0];
reg [31:0] dbus_wr_data_c3 [3:0];
reg [31:0] dbus_wr_data_c4 [3:0];
assign dbus_wr_data[0] = dbus_wr_data_in[ 31: 0];
assign dbus_wr_data[1] = dbus_wr_data_in[ 63:32];
assign dbus_wr_data[2] = dbus_wr_data_in[ 95:64];
assign dbus_wr_data[3] = dbus_wr_data_in[127:96];

reg [31:0] dbus_rd_data [3:0];
assign  dbus_rd_data_out = {dbus_rd_data[3], dbus_rd_data[2], dbus_rd_data[1], dbus_rd_data[0]};

reg [31:0] ca_command_reg [3:0];
reg [31:0] ca_command_pipe_reg [3:0];
reg [3:0] ca_match_reg;
reg [3:0] ca_match_pipe_reg;

always @(posedge clk or negedge rstn)
if (~rstn) begin
   for (l = 0; l < 4; l = l + 1) begin
      ca_command_pipe_reg[l] <= 0;
      ca_command_reg[l] <= 0;
   end
   ca_match_pipe_reg <= 0;
   ca_match_reg <= 0;
end else begin
   for (l = 0; l < 4; l = l + 1) begin
      ca_command_reg[l] <= ca_command_pipe_reg[l];
   end
   ca_command_pipe_reg[0] <= ca_command_in[ 31: 0];
   ca_command_pipe_reg[1] <= ca_command_in[ 63:32];
   ca_command_pipe_reg[2] <= ca_command_in[ 95:64];
   ca_command_pipe_reg[3] <= ca_command_in[127:96];
   ca_match_reg <= ca_match_pipe_reg;
   ca_match_pipe_reg <= ca_match;
end

reg [31:0] gpio_command [3:0];
assign  gpio_command_out = {gpio_command[3], gpio_command[2], gpio_command[1], gpio_command[0]};

reg [111:0] gpio_dir_reg;
reg [111:0] gpio_out_reg;
reg [111:0] gpio_level_reg;
reg [127:0] gpio_armed_reg;
reg [127:0] gpio_event_reg;
reg [111:0] gpio_cap_reg;
reg [1:0] gpio_core_reg [127:0];
reg [111:0] gpio_in_l1_reg;
reg [111:0] gpio_in_l2_reg;
reg [111:0] gpio_in_l3_reg;
reg [111:0] gpio_in_reg;

integer k, l, m, n;

// CLEAR DIR
reg [14:0] gpio_clr_dir_dbus_reg [3:0];

always @(posedge clk or negedge rstn)
if (~rstn) begin
   for (l = 0; l < 4; l = l + 1)
      gpio_clr_dir_dbus_reg[l] <= 0;
end else begin
   for (l = 0; l < 4; l = l + 1)
      gpio_clr_dir_dbus_reg[l] <= 0;
   for (k = 0; k < 15; k = k + 1) begin            // go through all 8 bit wide banks
      for (l = 0; l < 4; l = l + 1) begin         // go through all 4 cores addressing possibilities
         if (dbus_wr_en_c3[l] & dbus_rdwr_gpio_base_c3[l] & (dbus_wr_add_c3[l][15:0] == (k * GPIO_STEP) + GPIO_DIR_CLR))
           gpio_clr_dir_dbus_reg[l][k] <= 1'b1;
      end
   end
end

reg [14:0] gpio_clr_dir_ca_reg [3:0];

always @(posedge clk or negedge rstn)
if (~rstn) begin
   for (l = 0; l < 4; l = l + 1)
      gpio_clr_dir_ca_reg[l] <= 0;
end else begin
   for (l = 0; l < 4; l = l + 1)
      gpio_clr_dir_ca_reg[l] <= 0;
   for (k = 0; k < 15; k = k + 1) begin            // go through all 8 bit wide banks
      for (l = 0; l < 4; l = l + 1) begin         // go through all 4 cores addressing possibilities
         if (ca_match_reg[l] & (ca_command_reg[l][31:29] == 3'b010) & (ca_command_reg[l][11:8] == k))
            gpio_clr_dir_ca_reg[l][k] <= 1'b1;
      end
   end
end

reg [111:0] gpio_clr_dir_sig;

always @(gpio_clr_dir_dbus_reg or dbus_wr_data_c4 or gpio_clr_dir_ca_reg or ca_command_reg)
begin
   gpio_clr_dir_sig = 0;                           // overwrite bit-wise
   for (k = 0; k < 15; k = k + 1) begin            // go through all 8 bit wide banks
      for (l = 0; l < 4; l = l + 1) begin         // go through all 4 cores addressing possibilities
         if (gpio_clr_dir_dbus_reg[l][k])
            for (m = 0; m < 8; m = m + 1) begin   // go through all bits, bus wide didn't work 
               if (dbus_wr_data_c4[l][m])
                  gpio_clr_dir_sig[k * 8 + m] = 1'b1;
            end
         if (gpio_clr_dir_ca_reg[l][k])
            for (m = 0; m < 8; m = m + 1) begin   // go through all bits, bus wide didn't work 
               if (ca_command_reg[l][m])
                  gpio_clr_dir_sig[k * 8 + m] = 1'b1;
            end 
      end
   end
end

// SET DIR
reg [14:0] gpio_set_dir_dbus_reg [3:0];

always @(posedge clk or negedge rstn)
if (~rstn) begin
   for (l = 0; l < 4; l = l + 1)
      gpio_set_dir_dbus_reg[l] <= 0;
end else begin
   for (l = 0; l < 4; l = l + 1)
      gpio_set_dir_dbus_reg[l] <= 0;
   for (k = 0; k < 15; k = k + 1) begin            // go through all 8 bit wide banks
      for (l = 0; l < 4; l = l + 1) begin         // go through all 4 cores addressing possibilities
         if (dbus_wr_en_c3[l] & dbus_rdwr_gpio_base_c3[l] & (dbus_wr_add_c3[l][15:0] == (k * GPIO_STEP) + GPIO_DIR_SET))
            gpio_set_dir_dbus_reg[l][k] <= 1'b1;
      end
   end
end

reg [14:0] gpio_set_dir_ca_reg [3:0];

always @(posedge clk or negedge rstn)
if (~rstn) begin
   for (l = 0; l < 4; l = l + 1)
      gpio_set_dir_ca_reg[l] <= 0;
end else begin
   for (l = 0; l < 4; l = l + 1)
      gpio_set_dir_ca_reg[l] <= 0;
   for (k = 0; k < 15; k = k + 1) begin            // go through all 8 bit wide banks
      for (l = 0; l < 4; l = l + 1) begin         // go through all 4 cores addressing possibilities
         if (ca_match_reg[l] & (ca_command_reg[l][31:29] == 3'b011) & (ca_command_reg[l][11:8] == k))
            gpio_set_dir_ca_reg[l][k] <= 1'b1;
      end
   end
end

reg [111:0] gpio_set_dir_sig;

always @(gpio_set_dir_dbus_reg or dbus_wr_data_c4 or gpio_set_dir_ca_reg or ca_command_reg)
begin
   gpio_set_dir_sig = 0;                           // overwrite bit-wise
   for (k = 0; k < 15; k = k + 1) begin            // go through all 8 bit wide banks
      for (l = 0; l < 4; l = l + 1) begin         // go through all 4 cores addressing possibilities
         if (gpio_set_dir_dbus_reg[l][k])
            for (m = 0; m < 8; m = m + 1) begin   // go through all bits, bus wide didn't work 
               if (dbus_wr_data_c4[l][m])
                  gpio_set_dir_sig[k * 8 + m] = 1'b1;
            end 
         if (gpio_set_dir_ca_reg[l][k])
            for (m = 0; m < 8; m = m + 1) begin   // go through all bits, bus wide didn't work 
               if (ca_command_reg[l][m])
                  gpio_set_dir_sig[k * 8 + m] = 1'b1;
            end 
      end
   end
end

// DIR
always @(posedge clk or negedge rstn)
if (~rstn) begin
   gpio_dir_reg <= 0;
end else begin
   gpio_dir_reg <= (gpio_dir_reg | gpio_set_dir_sig) & (~gpio_clr_dir_sig);
end


// CLEAR OUT
reg [14:0] gpio_clr_out_dbus_reg [3:0];
always @(posedge clk or negedge rstn)
if (~rstn) begin
   for (l = 0; l < 4; l = l + 1)
      gpio_clr_out_dbus_reg[l] <= 0;
end else begin
   for (l = 0; l < 4; l = l + 1)
      gpio_clr_out_dbus_reg[l] <= 0;
   for (k = 0; k < 15; k = k + 1) begin            // go through all 8 bit wide banks
      for (l = 0; l < 4; l = l + 1) begin         // go through all 4 cores addressing possibilities
         if (dbus_wr_en_c3[l] & dbus_rdwr_gpio_base_c3[l] & (dbus_wr_add_c3[l][15:0] == (k * GPIO_STEP) + GPIO_OUT_CLR))
            gpio_clr_out_dbus_reg[l][k] <= 1'b1;
       end
   end
end

reg [14:0] gpio_clr_out_ca_reg [3:0];
always @(posedge clk or negedge rstn)
if (~rstn) begin
   for (l = 0; l < 4; l = l + 1)
      gpio_clr_out_ca_reg[l] <= 0;
end else begin
   for (l = 0; l < 4; l = l + 1)
      gpio_clr_out_ca_reg[l] <= 0;
   for (k = 0; k < 15; k = k + 1) begin            // go through all 8 bit wide banks
      for (l = 0; l < 4; l = l + 1) begin         // go through all 4 cores addressing possibilities
         if (ca_match_reg[l] & (ca_command_reg[l][31:29] == 3'b000) & (ca_command_reg[l][11:8] == k))
            gpio_clr_out_ca_reg[l][k] <= 1'b1;
       end
   end
end

reg [111:0] gpio_clr_out_sig;
always @(gpio_clr_out_dbus_reg or dbus_wr_data_c4 or gpio_clr_out_ca_reg or ca_command_reg)
begin
   gpio_clr_out_sig = 0;                               // overwrite bit-wise
   for (k = 0; k < 15; k = k + 1) begin            // go through all 8 bit wide banks
      for (l = 0; l < 4; l = l + 1) begin         // go through all 4 cores addressing possibilities
         if (gpio_clr_out_dbus_reg[l][k])
            for (m = 0; m < 8; m = m + 1) begin   // go through all bits, bus wide didn't work 
               if (dbus_wr_data_c4[l][m])
                  gpio_clr_out_sig[k * 8 + m] = 1'b1;
            end
         if (gpio_clr_out_ca_reg[l][k])
            for (m = 0; m < 8; m = m + 1) begin   // go through all bits, bus wide didn't work 
               if (ca_command_reg[l][m])
                  gpio_clr_out_sig[k * 8 + m] = 1'b1;
            end 
       end
   end
end

// SET OUT
reg [14:0] gpio_set_out_dbus_reg [3:0];

always @(posedge clk or negedge rstn)
if (~rstn) begin
   for (l = 0; l < 4; l = l + 1)
      gpio_set_out_dbus_reg[l] <= 0;
end else begin
   for (l = 0; l < 4; l = l + 1)
      gpio_set_out_dbus_reg[l] <= 0;
   for (k = 0; k < 15; k = k + 1) begin            // go through all 8 bit wide banks
      for (l = 0; l < 4; l = l + 1) begin         // go through all 4 cores addressing possibilities
         if (dbus_wr_en_c3[l] & dbus_rdwr_gpio_base_c3[l] & (dbus_wr_add_c3[l][15:0] == (k * GPIO_STEP) + GPIO_OUT_SET))
            gpio_set_out_dbus_reg[l][k] <= 1'b1;
       end
   end
end

reg [14:0] gpio_set_out_ca_reg [3:0];

always @(posedge clk or negedge rstn)
if (~rstn) begin
   for (l = 0; l < 4; l = l + 1)
      gpio_set_out_ca_reg[l] <= 0;
end else begin
   for (l = 0; l < 4; l = l + 1)
      gpio_set_out_ca_reg[l] <= 0;
   for (k = 0; k < 15; k = k + 1) begin            // go through all 8 bit wide banks
      for (l = 0; l < 4; l = l + 1) begin         // go through all 4 cores addressing possibilities
         if (ca_match_reg[l] & (ca_command_reg[l][31:29] == 3'b001) & (ca_command_reg[l][11:8] == k))
            gpio_set_out_ca_reg[l][k] <= 1'b1;
       end
   end
end

reg [111:0] gpio_set_out_sig;

always @(gpio_set_out_dbus_reg or dbus_wr_data_c4 or gpio_set_out_ca_reg or ca_command_reg)
begin
   gpio_set_out_sig = 0;                               // overwrite bit-wise
   for (k = 0; k < 15; k = k + 1) begin            // go through all 8 bit wide banks
      for (l = 0; l < 4; l = l + 1) begin         // go through all 4 cores addressing possibilities
         if (gpio_set_out_dbus_reg[l][k])
            for (m = 0; m < 8; m = m + 1) begin   // go through all bits, bus wide didn't work 
               if (dbus_wr_data_c4[l][m])
                  gpio_set_out_sig[k * 8 + m] = 1'b1;
            end 
         if (gpio_set_out_ca_reg[l][k])
            for (m = 0; m < 8; m = m + 1) begin   // go through all bits, bus wide didn't work 
               if (ca_command_reg[l][m])
                  gpio_set_out_sig[k * 8 + m] = 1'b1;
            end 
       end
   end
end

// OUT
always @(posedge clk or negedge rstn)
if (~rstn) begin
   gpio_out_reg <= 0;
end else begin
   gpio_out_reg <= (gpio_out_reg | gpio_set_out_sig) & (~gpio_clr_out_sig);
end

// Filter IN
always @(posedge clk or negedge rstn)
if (~rstn) begin
   gpio_in_l1_reg <= 0;
   gpio_in_l2_reg <= 0;
   gpio_in_l3_reg <= 0;
   gpio_in_reg <= 0;
end else begin
   gpio_in_reg <= (gpio_in_l3_reg & gpio_in_l2_reg) |
                  (gpio_in_l3_reg & gpio_in_l1_reg) |
                  (gpio_in_l2_reg & gpio_in_l1_reg);
   gpio_in_l3_reg <= gpio_in_l2_reg;
   gpio_in_l2_reg <= gpio_in_l1_reg;
   gpio_in_l1_reg <= gpio_in;
end

// read
reg [14:0] gpio_rd_sel_in_reg [3:0];
reg [14:0] gpio_rd_sel_cap_reg [3:0];

always @(posedge clk or negedge rstn)
if (~rstn) begin
   for (l = 0; l < 4; l = l + 1) begin
      gpio_rd_sel_in_reg[l] <= 0;
      gpio_rd_sel_cap_reg[l] <= 0;
   end
end else begin
   for (l = 0; l < 4; l = l + 1) begin
      gpio_rd_sel_in_reg[l] <= 0;
      gpio_rd_sel_cap_reg[l] <= 0;
   end
   for (k = 0; k < 15; k = k + 1) begin            // go through all 8 bit wide banks
      for (l = 0; l < 4; l = l + 1) begin         // go through all 4 cores addressing possibilities
         if (dbus_rd_req_c3[l] & dbus_rdwr_gpio_base_c3[l] &
             (dbus_rdwr_add_c3[l][15:0] == ((k * GPIO_STEP) + GPIO_IN))) begin
            gpio_rd_sel_in_reg[l][k] <= 1'b1;
         end else if (dbus_rd_req_c3[l] & dbus_rdwr_gpio_base_c3[l] &
                      (dbus_rdwr_add_c3[l][15:0] == ((k * GPIO_STEP) + GPIO_CAP))) begin
             gpio_rd_sel_cap_reg[l][k] <= 1'b1;
         end
      end
   end
end

reg [7:0] gpio_rd_data_in [3:0][14:0];
reg [7:0] gpio_rd_data_cap [3:0][14:0];

always @(gpio_rd_sel_in_reg or gpio_in_reg or gpio_rd_sel_cap_reg or gpio_cap_reg)
begin
   for (k = 0; k < 15; k = k + 1) begin            // go through all 8 bit wide banks
      for (l = 0; l < 4; l = l + 1) begin         // go through all 4 cores addressing possibilities
         if (gpio_rd_sel_in_reg[l][k]) begin
            for (m = 0; m < 8; m = m + 1) begin   // go through all bits, bus wide didn't work 
               gpio_rd_data_in[l][k][m] = gpio_in_reg[k * 8 + m];
            end
         end else
            gpio_rd_data_in[l][k] = 0;
         if (gpio_rd_sel_cap_reg[l][k]) begin
            for (m = 0; m < 8; m = m + 1) begin   // go through all bits, bus wide didn't work 
               gpio_rd_data_cap[l][k][m] = gpio_cap_reg[k * 8 + m];
            end
         end else
            gpio_rd_data_cap[l][k] = 0;
      end
   end
end

reg [7:0] dbus_rd_data_tmp [3:0][4:0];

always @(posedge clk or negedge rstn)
if (~rstn) begin
   for (l = 0; l < 4; l = l + 1) begin
      for (k = 0; k < 5; k = k + 1) begin
         dbus_rd_data_tmp[l][k] <= 0;
      end
   end
end else begin
   for (l = 0; l < 4; l = l + 1) begin
      dbus_rd_data_tmp[l][0] <= gpio_rd_data_in[l][0] |
                                gpio_rd_data_cap[l][0] |
                                gpio_rd_data_in[l][1] |
                                gpio_rd_data_cap[l][1] |
                                gpio_rd_data_in[l][2] |
                                gpio_rd_data_cap[l][2];
      dbus_rd_data_tmp[l][1] <= gpio_rd_data_in[l][3] |
                                gpio_rd_data_cap[l][3] |
                                gpio_rd_data_in[l][4] |
                                gpio_rd_data_cap[l][4] |
                                gpio_rd_data_in[l][5] |
                                gpio_rd_data_cap[l][5];
      dbus_rd_data_tmp[l][2] <= gpio_rd_data_in[l][6] |
                                gpio_rd_data_cap[l][6] |
                                gpio_rd_data_in[l][7] |
                                gpio_rd_data_cap[l][7] |
                                gpio_rd_data_in[l][8] |
                                gpio_rd_data_cap[l][8];
      dbus_rd_data_tmp[l][3] <= gpio_rd_data_in[l][9] |
                                gpio_rd_data_cap[l][9] |
                                gpio_rd_data_in[l][10] |
                                gpio_rd_data_cap[l][10] |
                                gpio_rd_data_in[l][11] |
                                gpio_rd_data_cap[l][11];
      dbus_rd_data_tmp[l][4] <= gpio_rd_data_in[l][12] |
                                gpio_rd_data_cap[l][12] |
                                gpio_rd_data_in[l][13] |
                                gpio_rd_data_cap[l][13] |
                                gpio_rd_data_in[l][14] |
                                gpio_rd_data_cap[l][14];
   end
end

always @(dbus_rd_data_tmp)
begin
   for (l = 0; l < 4; l = l + 1) begin         // go through all 4 cores addressing possibilities
      dbus_rd_data[l][7:0] = dbus_rd_data_tmp[l][0] |
                             dbus_rd_data_tmp[l][1] |
                             dbus_rd_data_tmp[l][2] |
                             dbus_rd_data_tmp[l][3] |
                             dbus_rd_data_tmp[l][4];
   end
end

wire [14:0] debug_gpio_pc = {gpio_command[2][13:0], 1'b0};
wire [6:0] debug_gpio_a0 = gpio_command[2][20:14];

// LEVEL 
reg [3:0] gpio_event;

always @(gpio_prop_p4_fsm or gpio_event_ack) begin
for (l = 0; l < 4; l = l + 1) begin             // go through all 4 core 
   gpio_event[l] = !(gpio_prop_p4_fsm[l] == 0) & !gpio_event_ack;
end
end

reg [3:0] gpio_command_reg;

always @(posedge clk or negedge rstn)
if (~rstn) begin
   gpio_command_reg <= 0;
end else begin
   gpio_command_reg <= 0;
   for (l = 0; l < 4; l = l + 1) begin         // go through all 4 cores addressing possibilities
      if (dbus_wr_en_c3[l] & dbus_rdwr_gpio_base_c3[l] & (dbus_wr_add_c3[l][15:0] == GPIO_EVENT_ADD))
         gpio_command_reg[l] <= 1'b1;
   end
end

reg [1:0] gpio_prop_p4_fsm [3:0];
reg [7:0] gpio_prop_p4_ack;

always @(posedge clk or negedge rstn)
if (~rstn) begin
   gpio_prop_p4_ack <= 0;
   for (l = 0; l < 4; l = l + 1) begin
      gpio_prop_p4_fsm[l] <= 0;  
   end
   for (l = 0; l < 4; l = l + 1) begin
      gpio_command[l] <= 0; 
   end
end else begin   
   gpio_command[0][30] <= 1'b1;
   gpio_command[1][30] <= 1'b1;
   gpio_command[2][30] <= 1'b1;
   gpio_command[3][30] <= 1'b1;
   for (l = 0; l < 4; l = l + 1)                   // go through all 4 cores addressing possibilities
      if (gpio_command_reg[l])
         gpio_command[l][13:0] <= dbus_wr_data_c4[l][13:0];
   /////////////////////////////////////
   // event propagation, pipe 4
   /////////////////////////////////////
   gpio_prop_p4_ack <= 0;  
   for (l = 0; l < 4; l = l + 1) begin             // go through all 4 core 
      if (gpio_prop_p4_fsm[l] == 0) begin          // empty pipe element
         if (gpio_prop_p3_fsm[l * 2 + 0]) begin    // go through nibble group
            gpio_prop_p4_fsm[l] <= 0 + 1;          // state = bit count + 1
            gpio_command[l][19:14] <= {gpio_prop_p3_fsm[l * 2 + 0][1:0] - 1, gpio_prop_p3_tag[l * 2 + 0]};  
            gpio_command[l][20] <= 1'b0;  
            gpio_prop_p4_ack[l * 2 + 0] <= 1'b1;
         end else if (gpio_prop_p3_fsm[l * 2 + 1]) begin
            gpio_prop_p4_fsm[l] <= 1 + 1;
            gpio_command[l][19:14] <= {gpio_prop_p3_fsm[l * 2 + 1][1:0] - 1, gpio_prop_p3_tag[l * 2 + 1]};  
            gpio_command[l][20] <= 1'b1;
            gpio_prop_p4_ack[l * 2 + 1] <= 1'b1;
         end
      end else if (gpio_event_ack[l]) begin 
         case (gpio_prop_p4_fsm[l])                 // synopsys full_case
         1: if (gpio_prop_p3_fsm[l * 2 + 1]) begin
               gpio_prop_p4_fsm[l] <= 1 + 1;
               gpio_command[l][19:14] <= {gpio_prop_p3_fsm[l * 2 + 1][1:0] - 1, gpio_prop_p3_tag[l * 2 + 1]};  
               gpio_command[l][20] <= 1'b1;
               gpio_prop_p4_ack[l * 2 + 1] <= 1'b1;
            end else
               gpio_prop_p4_fsm[l] <= 0;
         2: if (gpio_prop_p3_fsm[l * 2 + 0]) begin
               gpio_prop_p4_fsm[l] <= 0 + 1;
               gpio_command[l][19:14] <= {gpio_prop_p3_fsm[l * 2 + 0][1:0] - 1, gpio_prop_p3_tag[l * 2 + 0]};  
               gpio_command[l][20] <= 1'b0;
               gpio_prop_p4_ack[l * 2 + 0] <= 1'b1;
            end else
               gpio_prop_p4_fsm[l] <= 0;
         default : ;
         endcase
      end
   end
end

reg [2:0] gpio_prop_p3_fsm [7:0];
reg [3:0] gpio_prop_p3_tag [7:0];
reg [31:0] gpio_prop_p3_ack;

always @(posedge clk or negedge rstn)
if (~rstn) begin
   gpio_prop_p3_ack <= 0;  
   for (l = 0; l < 8; l = l + 1) begin
      gpio_prop_p3_fsm[l] <= 0;
      gpio_prop_p3_tag[l] <= 0;
   end
end else begin
   /////////////////////////////////////
   // event propagation, pipe 3
   /////////////////////////////////////
   gpio_prop_p3_ack <= 0;
   for (l = 0; l < 4; l = l + 1) begin             // go through all 4 core 
      for (k = 0; k < 2; k = k + 1) begin          // go through all nibble groups
         if (gpio_prop_p3_fsm[l * 2 + k] == 0) begin // empty pipe element
            if (gpio_prop_p2_fsm[l * 8 + k * 4 + 0]) begin // go through nibble group
                gpio_prop_p3_fsm[l * 2 + k] <= 0 + 1; // state = bit count + 1
                gpio_prop_p3_tag[l * 2 + k] <= {gpio_prop_p2_fsm[l * 8 + k * 4 + 0] - 1, gpio_prop_p2_tag[l * 8 + k * 4 + 0]};  
                gpio_prop_p3_ack[l * 8 + k * 4 + 0] <= 1'b1;
            end else if (gpio_prop_p2_fsm[l * 8 + k * 4 + 1]) begin
                gpio_prop_p3_fsm[l * 2 + k] <= 1 + 1;
                gpio_prop_p3_tag[l * 2 + k] <= {gpio_prop_p2_fsm[l * 8 + k * 4 + 1] - 1, gpio_prop_p2_tag[l * 8 + k * 4 + 1]};  
                gpio_prop_p3_ack[l * 8 + k * 4 + 1] <= 1'b1;
            end else if (gpio_prop_p2_fsm[l * 8 + k * 4 + 2]) begin
                gpio_prop_p3_fsm[l * 2 + k] <= 2 + 1;
                gpio_prop_p3_tag[l * 2 + k] <= {gpio_prop_p2_fsm[l * 8 + k * 4 + 2] - 1, gpio_prop_p2_tag[l * 8 + k * 4 + 2]};  
                gpio_prop_p3_ack[l * 8 + k * 4 + 2] <= 1'b1;
            end else if (gpio_prop_p2_fsm[l * 8 + k * 4 + 3]) begin
                gpio_prop_p3_fsm[l * 2 + k] <= 3 + 1;
                gpio_prop_p3_tag[l * 2 + k] <= {gpio_prop_p2_fsm[l * 8 + k * 4 + 3] - 1, gpio_prop_p2_tag[l * 8 + k * 4 + 3]};  
                gpio_prop_p3_ack[l * 8 + k * 4 + 3] <= 1'b1;
            end
         end else if (gpio_prop_p4_ack[l * 2 + k]) begin 
         case (gpio_prop_p3_fsm[l * 2 + k])       // synopsys full_case
            1: if (gpio_prop_p2_fsm[l * 8 + k * 4 + 1]) begin
                  gpio_prop_p3_fsm[l * 2 + k] <= 1 + 1;
                  gpio_prop_p3_tag[l * 2 + k] <= {gpio_prop_p2_fsm[l * 8 + k * 4 + 1] - 1, gpio_prop_p2_tag[l * 8 + k * 4 + 1]};  
                  gpio_prop_p3_ack[l * 8 + k * 4 + 1] <= 1'b1;
               end else if (gpio_prop_p2_fsm[l * 8 + k * 4 + 2]) begin
                  gpio_prop_p3_fsm[l * 2 + k] <= 2 + 1;
                  gpio_prop_p3_tag[l * 2 + k] <= {gpio_prop_p2_fsm[l * 8 + k * 4 + 2] - 1, gpio_prop_p2_tag[l * 8 + k * 4 + 2]};  
                  gpio_prop_p3_ack[l * 8 + k * 4 + 2] <= 1'b1;
               end else if (gpio_prop_p2_fsm[l * 8 + k * 4 + 3]) begin
                  gpio_prop_p3_fsm[l * 2 + k] <= 3 + 1;
                  gpio_prop_p3_tag[l * 2 + k] <= {gpio_prop_p2_fsm[l * 8 + k * 4 + 3] - 1, gpio_prop_p2_tag[l * 8 + k * 4 + 3]};  
                  gpio_prop_p3_ack[l * 8 + k * 4 + 3] <= 1'b1;
               end else
                  gpio_prop_p3_fsm[l * 2 + k] <= 0;
            2: if (gpio_prop_p2_fsm[l * 8 + k * 4 + 2]) begin
                  gpio_prop_p3_fsm[l * 2 + k] <= 2 + 1;
                  gpio_prop_p3_tag[l * 2 + k] <= {gpio_prop_p2_fsm[l * 8 + k * 4 + 2] - 1, gpio_prop_p2_tag[l * 8 + k * 4 + 2]};  
                  gpio_prop_p3_ack[l * 8 + k * 4 + 2] <= 1'b1;
               end else if (gpio_prop_p2_fsm[l * 8 + k * 4 + 3]) begin
                  gpio_prop_p3_fsm[l * 2 + k] <= 3 + 1;
                  gpio_prop_p3_tag[l * 2 + k] <= {gpio_prop_p2_fsm[l * 8 + k * 4 + 3] - 1, gpio_prop_p2_tag[l * 8 + k * 4 + 3]};  
                  gpio_prop_p3_ack[l * 8 + k * 4 + 3] <= 1'b1;
               end else if (gpio_prop_p2_fsm[l * 8 + k * 4 + 0]) begin
                  gpio_prop_p3_fsm[l * 2 + k] <= 0 + 1;
                  gpio_prop_p3_tag[l * 2 + k] <= {gpio_prop_p2_fsm[l * 8 + k * 4 + 0] - 1, gpio_prop_p2_tag[l * 8 + k * 4 + 0]};  
                  gpio_prop_p3_ack[l * 8 + k * 4 + 0] <= 1'b1;
               end else
                  gpio_prop_p3_fsm[l * 2 + k] <= 0;
            3: if (gpio_prop_p2_fsm[l * 8 + k * 4 + 3]) begin
                  gpio_prop_p3_fsm[l * 2 + k] <= 3 + 1;
                  gpio_prop_p3_tag[l * 2 + k] <= {gpio_prop_p2_fsm[l * 8 + k * 4 + 3] - 1, gpio_prop_p2_tag[l * 8 + k * 4 + 3]};  
                  gpio_prop_p3_ack[l * 8 + k * 4 + 3] <= 1'b1;
               end else if (gpio_prop_p2_fsm[l * 8 + k * 4 + 0]) begin
                  gpio_prop_p3_fsm[l * 2 + k] <= 0 + 1;
                  gpio_prop_p3_tag[l * 2 + k] <= {gpio_prop_p2_fsm[l * 8 + k * 4 + 0] - 1, gpio_prop_p2_tag[l * 8 + k * 4 + 0]};  
                  gpio_prop_p3_ack[l * 8 + k * 4 + 0] <= 1'b1;
               end else if (gpio_prop_p2_fsm[l * 8 + k * 4 + 1]) begin
                  gpio_prop_p3_fsm[l * 2 + k] <= 1 + 1;
                  gpio_prop_p3_tag[l * 2 + k] <= {gpio_prop_p2_fsm[l * 8 + k * 4 + 1] - 1, gpio_prop_p2_tag[l * 8 + k * 4 + 1]};  
                  gpio_prop_p3_ack[l * 8 + k * 4 + 1] <= 1'b1;
               end else
                  gpio_prop_p3_fsm[l * 2 + k] <= 0;
            4: if (gpio_prop_p2_fsm[l * 8 + k * 4 + 0]) begin
                  gpio_prop_p3_fsm[l * 2 + k] <= 0 + 1;
                  gpio_prop_p3_tag[l * 2 + k] <= {gpio_prop_p2_fsm[l * 8 + k * 4 + 0] - 1, gpio_prop_p2_tag[l * 8 + k * 4 + 0]};  
                  gpio_prop_p3_ack[l * 8 + k * 4 + 0] <= 1'b1;
               end else if (gpio_prop_p2_fsm[l * 8 + k * 4 + 1]) begin
                  gpio_prop_p3_fsm[l * 2 + k] <= 1 + 1;
                  gpio_prop_p3_tag[l * 2 + k] <= {gpio_prop_p2_fsm[l * 8 + k * 4 + 1] - 1, gpio_prop_p2_tag[l * 8 + k * 4 + 1]};  
                  gpio_prop_p3_ack[l * 8 + k * 4 + 1] <= 1'b1;
               end else if (gpio_prop_p2_fsm[l * 8 + k * 4 + 2]) begin
                  gpio_prop_p3_fsm[l * 2 + k] <= 2 + 1;
                  gpio_prop_p3_tag[l * 2 + k] <= {gpio_prop_p2_fsm[l * 8 + k * 4 + 2] - 1, gpio_prop_p2_tag[l * 8 + k * 4 + 2]};  
                  gpio_prop_p3_ack[l * 8 + k * 4 + 2] <= 1'b1;
               end else
                  gpio_prop_p3_fsm[l * 2 + k] <= 0;
            default : ;
            endcase
         end
      end
   end
end

reg [2:0] gpio_prop_p2_fsm [31:0];
reg [1:0] gpio_prop_p2_tag [31:0];
reg [127:0] gpio_prop_p2_ack;

always @(posedge clk or negedge rstn)
if (~rstn) begin
   gpio_prop_p2_ack <= 0;  
   for (l = 0; l < 32; l = l + 1) begin
      gpio_prop_p2_fsm[l] <= 0;
      gpio_prop_p2_tag[l] <= 0;
   end
end else begin
   /////////////////////////////////////
   // event propagation, pipe 2
   /////////////////////////////////////
   gpio_prop_p2_ack <= 0;  
   for (l = 0; l < 4; l = l + 1) begin             // go through all 4 core 
      for (k = 0; k < 8; k = k + 1) begin          // go through all nibble groups
         if (gpio_prop_p2_fsm[l * 8 + k] == 0) begin // empty pipe element
            if (gpio_prop_p1_fsm[l * 32 + k * 4 + 0]) begin // go through nibble group
                gpio_prop_p2_fsm[l * 8 + k] <= 0 + 1; // state = bit count + 1
                gpio_prop_p2_tag[l * 8 + k] <= {gpio_prop_p1_fsm[l * 32 + k * 4 + 0] - 1, gpio_prop_p1_tag[l * 32 + k * 4 + 0][0]};  
                gpio_prop_p2_ack[l * 32 + k * 4 + 0] <= 1'b1;
            end else if (gpio_prop_p1_fsm[l * 32 + k * 4 + 1]) begin
                gpio_prop_p2_fsm[l * 8 + k] <= 1 + 1;
                //gpio_prop_p2_tag[l * 8 + k] <= 0;  
                gpio_prop_p2_tag[l * 8 + k] <= {gpio_prop_p1_fsm[l * 32 + k * 4 + 1] - 1, gpio_prop_p1_tag[l * 32 + k * 4 + 1][0]};  
                gpio_prop_p2_ack[l * 32 + k * 4 + 1] <= 1'b1;
            end else if (gpio_prop_p1_fsm[l * 32 + k * 4 + 2]) begin
                gpio_prop_p2_fsm[l * 8 + k] <= 2 + 1;
                gpio_prop_p2_tag[l * 8 + k] <= {gpio_prop_p1_fsm[l * 32 + k * 4 + 2] - 1, gpio_prop_p1_tag[l * 32 + k * 4 + 2][0]};  
                gpio_prop_p2_ack[l * 32 + k * 4 + 2] <= 1'b1;
            end else if (gpio_prop_p1_fsm[l * 32 + k * 4 + 3]) begin
                gpio_prop_p2_fsm[l * 8 + k] <= 3 + 1;
                gpio_prop_p2_tag[l * 8 + k] <= {gpio_prop_p1_fsm[l * 32 + k * 4 + 3] - 1, gpio_prop_p1_tag[l * 32 + k * 4 + 3][0]};  
                gpio_prop_p2_ack[l * 32 + k * 4 + 3] <= 1'b1;
            end
         end else if (gpio_prop_p3_ack[l * 8 + k]) begin 
            case (gpio_prop_p2_fsm[l * 8 + k])       // round robin
            1: if (gpio_prop_p1_fsm[l * 32 + k * 4 + 1]) begin
                  gpio_prop_p2_fsm[l * 8 + k] <= 1 + 1;
                  gpio_prop_p2_tag[l * 8 + k] <= {gpio_prop_p1_fsm[l * 32 + k * 4 + 1] - 1, gpio_prop_p1_tag[l * 32 + k * 4 + 1][0]};  
                  gpio_prop_p2_ack[l * 32 + k * 4 + 1] <= 1'b1;
               end else if (gpio_prop_p1_fsm[l * 32 + k * 4 + 2]) begin
                  gpio_prop_p2_fsm[l * 8 + k] <= 2 + 1;
                  gpio_prop_p2_tag[l * 8 + k] <= {gpio_prop_p1_fsm[l * 32 + k * 4 + 2] - 1, gpio_prop_p1_tag[l * 32 + k * 4 + 2][0]};  
                  gpio_prop_p2_ack[l * 32 + k * 4 + 2] <= 1'b1;
               end else if (gpio_prop_p1_fsm[l * 32 + k * 4 + 3]) begin
                  gpio_prop_p2_fsm[l * 8 + k] <= 3 + 1;
                  gpio_prop_p2_tag[l * 8 + k] <= {gpio_prop_p1_fsm[l * 32 + k * 4 + 3] - 1, gpio_prop_p1_tag[l * 32 + k * 4 + 3][0]};  
                  gpio_prop_p2_ack[l * 32 + k * 4 + 3] <= 1'b1;
               end else
                  gpio_prop_p2_fsm[l * 8 + k] <= 0;
            2: if (gpio_prop_p1_fsm[l * 32 + k * 4 + 2]) begin
                  gpio_prop_p2_fsm[l * 8 + k] <= 2 + 1;
                  gpio_prop_p2_tag[l * 8 + k] <= {gpio_prop_p1_fsm[l * 32 + k * 4 + 2] - 1, gpio_prop_p1_tag[l * 32 + k * 4 + 2][0]};  
                  gpio_prop_p2_ack[l * 32 + k * 4 + 2] <= 1'b1;
               end else if (gpio_prop_p1_fsm[l * 32 + k * 4 + 3]) begin
                  gpio_prop_p2_fsm[l * 8 + k] <= 3 + 1;
                  gpio_prop_p2_tag[l * 8 + k] <= {gpio_prop_p1_fsm[l * 32 + k * 4 + 3] - 1, gpio_prop_p1_tag[l * 32 + k * 4 + 3][0]};  
                  gpio_prop_p2_ack[l * 32 + k * 4 + 3] <= 1'b1;
               end else if (gpio_prop_p1_fsm[l * 32 + k * 4 + 0]) begin
                  gpio_prop_p2_fsm[l * 8 + k] <= 0 + 1;
                  gpio_prop_p2_tag[l * 8 + k] <= {gpio_prop_p1_fsm[l * 32 + k * 4 + 0] - 1, gpio_prop_p1_tag[l * 32 + k * 4 + 0][0]};  
                  gpio_prop_p2_ack[l * 32 + k * 4 + 0] <= 1'b1;
               end else
                  gpio_prop_p2_fsm[l * 8 + k] <= 0;
            3: if (gpio_prop_p1_fsm[l * 32 + k * 4 + 3]) begin
                  gpio_prop_p2_fsm[l * 8 + k] <= 3 + 1;
                  gpio_prop_p2_tag[l * 8 + k] <= {gpio_prop_p1_fsm[l * 32 + k * 4 + 3] - 1, gpio_prop_p1_tag[l * 32 + k * 4 + 3][0]};  
                  gpio_prop_p2_ack[l * 32 + k * 4 + 3] <= 1'b1;
               end else if (gpio_prop_p1_fsm[l * 32 + k * 4 + 0]) begin
                  gpio_prop_p2_fsm[l * 8 + k] <= 0 + 1;
                  gpio_prop_p2_tag[l * 8 + k] <= {gpio_prop_p1_fsm[l * 32 + k * 4 + 0] - 1, gpio_prop_p1_tag[l * 32 + k * 4 + 0][0]};  
                  gpio_prop_p2_ack[l * 32 + k * 4 + 0] <= 1'b1;
               end else if (gpio_prop_p1_fsm[l * 32 + k * 4 + 1]) begin
                  gpio_prop_p2_fsm[l * 8 + k] <= 1 + 1;
                  gpio_prop_p2_tag[l * 8 + k] <= {gpio_prop_p1_fsm[l * 32 + k * 4 + 1] - 1, gpio_prop_p1_tag[l * 32 + k * 4 + 1][0]};  
                  gpio_prop_p2_ack[l * 32 + k * 4 + 1] <= 1'b1;
               end else
                  gpio_prop_p2_fsm[l * 8 + k] <= 0;
            4: if (gpio_prop_p1_fsm[l * 32 + k * 4 + 0]) begin
                  gpio_prop_p2_fsm[l * 8 + k] <= 0 + 1;
                  gpio_prop_p2_tag[l * 8 + k] <= {gpio_prop_p1_fsm[l * 32 + k * 4 + 0] - 1, gpio_prop_p1_tag[l * 32 + k * 4 + 0][0]};  
                  gpio_prop_p2_ack[l * 32 + k * 4 + 0] <= 1'b1;
               end else if (gpio_prop_p1_fsm[l * 32 + k * 4 + 1]) begin
                  gpio_prop_p2_fsm[l * 8 + k] <= 1 + 1;
                  gpio_prop_p2_tag[l * 8 + k] <= {gpio_prop_p1_fsm[l * 32 + k * 4 + 1] - 1, gpio_prop_p1_tag[l * 32 + k * 4 + 1][0]};  
                  gpio_prop_p2_ack[l * 32 + k * 4 + 1] <= 1'b1;
               end else if (gpio_prop_p1_fsm[l * 32 + k * 4 + 2]) begin
                  gpio_prop_p2_fsm[l * 8 + k] <= 2 + 1;
                  gpio_prop_p2_tag[l * 8 + k] <= {gpio_prop_p1_fsm[l * 32 + k * 4 + 2] - 1, gpio_prop_p1_tag[l * 32 + k * 4 + 2][0]};  
                  gpio_prop_p2_ack[l * 32 + k * 4 + 2] <= 1'b1;
               end else
                  gpio_prop_p2_fsm[l * 8 + k] <= 0;
            default : ;
            endcase
         end
      end
   end
end


reg [1:0] gpio_prop_p1_fsm [127:0];
reg [1:0] gpio_prop_p1_tag [127:0];
reg [255:0] gpio_prop_p1_ack;

always @(posedge clk or negedge rstn)
if (~rstn) begin
   gpio_prop_p1_ack <= 0;  
   for (l = 0; l < 128; l = l + 1) begin
      gpio_prop_p1_fsm[l] <= 0;
      gpio_prop_p1_tag[l] <= 0;
   end
end else begin
   /////////////////////////////////////
   // event propagation, pipe 1
   /////////////////////////////////////
   gpio_prop_p1_ack <= 0;  
   for (l = 0; l < 4; l = l + 1) begin             // go through all 4 core 
      for (k = 0; k < GPIO_EVENT_BANKS; k = k + 1) begin          // go through all nibble groups
         for (n = 0; n < 2; n = n + 1) begin       // go through all 2 nibbles
            if (gpio_prop_p1_fsm[l * 32 + k * 2 + n] == 0) begin // empty pipe element
               if (gpio_prop_p0_fsm[l * 64 + k * 4 + n * 2 + 0]) begin // go through nibble group
                   gpio_prop_p1_fsm[l * 32 + k * 2 + n] <= 0 + 1; // state = bit count + 1
                   gpio_prop_p1_tag[l * 32 + k * 2 + n] <= gpio_prop_p0_fsm[l * 64 + k * 4 + n * 2 + 0] - 1;  
                   gpio_prop_p1_ack[l * 64 + k * 4 + n * 2 + 0] <= 1'b1;
               end else if (gpio_prop_p0_fsm[l * 64 + k * 4 + n * 2 + 1]) begin
                   gpio_prop_p1_fsm[l * 32 + k * 2 + n] <= 1 + 1;
                   gpio_prop_p1_tag[l * 32 + k * 2 + n] <= gpio_prop_p0_fsm[l * 64 + k * 4 + n * 2 + 1] - 1;  
                   gpio_prop_p1_ack[l * 64 + k * 4 + n * 2 + 1] <= 1'b1;
               end
            end else if (gpio_prop_p2_ack[l * 32 + k * 2 + n]) begin 
               case (gpio_prop_p1_fsm[l * 32 + k * 2 + n])       // synopsys full_case
               1: if (gpio_prop_p0_fsm[l * 64 + k * 4 + n * 2 + 1]) begin
                     gpio_prop_p1_fsm[l * 32 + k * 2 + n] <= 1 + 1;
                     gpio_prop_p1_tag[l * 32 + k * 2 + n] <= gpio_prop_p0_fsm[l * 64 + k * 4 + n * 2 + 1] - 1;  
                     gpio_prop_p1_ack[l * 64 + k * 4 + n * 2 + 1] <= 1'b1;
                  end else
                     gpio_prop_p1_fsm[l * 32 + k * 2 + n] <= 0;
               2: if (gpio_prop_p0_fsm[l * 64 + k * 4 + n * 2 + 0]) begin
                     gpio_prop_p1_fsm[l * 32 + k * 2 + n] <= 0 + 1;
                     gpio_prop_p1_tag[l * 32 + k * 2 + n] <= gpio_prop_p0_fsm[l * 64 + k * 4 + n * 2 + 0] - 1;  
                     gpio_prop_p1_ack[l * 64 + k * 4 + n * 2 + 0] <= 1'b1;
                  end else
                     gpio_prop_p1_fsm[l * 32 + k * 2 + n] <= 0;
               default : ;
               endcase
            end
         end
      end
   end
end

reg [14:0] gpio_sel_lvl0_reg [3:0];
reg [14:0] gpio_sel_lvl1_reg [3:0];

always @(posedge clk or negedge rstn)
if (~rstn) begin
   for (l = 0; l < 4; l = l + 1) begin
      gpio_sel_lvl0_reg[l] <= 0;
      gpio_sel_lvl1_reg[l] <= 0;
   end
end else begin
   for (l = 0; l < 4; l = l + 1) begin
      gpio_sel_lvl0_reg[l] <= 0;
      gpio_sel_lvl1_reg[l] <= 0;
   end
   for (k = 0; k < GPIO_EVENT_BANKS; k = k + 1) begin            // go through all 8 bit wide banks
      for (l = 0; l < 4; l = l + 1) begin         // go through all 4 cores addressing possibilities
         if ((dbus_wr_add_c2[l][15:0] == (k * GPIO_STEP) + GPIO_LVL0))
            gpio_sel_lvl0_reg[l][k] <= 1'b1;
         if ((dbus_wr_add_c2[l][15:0] == (k * GPIO_STEP) + GPIO_LVL1))
            gpio_sel_lvl1_reg[l][k] <= 1'b1;
      end
   end
end

reg [14:0] gpio_lvl0_reg [3:0];
reg [14:0] gpio_lvl1_reg [3:0];

always @(posedge clk or negedge rstn)
if (~rstn) begin
   for (l = 0; l < 4; l = l + 1) begin
      gpio_lvl0_reg[l] <= 0;
      gpio_lvl1_reg[l] <= 0;
   end
end else begin
   for (l = 0; l < 4; l = l + 1) begin
      gpio_lvl0_reg[l] <= 0;
      gpio_lvl1_reg[l] <= 0;
   end
   for (k = 0; k < GPIO_EVENT_BANKS; k = k + 1) begin            // go through all 8 bit wide banks
      for (l = 0; l < 4; l = l + 1) begin         // go through all 4 cores addressing possibilities
         if (dbus_wr_en_c3[l] & dbus_rdwr_gpio_base_c3[l] & gpio_sel_lvl0_reg[l][k])
            gpio_lvl0_reg[l][k] <= 1'b1;
         if (dbus_wr_en_c3[l] & dbus_rdwr_gpio_base_c3[l] & gpio_sel_lvl1_reg[l][k])
            gpio_lvl1_reg[l][k] <= 1'b1;
      end
   end
end

reg [1:0] gpio_prop_p0_fsm [255:0];

always @(posedge clk or negedge rstn)
if (~rstn) begin
   gpio_level_reg <= 0;
   gpio_armed_reg <= 0;
   for (l = 0; l < 128; l = l + 1) begin
       gpio_core_reg[l] <= 0;
   end
   gpio_event_reg <= 0;
   for (l = 0; l < 256; l = l + 1) begin
      gpio_prop_p0_fsm[l] <= 0;  
   end
   gpio_cap_reg <= 0;
end else begin
   /////////////////////////////////////
   // event propagation, pipe 0 
   /////////////////////////////////////
   for (l = 0; l < 4; l = l + 1) begin             // go through all 4 core sources
      for (k = 0; k < GPIO_EVENT_BANKS; k = k + 1) begin         // go through all banks
         for (n = 0; n < 4; n = n + 1) begin       // go through all 2 nibbles
            if (gpio_prop_p0_fsm[l * 64 + k * 4 + n] == 0) begin // empty pipe element
               if (gpio_event_reg[k * 8 + n * 2 + 0] & (gpio_core_reg[k * 8 + n * 2 + 0] == l)) begin
                  gpio_prop_p0_fsm[l * 64 + k * 4 + n] <= 1; // state = nibble count + 1
                  gpio_event_reg[k * 8 + n * 2 + 0] <= 1'b0;
               end else if (gpio_event_reg[k * 8 + n * 2 + 1] & (gpio_core_reg[k * 8 + n * 2 + 1] == l)) begin
                  gpio_prop_p0_fsm[l * 64 + k * 4 + n] <= 2; // state = nibble count + 1
                  gpio_event_reg[k * 8 + n * 2 + 1] <= 1'b0;
               end
            end else if (gpio_prop_p1_ack[l * 64 + k * 4 + n]) begin 
               case (gpio_prop_p0_fsm[l * 64 + k * 4 + n])       // round robin
               1: if ((gpio_event_reg[k * 8 + n * 2 + 1]) & (gpio_core_reg[k * 8 + n * 2 + 1] == l)) begin
                     gpio_prop_p0_fsm[l * 64 + k * 4 + n] <= 2;
                     gpio_event_reg[k * 8 + n * 2 + 1] <= 1'b0;
                  end else
                     gpio_prop_p0_fsm[l * 64 + k * 4 + n] <= 0;
               2: if ((gpio_event_reg[k * 8 + n * 2 + 0]) & (gpio_core_reg[k * 8 + n * 2 + 0] == l)) begin
                     gpio_prop_p0_fsm[l * 64 + k * 4 + n] <= 1;
                     gpio_event_reg[k * 8 + n * 2 + 0] <= 1'b0;
                  end else
                     gpio_prop_p0_fsm[l * 64 + k * 4 + n] <= 0;
               default : ;
               endcase
            end
         end
      end
   end
   /////////////////////////////////////
   // detect event, unarm
   /////////////////////////////////////
   for (k = 0; k < GPIO_EVENT_BANKS; k = k + 1) begin            // go through all 8 bit wide banks
      for (m = 0; m < 8; m = m + 1) begin          // go through all bits
         if (!gpio_event_reg[k * 8 + m])
            if (gpio_armed_reg[k * 8 + m])
               if (gpio_level_reg[k * 8 + m] == gpio_in_reg[k * 8 + m]) begin
                  gpio_event_reg[k * 8 + m] <= 1'b1;
                  gpio_armed_reg[k * 8 + m] <= 1'b0;
                  if (m < 7)
                     gpio_cap_reg[k * 8 + m + 1] <= gpio_in_reg[k * 8 + m + 1];
                  else
                     gpio_cap_reg[k * 8 + m - 7] <= gpio_in_reg[k * 8 + m - 7];
               end
      end
   end
   /////////////////////////////////////
   // write data bus:
   //      save level and core
   //      set armed flag
   /////////////////////////////////////
   for (k = 0; k < GPIO_EVENT_BANKS; k = k + 1) begin            // go through all 8 bit wide banks
      for (l = 0; l < 4; l = l + 1) begin         // go through all 4 cores addressing possibilities
         if (gpio_lvl0_reg[l][k])
            for (m = 0; m < 8; m = m + 1) begin   // go through all bits
               if (dbus_wr_data_c4[l][m]) begin
                  gpio_level_reg[k * 8 + m] <= 1'b0;
                  gpio_armed_reg[k * 8 + m] <= 1'b1;
                  gpio_core_reg[k * 8 + m] <= l;
               end
            end
         if (gpio_lvl1_reg[l][k])
            for (m = 0; m < 8; m = m + 1) begin   // go through all bits, bus wide didn't work 
               if (dbus_wr_data_c4[l][m]) begin
                  gpio_level_reg[k * 8 + m] <= 1'b1;
                  gpio_armed_reg[k * 8 + m] <= 1'b1;
                  gpio_core_reg[k * 8 + m] <= l;
               end
            end
      end
   end
   /////////////////////////////////////
   // remove unconnected and ARTY LED related banks
   /////////////////////////////////////
   for (k = GPIO_EVENT_BANKS; k < 16; k = k + 1) begin    // go through all 8 bit wide banks
      for (m = 0; m < 8; m = m + 1) begin   // go through all bits
         gpio_level_reg[k * 8 + m] <= 1'b0;
         gpio_armed_reg[k * 8 + m] <= 1'b0;
         gpio_core_reg[k * 8 + m] <= 0;
      end
   end
end

always @(posedge clk or negedge rstn)
if (~rstn) begin
   for (l = 0; l < 4; l = l + 1) begin
      dbus_wr_add_c2[l] <= 0;
      dbus_wr_data_c2[l] <= 0;
   end
end else begin
   for (l = 0; l < 4; l = l + 1) begin
      dbus_wr_add_c2[l] <= dbus_wr_add[l];
      dbus_wr_data_c2[l] <= dbus_wr_data[l];
   end
end

always @(posedge clk or negedge rstn)
if (~rstn) begin
   for (l = 0; l < 4; l = l + 1) begin
      dbus_rdwr_add_c3[l] <= 0;
      dbus_wr_add_c3[l] <= 0;
      dbus_wr_data_c3[l] <= 0;
   end
   dbus_rdwr_gpio_base_c3 <= 0;
   dbus_rd_req_c3 <= 0;
   dbus_wr_en_c3 <= 0;
end else begin
   for (l = 0; l < 4; l = l + 1) begin
      dbus_rdwr_add_c3[l] <= dbus_rdwr_add[l];
      dbus_wr_add_c3[l] <= dbus_wr_add_c2[l];
      dbus_wr_data_c3[l] <= dbus_wr_data_c2[l];
   end
   dbus_rdwr_gpio_base_c3 <= dbus_rdwr_gpio_base;
   dbus_rd_req_c3 <= dbus_rd_req;
   dbus_wr_en_c3 <= dbus_wr_en;
end

always @(posedge clk or negedge rstn)
if (~rstn) begin
   for (l = 0; l < 4; l = l + 1) begin
      dbus_wr_data_c4[l] <= 0;
   end
end else begin
   for (l = 0; l < 4; l = l + 1) begin
      dbus_wr_data_c4[l] <= dbus_wr_data_c3[l];
   end
end

endmodule
