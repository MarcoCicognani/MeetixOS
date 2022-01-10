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

namespace TC::Tags {

enum class Wrap : bool { __Value };

/**
 * @brief The object must wrap the memory without copying it nor taking the ownership
 */
constexpr auto C_WRAP = Wrap::__Value;

} /* namespace TC::Tags */