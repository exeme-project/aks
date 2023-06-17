/**
 * Part of the Exeme Project, under the MIT license. See '/LICENSE' for
 * license information. SPDX-License-Identifier: MIT License.
 */

#pragma once

#include "../includes.cpp"

#include "./console.cpp"

/**
 * Prints a panic and exits.
 *
 * @param PANIC_MSG The panic message.
 */
void panic(const char *PANIC_MSG) {
	std::cerr << F_BRIGHT_RED << S_BOLD << "panic: " << S_RESET << PANIC_MSG
			  << "\n";
	exit(EXIT_FAILURE);
}
