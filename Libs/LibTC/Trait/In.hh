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

#include <LibTC/Trait/IsSame.hh>

namespace TC {
namespace Trait {

template<typename T, typename... Ts>
concept In = (IsSame<T, Ts> || ...);

} /* namespace Trait */

using Trait::In;

} /* namespace TC */