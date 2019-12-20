//
// Copyright 2019 Tobias Strauch, Munich, Bavaria
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//

module core_D16 (clk,
                 rstn,
                 uart_prog_access,
                 uart_prog_wea,
                 uart_prog_addra,
                 uart_prog_dina,
                 uart_prog_douta,
                 dbus_rdwr_gpio_base,
                 dbus_rd_add_gpio,
                 dbus_rdwr_add_mp,
                 dbus_wr_add,
                 dbus_rdwr_be,
                 dbus_wr_en,
                 dbus_wr_data,
                 dbus_rd_req_gpio,
                 dbus_rd_req_mp,
                 dbus_rd_data_gpio,
                 dbus_rd_data_mp,
                 uart_implement,
                 uart_rec_valid,
                 uart_rec_data,
                 uart_send_ready,
                 uart_send_req,
                 uart_send_data,
                 ca_match,
                 ca_command,
                 gpio_event,
                 gpio_event_ack,
                 gpio_command,
                 mp_access,
                 mp_access_ack,
                 mp_command_pc,
                 mp_command_data,
                 mp_stalled_by_mp);
 
`include "MPSOC_parameters.v"

input clk;
input rstn;
input uart_prog_access;
input [7:0] uart_prog_wea;
input [12:0] uart_prog_addra;
input [31:0] uart_prog_dina;
output [63:0] uart_prog_douta;
output dbus_rdwr_gpio_base;
output [31:0] dbus_rd_add_gpio;
output [31:0] dbus_rdwr_add_mp;
output [31:0] dbus_wr_add;
output [3:0] dbus_rdwr_be;
output dbus_wr_en;
output [31:0] dbus_wr_data;
output dbus_rd_req_gpio;
output dbus_rd_req_mp;
input [31:0] dbus_rd_data_gpio;
input [31:0] dbus_rd_data_mp;
input uart_implement;
input uart_rec_valid;
input [7:0] uart_rec_data;
input uart_send_ready;
output uart_send_req;
output [7:0] uart_send_data;
output ca_match;
output [31:0] ca_command; 
input gpio_event;
output gpio_event_ack;
input [30:0] gpio_command;
input mp_access;
output mp_access_ack;
input [13:0] mp_command_pc;
input [31:0] mp_command_data;
input mp_stalled_by_mp;

wire [31:0] pc;
wire [1:0] dbus_rdwr_width;
wire dbus_rd_req_c1;
wire [31:0] dbus_rd_add;
wire [31:0] dbus_rdwr_add_c1;
wire [31:0] dbus_rd_add_fast;
wire [31:0] dbus_wr_data_c1;
reg [31:0] dbus_wr_data_c2;
wire dbus_wr_en_c2;
reg dbus_rd_req_c3;
reg dbus_wr_en_c3;
wire [31:0] dbus_wr_data;
reg init_state;
reg [15:0] tid_active;
reg [3:0] tid_fifo [11:0];
reg [3:0] tid_fifo_ptr;
reg [15:0] fifo_valid;
reg fifo_full;
reg [4:0] thread_pipe_valid;
reg [4:0] pc_set;
reg [46:0] pc_set_data [4:0];
reg [3:0] tid_rd0;
reg [3:0] tid_rd1;
reg [3:0] tid_rd2;
reg [3:0] tid_rd3;
reg [3:0] tid_wr0;
reg gpio_event_ack;
reg mp_access_ack;
wire uart_rx_access;
wire [13:0] uart_rx_command_pc;
wire [7:0] uart_rx_command_data;
reg uart_rx_access_ack;
wire uart_tx_access;
wire [13:0] uart_tx_command_pc;
reg uart_tx_access_ack;
wire [22:0] ca_time;
wire stalled_by_ca;
wire ca_match_int = ca_match;
wire [31:0] ca_command_int = ca_command;
wire ca_match_block = ca_match_reg;
wire dbus_wr_en = dbus_wr_en_c2;
wire [31:0] tc_kill_param = TC_KILL;
wire kill = dbus_wr_en_c2 & (dbus_rdwr_add_c2[31:30] == 2'h2) & (dbus_rdwr_add_c2[19:0] == tc_kill_param[19:0]); //(dbus_rdwr_add_c2 == TC_KILL);
wire [31:0] tc_start_param = TC_START;
wire start = dbus_wr_en_c2 & (dbus_rdwr_add_c2[31:30] == 2'h2) & (dbus_rdwr_add_c2[19:0] == tc_start_param[19:0]); //(dbus_rdwr_add_c2 == TC_START);
wire [31:0] tc_sak_param = TC_SAK;
wire sak = dbus_wr_en_c2 & (dbus_rdwr_add_c2[31:30] == 2'h2) & (dbus_rdwr_add_c2[19:0] == tc_sak_param[19:0]); //(dbus_rdwr_add_c2 == TC_SAK);
wire stalled_by_tc = start & fifo_full;

integer i; 

///////////////////////////////////////////////////////////
// event pre-selection
/////////////////////////////////////////////////////////// 
reg [3:0] event_fsm;
reg [46:0] event_command;  

always @ (posedge clk or negedge rstn)
if (~rstn) begin
   event_fsm <= 0;
   event_command <= 0;
   gpio_event_ack <= 0;
   mp_access_ack <= 0;
   uart_rx_access_ack <= 0;
   uart_tx_access_ack <= 0;
end else begin
   gpio_event_ack <= 0;
   mp_access_ack <= 0;
   uart_rx_access_ack <= 0;
   uart_tx_access_ack <= 0;
   case (event_fsm) 
      0: if (gpio_event) begin
            event_fsm <= 1;
            event_command <= {gpio_command[30], 16'h0000, gpio_command[29:0]};
         end else
         if (mp_access) begin
            event_fsm <= 2;
            event_command <= {1'b1, mp_command_data, mp_command_pc};
         end else
         if (uart_rx_access) begin
            event_fsm <= 3;
            event_command <= {1'b1, 24'h000000, uart_rx_command_data, uart_rx_command_pc};
         end else
         if (uart_tx_access) begin
            event_fsm <= 4;
            event_command <= {1'b1, 24'h000000, 8'h00, uart_tx_command_pc};
         end
      1: if (event_ack) begin
            event_fsm <= 0;
            gpio_event_ack <= 1'b1;
         end
      2: if (event_ack) begin
            event_fsm <= 0;
            mp_access_ack <= 1'b1;
         end
      3: if (event_ack) begin
            event_fsm <= 0;
            uart_rx_access_ack <= 1'b1;
         end
      4: if (event_ack) begin
            event_fsm <= 0;
            uart_tx_access_ack <= 1'b1;
        end
      default: ;
   endcase
end

///////////////////////////////////////////////////////////
// central thread handling 
///////////////////////////////////////////////////////////
reg event_ack;
reg ca_match_reg;
reg [31:0] ca_command_reg;
reg [4:0] rd_valid;
reg tid_fifo_write;
reg tid_fifo_write_minus_one;
reg tid_fifo_shift;
reg tid_fifo_ptr_inc;
reg tid_fifo_ptr_dec;

always @ (posedge clk) begin
   if (tid_fifo_shift)
      for (i = 0; i < 12; i = i + 1) begin
         tid_fifo[i] <= tid_fifo[i + 1];
      end
   if (tid_fifo_write) begin
      if (tid_fifo_shift)
         tid_fifo[tid_fifo_ptr - 1] <= tid_wr0;
      else begin
         tid_fifo[tid_fifo_ptr] <= tid_wr0;
      end
   end
end

always @ (posedge clk or negedge rstn)
if (~rstn) begin
   tid_fifo_ptr <= 0;
end else begin     
   if (tid_fifo_write) begin
      if (!tid_fifo_shift)
         tid_fifo_ptr <= tid_fifo_ptr + 1;
   end else if (tid_fifo_shift)
      tid_fifo_ptr <= tid_fifo_ptr - 1;
end

wire tid_fifo_12 = (tid_fifo_ptr == 4'hc);

always @ (posedge clk or negedge rstn)
if (~rstn) begin
   tid_active <= 0;
   tid_rd0 <= 0;
   tid_rd1 <= 0;
   tid_rd2 <= 0;
   tid_rd3 <= 0;
   tid_wr0 <= 0;
   thread_pipe_valid <= 0;
   init_state <= 0;
   pc_set <= 0;
   for (i = 0; i < 5; i = i + 1) begin
      pc_set_data[i] <= 0;
   end
   fifo_full <= 0;
   ca_match_reg <= 0;
   ca_command_reg <= 0;
   event_ack <= 0;
   rd_valid <= 0;
   tid_fifo_write <= 0;
   tid_fifo_shift <= 0;
   tid_fifo_ptr_inc <= 0;
   tid_fifo_ptr_dec <= 0;
end else begin
   tid_wr0 <= tid_rd3;
   tid_rd3 <= tid_rd2;
   tid_rd2 <= tid_rd1;
   tid_rd1 <= tid_rd0;
   pc_set <= {1'b0, pc_set[4:1]};
   for (i = 0; i < 4; i = i + 1) begin
      pc_set_data[i] <= pc_set_data[i + 1];
   end
   pc_set_data[4] <= 0;
   event_ack <= 0;
   tid_fifo_ptr_inc <= 0;
   tid_fifo_ptr_dec <= 0;
   fifo_full <= 1'b0;
   thread_pipe_valid[3:0] <= {thread_pipe_valid[4], thread_pipe_valid[3] & tid_active[tid_rd1], thread_pipe_valid[2:1]};               
   rd_valid <= {1'b0, rd_valid[4:1]};
   //////////////////////////////////////////////////////// initialize
   if (!init_state) begin
      tid_fifo_write <= 0;
      tid_fifo_shift <= 0;
      tid_active[tid_next_avail] <= 1'b1;
      tid_rd0 <= tid_next_avail;
      thread_pipe_valid[4] <= 1'b1;
      pc_set[4] <= 1'b1;
      pc_set_data[4] <= 0;
   end else
   //////////////////////////////////////////////////////// execute direct start
   if (start & !fifo_full) begin
      tid_fifo_write <= 0;
      tid_fifo_shift <= 0;
      tid_active[tid_next_avail] <= 1'b1;
      tid_rd0 <= tid_next_avail;
      thread_pipe_valid[4] <= 1'b1;
      pc_set[4] <= 1'b1;
      pc_set_data[4] <= {dbus_wr_data_c2[30], 16'h0000, dbus_wr_data_c2[29:0]};
      if (thread_pipe_valid[1] == 1'b1) begin
          tid_fifo_write <= 1'b1;
          tid_fifo_ptr_inc <= 1'b1;
      end
      if (ca_match_int & ca_command_int[31]) begin
         ca_match_reg <= 1'b1;
         ca_command_reg <= ca_command_int;
      end
   end else 
   //////////////////////////////////////////////////////// execute direct start and kill
   if (sak) begin
      tid_fifo_write <= 0;
      tid_fifo_shift <= 0;
      tid_rd0 <= tid_rd3;
      thread_pipe_valid[4] <= 1'b1;
      pc_set[4] <= 1'b1;
      pc_set_data[4] <= {dbus_wr_data_c2[30], 16'h0000, dbus_wr_data_c2[29:0]};
      if (ca_match_int & ca_command_int[31]) begin
         ca_match_reg <= 1'b1;
         ca_command_reg <= ca_command_int;
      end
   end else 
   //////////////////////////////////////////////////////// execute ca command
   if (ca_match_reg & ca_command_reg[31]) begin
      tid_fifo_write <= 0;
      tid_fifo_shift <= 0;
      tid_active[tid_next_avail] <= 1'b1;
      tid_rd0 <= tid_next_avail;
      thread_pipe_valid[4] <= 1'b1;
      pc_set[4] <= 1'b1;
      pc_set_data[4] <= {ca_command_reg[30], 16'h0000, ca_command_reg[29:0]};
      ca_match_reg <= 1'b0;
      if (thread_pipe_valid[1] == 1'b1 & !kill) begin /// write active thread into fifo
          tid_fifo_write <= 1'b1; 
          tid_fifo_ptr_inc <= 1'b1;
      end
   end else 
   //////////////////////////////////////////////////////// execute ca command
   if (ca_match_int & ca_command_int[31] & !fifo_full) begin
      tid_fifo_write <= 0;
      tid_fifo_shift <= 0;
      tid_active[tid_next_avail] <= 1'b1;
      tid_rd0 <= tid_next_avail;
      thread_pipe_valid[4] <= 1'b1;
      pc_set[4] <= 1'b1;
      pc_set_data[4] <= {ca_command_int[30], 16'h0000, ca_command_int[29:0]};
      if (thread_pipe_valid[1] == 1'b1 & !kill) begin /// write active thread into fifo
          tid_fifo_write <= 1'b1;
          tid_fifo_ptr_inc <= 1'b1;
      end
   end else 
   //////////////////////////////////////////////////////// gpio read, re-enter thread
   if (rd_gpio & !rd_valid[1]) begin
      tid_fifo_write <= 0;
      tid_fifo_shift <= 0;
      thread_pipe_valid[4] <= 1'b1;
      tid_rd0 <= tid_rd3;
      rd_valid[4] <= !rd_valid[1];
   end else 
   //////////////////////////////////////////////////////// handle pre selected event
   if (!(event_fsm == 0) & !event_ack & !fifo_full) begin
      tid_fifo_write <= 0;
      tid_fifo_shift <= 0;
      tid_active[tid_next_avail] <= 1'b1;
      tid_rd0 <= tid_next_avail;
      thread_pipe_valid[4] <= 1'b1;
      pc_set[4] <= 1'b1;
      pc_set_data[4] <= event_command;
      event_ack <= 1'b1;
      if (thread_pipe_valid[1] == 1'b1 & !kill) begin /// write active thread into fifo
          tid_fifo_write <= 1'b1;
          tid_fifo_ptr_inc <= 1'b1;
      end
   end else 
   //////////////////////////////////////////////////////// execute thread from fifo
   if (!(tid_fifo_ptr == 0)) begin
      if (!tid_fifo_shift) begin // no shift is ongoing, take fifo[0]
         tid_rd0 <= tid_fifo[0];
         thread_pipe_valid[4] <= 1'b1;
         tid_fifo_shift <= 1'b1;
         if (thread_pipe_valid[1] == 1'b1 & !kill) begin /// write active thread into fifo
             tid_fifo_write <= 1'b1;
         end else
             tid_fifo_write <= 0;
      end else begin             // shift is going on
         if (tid_fifo_write) begin
            tid_fifo_shift <= 1'b1;
            if (tid_fifo_ptr == 1)
               tid_rd0 <= tid_wr0; // short-cut
            else
               tid_rd0 <= tid_fifo[1]; // pre-fetch
            thread_pipe_valid[4] <= 1'b1;
            if (thread_pipe_valid[1] == 1'b1 & !kill) begin /// write active thread into fifo
                tid_fifo_write <= 1'b1;
            end else
                tid_fifo_write <= 0;
         end else begin
            tid_fifo_shift <= 0;
            if (thread_pipe_valid[1] == 1'b1 & !kill) begin /// write active thread into fifo
               tid_fifo_write <= 0;
               tid_fifo_shift <= 0;
               thread_pipe_valid[4] <= 1'b1;
               tid_rd0 <= tid_rd3;
            end else begin ///////////////////////////////////////// no new thread
               tid_fifo_write <= 0;
               tid_fifo_shift <= 0;
               thread_pipe_valid[4] <= 1'b0;
               tid_rd0 <= 4'bxxxx;
            end
         end
      end
   end else
   //////////////////////////////////////////////////////// re-enter thread
   if (thread_pipe_valid[1] == 1'b1 & !kill) begin
      tid_fifo_write <= 0;
      tid_fifo_shift <= 0;
      thread_pipe_valid[4] <= 1'b1;
      tid_rd0 <= tid_rd3;
   end else begin ///////////////////////////////////////// no new thread
      tid_fifo_write <= 0;
      tid_fifo_shift <= 0;
      thread_pipe_valid[4] <= 1'b0;
      tid_rd0 <= 4'bxxxx;
   end
      
   //////////////////////////////////////////////////////// killing a thread
   if (kill) begin
      tid_active[tid_rd3] <= 1'b0;
   end
   //////////////////////////////////////////////////////// init_state
   if (init_state == 1'b0)
      init_state <= 1'b1;
   //////////////////////////////////////////////////////// fifo_full
   if (tid_fifo_12)
      fifo_full <= 1'b1;
end

wire rdwr_gpio_base = dbus_rdwr_add_c1[31:16] == GPIO_BASE;
assign dbus_rdwr_gpio_base = (dbus_rdwr_add_c2[31:16] == GPIO_BASE);

wire rd_gpio = dbus_rd_req_c2 & (dbus_rdwr_add_c2[31:16] == GPIO_BASE) &
              ((dbus_rdwr_add_c2[7:0] == 8'h20) |
               (dbus_rdwr_add_c2[7:0] == 8'h40));

wire [3:0] thread_pipe_valid_clear = {thread_pipe_valid[4:1], (thread_pipe_valid[0])};
reg [7:0] rd_gpio_data [3:0];
reg [31:0] rom_read_data_reg [4:0];

always @ (posedge clk or negedge rstn)
if (~rstn) begin
   for (i = 0; i < 4; i = i + 1) begin
      rd_gpio_data[i] <= 0;
      rom_read_data_reg[i] <= 0;
   end
end else begin
   if (!pc_set_data_2[46] & rd_valid[2])
      rd_gpio_data[0] <= dbus_rd_data_gpio;
   else
      rd_gpio_data[0] <= 0;
   rd_gpio_data[1] <= dbus_rd_data_gpio;
   for (i = 0; i < 2; i = i + 1) begin
      rom_read_data_reg[i] <= rom_read_data_reg[i + 1];
   end
   rom_read_data_reg[2] <= rom_read_data;
end

wire [3:0] tid_next_avail = !tid_active[0] ? 0 :
                            !tid_active[1] ? 1 :
                            !tid_active[2] ? 2 :
                            !tid_active[3] ? 3 :
                            !tid_active[4] ? 4 :
                            !tid_active[5] ? 5 :
                            !tid_active[6] ? 6 :
                            !tid_active[7] ? 7 :
                            !tid_active[8] ? 8 :
                            !tid_active[9] ? 9 :
                            !tid_active[10] ? 10 :
                            !tid_active[11] ? 11 :
                            !tid_active[12] ? 12 :
                            !tid_active[13] ? 13 :
                            !tid_active[14] ? 14 : 15;

//////////////////////////////////////////////////
// UART communication section
//////////////////////////////////////////////////
wire wr_add_uart_send = dbus_rdwr_add_c1 == UART_SEND;	
wire wr_uart_send = dbus_wr_en_c2 & (dbus_rdwr_add_c2 == UART_SEND);
wire stalled_by_uart = (rd_uart_rec_c2 & !uart_rec_fsm) |
                       (wr_uart_send & uart_send_fsm);
assign uart_rx_command_pc = uart_rx_command[13:0];
assign uart_rx_command_data = uart_rec_data_reg;
assign uart_tx_command_pc = uart_tx_command[13:0];
wire wr_add_uart_tx_com = dbus_rdwr_add_c1 == UART_TX_COM;
wire wr_add_uart_rx_com = dbus_rdwr_add_c1 == UART_RX_COM;

reg [14:0] uart_tx_command;
reg [14:0] uart_rx_command;

always @ (posedge clk or negedge rstn)
if (~rstn) begin
   uart_rx_command <= 0;
   uart_tx_command <= 0;
end else begin
if (uart_implement) begin
   if (dbus_wr_en_c2 & (dbus_rdwr_add_c2 == UART_TX_COM))
      uart_tx_command <= {dbus_wr_data_c2[31], dbus_wr_data_c2[13:0]};
   if (dbus_wr_en_c2 & (dbus_rdwr_add_c2 == UART_RX_COM))
      uart_rx_command <= {dbus_wr_data_c2[31], dbus_wr_data_c2[13:0]};
end
end

reg uart_rec_fsm; 
reg uart_rx_access_int;
reg [7:0] uart_rec_data_reg;
reg uart_rec_fsm_d1;
reg [7:0] uart_rec_data_d1;
reg uart_rec_valid_d1;

always @ (posedge clk or negedge rstn)
if (~rstn) begin
   uart_rec_fsm <= 0;
   uart_rec_data_reg <= 0;
   uart_rx_access_int <= 0;
   uart_rec_fsm_d1 <= 0;
   uart_rec_data_d1 <= 0;
   uart_rec_valid_d1 <= 0;
end else begin
   if (uart_implement) begin 
      uart_rec_fsm_d1 <= uart_rec_fsm;
      uart_rec_data_d1 <= uart_rec_data;
      case (uart_rec_fsm)
      0:         if (uart_rec_valid_d1) begin
                    uart_rec_fsm <= 1;
                    uart_rec_data_reg <= uart_rec_data;
                       if (uart_rx_command[14])
                          uart_rx_access_int <= 1'b1; 
                 end
      default :  if (uart_rx_command[14]) begin
                    if (uart_rx_access_ack) begin
                       uart_rx_access_int <= 1'b0;
                       uart_rec_fsm <= 0;
                    end
                 end else if (rd_uart_rec_c2)
                    uart_rec_fsm <= 0;
      endcase
      uart_rec_valid_d1 <= uart_rec_valid;
   end
end

assign uart_rx_access = uart_rx_access_int & !uart_rx_access_ack;

reg uart_send_fsm;
reg uart_tx_access_int;
reg [7:0] uart_send_data;
reg uart_send_req;
reg uart_send_ready_d1;

always @ (posedge clk or negedge rstn)
if (~rstn) begin
   uart_send_fsm <= 0;
   uart_send_data <= 0;
   uart_send_req <= 0;
   uart_tx_access_int <= 0;
   uart_send_ready_d1 <= 0;
end else begin
   if (uart_implement) begin
      uart_send_req <= 0;
      uart_tx_access_int <= 1'b0;
      case (uart_send_fsm)
      0:         begin
                    if (wr_uart_send) begin
                       uart_send_fsm <= 1;
                       uart_send_data <= dbus_wr_data_c2[7:0];
                       uart_send_req <= 1'b1;
                    end
                    uart_send_ready_d1 <= 1'b0;
                 end
      default :  if (uart_send_ready) begin
                    uart_send_ready_d1 <= 1'b1;
                    if (uart_tx_command[14]) begin
                       uart_tx_access_int <= 1'b1;
                       if (uart_tx_access_ack) begin
                         uart_tx_access_int <= 1'b0;
                         uart_send_fsm <= 0;
                       end
                    end else
                       uart_send_fsm <= 0;
                 end
      endcase
   end
end

assign uart_tx_access = uart_tx_access_int & !uart_tx_access_ack;

//////////////////////////////////////////////////
// Calendar
//////////////////////////////////////////////////
ca cai (.clk(clk),
        .rstn(rstn),
        .test_overflow(1'b0),
        .dbus_wr_add(dbus_rdwr_add_c1),
        .dbus_wr_en_int(dbus_wr_en_c2),
        .dbus_wr_data(dbus_wr_data_c2),
        .tid_wr0(tid_wr0),
        .thread_pipe_valid({1'b0, thread_pipe_valid}),
        .ca_time(ca_time),
        .ca_stalled_by_tc(1'b0),
        .ca_match(ca_match),
        .ca_match_block(ca_match_block),
        .ca_command(ca_command),
        .ca_stalled_by_ca(stalled_by_ca));

//////////////////////////////////////////////////
// ROM
//////////////////////////////////////////////////
wire [15:0] instrl;
wire [15:0] instrh;
wire [31:0] instr = pc_1_c2 ? {instrl, instrh} : {instrh, instrl};

reg pc_1_c1;
reg pc_1_c2;
reg [12:0] pcl_c1;
reg [12:0] pch_c1;
always @ (posedge clk or negedge rstn)
if (~rstn) begin
   pc_1_c1 <= 0;
   pc_1_c2 <= 0;
   pcl_c1 <= 0;
   pch_c1 <= 0;
end else begin
   pc_1_c2 <= pc_1_c1;
   pc_1_c1 <= pc[1];
   pcl_c1 <= pcl;
   pch_c1 <= pch;
end

wire [12:0] pcl = pc[1] ? (pc[14:2] + 1) : pc[14:2];
wire [14:0] pch = pc[14:2];

wire [12:0] rom_access_adr = uart_prog_access ? uart_prog_addra : dbus_rd_add_fast_c1[14:2];
wire [31:0] rom_read_data;

tdp_ram_6144x16x2 progli (.clka(clk),
                          .wea(uart_prog_wea[1:0]),
                          .addra(rom_access_adr),
                          .dina(uart_prog_dina[15:0]),
                          .douta(rom_read_data[15:0]),
                          .clkb(clk),
                          .web(2'b00),
                          .addrb(pcl_c1),
                          .dinb(32'h00000000),
                          .doutb(instrl));

tdp_ram_6144x16x2 proghi (.clka(clk),
                          .wea(uart_prog_wea[3:2]),
                          .addra(rom_access_adr),
                          .dina(uart_prog_dina[31:16]),
                          .douta(rom_read_data[31:16]),
                          .clkb(clk),
                          .web(2'b00),
                          .addrb(pch_c1),
                          .dinb(32'h00000000),
                          .doutb(instrh));

reg [16:0] active_rom;
reg active_rom_eq_lo;
reg active_rom_eq_hi; 

always @ (posedge clk)
if (uart_prog_access) begin
   if (uart_prog_addra == 0)
      active_rom <= 17'h10004;
   else if (active_rom[12:0] == uart_prog_addra)
      active_rom <= active_rom + 4;
   if (active_rom[6:0] == uart_prog_addra[6:0])
      active_rom_eq_lo <= 1'b1;
   else
      active_rom_eq_lo <= 1'b0;
   if (active_rom[12:7] == uart_prog_addra[12:7])
      active_rom_eq_hi <= 1'b1;
   else
      active_rom_eq_hi <= 1'b0;
end

reg [63:0] uart_prog_douta;

always @ (posedge clk)
   uart_prog_douta <= {dbus_rd_data_ram, rom_read_data};

wire dbus_rdwr_dram = (dbus_wr_en_c2 & !(dbus_rdwr_add_c2[31:30] == 2'b10));
wire [3:0] dbus_rdwr_be = (dbus_wr_en_c3 & !(dbus_rdwr_add_c3[31:30] == 2'b10)) ?
                            ((dbus_rdwr_width_c3 == 0) ? 4'h1 :
                            (dbus_rdwr_width_c3 == 1) ? 4'h3 : 4'hf) : uart_prog_wea[7:4];

wire [11:0] dbus_wr_add_tmp = uart_prog_access ? {uart_prog_addra[9:0], 2'b00} : dbus_rdwr_add_c3[11:0];
wire [31:0] dbus_wr_data_tmp = uart_prog_access ? uart_prog_dina : dbus_wr_data_c3;

//////////////////////////////////////////
// Byte oriented RAM
//////////////////////////////////////////
reg [7:0] dbus_rdwr_be_final;
reg [63:0] dbus_wr_data_final;
reg [31:0] dbus_rd_data_ram;

always @ (dbus_wr_add_tmp or dbus_rdwr_be or dbus_wr_data_tmp) begin
case (dbus_wr_add_tmp[2:0]) // synopsys full_case
   0:       begin
               dbus_rdwr_be_final = {4'h0, dbus_rdwr_be};
               dbus_wr_data_final = {32'hxxxxxxxx, dbus_wr_data_tmp};
            end
   1:       begin
               dbus_rdwr_be_final = {3'h0, dbus_rdwr_be, 1'h0};
               dbus_wr_data_final = {24'hxxxxxx, dbus_wr_data_tmp, 8'hxx};
            end
   2:       begin
               dbus_rdwr_be_final = {2'h0, dbus_rdwr_be, 2'h0};
               dbus_wr_data_final = {16'hxxxx, dbus_wr_data_tmp, 16'hxxxx};
            end
   3:       begin
               dbus_rdwr_be_final = {1'h0, dbus_rdwr_be, 3'h0};
               dbus_wr_data_final = {8'hxx, dbus_wr_data_tmp, 24'hxxxxxx};
            end
   4:       begin
               dbus_rdwr_be_final = {dbus_rdwr_be, 4'h0};
               dbus_wr_data_final = {dbus_wr_data_tmp, 32'hxxxxxx};
            end
   5:       begin
               dbus_rdwr_be_final = {dbus_rdwr_be[2:0], 4'h0, dbus_rdwr_be[3]};
               dbus_wr_data_final = {dbus_wr_data_tmp[23:0], 32'hxxxxxxxx, dbus_wr_data_tmp[31:24]};
            end
   6:       begin
               dbus_rdwr_be_final = {dbus_rdwr_be[1:0], 4'h0, dbus_rdwr_be[3:2]};
               dbus_wr_data_final = {dbus_wr_data_tmp[15:0], 32'hxxxxxxxx, dbus_wr_data_tmp[31:16]};
            end
   7:       begin
               dbus_rdwr_be_final = {dbus_rdwr_be[0], 4'h0, dbus_rdwr_be[3:1]};
               dbus_wr_data_final = {dbus_wr_data_tmp[7:0], 32'hxxxxxxxx, dbus_wr_data_tmp[31:8]};
            end
   default: ;
endcase
end

always @ (data_access_adr_c2 or dbus_rd_data_ram_64) begin
case (data_access_adr_c2[2:0]) // synopsys full_case
   0:       begin
               dbus_rd_data_ram = dbus_rd_data_ram_64[31:0];
            end
   1:       begin
               dbus_rd_data_ram = dbus_rd_data_ram_64[39:8];
            end
   2:       begin
               dbus_rd_data_ram = dbus_rd_data_ram_64[47:16];
            end
   3:       begin
               dbus_rd_data_ram = dbus_rd_data_ram_64[55:24];
            end
   4:       begin
               dbus_rd_data_ram = dbus_rd_data_ram_64[63:32];
            end
   5:       begin
               dbus_rd_data_ram = {dbus_rd_data_ram_64[7:0], dbus_rd_data_ram_64[63:40]};
            end
   6:       begin
               dbus_rd_data_ram = {dbus_rd_data_ram_64[15:0], dbus_rd_data_ram_64[63:48]};
            end
   7:       begin
               dbus_rd_data_ram = {dbus_rd_data_ram_64[23:0], dbus_rd_data_ram_64[63:56]};
            end
   default: ;
endcase
end

wire [63:0] dbus_rd_data_ram_64;
wire [11:0] data_access_adr = uart_prog_access ? {uart_prog_addra[9:0], 2'b00} : dbus_rd_add_fast_c1[11:0];

sdp_ram_512x32x4 datahi (.clka(clk),
                         .wea(dbus_rdwr_be_final[7:4]),
                         .addra(dbus_wr_add_tmp[11:2]),
                         .dina(dbus_wr_data_final[63:32]),
                         .clkb(clk),
                         .addrb(data_access_adr[11:2]),
                         .doutb(dbus_rd_data_ram_64[63:32]));

sdp_ram_512x32x4 datali (.clka(clk),
                         .wea(dbus_rdwr_be_final[3:0]),
                         .addra(dbus_wr_add_tmp[11:2]),
                         .dina(dbus_wr_data_final[31:0]),
                         .clkb(clk),
                         .addrb(data_access_adr[11:2]),
                         .doutb(dbus_rd_data_ram_64[31:0]));

////////////////////
// read
////////////////////
wire rd_ca_time = dbus_rd_req_c1 & (dbus_rd_add_fast_c1[18:0] == CA_CT);
wire rd_active_rom = dbus_rd_req_c1 & (dbus_rdwr_add_c1[18:16] == 3'h1) & (dbus_rd_add_fast_c1[18:0] < {2'h0, active_rom});
wire rd_uart_rec = dbus_rd_req_c1 & (dbus_rd_add_fast_c1[18:0] == UART_REC);	
wire rd_uart_rec_stat = dbus_rd_req_c1 & (dbus_rd_add_fast_c1[18:0] == UART_REC_STAT);	
wire rd_uart_send_stat = dbus_rd_req_c1 & (dbus_rd_add_fast_c1[18:0] == UART_SEND_STAT);	
wire [31:0] dbus_rd_data_ca_time = rd_ca_time ? {9'h000, ca_time} : 0;
wire [31:0] dbus_rd_data_uart_rec = (uart_implement & rd_uart_rec) ? {24'h000000, uart_rec_data_d1} : 0;
wire [31:0] dbus_rd_data_uart_rec_stat = (uart_implement & rd_uart_rec_stat) ? {31'h00000000, !uart_rec_fsm_d1} : 0;
wire [31:0] dbus_rd_data_uart_send_stat = (uart_implement & rd_uart_send_stat) ? {31'h00000000, !uart_send_ready_d1} : 0;
wire [31:0] dbus_rd_data_pre = (pc_set_data_2[46] ? pc_set_data_2[45:14] : 0) |
                               dbus_rd_data_ca_time |
                               dbus_rd_data_uart_rec |
                               dbus_rd_data_uart_rec_stat |
                               dbus_rd_data_uart_send_stat;
wire dbus_rd_data_ram_clear_sel = (rd_active_rom |
                                   pc_set_data_2[46]);
wire [31:0] dbus_rd_data_ram_clear = (!(dbus_rdwr_add_c2[18:16] == 3'h0 |
                                        dbus_rdwr_add_c2[18:16] == 3'h2 |
                                        dbus_rdwr_add_c2[18:16] == 3'h3 |
                                        dbus_rdwr_add_c2[18:16] == 3'h4) &
                                      !dbus_rd_data_ram_clear_sel_c2) ? dbus_rd_data_ram : 0;
wire [31:0] dbus_rd_data_rom = (!pc_set_data_1[46] & rd_active_rom_c2) ? rom_read_data : 0;
wire [31:0] dbus_rd_data = dbus_rd_data_mp |
                           {24'h000000, rd_gpio_data[0]} |
                           dbus_rd_data_rom |
                           dbus_rd_data_pre_c2;
wire [46:0] pc_set_data_0 = pc_set_data[0];
wire [46:0] pc_set_data_1 = pc_set_data[1];
wire [46:0] pc_set_data_2 = pc_set_data[2];
wire [46:0] pc_set_data_3 = pc_set_data[3];
wire [31:0] dbus_rd_data_int = dbus_rd_data_c3 | dbus_rd_data_ram_clear_c3;
wire stalled_by_ext = stalled_by_ca_c3 | 
                      stalled_by_tc_c3 | 
                      mp_stalled_by_mp | 
                      stalled_by_uart_c3 |
                      (rd_gpio_c3 & !rd_valid[0]);
wire [4:0] data_set = pc_set & {4'b0000, pc_set_data_0[46]};


////////////////////
// CUBE-V
////////////////////
CUBEV_RV32iMC_P3C4D16 RV32i (.clk(clk),
                             .clk_c1(clk),
                             .clk_c2(clk),
                             .clk_c3(clk),
                             .rstn(rstn),
                             .tidwr(tid_wr0),
                             .thread_pipe_valid({1'b0, thread_pipe_valid_clear}),
                             .tidrd0(tid_rd0),
                             .tidrd1(tid_rd1),
                             .tidrd2(tid_rd2),
                             .tidrd3(tid_rd3),
                             .pc(pc),
                             .instr(instr),
                             .pc_set(pc_set[4:0]),
                             .pc_set_data(pc_set_data_3[13:0]),
                             .data_set(data_set),
                             .stalled_by_ext(stalled_by_ext),
                             .dbus_rdwr_add(dbus_rdwr_add_c1),
                             .dbus_rd_add_fast(dbus_rd_add_fast),
                             .dbus_rdwr_width(dbus_rdwr_width),
                             .dbus_rd_i_req(dbus_rd_req_c1),
                             .dbus_rd_i_data(dbus_rd_data_int),
                             .dbus_wr_en(dbus_wr_en_c2),
                             .dbus_wr_data(dbus_wr_data_c1));

////////////////////
// CSR
////////////////////
reg [31:0] dbus_rd_add_fast_c1;

always @ (posedge clk or negedge rstn)
if (~rstn) begin
   dbus_rd_add_fast_c1 <= 0;
end else begin
   dbus_rd_add_fast_c1 <= dbus_rd_add_fast;
end

reg [31:0] dbus_rdwr_add_c2; 
reg rd_active_rom_c2;
reg rd_uart_rec_c2;
reg dbus_rd_req_c2;
reg [3:0] dbus_rdwr_width_c2;
reg [31:0] dbus_rd_data_pre_c2;
reg dbus_rd_data_ram_clear_sel_c2;

always @ (posedge clk or negedge rstn)
if (~rstn) begin
   dbus_rdwr_add_c2 <= 0;
   dbus_wr_data_c2 <= 0;
   rd_active_rom_c2 <= 0;
   rd_uart_rec_c2 <= 0;
   dbus_rd_req_c2 <= 0;
   dbus_rdwr_width_c2 <= 0;
   dbus_rd_data_pre_c2 <= 0;
   dbus_rd_data_ram_clear_sel_c2 <= 0;
end else begin
   dbus_rdwr_add_c2 <= dbus_rdwr_add_c1;
   dbus_wr_data_c2 <= dbus_wr_data_c1;
   rd_active_rom_c2 <= rd_active_rom;
   rd_uart_rec_c2 <= rd_uart_rec;
   dbus_rd_req_c2 <= dbus_rd_req_c1;
   dbus_rdwr_width_c2 <= dbus_rdwr_width;
   dbus_rd_data_pre_c2 <= dbus_rd_data_pre;
   dbus_rd_data_ram_clear_sel_c2 <= dbus_rd_data_ram_clear_sel;
end

reg [11:0] data_access_adr_c2;

always @ (posedge clk)
begin
   data_access_adr_c2 <= data_access_adr;
end

reg [31:0] dbus_rdwr_add_c3;
reg [31:0] dbus_rd_data_c3;
reg [31:0] dbus_wr_data_c3;
reg stalled_by_uart_c3;
reg stalled_by_ca_c3;
reg stalled_by_tc_c3;
reg rd_gpio_c3;
reg [3:0] dbus_rdwr_width_c3;
reg [31:0] dbus_rd_data_ram_clear_c3;

always @ (posedge clk or negedge rstn)
if (~rstn) begin
   dbus_rdwr_add_c3 <= 0;
   dbus_rd_data_c3 <= 0;
   dbus_wr_data_c3 <= 0;
   dbus_rd_req_c3 <= 0;
   dbus_wr_en_c3 <= 0;
   stalled_by_uart_c3 <= 0;
   stalled_by_ca_c3 <= 0;
   stalled_by_tc_c3 <= 0;
   rd_gpio_c3 <= 0;
   dbus_rdwr_width_c3 <= 0;
   dbus_rd_data_ram_clear_c3 <= 0;
end else begin
   dbus_rdwr_add_c3 <= dbus_rdwr_add_c2;
   dbus_rd_data_c3 <= dbus_rd_data;
   dbus_wr_data_c3 <= dbus_wr_data_c2;
   dbus_rd_req_c3 <= dbus_rd_req_c2;
   dbus_wr_en_c3 <= dbus_wr_en_c2;
   stalled_by_uart_c3 <= stalled_by_uart;
   stalled_by_ca_c3 <= stalled_by_ca;
   stalled_by_tc_c3 <= stalled_by_tc;
   rd_gpio_c3 <= rd_gpio;
   dbus_rdwr_width_c3 <= dbus_rdwr_width_c2;
   dbus_rd_data_ram_clear_c3 <= dbus_rd_data_ram_clear;
end

assign dbus_rd_add_gpio = dbus_rdwr_add_c2;
assign dbus_rdwr_add_mp = dbus_rdwr_add_c1;
assign dbus_rd_req_gpio = dbus_rd_req_c2;
assign dbus_rd_req_mp = dbus_rd_req_c1;
assign dbus_wr_add = dbus_rdwr_add_c1;
assign dbus_wr_data = dbus_wr_data_c1;

endmodule
