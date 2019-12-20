//
// Copyright 2019 Tobias Strauch, Munich, Bavaria, Germany
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//

#ifndef DRIVER_I2C_H
#define DRIVER_I2C_H

#define I2C_baud_10k 4500
#define I2C_baud_50k 900
#define I2C_baud_100k 450

int i2c_master_state [i2c_master_vp];
unsigned i2c_master_delay [i2c_master_vp];
unsigned i2c_master_scl_bit [i2c_master_vp];
unsigned i2c_master_scl_clr_add [i2c_master_vp];
unsigned i2c_master_scl_set_add [i2c_master_vp];
unsigned i2c_master_sda_bit [i2c_master_vp];
unsigned i2c_master_sda_clr_add [i2c_master_vp];
unsigned i2c_master_sda_set_add [i2c_master_vp];
unsigned i2c_master_hash [i2c_master_vp];
unsigned i2c_master_write_data [i2c_master_vp];
unsigned i2c_master_read_data [i2c_master_vp];
unsigned i2c_master_trigger [i2c_master_vp];
unsigned i2c_master_sda_bank [i2c_master_vp];

unsigned i2c_slave_state [i2c_slave_vp];
unsigned i2c_slave_scl_bank [i2c_slave_vp];
unsigned i2c_slave_scl_bit [i2c_slave_vp];
unsigned i2c_slave_sda_bank [i2c_slave_vp];
unsigned i2c_slave_sda_bit [i2c_slave_vp];
unsigned i2c_slave_add [i2c_slave_vp];
unsigned i2c_slave_write_data [i2c_slave_vp];
unsigned i2c_slave_read_data [i2c_slave_vp];
unsigned i2c_slave_hash [i2c_slave_vp];

void i2c_master_init(unsigned id, unsigned scl_pin, unsigned sda_pin, unsigned delay);
void i2c_master_write(unsigned tag, unsigned add, unsigned data);
void i2c_master_read(unsigned tag, unsigned add);
void i2c_master_event(unsigned tag, int start_time);
unsigned i2c_master_final(unsigned tag, int start_time);

void i2c_slave_init(unsigned id, unsigned scl_pin, unsigned sda_pin, unsigned add);
unsigned i2c_slave_enable(unsigned tag, unsigned data);
unsigned i2c_slave_event(unsigned tag, int start_time);
unsigned i2c_slave_final(unsigned tag, int start_time);

unsigned program_state;

#endif