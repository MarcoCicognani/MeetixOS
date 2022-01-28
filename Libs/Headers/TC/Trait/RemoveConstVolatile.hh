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

#include <TC/Trait/RemoveConst.hh>
#include <TC/Trait/RemoveVolatile.hh>

namespace TC::Trait {

template<typename T>
using RemoveConstVolatile = RemoveVolatile<RemoveConst<T>>;

} /* namespace TC::Trait */