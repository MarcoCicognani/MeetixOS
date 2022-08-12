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

enum class KeepStorageCapacity {
    Yes,
    No
};

} /* namespace Enums */

using Enums::KeepStorageCapacity;

} /* namespace Collection */

using Collection::KeepStorageCapacity;

} /* namespace TC */
