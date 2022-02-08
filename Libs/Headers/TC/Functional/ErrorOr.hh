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

#include <errno.h>
#include <TC/Functional/Result.hh>

namespace TC {
namespace Functional {

template<typename T>
using ErrorOr = Result<T, OSError>;

} /* namespace Functional */

using Functional::ErrorOr;

} /* namespace TC */