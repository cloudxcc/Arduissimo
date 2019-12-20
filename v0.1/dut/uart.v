//
// Copyright 2019 Tobias Strauch, Munich, Bavaria
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//

// no handshake mechanism to core 0, very relaxed timing

module uart ( clk,
              rstn,
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

input clk;
input rstn;
input uart_rx_in;
output uart_tx_out; 
output [3:0] uart_quad_rstn;
output uart_rec_valid;
output [7:0] uart_rec_data;
output uart_send_ready;
input uart_send_req;
input [7:0] uart_send_data; 
output uart_prog_access;
output [31:0] uart_prog_wea;
output [12:0] uart_prog_addra;
output [7:0] uart_prog_dina;
input [255:0] uart_prog_douta;


reg uart_rx_in_regn;
reg [2:0] uart_rx_in_filtern;

reg rec_state;
reg [10:0] rec_time_cnt;
reg [8:0] rec_byte;    // bit 8 will be removed during synthesis
reg [3:0] rec_byte_bit_cnt;  
reg rec_received;  
reg [4:0] cntrl_state;
reg [3:0] uart_quad_rstn;
reg [3:0] uart_quad_shdw_rstn;
reg send_req;
reg read_req;
reg [3:0] read_req_d;
reg [9:0] send_byte;   // bit 8 will be removed during synthesis
reg send_state;
reg [10:0] send_time_cnt;
reg [3:0] send_byte_bit_cnt;
reg uart_tx_outn;
reg [23:0] mem_addr_reg;
reg [15:0] write_length;
reg prog;
reg com;
reg read;
reg uart_prog_access;
reg uart_prog_access_d;
reg [31:0] uart_prog_wea;
reg [31:0] uart_prog_wea_d;
reg [12:0] uart_prog_addra;
reg [12:0] uart_prog_addra_d;
reg [7:0] uart_prog_dina;  
reg [7:0] uart_prog_dina_d;
reg uart_rec_valid;
wire [7:0] uart_rec_data;
reg [255:0] uart_prog_douta_reg;


always @ (posedge clk or negedge rstn)
if (~rstn) begin
   uart_rx_in_regn <= 0;
   uart_rx_in_filtern <= 0; 
end else begin
   uart_rx_in_filtern <= {uart_rx_in_filtern[1:0], ~uart_rx_in};
   uart_rx_in_regn <= (uart_rx_in_filtern[2] & uart_rx_in_filtern[1]) |
                      (uart_rx_in_filtern[2] & uart_rx_in_filtern[0]) |
                      (uart_rx_in_filtern[1] & uart_rx_in_filtern[0]);
end

// 5000000 Baud => 8680,5 ns =>   36 cycles @ 180MHz
//  115200 Baud => 8680,5 ns => 1562 cycles @ 180MHz
//  115200 Baud => 8680,5 ns =>  868 cycles @ 100MHz
//  115200 Baud => 8680,5 ns =>  434 cycles @  50MHz
//  115200 Baud => 8680,5 ns =>  347 cycles @  40MHz     

parameter RS232_BIT_CYCLES = 36;

always @ (posedge clk or negedge rstn)
if (~rstn) begin
   rec_state <= 0;
   rec_time_cnt <= 0;
   rec_byte <= 0;
   rec_byte_bit_cnt <= 0;
   rec_received <= 0;
end else begin
   case (rec_state)
      0: begin 
            rec_received <= 1'b0;
            if (uart_rx_in_regn)
               if (rec_time_cnt == (RS232_BIT_CYCLES / 2)) begin
                  rec_time_cnt <= 0;
                  rec_state <= 1;
               end else
                  rec_time_cnt <= rec_time_cnt + 1;
            else
               rec_time_cnt <= 0;
         end
      1: if (rec_time_cnt == RS232_BIT_CYCLES) begin
            rec_time_cnt <= 0;
            rec_byte[rec_byte_bit_cnt] <= ~uart_rx_in_regn;
            if (rec_byte_bit_cnt == 8) begin
               rec_byte_bit_cnt <= 0;
               rec_state <= 0;
               rec_received <= 1'b1;
            end else
               rec_byte_bit_cnt <= rec_byte_bit_cnt + 1;
         end else
            rec_time_cnt <= rec_time_cnt + 1;
      default : ;
   endcase
end

assign uart_rec_data = rec_byte[7:0];

always @ (posedge clk or negedge rstn)
if (~rstn) begin 
   cntrl_state <= 0;
   uart_quad_shdw_rstn <= 0;
   uart_quad_rstn <= 0;
   send_req <= 0;
   read_req <= 0;
   read_req_d <= 0;
   mem_addr_reg <= 0;
   write_length <= 0;  
   uart_prog_access <= 0;
   uart_prog_access_d <= 0;
   uart_prog_wea <= 0;
   uart_prog_wea_d <= 0;
   uart_prog_addra <= 0;
   uart_prog_addra_d <= 0;
   uart_prog_dina <= 0;
   uart_prog_dina_d <= 0;
   prog <= 0;
   com <= 0;
   read <= 0;
   uart_rec_valid <= 0;
end else begin
   uart_quad_rstn <= uart_quad_shdw_rstn;
   uart_rec_valid <= 0;
   send_req <= 1'b0;
   read_req_d <= {read_req_d[2:0], read_req};
   read_req <= 1'b0;
   uart_prog_access <= uart_prog_access_d;
   uart_prog_wea <= uart_prog_wea_d;
   uart_prog_addra <= uart_prog_addra_d;
   uart_prog_dina <= uart_prog_dina_d;
   case (cntrl_state)
      0: begin
            uart_prog_access_d <= 0;
            uart_prog_wea_d <= 0;
            prog <= 1'b0;
            com <= 1'b0;
            read <= 1'b0;
            if (rec_received) begin
               if (rec_byte[7:4] == 4'h1)
                  uart_quad_shdw_rstn <= ~rec_byte[3:0];
               else
               case (rec_byte[7:0]) 
                  8'h20 : cntrl_state <= 1;
                  8'h30 : begin
                             cntrl_state <= 2;
                             prog <= 1'b1;
                             end
                  8'h40 : begin
                             cntrl_state <= 2;
                             read <= 1'b1;
                          end
                  8'h50 : begin
                             cntrl_state <= 5;
                             com <= 1'b1;
                          end
                  default : ;
                  endcase
            end
         end
      1: if (rec_received) begin
            cntrl_state <= 0;
            send_req <= 1'b1;
         end
      2: if (rec_received) begin
            cntrl_state <= 3;
            mem_addr_reg[18:16] <= rec_byte[2:0];
         end
      3: if (rec_received) begin
            cntrl_state <= 4;
            mem_addr_reg[15:8] <= rec_byte[7:0];
         end
      4: if (rec_received) begin
            cntrl_state <= 5;
            mem_addr_reg[7:0] <= rec_byte[7:0];
         end
      5: if (rec_received) begin
            cntrl_state <= 6;
            write_length[15:8] <= rec_byte[7:0];
         end
      6: if (rec_received) begin
            if (prog | com)  
               cntrl_state <= 7;
            else
               cntrl_state <= 8;
            write_length[7:0] <= rec_byte[7:0];
            if (!(com)) uart_prog_access_d <= 1;
         end
      7: if (rec_received) begin 
            if (prog) begin  
               uart_prog_wea_d <= 1'b1 << {mem_addr_reg[18:16], mem_addr_reg[1:0]};
               uart_prog_addra_d <= mem_addr_reg[14:2];
               uart_prog_dina_d <= rec_byte[7:0];
               send_req <= 1'b1;
            end else begin
               uart_rec_valid <= 1'b1;
            end
            mem_addr_reg <= mem_addr_reg + 1;
            if (write_length == 0)
               cntrl_state <= 0;
            write_length <= write_length - 1;
         end else
            uart_prog_wea_d <= 0;
      8: begin     
            if ((send_state == 0) & (read_req_d == 0)) begin
               uart_prog_addra_d <= mem_addr_reg[14:2];
               if (write_length == 0)
                  cntrl_state <= 0;
               if (read_req) 
                  write_length <= write_length - 1;
               read_req <= 1'b1;
            end
            if (read_req_d == 4'h8) 
               mem_addr_reg <= mem_addr_reg + 1;
         end
      default : ;
   endcase
end

assign uart_send_ready = send_state == 0;

integer k, l, m;

always @ (posedge clk or negedge rstn)
if (~rstn) begin 
   send_state <= 0;
   send_time_cnt <= 0;
   send_byte_bit_cnt <= 0;
   send_byte <= 0;
   uart_tx_outn <= 0;
   uart_prog_douta_reg <= 0;
end else begin
   case (send_state)
      0: if (send_req) begin
            send_byte <= {2'b11, rec_byte[7:0]};
            send_state <= 1;
            uart_tx_outn <= 1'b1;
         end else if (read_req_d[3]) begin
            send_byte[9:8] <= 2'b11;     
            for (m = 0; m < 8; m = m + 1) // all bits
               for (k = 0; k < 8; k = k + 1) // all memories     
                  if (mem_addr_reg[18:16] == k)
                     for (l = 0; l < 4; l = l + 1) // all byte 
                        if (mem_addr_reg[1:0] == l)
                           send_byte[m] <= uart_prog_douta_reg[k * 32 + l * 8 + m];
            send_state <= 1;
            uart_tx_outn <= 1'b1;
         end else if (uart_send_req) begin
            send_byte <= {2'b11, uart_send_data};
            send_state <= 1;
            uart_tx_outn <= 1'b1;
         end
      1: if (send_time_cnt == RS232_BIT_CYCLES) begin
            send_time_cnt <= 0;
            uart_tx_outn <= ~send_byte[send_byte_bit_cnt];
            if (send_byte_bit_cnt == 9) begin
               send_byte_bit_cnt <= 0;
               send_state <= 0;  
            end else
               send_byte_bit_cnt <= send_byte_bit_cnt + 1;
         end else
            send_time_cnt <= send_time_cnt + 1;
      default : ;
   endcase
   uart_prog_douta_reg <= uart_prog_douta; 
end

assign uart_tx_out = ~uart_tx_outn;

endmodule
