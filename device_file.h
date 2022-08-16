//
// Copyright © 2022 Bob Aaldering
//

#ifndef DEVICE_FILE_H
#define DEVICE_FILE_H

#include <linux/compiler.h> // Include the 'linux/compiler' header. It contains compiler information..

__must_check int register_device(void); // This function registers a device.
void unregister_device(void); // This function de-initializes a device.

#endif
