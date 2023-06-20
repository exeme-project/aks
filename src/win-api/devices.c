/**
 * Part of the Exeme Project, under the MIT license. See '/LICENSE' for
 * license information. SPDX-License-Identifier: MIT License.
 */

#pragma once

#include "../includes.c"

#include "../utils/array.c"
#include "../utils/panic.c"

struct InputDevice {};

#define INPUTDEVICE_STRUCT_SIZE sizeof(struct InputDevice)

struct InputDevices {
	struct Array *devices;
};

#define INPUTDEVICES_STRUCT_SIZE sizeof(struct InputDevices)

struct InputDevices *devices_create(void) {
	struct InputDevices *self = malloc(INPUTDEVICES_STRUCT_SIZE);

	if (self == NULL) {
		panic("failed to malloc Devices struct");
	}

    self->devices = array_new();

	return self;
}
