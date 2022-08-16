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

#include <LibTC/Forward.hh>

namespace TC {
namespace Collection {
namespace Enums {

enum class IntBase {
    Binary  = 2,
    Octal   = 8,
    Decimal = 10,
    Hex     = 16
};

} /* namespace Enums */

using Enums::IntBase;

} /* namespace Collection */

using Collection::IntBase;

} /* namespace TC */
