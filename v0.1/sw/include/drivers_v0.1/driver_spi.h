//
// Copyright 2019 Tobias Strauch, Munich, Bavaria, Germany
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//

#ifndef DRIVER_SPI_H
#define DRIVER_SPI_H

#define SPI_baud_25k 1800
#define SPI_baud_50k  900
#define SPI_CPOL_1_CPHA_1 3

unsigned spi_master_slave_pin [spi_master_slave_selects];

int spi_master_state [spi_master_vp];
unsigned spi_master_mode [spi_master_vp];
unsigned spi_master_delay [spi_master_vp];
unsigned spi_master_sclk_bit [spi_master_vp];
unsigned spi_master_sclk_trail_add [spi_master_vp];
unsigned spi_master_sclk_lead_add [spi_master_vp];
unsigned spi_master_mosi_bit [spi_master_vp];
unsigned spi_master_mosi_clr_add [spi_master_vp];
unsigned spi_master_mosi_set_add [spi_master_vp];
unsigned spi_master_miso_bank [spi_master_vp];
unsigned spi_master_miso_bit [spi_master_vp];
unsigned spi_master_write_data [spi_master_vp];
unsigned spi_master_read_data [spi_master_vp];
unsigned spi_master_trigger [spi_master_vp];
unsigned spi_master_hash [spi_master_vp];

unsigned spi_slave_mode [spi_slave_vp];
unsigned spi_slave_ss_bank [spi_slave_vp];
unsigned spi_slave_ss_bit [spi_slave_vp];
unsigned spi_slave_sclk_bank [spi_slave_vp];
unsigned spi_slave_sclk_bit [spi_slave_vp];
unsigned spi_slave_mosi_bank [spi_slave_vp];
unsigned spi_slave_mosi_bit [spi_slave_vp];
unsigned spi_slave_miso_clr_add [spi_slave_vp];
unsigned spi_slave_miso_set_add [spi_slave_vp];
unsigned spi_slave_miso_bit [spi_slave_vp];
unsigned spi_slave_state [spi_slave_vp];
unsigned spi_slave_read_data [spi_slave_vp];
unsigned spi_slave_hash [spi_slave_vp];


void spi_master_init(unsigned id, char sclk_pin, char mosi_pin, char miso_pin, char mode, unsigned delay);
void spi_master_ss_init(unsigned tag, char ss_pin);
void spi_master_write(unsigned tag, char ss_pin, unsigned data);
unsigned spi_master_final(unsigned tag, int start_time);

void spi_slave_init(unsigned id, char ss_pin, char sclk_pin, char mosi_pin, char miso_pin, char mode);
unsigned spi_slave_enable(unsigned tag, unsigned data);
unsigned spi_slave_event(unsigned tag, int start_time);
unsigned spi_slave_final(unsigned tag, int start_time);

#endif
