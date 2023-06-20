/**
 * Part of the Exeme Project, under the MIT license. See '/LICENSE' for
 * license information. SPDX-License-Identifier: MIT License.
 */

#pragma once

#include "../includes.c"
#include <winuser.h>

struct Device {
	const RAWINPUTDEVICE *device
};

#define DEVICE_STRUCT_SIZE sizeof(struct Device)
