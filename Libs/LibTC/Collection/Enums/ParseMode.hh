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

enum class ParseMode {
    Begin,
    TrimWhitesAndBegin,
    BeginToEnd,
    TrimWhitesAndBeginToEnd,
};

} /* namespace Enums */

using Enums::ParseMode;

} /* namespace Collection */

using Collection::ParseMode;

} /* namespace TC */
