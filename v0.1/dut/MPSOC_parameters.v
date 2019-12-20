//
// Copyright 2019 Tobias Strauch, Munich, Bavaria
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//

parameter CA_COM = 32'h80001000;
parameter CA_ET  = 32'h80001004;
parameter CA_CT  =    19'h01008;

parameter UART_SEND      = 32'h80020000;
parameter UART_TX_COM    = 32'h80020010;
parameter UART_RX_COM    = 32'h80020030;
parameter UART_REC       =    19'h20020;
parameter UART_REC_STAT  =    19'h20024;
parameter UART_SEND_STAT =    19'h20004;

parameter TC_START       = 32'h80000000;
parameter TC_KILL        = 32'h80000004;
parameter TC_SAK         = 32'h80000008;

parameter GPIO_EVENT_BANKS = 9;
parameter GPIO_OFFSET      = 32'h80030000;
parameter GPIO_BASE        = 16'h8003;
parameter GPIO_STEP        =      12'h100;
parameter GPIO_DIR_CLR     =     16'h0000;
parameter GPIO_DIR_SET     =     16'h0004;
parameter GPIO_OUT_CLR     =     16'h0010;
parameter GPIO_OUT_SET     =     16'h0014;
parameter GPIO_IN          =     16'h0020;
parameter GPIO_LVL0        =     16'h0030;
parameter GPIO_LVL1        =     16'h0034;
parameter GPIO_CAP         =     16'h0040;
parameter GPIO_EVENT_ADD   =     16'h1000;

parameter MP_OFFSET     = 32'h80040000;
parameter MP_BASE       = 16'h8004;
parameter MP_OUT_OFFSET =     16'h0000;
parameter MP_IN_OFFSET  =     16'h0010;
parameter MP_COM_OFFSET =     16'h0020;
parameter MP_STEP       =     16'h0004;

