//
// Copyright 2019 Tobias Strauch, Munich, Bavaria
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//

// Simple calendar (ca) implementation.
// Goal is to have a working calendar first.
// Timing optimizations second.
// C-slow-retiming to enable multithreaded access not implemented yet.
// Calendar time bit width = 23;

module ca (clk,
           rstn,
           test_overflow,
           dbus_wr_add,
           dbus_wr_en_int,
           dbus_wr_data,
           tid_wr0,
           thread_pipe_valid,
           ca_time,
           ca_stalled_by_tc,
           ca_match,
           ca_match_block,
           ca_command,
           ca_stalled_by_ca);

`include "MPSOC_parameters.v"

input clk;
input rstn;
input test_overflow;
input [31:0] dbus_wr_add;
input dbus_wr_en_int;
input [31:0] dbus_wr_data;
input [3:0] tid_wr0;
input [5:0] thread_pipe_valid;
output [22:0] ca_time;
input ca_stalled_by_tc;
output ca_match;
input ca_match_block;
output [31:0] ca_command;
output ca_stalled_by_ca;

wire [32:0] ca_wr_douta;
reg ca_wr_wea;
reg [8:0] ca_wr_add;
reg [8:0] ca_wr_add_start;
reg ca_wr_add_start_marker;
reg [8:0] ca_wr_add_fill;
reg [8:0] ca_wr_add_fill_next;
reg ca_end_of_wr_list;
reg [8:0] ca_wr_add_ptr;
reg [32:0] ca_wr_dina;
wire [32:0] ca_rd_doutb;
reg [22:0] ca_time;
reg [5:0] ca_wr_fsm_state;
reg ca_ready;
reg [22:0] ca_insert_time;
reg [8:0] ca_rd_add;
reg [3:0] ca_rd_fsm_state;
reg [2:0] ca_rd_delay;
reg ca_update_rd_add;
reg [31:0] ca_command;
reg ca_wr_sync_update;
reg ca_wr_sync_update_done;
reg ca_rd_doutb_31_23;


always @ (posedge clk or negedge rstn)
if (~rstn)
   ca_time <= 0;
else
   ca_time <= ca_time + 1;

wire wr_add_ca_com = dbus_wr_add == CA_COM;
wire wr_add_ca_et = dbus_wr_add == CA_ET;
wire ca_wr_com = dbus_wr_en_int & (dbus_wr_add_c2 == CA_COM);
wire ca_wr_et  = dbus_wr_en_int & (dbus_wr_add_c2 == CA_ET);
assign ca_stalled_by_ca = ((ca_wr_com | ca_wr_et) & (!ca_ready | ca_wr_sync_update | ca_wr_et_c3));

reg ca_insert_lesser_low;
reg ca_insert_lesser_high;
reg ca_insert_equal_high;
reg ca_compare_check;

always @ (posedge clk or negedge rstn)
if (~rstn) begin
   ca_insert_lesser_low <= 0;
   ca_insert_lesser_high <= 0;
   ca_insert_equal_high <= 0;
   ca_compare_check <= 0;
end else begin
   if (test_overflow) begin
      ca_insert_lesser_low <= ca_insert_time[5:0] < ca_wr_douta[5:0];
      ca_insert_lesser_high <= ca_insert_time[11:6] < ca_wr_douta[11:6];
      ca_insert_equal_high <= ca_insert_time[11:6] == ca_wr_douta[11:6];
      ca_compare_check <= ca_insert_time[11:10] == ca_wr_douta[11:10];
      if (((ca_wr_douta[11:10] == 2'h0) & (ca_insert_time[11:10] == 2'h3)) |
          ((ca_wr_douta[11:10] == 2'h1) & (ca_insert_time[11:10] == 2'h0)) |
          ((ca_wr_douta[11:10] == 2'h2) & (ca_insert_time[11:10] == 2'h1)) |
          ((ca_wr_douta[11:10] == 2'h3) & (ca_insert_time[11:10] == 2'h2)) ) begin
         ca_insert_lesser_high <= 1'b1;
         ca_compare_check <= 1'b1;
      end
   end else begin
      ca_insert_lesser_low <= ca_insert_time[10:0] < ca_wr_douta[10:0];
      ca_insert_lesser_high <= ca_insert_time[22:11] < ca_wr_douta[22:11];
      ca_insert_equal_high <= ca_insert_time[22:11] == ca_wr_douta[22:11];
      ca_compare_check <= ca_insert_time[22:21] == ca_wr_douta[22:21];
      if (((ca_wr_douta[22:21] == 2'h0) & (ca_insert_time[22:21] == 2'h3)) |
          ((ca_wr_douta[22:21] == 2'h1) & (ca_insert_time[22:21] == 2'h0)) |
          ((ca_wr_douta[22:21] == 2'h2) & (ca_insert_time[22:21] == 2'h1)) |
          ((ca_wr_douta[22:21] == 2'h3) & (ca_insert_time[22:21] == 2'h2)) ) begin
         ca_insert_lesser_high <= 1'b1;
         ca_compare_check <= 1'b1;
      end
   end
end

wire ca_insert = ca_compare_check ?
                    (ca_insert_equal_high ? ca_insert_lesser_low : ca_insert_lesser_high) :
                    1'b0;

always @ (ca_wr_add_fill)
   if ((test_overflow & (ca_wr_add_fill == 9'h02e)) |
       (!test_overflow & (ca_wr_add_fill == 510))) begin
      ca_wr_add_fill_next = 9'h010;
   end else begin
      ca_wr_add_fill_next = ca_wr_add_fill + 2;
   end

always @ (posedge clk or negedge rstn)
if (~rstn) begin
   ca_wr_add <= 0;
   ca_wr_wea <= 0;
   ca_wr_dina <= 0;
   ca_wr_add_start <= 0;
   ca_wr_fsm_state <= 0;
   ca_wr_add_start <= 0;
   ca_wr_add_fill <= 0;  
   ca_wr_add_ptr <= 0;
   ca_ready <= 0; 
   ca_insert_time <= 0;    
   ca_update_rd_add <= 0;  
   ca_wr_add_start_marker <= 0; 
   ca_end_of_wr_list <= 0;
   ca_wr_sync_update_done <= 0;    
end else begin
   ca_wr_wea <= 1'b0;
   case (ca_wr_fsm_state) 
      0: begin /////////////////////////////////////////////////// reset
           ca_wr_wea <= 1'b1;
           ca_wr_add_start <= 8'h010;
           ca_wr_add_fill <= 8'h010;
           if (ca_wr_add == 511) begin
              ca_wr_add <= 8'h010;
              ca_wr_fsm_state <= 1;
              ca_ready <= 1'b1;
           end else
              ca_wr_add <= ca_wr_add + 1;
         end
      1: begin /////////////////////////////////////////////////// wait
            ca_ready <= 1'b1;
            ca_wr_wea <= 1'b0;
            ca_wr_add_start_marker <= 1'b0;
            if (ca_wr_com_c3) begin  //////////////////////////////// handle COM write
               ca_wr_add <= {5'h00, tid_wr0};
               ca_wr_dina <= {1'b0, dbus_wr_data_c3};
               ca_wr_wea <= 1'b1;
            end else if (ca_wr_et_c3) begin // & ////////////////////////////// handle ET write when no match executed
               ca_wr_add <= {5'h00, tid_wr0}; //////////////////// read thread specific last command entry
               ca_insert_time <= dbus_wr_data_c3[22:0];
               ca_ready <= 1'b0;
               ca_wr_fsm_state <= 2;
               ca_wr_add_ptr <= ca_wr_add_start;
               ca_wr_add_start_marker <= 1'b1;
            end else if (ca_wr_sync_update) begin //////////////// clear entry and progress start ptr after match
               ca_wr_add <= ca_wr_add_start;
               ca_wr_dina[32:23] <= 0;
               ca_wr_wea <= 1'b1;
               if (ca_rd_doutb_31_23) //////////////////////////// read section points to start add
                  ca_wr_add_start <= ca_rd_doutb[31:23];
               ca_wr_fsm_state <= 19;       
               ca_ready <= 1'b0;
            end 
         end
      2: begin ///////////////////////////////////////////////////
            ca_wr_fsm_state <= 3;
         end
      3: begin ///////////////////////////////////////////////////
            ca_wr_fsm_state <= 4;
            ca_wr_add <= ca_wr_add_start;
         end
      4: begin /////////////////////////////////////////////////// write command at new entry
            ca_wr_fsm_state <= 5;
            ca_wr_add <= ca_wr_add_fill + 1;
            ca_wr_dina <= ca_wr_douta; // command entry
            ca_wr_wea <= 1'b1;
         end
      5: begin ///////////////////////////////////////////////////
            ca_wr_fsm_state <= 6;
            ca_wr_wea <= 1'b0;
            ca_wr_add <= ca_wr_add_start;
         end
      6: begin /////////////////////////////////////////////////// make decision
            if (!ca_wr_douta[32]) begin ////////////////////////// !ca_wr_douta[32]
               ca_wr_add <= ca_wr_add_fill;
               ca_wr_dina <= {10'h200, ca_insert_time};
               ca_wr_wea <= 1'b1;
               if (ca_wr_add_start_marker) begin /////// insert at start
                  ca_wr_add_start <= ca_wr_add_fill;
                  ca_update_rd_add <= 1'b1;
               end
               ca_wr_fsm_state <= 13;
               ca_wr_add_fill <= ca_wr_add_fill_next;
            end else begin
               ca_wr_fsm_state <= 7;
               ca_wr_dina <= {1'b1, ca_wr_add_ptr, ca_wr_douta[22:0]};
               ca_wr_add_ptr <= ca_wr_add;
               ca_wr_add <= ca_wr_douta[31:23];
            end
         end
      7: begin /////////////////////////////////////////////////// check for insertion or appending
            if (ca_end_of_wr_list & !ca_insert) begin
               ca_wr_add <= ca_wr_add_ptr;
               ca_wr_dina[31:23] <= ca_wr_add_fill;
               ca_wr_wea <= 1'b1;
               ca_wr_fsm_state <= 12;
               ca_wr_add_ptr <= 0;
            end else
            if (ca_insert) begin ///////////////////////////////// insert 
               if (ca_wr_add_start_marker) begin /////// insert at start
                  ca_wr_dina <= {1'b1, ca_wr_add_start, ca_insert_time};
                  ca_wr_add <= ca_wr_add_fill;
                  ca_wr_wea <= 1'b1;
                  ca_wr_fsm_state <= 13;
                  ca_wr_add_start <= ca_wr_add_fill;
                  ca_update_rd_add <= 1'b1;
                  ca_wr_add_fill <= ca_wr_add_fill_next;
               end else begin
                  ca_wr_add <= ca_wr_dina[31:23];
                  ca_wr_fsm_state <= 9;
               end
            end else begin /////////////////////////////////////// no insert, read next entry
               ca_wr_fsm_state <= 8;
            end
            ca_wr_add_start_marker <= 1'b0;
         end
      8: begin /////////////////////////////////////////////////// delay due to read next entry
            ca_wr_fsm_state <= 6;
         end
      9: begin /////////////////////////////////////////////////// 
            ca_wr_fsm_state <= 10;
         end
      10: begin /////////////////////////////////////////////////// 
            ca_wr_fsm_state <= 11;
          end  
      11: begin //////////////////////////////////////////////////
            ca_wr_dina <= {1'b1, ca_wr_add_fill, ca_wr_douta[22:0]};
            ca_wr_wea <= 1'b1;
            ca_wr_fsm_state <= 12;
          end
      12: begin ////////////////////////////////////////////////// 
            ca_wr_add <= ca_wr_add_fill;
            ca_wr_dina <= {1'b1, ca_wr_add_ptr, ca_insert_time};
            ca_wr_wea <= 1'b1;
            ca_wr_fsm_state <= 13;
            ca_wr_add_fill <= ca_wr_add_fill_next;
          end
      13: begin ////////////////////////////////////////////////// 
            ca_wr_add <= ca_wr_add_fill;
            ca_wr_fsm_state <= 14;
            ca_wr_wea <= 1'b0;
            ca_update_rd_add <= 1'b0;
          end
      14: begin ////////////////////////////////////////////////// 
            ca_wr_fsm_state <= 15;
          end
      15: begin ////////////////////////////////////////////////// 
            ca_wr_fsm_state <= 16;
          end
      16: if (ca_wr_douta[32]) begin ////////////////////////// ca_wr_douta[32]
             ca_wr_fsm_state <= 13;
             ca_wr_add_fill <= ca_wr_add_fill_next;
          end else begin
             ca_wr_fsm_state <= 1;
             ca_ready <= 1'b1;
          end
      19: begin ////////////////////////////////////////////////// 
             ca_wr_add <= ca_wr_add_start;
            ca_wr_wea <= 1'b0;
            ca_wr_sync_update_done <= 1'b1;
            if (!ca_wr_sync_update) begin
               ca_wr_sync_update_done <= 1'b0; 
               ca_wr_fsm_state <= 1;
               ca_ready <= 1'b1;
            end
          end
      default : ;
   endcase
   ca_end_of_wr_list <= ca_wr_douta[31:23] == 0;
end

tdp_ram_512x33 memi (.clka(clk),
                     .wea(ca_wr_wea),
                     .addra(ca_wr_add),
                     .dina(ca_wr_dina),
                     .douta(ca_wr_douta),
                     .clkb(clk),
                     .web(1'b0),
                     .addrb(ca_rd_add),
                     .dinb(33'h000000000),
                     .doutb(ca_rd_doutb));

// test_overflow:  late-bits: 8
// !test_overflow: late-bits: 12

reg ca_match_check;
reg [22:0] ca_time_diff; 
reg time_dif_neg;    

always @ (posedge clk or negedge rstn)
if (~rstn) begin
   ca_match_check <= 0;
   ca_time_diff <= 0;
   time_dif_neg <= 0;
end else
begin
   if (test_overflow) begin 
      time_dif_neg <= ca_time_diff[11:8] == 4'hf;
      if ((ca_time[11:8] == 0) &
          (ca_rd_doutb[11:8] == 4'hf)) ///////////////// overflow
         ca_match_check <= 1'b0;
      else
         ca_match_check <= 1'b1;
      ca_time_diff[11:0] <= ca_rd_doutb[11:0] - ca_time[11:0];
   end else begin
      time_dif_neg <= ca_time_diff[22:12] == 11'h7ff;
      if ((ca_time[22:12] == 0) &
          (ca_rd_doutb[22:12] == 11'h7ff)) ///////////////// overflow
         ca_match_check <= 1'b0;
      else
         ca_match_check <= 1'b1;
      ca_time_diff <= ca_rd_doutb[22:0] - ca_time;
   end
end

wire ca_match_int = ca_rd_doutb_32 & 
                    ((ca_match_check) ?
                        ((test_overflow & (ca_time_diff[11:8] == 4'hf)) | 
                        (!test_overflow & (ca_time_diff[22:12] == 11'h7ff))) : 
                        1'b1);

reg ca_rd_doutb_32;
reg ca_match;
reg ca_end_of_rd_list;  

always @ (posedge clk or negedge rstn)
if (~rstn) begin
   ca_rd_add <= 0;
   ca_rd_fsm_state <= 0;
   ca_rd_doutb_32 <= 0;
   ca_match <= 0;
   ca_command <= 0;
   ca_end_of_rd_list <= 0;
   ca_wr_sync_update <= 0;
   ca_rd_doutb_31_23 <= 0;
end else begin
   ca_rd_doutb_32 <= ca_rd_doutb[32];
   ca_match <= 0;
   ca_rd_doutb_31_23 <= (ca_rd_doutb[31:23] != 0);
   if (ca_update_rd_add) begin /////////////////////////////////// new entry before current one !!!
      ca_rd_add <= ca_wr_add_start;
      ca_rd_fsm_state <= 6;
   end else
   case (ca_rd_fsm_state)
      0: begin /////////////////////////////////////////////////// reset
            ca_rd_add <= 8'h010;
            if (ca_ready) 
               ca_rd_fsm_state <= 1;
         end
      1: begin /////////////////////////////////////////////////// wait
            if (ca_match_int &
               !ca_wr_add_start_marker) begin
               ca_rd_fsm_state <= 2;
               ca_rd_add <= ca_wr_add_start + 1;
            end
         end
      2: begin /////////////////////////////////////////////////// end clean bit
            ca_rd_fsm_state <= 3;
            ca_rd_add <= ca_wr_add_start;
         end
      3: begin
            ca_rd_fsm_state <= 4;
         end
      4: begin
            ca_match <= 1'b1;
            ca_command <= ca_rd_doutb[31:0];
            if (!ca_match_block)
               ca_rd_fsm_state <= 5;
        end
      5: begin
            ca_wr_sync_update <= 1'b1;
            if (ca_wr_sync_update_done) begin
               ca_rd_fsm_state <= 6;
               ca_rd_add <= ca_wr_add_start;
               ca_wr_sync_update <= 1'b0;
            end
         end
      6: begin
           ca_rd_fsm_state <= 7;
         end
      7: begin  
            ca_rd_fsm_state <= 8;
         end
      8: if (!ca_wr_sync_update_done)
            ca_rd_fsm_state <= 1;
      default : ;
   endcase
end

reg [31:0] dbus_wr_add_c2;

always @ (posedge clk or negedge rstn)
if (~rstn) begin
   dbus_wr_add_c2 <= 0;
end else begin
   dbus_wr_add_c2 <= dbus_wr_add;
end

reg ca_wr_com_c3;
reg ca_wr_et_c3;
reg [31:0] dbus_wr_data_c3;

always @ (posedge clk or negedge rstn)
if (~rstn) begin
   ca_wr_com_c3 <= 0;
   ca_wr_et_c3 <= 0;
   dbus_wr_data_c3 <= 0;
end else begin
   ca_wr_com_c3 <= ca_wr_com;
   ca_wr_et_c3 <= ca_wr_et & ca_ready;
   dbus_wr_data_c3 <= dbus_wr_data;
end

endmodule
