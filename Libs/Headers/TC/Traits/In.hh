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

#include <TC/Traits/IsSame.hh>

namespace TC::Traits {

template<typename T, typename... Ts>
concept In = (IsSame<T, Ts> || ...);

} /* namespace TC::Traits */