//
// Copyright 2019 Tobias Strauch, Munich, Bavaria
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//

module quad (clk,
             rstn,
             uart_prog_access,
             uart_prog_wea,
             uart_prog_addra,
             uart_prog_dina,
             uart_prog_douta,
             uart_rec_valid,
             uart_rec_data,
             uart_send_ready,
             uart_send_req,
             uart_send_data,
             gpio_dir_reg,
             gpio_out_reg,
             gpio_in);

input clk;
input [3:0] rstn;
input uart_prog_access;
input [31:0] uart_prog_wea;
input [12:0] uart_prog_addra;
input [31:0] uart_prog_dina;
output [255:0] uart_prog_douta;
input uart_rec_valid;
input [7:0] uart_rec_data;
input uart_send_ready;
output uart_send_req;
output [7:0] uart_send_data;
output [111:0] gpio_dir_reg;
output [111:0] gpio_out_reg;
input [111:0] gpio_in;

wire [3:0] dbus_rdwr_gpio_base;
wire [31:0] dbus_rd_add_gpio [3:0];
wire [31:0] dbus_rdwr_add_mp [3:0];
wire [31:0] dbus_wr_add [3:0];
wire [1:0] dbus_rdwr_be [3:0];
wire [3:0] dbus_wr_en;
wire [31:0] dbus_wr_data [3:0];
wire [3:0] dbus_rd_req_gpio;
wire [3:0] dbus_rd_req_mp;
wire [127:0] dbus_rd_data_gpio;
wire [127:0] dbus_rd_data_mp;
wire [3:0] ca_match;
wire [127:0] ca_command;
wire [3:0] gpio_event;
wire [3:0] gpio_event_ack;
wire [127:0] gpio_command;
wire [3:0] mp_access;
wire [3:0] mp_access_ack;
wire [55:0] mp_command_pc;
wire [127:0] mp_command_data;
wire [3:0] mp_stalled_by_mp;

gpio gpio_i (.clk(clk),
             .rstn(rstn[0]),
             .gpio_dir_reg(gpio_dir_reg),
             .gpio_out_reg(gpio_out_reg),
             .gpio_in(gpio_in),
             .dbus_rdwr_gpio_base(dbus_rdwr_gpio_base),
             .dbus_rd_add_bus({dbus_rd_add_gpio[3], dbus_rd_add_gpio[2], dbus_rd_add_gpio[1], dbus_rd_add_gpio[0]}),
             .dbus_wr_add_bus({dbus_wr_add[3], dbus_wr_add[2], dbus_wr_add[1], dbus_wr_add[0]}),
             .dbus_rdwr_be_in({dbus_rdwr_be[3], dbus_rdwr_be[2], dbus_rdwr_be[1], dbus_rdwr_be[0]}),
             .dbus_wr_en(dbus_wr_en),
             .dbus_wr_data_in({dbus_wr_data[3], dbus_wr_data[2], dbus_wr_data[1], dbus_wr_data[0]}),
             .dbus_rd_req(dbus_rd_req_gpio),
             .dbus_rd_data_out(dbus_rd_data_gpio),
             .ca_match(ca_match),
             .ca_command_in(ca_command),
             .gpio_event(gpio_event),
             .gpio_event_ack(gpio_event_ack),
             .gpio_command_out(gpio_command));

mp mp_i (.clk(clk),
         .rstn(rstn[0]),
         .dbus_rdwr_add_bus({dbus_rdwr_add_mp[3], dbus_rdwr_add_mp[2], dbus_rdwr_add_mp[1], dbus_rdwr_add_mp[0]}),
         .dbus_rdwr_be_in({dbus_rdwr_be[3], dbus_rdwr_be[2], dbus_rdwr_be[1], dbus_rdwr_be[0]}),
         .dbus_wr_en(dbus_wr_en),
         .dbus_wr_data_in({dbus_wr_data[3], dbus_wr_data[2], dbus_wr_data[1], dbus_wr_data[0]}),
         .dbus_rd_req(dbus_rd_req_mp),
         .dbus_rd_data_out(dbus_rd_data_mp),
         .mp_access(mp_access),
         .mp_access_ack(mp_access_ack),
         .mp_command_pc_out(mp_command_pc),
         .mp_command_data_out(mp_command_data),
         .mp_stalled_by_mp(mp_stalled_by_mp));

core_D16 c0 (.clk(clk),
             .rstn(rstn[0]),
             .uart_prog_access(uart_prog_access),
             .uart_prog_wea(uart_prog_wea[7:0]),
             .uart_prog_addra(uart_prog_addra),
             .uart_prog_dina(uart_prog_dina),
             .uart_prog_douta(uart_prog_douta[63:0]),
             .dbus_rdwr_gpio_base(dbus_rdwr_gpio_base[0]),
             .dbus_rd_add_gpio(dbus_rd_add_gpio[0]),
             .dbus_rdwr_add_mp(dbus_rdwr_add_mp[0]),
             .dbus_wr_add(dbus_wr_add[0]),
             .dbus_rdwr_be(dbus_rdwr_be[0]),
             .dbus_wr_en(dbus_wr_en[0]),
             .dbus_wr_data(dbus_wr_data[0]),
             .dbus_rd_req_gpio(dbus_rd_req_gpio[0]),
             .dbus_rd_req_mp(dbus_rd_req_mp[0]),
             .dbus_rd_data_gpio(dbus_rd_data_gpio[31:0]),
             .dbus_rd_data_mp(dbus_rd_data_mp[31:0]),
             .uart_implement(1'b1),
             .uart_rec_valid(uart_rec_valid),
             .uart_rec_data(uart_rec_data),
             .uart_send_ready(uart_send_ready),
             .uart_send_req(uart_send_req),
             .uart_send_data(uart_send_data),
             .ca_match(ca_match[0]),
             .ca_command(ca_command[31:0]),
             .gpio_event(gpio_event[0]),
             .gpio_event_ack(gpio_event_ack[0]),
             .gpio_command(gpio_command[31:0]),
             .mp_access(mp_access[0]),
             .mp_access_ack(mp_access_ack[0]),
             .mp_command_pc(mp_command_pc[13:0]),
             .mp_command_data(mp_command_data[31:0]),
             .mp_stalled_by_mp(mp_stalled_by_mp[0]));

core_D16 c1 (.clk(clk),
             .rstn(rstn[1]),
             .uart_prog_access(uart_prog_access),
             .uart_prog_wea(uart_prog_wea[15:8]),
             .uart_prog_addra(uart_prog_addra),
             .uart_prog_dina(uart_prog_dina),
             .uart_prog_douta(uart_prog_douta[127:64]),
             .dbus_rdwr_gpio_base(dbus_rdwr_gpio_base[1]),
             .dbus_rd_add_gpio(dbus_rd_add_gpio[1]),
             .dbus_rdwr_add_mp(dbus_rdwr_add_mp[1]),
             .dbus_wr_add(dbus_wr_add[1]),
             .dbus_rdwr_be(dbus_rdwr_be[1]),
             .dbus_wr_en(dbus_wr_en[1]),
             .dbus_wr_data(dbus_wr_data[1]),
             .dbus_rd_req_gpio(dbus_rd_req_gpio[1]),
             .dbus_rd_req_mp(dbus_rd_req_mp[1]),
             .dbus_rd_data_gpio(dbus_rd_data_gpio[63:32]),
             .dbus_rd_data_mp(dbus_rd_data_mp[63:32]),
             .uart_implement(1'b0),
             .uart_rec_valid(1'b0),
             .uart_rec_data(8'h00),
             .uart_send_ready(1'b0),
             .uart_send_req(),
             .uart_send_data(),
             .ca_match(ca_match[1]),
             .ca_command(ca_command[63:32]),
             .gpio_event(gpio_event[1]),
             .gpio_event_ack(gpio_event_ack[1]),
             .gpio_command(gpio_command[63:32]),
             .mp_access(mp_access[1]),
             .mp_access_ack(mp_access_ack[1]),
             .mp_command_pc(mp_command_pc[27:14]),
             .mp_command_data(mp_command_data[63:32]),
             .mp_stalled_by_mp(mp_stalled_by_mp[1]));

core_D16 c2 (.clk(clk),
             .rstn(rstn[2]),
             .uart_prog_access(uart_prog_access),
             .uart_prog_wea(uart_prog_wea[23:16]),
             .uart_prog_addra(uart_prog_addra),
             .uart_prog_dina(uart_prog_dina),
             .uart_prog_douta(uart_prog_douta[191:128]),
             .dbus_rdwr_gpio_base(dbus_rdwr_gpio_base[2]),
             .dbus_rd_add_gpio(dbus_rd_add_gpio[2]),
             .dbus_rdwr_add_mp(dbus_rdwr_add_mp[2]),
             .dbus_wr_add(dbus_wr_add[2]),
             .dbus_rdwr_be(dbus_rdwr_be[2]),
             .dbus_wr_en(dbus_wr_en[2]),
             .dbus_wr_data(dbus_wr_data[2]),
             .dbus_rd_req_gpio(dbus_rd_req_gpio[2]),
             .dbus_rd_req_mp(dbus_rd_req_mp[2]),
             .dbus_rd_data_gpio(dbus_rd_data_gpio[95:64]),
             .dbus_rd_data_mp(dbus_rd_data_mp[95:64]),
             .uart_implement(1'b0),
             .uart_rec_valid(1'b0),
             .uart_rec_data(8'h00),
             .uart_send_ready(1'b0),
             .uart_send_req(),
             .uart_send_data(),
             .ca_match(ca_match[2]),
             .ca_command(ca_command[95:64]),
             .gpio_event(gpio_event[2]),
             .gpio_event_ack(gpio_event_ack[2]),
             .gpio_command(gpio_command[95:64]),
             .mp_access(mp_access[2]),
             .mp_access_ack(mp_access_ack[2]),
             .mp_command_pc(mp_command_pc[41:28]),
             .mp_command_data(mp_command_data[95:64]),
             .mp_stalled_by_mp(mp_stalled_by_mp[2]));

core_D16 c3 (.clk(clk),
             .rstn(rstn[3]),
             .uart_prog_access(uart_prog_access),
             .uart_prog_wea(uart_prog_wea[31:24]),
             .uart_prog_addra(uart_prog_addra),
             .uart_prog_dina(uart_prog_dina),
             .uart_prog_douta(uart_prog_douta[255:192]),
             .dbus_rdwr_gpio_base(dbus_rdwr_gpio_base[3]),
             .dbus_rd_add_gpio(dbus_rd_add_gpio[3]),
             .dbus_rdwr_add_mp(dbus_rdwr_add_mp[3]),
             .dbus_wr_add(dbus_wr_add[3]),
             .dbus_rdwr_be(dbus_rdwr_be[3]),
             .dbus_wr_en(dbus_wr_en[3]),
             .dbus_wr_data(dbus_wr_data[3]),
             .dbus_rd_req_gpio(dbus_rd_req_gpio[3]),
             .dbus_rd_req_mp(dbus_rd_req_mp[3]),
             .dbus_rd_data_gpio(dbus_rd_data_gpio[127:96]),
             .dbus_rd_data_mp(dbus_rd_data_mp[127:96]),
             .uart_implement(1'b0),
             .uart_rec_valid(1'b0),
             .uart_rec_data(8'h00),
             .uart_send_ready(1'b0),
             .uart_send_req(),
             .uart_send_data(),
             .ca_match(ca_match[3]),
             .ca_command(ca_command[127:96]),
             .gpio_event(gpio_event[3]),
             .gpio_event_ack(gpio_event_ack[3]),
             .gpio_command(gpio_command[127:96]),
             .mp_access(mp_access[3]),
             .mp_access_ack(mp_access_ack[3]),
             .mp_command_pc(mp_command_pc[55:42]),
             .mp_command_data(mp_command_data[127:96]),
             .mp_stalled_by_mp(mp_stalled_by_mp[3]));

endmodule
