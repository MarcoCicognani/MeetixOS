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

enum class None : bool { __Value };

/**
 * @brief The object must be empty initialized
 */
constexpr auto C_NONE = None::__Value;

} /* namespace TC::Tags */