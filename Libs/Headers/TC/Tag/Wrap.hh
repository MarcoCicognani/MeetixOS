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

namespace TC::Tag {

enum class Wrap : bool { Value };

/**
 * @brief The object must wrap the memory without copying it nor taking the ownership
 */
constexpr auto C_WRAP = Wrap::Value;

} /* namespace TC::Tag */