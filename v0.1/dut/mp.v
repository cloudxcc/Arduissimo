//
// Copyright 2019 Tobias Strauch, Munich, Bavaria
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//

module mp (clk,
           rstn,
           dbus_rdwr_add_bus,
           dbus_rdwr_be_in,
           dbus_wr_en,
           dbus_wr_data_in,
           dbus_rd_req,
           dbus_rd_data_out,
           mp_access,
           mp_access_ack,
           mp_command_pc_out,
           mp_command_data_out,
           mp_stalled_by_mp);

`include "MPSOC_parameters.v"

input clk;
input rstn;
input [127:0] dbus_rdwr_add_bus;
input [7:0] dbus_rdwr_be_in;
input [3:0] dbus_wr_en;
input [127:0] dbus_wr_data_in;
input [3:0] dbus_rd_req;
output [127:0] dbus_rd_data_out;
output [3:0] mp_access;
input [3:0] mp_access_ack;
output [55:0] mp_command_pc_out;
output [127:0] mp_command_data_out;
output [3:0] mp_stalled_by_mp;

reg [3:0] dbus_rdwr_mp_base;
wire [31:0] dbus_rdwr_add [3:0];
reg [31:0] dbus_rdwr_add_c2 [3:0];
assign dbus_rdwr_add[0] = dbus_rdwr_add_bus[ 31: 0];
assign dbus_rdwr_add[1] = dbus_rdwr_add_bus[ 63:32];
assign dbus_rdwr_add[2] = dbus_rdwr_add_bus[ 95:64];
assign dbus_rdwr_add[3] = dbus_rdwr_add_bus[127:96];
reg [3:0] dbus_rd_req_c2;
wire [31:0] dbus_wr_data [3:0];
reg [31:0] dbus_wr_data_c2 [3:0];
reg [31:0] dbus_wr_data_c3 [3:0];
assign dbus_wr_data[0] = dbus_wr_data_in[ 31: 0];
assign dbus_wr_data[1] = dbus_wr_data_in[ 63:32];
assign dbus_wr_data[2] = dbus_wr_data_in[ 95:64];
assign dbus_wr_data[3] = dbus_wr_data_in[127:96];

reg [31:0] dbus_rd_data [3:0];
assign  dbus_rd_data_out = {dbus_rd_data[3], dbus_rd_data[2], dbus_rd_data[1], dbus_rd_data[0]};

reg [13:0] mp_command_pc [3:0];
assign  mp_command_pc_out = {mp_command_pc[3], mp_command_pc[2], mp_command_pc[1], mp_command_pc[0]};

reg [31:0] mp_command_data [3:0];
assign  mp_command_data_out = {mp_command_data[3], mp_command_data[2], mp_command_data[1], mp_command_data[0]};

reg [14:0] mp_command_reg [15:0];

integer k, l, m, n;

wire [15:0] rdwr_mp_base = MP_BASE;

reg [1:0] rdwr_mp_reg [3:0];
always @ (posedge clk or negedge rstn)
if (~rstn)
   for (l = 0; l < 4; l = l + 1)
      rdwr_mp_reg[l] <= 0;
else begin
   for (l = 0; l < 4; l = l + 1) begin
      rdwr_mp_reg[l][0] <= (dbus_rdwr_add_bus[l * 32 + 16] == rdwr_mp_base[ 0]) &
                           (dbus_rdwr_add_bus[l * 32 + 17] == rdwr_mp_base[ 1]) &
                           (dbus_rdwr_add_bus[l * 32 + 18] == rdwr_mp_base[ 2]) &
                           (dbus_rdwr_add_bus[l * 32 + 19] == rdwr_mp_base[ 3]) &
                           (dbus_rdwr_add_bus[l * 32 + 20] == rdwr_mp_base[ 4]) &
                           (dbus_rdwr_add_bus[l * 32 + 21] == rdwr_mp_base[ 5]) &
                           (dbus_rdwr_add_bus[l * 32 + 22] == rdwr_mp_base[ 6]) &
                           (dbus_rdwr_add_bus[l * 32 + 23] == rdwr_mp_base[ 7]);
      rdwr_mp_reg[l][1] <= (dbus_rdwr_add_bus[l * 32 + 24] == rdwr_mp_base[ 8]) &
                           (dbus_rdwr_add_bus[l * 32 + 25] == rdwr_mp_base[ 9]) &
                           (dbus_rdwr_add_bus[l * 32 + 26] == rdwr_mp_base[10]) &
                           (dbus_rdwr_add_bus[l * 32 + 27] == rdwr_mp_base[11]) &
                           (dbus_rdwr_add_bus[l * 32 + 28] == rdwr_mp_base[12]) &
                           (dbus_rdwr_add_bus[l * 32 + 29] == rdwr_mp_base[13]) &
                           (dbus_rdwr_add_bus[l * 32 + 30] == rdwr_mp_base[14]) &
                           (dbus_rdwr_add_bus[l * 32 + 31] == rdwr_mp_base[15]);
   end
end

always @(dbus_rdwr_mp_base or rdwr_mp_reg)
   for (l = 0; l < 4; l = l + 1)
      dbus_rdwr_mp_base[l] = rdwr_mp_reg[l] == 2'h3;

// Command
reg [15:0] mp_command_sel;
always @(posedge clk or negedge rstn)
if (~rstn)
   for (l = 0; l < 16; l = l + 1)
      mp_command_sel[l] <= 0;
else begin
   for (l = 0; l < 16; l = l + 1)
      mp_command_sel[l] <= 0;
   for (l = 0; l < 4; l = l + 1)               // go through all 4 cores addressing possibilities
      for (k = 0; k < 4; k = k + 1)            // go through all 4 directions
         if (dbus_wr_en[l] & dbus_rdwr_mp_base[l] & (dbus_rdwr_add_c2[l][15:0] == MP_COM_OFFSET + (k * MP_STEP)))
            mp_command_sel[(k * 4) + l] <= 1'b1;
end

always @(posedge clk or negedge rstn)
if (~rstn)
   for (l = 0; l < 16; l = l + 1)
      mp_command_reg[l] <= 0;
else
   for (l = 0; l < 4; l = l + 1)               // go through all 4 cores addressing possibilities
      for (k = 0; k < 4; k = k + 1)            // go through all 4 directions
         if (l != k)
            if (mp_command_sel[(k * 4) + l])
               mp_command_reg[(k * 4) + l][14:0] <= dbus_wr_data_c3[l][14:0];

reg [15:0] mp_wr_out;

always @(dbus_wr_en or dbus_rdwr_mp_base or dbus_rdwr_add_c2) begin
   mp_wr_out = 0;
   for (l = 0; l < 4; l = l + 1)               // go through all 4 cores
      for (k = 0; k < 4; k = k + 1)            // go through all 4 directions
          if (l != k)
             if (dbus_wr_en[l] & dbus_rdwr_mp_base[l] & (dbus_rdwr_add_c2[l][15:0] == MP_OUT_OFFSET + (k * MP_STEP)))
                mp_wr_out[(l * 4) + k] = 1'b1;
end

reg [15:0] mp_rd_in;

always @(dbus_rd_req_c2 or dbus_rdwr_mp_base or dbus_rdwr_add_c2) begin
   mp_rd_in = 0;
   for (l = 0; l < 4; l = l + 1)               // go through all 4 cores
      for (k = 0; k < 4; k = k + 1)            // go through all 4 directions
          if (l != k)
             if (dbus_rd_req_c2[k] & dbus_rdwr_mp_base[k] & (dbus_rdwr_add_c2[k][15:0] == MP_IN_OFFSET + (l * MP_STEP)))
                mp_rd_in[(l * 4) + k] = 1'b1;
end
            
reg [3:0] mp_stalled_by_mp;

always @(posedge clk or negedge rstn)
if (~rstn) begin
   mp_stalled_by_mp <= 0;
end else begin
   mp_stalled_by_mp <= 0;
   for (l = 0; l < 4; l = l + 1)               // go through all 4 cores
      for (k = 0; k < 4; k = k + 1) begin      // go through all 4 directions
          if (mp_wr_out[(l * 4) + k] & !mp_rd_in[(l * 4) + k] & !(mp_fsm[(l * 4) + k]) == 0)
             mp_stalled_by_mp[l] <= 1'b1;
          if (mp_rd_in[(l * 4) + k] & (mp_fsm[(l * 4) + k]) == 0)
             mp_stalled_by_mp[k] <= 1'b1;
      end
end

assign mp_access = mp_access_int & ~mp_access_ack;

// Round robin (not fully tested)
reg [1:0] mp_access_next [3:0];
reg [1:0] mp_access_last [3:0]; 

always @(mp_access_last or mp_command_reg) begin
   for (l = 0; l < 4; l = l + 1)              // go through all 4 cores
      case (mp_access_last[l])
      0: if (mp_command_reg[(l * 4) + 1][14])
            mp_access_next[l] = 1;
         else if (mp_command_reg[(l * 4) + 2][14])
            mp_access_next[l] = 2;
         else if (mp_command_reg[(l * 4) + 3][14])
            mp_access_next[l] = 3;
         else
            mp_access_next[l] = 0;
      1: if (mp_command_reg[(l * 4) + 2][14])
            mp_access_next[l] = 2;
         else if (mp_command_reg[(l * 4) + 3][14])
            mp_access_next[l] = 3;
         else if (mp_command_reg[(l * 4) + 0][14])
            mp_access_next[l] = 0;
         else
            mp_access_next[l] = 1;
      2: if (mp_command_reg[(l * 4) + 3][14])
            mp_access_next[l] = 3;
         else if (mp_command_reg[(l * 4) + 0][14])
            mp_access_next[l] = 0;
         else if (mp_command_reg[(l * 4) + 1][14])
            mp_access_next[l] = 1;
         else
            mp_access_next[l] = 2;
      3: if (mp_command_reg[(l * 4) + 0][14])
            mp_access_next[l] = 0;
         else if (mp_command_reg[(l * 4) + 1][14])
            mp_access_next[l] = 1;
         else if (mp_command_reg[(l * 4) + 2][14])
            mp_access_next[l] = 2;
         else
            mp_access_next[l] = 3;
      default : ;
      endcase
end

// FSM
reg [3:0] mp_message_sel [3:0];
always @(posedge clk or negedge rstn)
if (~rstn) begin
   for (l = 0; l < 4; l = l + 1)               // go through all 4 cores
      for (k = 0; k < 4; k = k + 1)            // go through all 4 directions
         mp_message_sel[l][k] <= 0;
end else begin
   for (l = 0; l < 4; l = l + 1)               // go through all 4 cores
      for (k = 0; k < 4; k = k + 1)            // go through all 4 directions
         mp_message_sel[l][k] <= 0;
   for (l = 0; l < 4; l = l + 1)               // go through all 4 cores
      for (k = 0; k < 4; k = k + 1)            // go through all 4 directions
         if (dbus_wr_en[l] & dbus_rdwr_mp_base[l] & (dbus_rdwr_add_c2[l][15:0] == MP_OUT_OFFSET + (k * MP_STEP)))
           mp_message_sel[l][k] <= 1'b1;
end

reg [1:0] mp_fsm [15:0];
reg [31:0] mp_message [15:0];
reg [3:0] mp_access_int;

always @(posedge clk or negedge rstn)
if (~rstn) begin
   for (l = 0; l < 16; l = l + 1) begin
      mp_fsm[l] <= 0;
      mp_message[l] <= 0;
      mp_command_pc[l] <= 0;
      mp_command_data[l] <= 0;
   end
   for (l = 0; l < 4; l = l + 1)
      mp_access_last[l] <= 0;
   mp_access_int <= 0;
end else begin
   mp_access_int <= 0;
   for (l = 0; l < 4; l = l + 1) begin         // go through all 4 cores
      for (k = 0; k < 4; k = k + 1) begin      // go through all 4 directions
            case (mp_fsm[(l * 4) + k]) 
            0: if (mp_message_sel[l][k]) begin
                  mp_message[(l * 4) + k] <= dbus_wr_data_c3[l];
                  mp_fsm[(l * 4) + k] <= 1;
               end
            1: if (mp_command_reg[(l * 4) + k][14] & (mp_access_next[l] == k)) begin
                  mp_command_pc[k] <= mp_command_reg[(l * 4) + k][13:0];
                  mp_command_data[k] <= mp_message[(l * 4) + k];
                  mp_access_last[l] <= k;
                  if (mp_access_ack[k])
                     mp_fsm[(l * 4) + k] <= 0;
                  else
                     mp_access_int[k] <= 1'b1;
               end else                        // read directly
               if (mp_rd_in[(l * 4) + k]) begin
                  mp_fsm[(l * 4) + k] <= 0;
               end
            default : ;
            endcase
      end  
      // remove core message passing to itself
      mp_fsm[l + (l * 4)] <= 0;
      mp_message[l + (l * 4)] <= 0;
   end
end 

// read
reg [3:0] mp_rd_sel [3:0];

always @(dbus_rd_req_c2 or dbus_rdwr_add_c2) begin
   for (l = 0; l < 4; l = l + 1) begin
      for (k = 0; k < 4; k = k + 1) begin      // go through all 4 directions
          mp_rd_sel[l][k] = 0;
      end
   end
   for (l = 0; l < 4; l = l + 1) begin         // go through all 4 cores
      for (k = 0; k < 4; k = k + 1) begin      // go through all 4 directions
         if (k != l)
         if (dbus_rd_req_c2[k] & (dbus_rdwr_add_c2[k][15:0] == MP_IN_OFFSET + (l * MP_STEP))) begin
            mp_rd_sel[l][k] = 1'b1;
         end
      end
   end
end

reg [31:0] mp_rd_data [3:0][3:0];
always @(mp_rd_sel or dbus_rdwr_mp_base or mp_message)
begin
   for (l = 0; l < 4; l = l + 1) begin         // go through all 4 cores
      for (k = 0; k < 4; k = k + 1) begin      // go through all 4 directions
         if (mp_rd_sel[l][k] & dbus_rdwr_mp_base[k])
            mp_rd_data[k][l] = mp_message[(l * 4) + k];
         else
            mp_rd_data[k][l] = 0;
      end
   end
end

always @(mp_rd_data)
begin
   for (l = 0; l < 4; l = l + 1) begin         // go through all 4 cores
      dbus_rd_data[l] = mp_rd_data[l][0] |
                        mp_rd_data[l][1] |
                        mp_rd_data[l][2] |
                        mp_rd_data[l][3];
   end
end

always @(posedge clk or negedge rstn)
if (~rstn) begin
   for (l = 0; l < 4; l = l + 1) begin
      dbus_rdwr_add_c2[l] <= 0;
      dbus_wr_data_c2[l] <= 0;
   end
   dbus_rd_req_c2 <= 0;
end else begin
   for (l = 0; l < 4; l = l + 1) begin
      dbus_rdwr_add_c2[l] <= dbus_rdwr_add[l];
      dbus_wr_data_c2[l] <= dbus_wr_data[l];
   end
   dbus_rd_req_c2 <= dbus_rd_req;
end

always @(posedge clk or negedge rstn)
if (~rstn) begin
   for (l = 0; l < 4; l = l + 1)
      dbus_wr_data_c3[l] <= 0;
end else begin
   for (l = 0; l < 4; l = l + 1)
      dbus_wr_data_c3[l] <= dbus_wr_data_c2[l];
end

endmodule
