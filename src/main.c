/**
 * Part of the Exeme Project, under the MIT license. See '/LICENSE' for
 * license information. SPDX-License-Identifier: MIT License.
 */

#include "./includes.c"

#include "./win-api/devices.c"

int main(void) {
	struct InputDevices *self = inputDevices_new();
	inputDevices_getDevices(self);

	for (size_t i = 0; i < self->inputDevices->length; i++) {
		struct InputDevice *inputDevice =
			(struct InputDevice *)array_get(self->inputDevices, i);

		switch (inputDevice->device->dwType) {
		case RIM_TYPEKEYBOARD:
			printf("KEYBOARD %s\n", inputDevice->name->_value);
			printf("Vendor ID: %lu\n", inputDevice->deviceInfo->hid.dwVendorId);
			printf("Product ID: %lu\n",
				   inputDevice->deviceInfo->hid.dwProductId);
			printf("Version Number: %lu\n",
				   inputDevice->deviceInfo->hid.dwVersionNumber);
			printf("Keyboard Mode: %lu\n",
				   inputDevice->deviceInfo->keyboard.dwKeyboardMode);
			printf("Number of Function Keys: %lu\n",
				   inputDevice->deviceInfo->keyboard.dwNumberOfFunctionKeys);
			printf("Number of Indicators: %lu\n",
				   inputDevice->deviceInfo->keyboard.dwNumberOfIndicators);
			printf("Number of Keys Total: %lu\n",
				   inputDevice->deviceInfo->keyboard.dwNumberOfKeysTotal);
			printf("Type of Keyboard: %lu\n",
				   inputDevice->deviceInfo->keyboard.dwType);
			printf("Sub-Type of Keyboard: %lu\n",
				   inputDevice->deviceInfo->keyboard.dwSubType);
			printf("Usage Page: %hu\n",
				   inputDevice->deviceInfo->hid.usUsagePage);
			printf("Usage: %hu\n\n", inputDevice->deviceInfo->hid.usUsage);
			break;
		case RIM_TYPEMOUSE:
			printf("MOUSE %s\n", inputDevice->name->_value);
			printf("Vendor ID: %lu\n", inputDevice->deviceInfo->hid.dwVendorId);
			printf("Product ID: %lu\n",
				   inputDevice->deviceInfo->hid.dwProductId);
			printf("Version Number: %lu\n",
				   inputDevice->deviceInfo->hid.dwVersionNumber);
			printf("Number of Buttons: %lu\n",
				   inputDevice->deviceInfo->mouse.dwNumberOfButtons);
			printf("Sample Rate: %lu\n",
				   inputDevice->deviceInfo->mouse.dwSampleRate);
			printf("Horizontal Wheel Present: %s\n",
				   inputDevice->deviceInfo->mouse.fHasHorizontalWheel
					   ? "true"
					   : "false");
			printf("Usage Page: %hu\n",
				   inputDevice->deviceInfo->hid.usUsagePage);
			printf("Usage: %hu\n\n", inputDevice->deviceInfo->hid.usUsage);
			break;
		case RIM_TYPEHID:
			printf("HID %s\n", inputDevice->name->_value);
			printf("Vendor ID: %lu\n", inputDevice->deviceInfo->hid.dwVendorId);
			printf("Product ID: %lu\n",
				   inputDevice->deviceInfo->hid.dwProductId);
			printf("Version Number: %lu\n",
				   inputDevice->deviceInfo->hid.dwVersionNumber);
			printf("Usage Page: %hu\n",
				   inputDevice->deviceInfo->hid.usUsagePage);
			printf("Usage: %hu\n\n", inputDevice->deviceInfo->hid.usUsage);
			break;
		}
	}

	inputDevices_free(&self);

	return 0;
}
