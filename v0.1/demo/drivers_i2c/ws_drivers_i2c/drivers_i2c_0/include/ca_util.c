//
// Copyright 2019 Tobias Strauch, Munich, Bavaria
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//

#include "device.h"
#include "ca_util.h"

void ca_delay(unsigned delay) {
   unsigned end_time = CA_CT + delay;
   while (CA_CT < end_time) {}
}

