/**
 * Part of the Exeme Project, under the MIT license. See '/LICENSE' for
 * license information. SPDX-License-Identifier: MIT License.
 */

#pragma once

#include "../includes.cpp"

#include "../utils/panic.cpp"
#include "./devices.cpp"

class DeviceWatcher {
  private:
	std::thread watcherThread;

	[[noreturn]] static void watcher() {
		while (true) {
			std::cout << "Devices\n";

			auto devices = DeviceWatcher::getDevices();

			for (auto device : devices->inputDevices) {
				if (device->isKeyboard()) {
					std::cout << device->getDeviceName()
							  << '\n'; // TODO: Use device ID or smth to
									   // determine duplicates.
				}
			}

			std::cout << "\033[2J\033[1;1H";

			delete devices;
		}
	}

  public:
	void start() {
		if (this->watcherThread.joinable()) {
			panic("DeviceWatcher already started");
		}

		this->watcherThread = std::thread(DeviceWatcher::watcher);
		this->watcherThread.join();
	}

	static InputDevices *getDevices() {
		UINT devicesNumber;
		PRAWINPUTDEVICELIST devices;

		while (true) {
			if (GetRawInputDeviceList(nullptr, &devicesNumber,
									  sizeof(RAWINPUTDEVICELIST)) != 0) {
				panic("failed to get number of devices connected to host");
			}

			if (devicesNumber != 0) {
				devices = new RAWINPUTDEVICELIST[devicesNumber];

				if (GetRawInputDeviceList(devices, &devicesNumber,
										  sizeof(RAWINPUTDEVICELIST)) ==
					static_cast<UINT>(-1)) {
					if (GetLastError() != ERROR_INSUFFICIENT_BUFFER) {
						panic(
							"failed to get list of devices connected to host");
					}

					// Devices were added.
					free(devices);
					continue;
				}

				break;
			} else {
				devices = new RAWINPUTDEVICELIST[0];
			}
		}

		return new InputDevices(devices, devicesNumber);
	}
};
