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

enum class TrimMode {
    Left,
    Right,
    Both
};

} /* namespace Enums */

using Enums::TrimMode;

} /* namespace Collection */

using Collection::TrimMode;

} /* namespace TC */