//
// Copyright 2019 Tobias Strauch, Munich, Bavaria
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//

module tb_CUBEV;

reg clk;
reg rstn;

wire [31:0] pc;
reg [31:0] pc_c1;
reg [31:0] instr;
wire [1:0] dbus_rdwr_width;
wire [1:0] dbus_rd_width;
wire dbus_rd_req;
wire [31:0] dbus_rd_data;
wire [31:0] dbus_rdwr_add;
wire [1:0] dbus_wr_width;
wire dbus_wr_en;
wire [31:0] dbus_wr_data;

reg [7:0] program [100000:0];
reg [7:0] data8 [1048575:0];

integer passcnt;
reg go;

wire [3:0] tidwr;
wire [3:0] tidrd0;
wire [3:0] tidrd1;
wire [3:0] tidrd2;
wire [3:0] tidrd3;
reg [5:0] thread_pipe_valid;
reg [5:0] pc_set;

assign tidwr = 0;
assign tidrd0 = 0;
assign tidrd1 = 0;
assign tidrd2 = 0;
assign tidrd3 = 0;

always @ (posedge clk)
   thread_pipe_valid <= {2'b00, thread_pipe_valid[0], thread_pipe_valid[3:1]};

integer i, ptr;
initial begin
   clk <= 1'b0;
   thread_pipe_valid <= 6'h01;
   pc_set <= 1'b0;
   for (i = 0; i < 16; i = i + 1) begin
      sp = 0;
      go = 1'b1;
      passcnt = 0;
      cycle_cnt = 0;
      rstn <= 1'b0;
      if (i == 0) begin $readmemh("../../../../../../tests/codeGenHex/generate_0.hex", program); end
      if (i == 1) begin $readmemh("../../../../../../tests/codeGenHex/generate_1.hex", program); end
      if (i == 2) begin $readmemh("../../../../../../tests/codeGenHex/generate_2.hex", program); end
      if (i == 3) begin $readmemh("../../../../../../tests/codeGenHex/generate_3.hex", program); end
      if (i == 4) begin $readmemh("../../../../../../tests/codeGenHex/generate_4.hex", program); end
      if (i == 5) begin $readmemh("../../../../../../tests/codeGenHex/generate_5.hex", program); end
      if (i == 6) begin $readmemh("../../../../../../tests/basicC/math/math1/main.hex", program); end
      if (i == 7) begin $readmemh("../../../../../../tests/basicC/branch/branch1/main.hex", program); end
      if (i == 8) begin $readmemh("../../../../../../tests/basicC/branch/branch2/main.hex", program); end
      if (i == 9) begin $readmemh("../../../../../../tests/basicC/branch/branch3/main.hex", program); end
      if (i >= 10) begin 
         for (ptr = 0; ptr < 100000; ptr = ptr + 4) begin
            program[(ptr) + 0] = 8'h13;
            program[(ptr) + 1] = 8'h00;
            program[(ptr) + 2] = 8'h00;
            program[(ptr) + 3] = 8'h00;
         end
      end
      if (i == 10) begin 
         $readmemh("../../../../../../tests/chstone/gsm/work/main.hex", program); 
         $readmemh("../../../../../../tests/chstone/gsm/work/main.hex", data8); 
      end
      if (i == 11) begin 
         $readmemh("../../../../../../tests/chstone/aes/work/main.hex", program); 
         $readmemh("../../../../../../tests/chstone/aes/work/main.hex", data8); 
      end
      if (i == 12) begin 
         $readmemh("../../../../../../tests/chstone/motion/work/main.hex", program); 
         $readmemh("../../../../../../tests/chstone/motion/work/main.hex", data8); 
      end
      if (i == 13) begin 
         $readmemh("../../../../../../tests/chstone/adpcm/work/main.hex", program); 
         $readmemh("../../../../../../tests/chstone/adpcm/work/main.hex", data8); 
      end
      if (i == 14) begin 
         $readmemh("../../../../../../tests/chstone/sha/work/main.hex", program); 
         $readmemh("../../../../../../tests/chstone/sha/work/main.hex", data8); 
      end
      if (i == 15) begin 
         $readmemh("../../../../../../tests/chstone/blowfish/work/main.hex", program); 
         $readmemh("../../../../../../tests/chstone/blowfish/work/main.hex", data8); 
      end
      $display("Running test: ", i);
      #90;
      @ (negedge clk);
         pc_set <= thread_pipe_valid;
      @ (negedge clk);
         rstn <= 1'b1;
      pc_set <= thread_pipe_valid;
      @ (negedge clk);
         pc_set <= thread_pipe_valid;
      @ (negedge clk);
         pc_set <= thread_pipe_valid;
      @ (negedge clk);
         pc_set <= 0;
      while (passcnt < 2) #10;
      #3;
   end
   $finish;
end

always #5 clk <= ~clk;

integer cycle_cnt;
always @ (posedge clk)
   if (cycle_cnt == 800000 * 4) begin
      $display("Test runaway.\n"); 
      $finish;
   end else
   if (duti.pvalid == 2)
   if (!duti.stalled_next)
      cycle_cnt <= cycle_cnt + 1;



CUBEV_RV32iMC_P3C4D16 duti (.clk(clk & go),
                            .clk_c1(clk & go),
                            .clk_c2(clk & go),
                            .clk_c3(clk & go),
                            .rstn(rstn),
                            .tidwr(tidwr),
                            .tidrd0(tidrd0),
                            .tidrd1(tidrd1),
                            .tidrd2(tidrd2),
                            .tidrd3(tidrd3),
                            .thread_pipe_valid(thread_pipe_valid),
                            .pc(pc),
                            .instr(instr),
                            .pc_set(pc_set),
                            .pc_set_data(14'h0000),
                            .data_set(5'h0000),
                            .stalled_by_ext(1'b0),
                            .dbus_rdwr_add(dbus_rdwr_add),
                            .dbus_rdwr_width(dbus_rdwr_width),
                            .dbus_rd_i_req(dbus_rd_req),
                            .dbus_rd_i_data(dbus_rd_data),
                            .dbus_wr_en(dbus_wr_en),
                            .dbus_wr_data(dbus_wr_data));



always @ (posedge clk)
   pc_c1 <= pc;

always @ (posedge clk)   
   instr <= {program[pc_c1[19:0] + 3], program[pc_c1[19:0] + 2], program[pc_c1[19:0] + 1], program[pc_c1[19:0] + 0]};


integer sp;

always @ (negedge clk)
if(thread_pipe_valid == 1)
if (go == 1'b1)
if (dbus_wr_en_c3 & (dbus_rdwr_add_c3 == 0)) begin 
   data8[sp] <= dbus_wr_data_c3[7:0];
   data8[sp + 1] <= dbus_wr_data_c3[15:8];
   data8[sp + 2] <= dbus_wr_data_c3[23:16];
   data8[sp + 3] <= dbus_wr_data_c3[31:24];
   sp <= sp + 4;
end else if (dbus_rd_req_c3 & (dbus_rdwr_add_c3 == 0) ) begin 
   if (sp > 0)
      sp <= sp - 4;
end else if (dbus_wr_en_c3) begin   
   if (dbus_rdwr_add_c3[1:0] == 0) begin
      if (1'b1)            data8[dbus_rdwr_add_c3[19:0]]    <= dbus_wr_data_c3[ 7: 0];
      if (dbus_rdwr_width != 0) data8[dbus_rdwr_add_c3[19:0] + 1] <= dbus_wr_data_c3[15: 8];
      if (dbus_rdwr_width[1])   data8[dbus_rdwr_add_c3[19:0] + 2] <= dbus_wr_data_c3[23:16];
      if (dbus_rdwr_width[1])   data8[dbus_rdwr_add_c3[19:0] + 3] <= dbus_wr_data_c3[31:24];
   end
   if (dbus_rdwr_add_c3[1:0] == 1) begin
      if (1'b1)            data8[dbus_rdwr_add_c3[19:0]] <= dbus_wr_data_c3[ 7: 0];
      if (dbus_rdwr_width != 0) data8[dbus_rdwr_add_c3[19:0] + 1] <= dbus_wr_data_c3[15: 8];
      if (dbus_rdwr_width[1])   data8[dbus_rdwr_add_c3[19:0] + 2] <= dbus_wr_data_c3[23:16];
      if (dbus_rdwr_width[1])   data8[dbus_rdwr_add_c3[19:0] + 3] <= dbus_wr_data_c3[31:24];
   end
   if (dbus_rdwr_add_c3[1:0] == 2) begin
      if (1'b1)            data8[dbus_rdwr_add_c3[19:0]] <= dbus_wr_data_c3[ 7: 0];
      if (dbus_rdwr_width != 0) data8[dbus_rdwr_add_c3[19:0] + 1] <= dbus_wr_data_c3[15: 8];
      if (dbus_rdwr_width[1])   data8[dbus_rdwr_add_c3[19:0] + 2] <= dbus_wr_data_c3[23:16];
      if (dbus_rdwr_width[1])   data8[dbus_rdwr_add_c3[19:0] + 3] <= dbus_wr_data_c3[31:24];
   end
   if (dbus_rdwr_add_c3[1:0] == 3) begin
      if (1'b1)            data8[dbus_rdwr_add_c3[19:0]] <= dbus_wr_data_c3[ 7: 0];
      if (dbus_rdwr_width != 0) data8[dbus_rdwr_add_c3[19:0] + 1] <= dbus_wr_data_c3[15: 8];
      if (dbus_rdwr_width[1])   data8[dbus_rdwr_add_c3[19:0] + 2] <= dbus_wr_data_c3[23:16];
      if (dbus_rdwr_width[1])   data8[dbus_rdwr_add_c3[19:0] + 3] <= dbus_wr_data_c3[31:24];
   end
end

assign dbus_rd_data = (dbus_rdwr_add_c3 == 0) ? {data8[sp + 3], data8[sp + 2], data8[sp + 1], data8[sp]} :
                                                {data8[dbus_rdwr_add_c3[19:0] + 3], data8[dbus_rdwr_add_c3[19:0] + 2], data8[dbus_rdwr_add_c3[19:0] + 1], data8[dbus_rdwr_add_c3[19:0]]};

integer stalled_cnt = 0;
integer execute_cnt = 0;

always @ (posedge clk)
   if ((rstn == 1'b1) & 
      (go == 1'b1) &
      (thread_pipe_valid == 1) &
      (i >= 10)) begin
         execute_cnt <= execute_cnt + 1;
         if (!(duti.state_s2 == 0) | (duti.stalled) | (duti.pvalid < 2)) begin
            stalled_cnt <= stalled_cnt + 1;
         end
   end

always @ (posedge clk)
if(thread_pipe_valid == 1)
if (dbus_wr_en_c3 & (dbus_rdwr_add_c3 == 32'h1001200c)) begin
   if (dbus_wr_data_c3 == 32'h00400000) begin
     if (passcnt == 1) begin
        $display("Test passed.");
        go = 1'b0;
        $display("cycles: %d stalled: %d ratio (decimal): %d", execute_cnt, stalled_cnt, execute_cnt / stalled_cnt); 
        $display("==============================================================================");
     end
     passcnt <= passcnt + 1;
   end else if (dbus_wr_data_c3 == 32'h00080000) begin
     $display("RV32IMAC test failed. ", i, "\n");
     $finish;
   end
end



reg [31:0] dbus_rdwr_add_c2;
reg [31:0] dbus_wr_data_c2;
reg dbus_rd_req_c2;

always @ (posedge clk or negedge rstn)
if (~rstn) begin
   dbus_rdwr_add_c2 <= 0;   
   dbus_wr_data_c2 <= 0;   
   dbus_rd_req_c2 <= 0;
end else begin         
   dbus_rdwr_add_c2 <= dbus_rdwr_add;
   dbus_wr_data_c2 <= dbus_wr_data;
   dbus_rd_req_c2 <= dbus_rd_req;
end

reg [31:0] dbus_rdwr_add_c3;
reg [31:0] dbus_wr_data_c3;
reg dbus_rd_req_c3;
reg dbus_wr_en_c3;

always @ (posedge clk or negedge rstn)
if (~rstn) begin
   dbus_rdwr_add_c3 <= 0;
   dbus_wr_data_c3 <= 0;
   dbus_rd_req_c3 <= 0;
   dbus_wr_en_c3 <= 0;
end else begin
   dbus_rdwr_add_c3 <= dbus_rdwr_add_c2;
   dbus_wr_data_c3 <= dbus_wr_data_c2;
   dbus_rd_req_c3 <= dbus_rd_req_c2;
   dbus_wr_en_c3 <= dbus_wr_en;
end

endmodule
