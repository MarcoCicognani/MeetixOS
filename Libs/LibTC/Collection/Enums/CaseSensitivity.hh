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

namespace TC {
namespace Collection {
namespace Enums {

enum class CaseSensitivity {
    Sensitive,
    Insensitive
};

} /* namespace Enums */

using Enums::CaseSensitivity;

} /* namespace Collection */

using Collection::CaseSensitivity;

} /* namespace TC */