//
// Copyright 2019 Tobias Strauch, Munich, Bavaria
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//

#ifndef DRIVER_GPIO_H
#define DRIVER_GPIO_H

unsigned gpio_event_hash [9*8];

void set_pin (unsigned pin, unsigned level);
void set_dir (unsigned pin, unsigned dir);
void gpio_event(unsigned tag, int start_time);

#endif