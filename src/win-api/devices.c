/**
 * Part of the Exeme Project, under the MIT license. See '/LICENSE' for
 * license information. SPDX-License-Identifier: MIT License.
 */

#pragma once

#include "../includes.c"

#include "../utils/array.c"
#include "../utils/panic.c"
#include "../utils/string.c"

#define RAWINPUTDEVICELIST_STRUCT_SIZE sizeof(struct tagRAWINPUTDEVICELIST)
#define RID_DEVICE_INFO_STRUCT_SIZE sizeof(struct tagRID_DEVICE_INFO)

/**
 * Represents an input device.
 */
struct InputDevice {
	struct tagRID_DEVICE_INFO *deviceInfo;
	struct String *name;
	struct tagRAWINPUTDEVICELIST *device;
};

#define INPUTDEVICE_STRUCT_SIZE sizeof(struct InputDevice)

/**
 * Gets the input device's name.
 *
 * @param self The current InputDevice struct.
 */
void inputDevice_getName(struct InputDevice *self) {
	if (!self->name) {
		char *name = NULL;
		UINT nameLength = 0;

		if (GetRawInputDeviceInfo(self->device->hDevice, RIDI_DEVICENAME, NULL,
								  &nameLength) == (UINT)-1) {
			panic("failed to get length of device name");
		}

		if (!(name = malloc(nameLength + 1))) {
			panic("failed to malloc device name string");
		}

		if (GetRawInputDeviceInfo(self->device->hDevice, RIDI_DEVICENAME, name,
								  &nameLength) == (UINT)-1) {
			panic("failed to get device name");
		}

		self->name = string_new(name, false);
	}
}

void inputDevice_getInfoStruct(struct InputDevice *self) {
	if (!self->deviceInfo) {
		UINT infoSize = 0;

		if (GetRawInputDeviceInfo(self->device->hDevice, RIDI_DEVICEINFO, NULL,
								  &infoSize) == (UINT)-1) {
			panic("failed to get size of RID_DEVICE_INFO struct");
		}

		if (!(self->deviceInfo = malloc(infoSize))) {
			panic("failed to malloc RID_DEVICE_INFO struct");
		}

		if (GetRawInputDeviceInfo(self->device->hDevice, RIDI_DEVICEINFO,
								  self->deviceInfo, &infoSize) == (UINT)-1) {
			panic("failed to get RID_DEVICE_INFO struct");
		}
	}
}

/**
 * Gets the input device's info.
 *
 * @param self The current InputDevice struct.
 */
void inputDevice_getInfo(struct InputDevice *self) {
	inputDevice_getName(self);
	inputDevice_getInfoStruct(self);
}

/**
 * Creates a new InputDevice struct.
 *
 * @return The created InputDevice struct.
 */
struct InputDevice *inputDevice_new(struct tagRAWINPUTDEVICELIST device) {
	struct InputDevice *self = malloc(INPUTDEVICE_STRUCT_SIZE);

	if (!self) {
		panic("failed to malloc InputDevice struct");
	}

	self->deviceInfo = NULL;
	self->name = NULL;

	self->device = malloc(RAWINPUTDEVICELIST_STRUCT_SIZE);

	if (!self->device) {
		panic("failed to malloc RAWINPUTDEVICELIST struct");
	}

	memcpy(self->device, &device, RAWINPUTDEVICELIST_STRUCT_SIZE);

	inputDevice_getInfo(self);

	return self;
}

/**
 * Frees an InputDevice struct.
 *
 * @param self The current InputDevice struct.
 */
void inputDevice_free(struct InputDevice **self) {
	if (self) {
		free((*self)->deviceInfo);
		string_free(&(*self)->name);
		free((*self)->device);

		free(*self);
		*self = NULL;
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
struct InputDevices *inputDevices_new(void) {
	struct InputDevices *self = malloc(INPUTDEVICES_STRUCT_SIZE);

	if (!self) {
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
void inputDevices_getDevices(struct InputDevices *self) {
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
			if (!(inputDevices =
					  malloc(sizeof(RAWINPUTDEVICELIST) * deviceNumber))) {
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

	array_clear(self->inputDevices, (void (*)(const void *))inputDevice_free);

	for (size_t i = 0; i < deviceNumber; i++) {
		array_insert(self->inputDevices, self->inputDevices->length,
					 inputDevice_new(inputDevices[i]));
	}

	free(inputDevices);
}

/**
 * Frees an InputDevices struct.
 *
 * @param self The current InputDevices struct.
 */
void inputDevices_free(struct InputDevices **self) {
	struct InputDevice *arrayItem;

	if (self) {
		for (size_t i = 0; i < (*self)->inputDevices->length; i++) {
			arrayItem =
				(struct InputDevice *)array_get((*self)->inputDevices, i);

			inputDevice_free(&arrayItem);
		}

		array_free(&(*self)->inputDevices);

		free(*self);
		*self = NULL;
	} else {
		panic("InputDevices struct has already been freed");
	}
}
