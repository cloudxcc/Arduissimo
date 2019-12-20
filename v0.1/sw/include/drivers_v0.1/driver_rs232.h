//
// Copyright 2019 Tobias Strauch, Munich, Bavaria, Germany
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//

#ifndef DRIVER_RS232_H
#define DRIVER_RS232_H

#define RS232_baud_9600 18750
#define RS232_baud_19200 9375
#define RS232_baud_57600 3125
#define RS232_baud_115200 1562
#define RS232_parity_non 0
#define RS232_parity_even 1
#define RS232_parity_odd 2
#define RS232_stop_1 0
#define RS232_stop_2 1

unsigned rs232_tx_conf [rs232_tx_vp];
unsigned short rs232_tx_data [rs232_tx_vp];
unsigned rs232_tx_clr_add [rs232_tx_vp];
unsigned rs232_tx_set_add [rs232_tx_vp];
unsigned rs232_tx_bit [rs232_tx_vp];
unsigned rs232_tx_delay [rs232_tx_vp];
unsigned rs232_tx_trigger [rs232_tx_vp];
unsigned rs232_tx_hash [rs232_tx_vp];

unsigned rs232_rx_conf [rs232_rx_vp];
unsigned short rs232_rx_data [rs232_rx_vp];
unsigned rs232_rx_bank [rs232_rx_vp];
unsigned rs232_rx_bit [rs232_rx_vp];
unsigned rs232_rx_delay [rs232_rx_vp];
unsigned rs232_rx_trigger [rs232_rx_vp];
unsigned rs232_rx_hash [rs232_rx_vp];

void rs232_tx_init(unsigned id, char tx_pin, char parity, char stop_bits, unsigned delay);
void rs232_tx(unsigned tag, unsigned data);
void rs232_tx_event(unsigned tag, int start_time);
unsigned rs232_tx_final(unsigned tag, int start_time);

void rs232_rx_init(unsigned id, char rx_pin, char parity, unsigned delay);
unsigned rs232_rx(unsigned tag, int start_time);
void rs232_rx_rearm(unsigned id);
unsigned rs232_rx_final(unsigned tag, int start_time);

#endif
