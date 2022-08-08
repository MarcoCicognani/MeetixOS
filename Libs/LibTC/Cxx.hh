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

#include <initializer_list>
#include <new>
#include <utility>

namespace TC {
namespace Cxx {

using std::nullptr_t;

using std::nothrow;

using std::declval;
using std::exchange;
using std::forward;
using std::move;
using std::swap;

using std::initializer_list;

} /* namespace Cxx */

using Cxx::nullptr_t;

using Cxx::nothrow;

using Cxx::declval;
using Cxx::exchange;
using Cxx::forward;
using Cxx::move;
using Cxx::swap;

using Cxx::initializer_list;

} /* namespace TC */