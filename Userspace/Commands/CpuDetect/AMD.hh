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

#define AMD_MAGIC_CODE 0x68747541

/**
 * @brief Inspects the AMD architecture and prints on stdout many information
 */
int inspect_amd(int argc, char** argv);