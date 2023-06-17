/**
 * Part of the Exeme Project, under the MIT license. See '/LICENSE' for
 * license information. SPDX-License-Identifier: MIT License.
 */

#pragma once

#include "../includes.cpp"

class InputDevice {
  private:
	const RAWINPUTDEVICELIST inputDevice;
	std::string deviceName;

  public:
	auto getDeviceName() {
		if (this->deviceName.empty()) {
			auto deviceNameLength = static_cast<UINT>(0);

			if (GetRawInputDeviceInfo(inputDevice.hDevice, RIDI_DEVICENAME,
									  nullptr, &deviceNameLength) ==
				static_cast<UINT>(-1)) {
				panic("failed to get input device name length");
			}

			auto deviceNameBuffer = new char[deviceNameLength];

			if (GetRawInputDeviceInfo(inputDevice.hDevice, RIDI_DEVICENAME,
									  deviceNameBuffer, &deviceNameLength) ==
				static_cast<UINT>(-1)) {
				panic("failed to get input device name");
			}

			this->deviceName = std::string(deviceNameBuffer);
		}

		return this->deviceName;
	}

	auto getInputDevice() { return this->inputDevice; }

	bool isKeyboard() {
		if (this->deviceName.empty()) {
			this->getDeviceName();
		}

		return this->inputDevice.dwType == RIM_TYPEKEYBOARD &&
			   this->deviceName.find("Microsoft Keyboard") == std::string::npos;
	}

	explicit InputDevice(RAWINPUTDEVICELIST device) : inputDevice(device) {
		this->getDeviceName();
	}
};

class InputDevices {
  public:
	std::vector<InputDevice *> inputDevices;

	typedef std::vector<InputDevice *>::iterator iterator;
	typedef std::vector<InputDevice *>::const_iterator const_iterator;

	iterator begin() { return this->inputDevices.begin(); }

	[[nodiscard]] const_iterator begin() const {
		return this->inputDevices.begin();
	}

	iterator end() { return this->inputDevices.end(); }

	[[nodiscard]] const_iterator end() const {
		return this->inputDevices.end();
	}

	explicit InputDevices(PRAWINPUTDEVICELIST devices, UINT devicesNumber) {
		for (auto i = static_cast<UINT>(0); i < devicesNumber; i++) {
			this->inputDevices.push_back(new InputDevice(devices[i]));
		}

		free(devices);
	}
};
