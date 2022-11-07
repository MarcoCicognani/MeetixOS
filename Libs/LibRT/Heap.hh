/**
 * @brief
 * This file is part of the MeetiX Operating System.
 * Copyright (c) 2017-2022, Marco Cicognani (marco.cicognani@meetixos.org)
 *
 * @developers
 * Marco Cicognani (marco.cicognani@meetixos.org)
 *
 * @license
 * GNU General Public License version 3
 */

#pragma once

#include <LibTC/Alloc/AllocLayout.hh>
#include <LibTC/Core/ErrorOr.hh>

namespace Heap {

enum class Clean : bool {
    Yes,
    No
};

auto alloc(AllocLayout, Clean) -> ErrorOr<void*>;
auto dealloc(AllocLayout, void* ptr) -> ErrorOr<void>;

} /* namespace Heap */
