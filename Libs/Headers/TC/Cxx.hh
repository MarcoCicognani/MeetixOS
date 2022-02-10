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

#include <new>
#include <utility>

namespace TC {
namespace Cxx {

using std::nullptr_t;

using std::nothrow;

using std::exchange;
using std::forward;
using std::move;
using std::swap;

} /* namespace Cxx */

using Cxx::nullptr_t;

using Cxx::nothrow;

using Cxx::exchange;
using Cxx::forward;
using Cxx::move;
using Cxx::swap;

} /* namespace TC */