/**
 * @brief
 * This file is part of the MeetiX Operating System.
 * Copyright (c) 2017-2021, Marco Cicognani (marco.cicognani@meetixos.org)
 *
 * @developers
 * Marco Cicognani (marco.cicognani@meetixos.org)
 *
 * @license
 * GNU General Public License version 3
 */

#pragma once

#define INTEL_MAGIC_CODE 0x756e6547

/**
 * @brief Inspects the INTEL architecture and prints on stdout many information
 */
int inspect_intel(int argc, char** argv);