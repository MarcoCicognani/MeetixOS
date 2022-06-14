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

enum class InsertResult {
    InsertedNew,
    ReplacedExisting,
    KeptExisting
};

} /* namespace Enums */

using Enums::InsertResult;

} /* namespace Collection */

using Collection::InsertResult;

} /* namespace TC */
