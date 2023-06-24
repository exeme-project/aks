/**
 * Part of the Exeme Project, under the MIT license. See '/LICENSE' for
 * license information. SPDX-License-Identifier: MIT License.
 */

#pragma once

#include "../includes.c"

#include "../utils/array.c"
#include "../utils/panic.c"
#include <winuser.h>

/**
 * Represents an input device.
 */
struct InputDevice {
	const struct String *NAME;
};

#define INPUTDEVICE_STRUCT_SIZE sizeof(struct InputDevice)

/*
 * Creates a new InputDevice struct.
 *
 * @return The created InputDevice struct.
 */
struct InputDevice *inputDevice_create(struct tagRAWINPUTDEVICELIST device) {
	struct InputDevice *self = malloc(INPUTDEVICE_STRUCT_SIZE);

	if (self == NULL) {
		panic("failed to malloc InputDevice struct");
	}

	return self;
}

/**
 * Frees an InputDevice struct.
 *
 * @param self The current InputDevice struct.
 */
void inputDevice_free(struct InputDevice *self) {
	if (self) {
		free(self);
		self = NULL;
	} else {
		panic("InputDevice struct has already been freed");
	}
}

/**
 * Represents a list of input devices.
 */
struct InputDevices {
	struct Array *inputDevices;
};

#define INPUTDEVICES_STRUCT_SIZE sizeof(struct InputDevices)

/*
 * Creates a new InputDevice struct.
 *
 * @return The created InputDevice struct.
 */
struct InputDevices *inputDevices_create(void) {
	struct InputDevices *self = malloc(INPUTDEVICES_STRUCT_SIZE);

	if (self == NULL) {
		panic("failed to malloc Devices struct");
	}

	self->inputDevices = array_new();

	return self;
}

/**
 * Gets the input devices.
 *
 * @param self The current InputDevices struct.
 */
void inputDevices_get(struct InputDevices *self) {
	UINT deviceNumber;
	PRAWINPUTDEVICELIST inputDevices = NULL;

	while (true) {
		if (GetRawInputDeviceList(NULL, &deviceNumber,
								  sizeof(RAWINPUTDEVICELIST)) != 0) {
			panic("failed to get number of devices connected to host");
		}

		if (deviceNumber == 0) {
			panic("no devices connected to host");
		} else {
			if ((inputDevices = malloc(sizeof(RAWINPUTDEVICELIST) *
									   deviceNumber)) == NULL) {
				panic("failed to malloc devices array");
			}

			if (GetRawInputDeviceList(inputDevices, &deviceNumber,
									  sizeof(RAWINPUTDEVICELIST)) == (UINT)-1) {
				if (GetLastError() != ERROR_INSUFFICIENT_BUFFER) {
					panic("failed to get devices connected to host");
				}

				// Devices were added while we were allocating the array.
				free(inputDevices);
				continue;
			}

			break;
		}
	}

	array_clear(self->inputDevices, inputDevice_free);

	for (size_t i = 0; i < deviceNumber; i++) {
		array_insert(self->inputDevices, self->inputDevices->length,
					 inputDevice_create(inputDevices[i]));
	}

	free(inputDevices);
}

/**
 * Frees an InputDevices struct.
 *
 * @param self The current InputDevices struct.
 */
void inputDevices_free(struct InputDevices *self) {
	if (self) {
		for (size_t i = 0; i < self->inputDevices->length; i++) {
			inputDevice_free(
				(struct InputDevice *)array_get(self->inputDevices, i));
		}

		array_free(self->inputDevices);

		free(self);
		self = NULL;
	} else {
		panic("InputDevices struct has already been freed");
	}
}
