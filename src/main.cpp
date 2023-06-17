/**
 * Part of the Exeme Project, under the MIT license. See '/LICENSE' for
 * license information. SPDX-License-Identifier: MIT License.
 */

#include "./includes.cpp"

#include "./win_api/interface.cpp"

int main() {
	auto *watcher = new DeviceWatcher();

	watcher->start();
}
