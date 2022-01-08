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

namespace TC::Traits {

template<typename... Ts>
struct First {
    /* Empty Body */
};

template<typename TFirst, typename... Ts>
struct First<TFirst, Ts...> {
    using Type = TFirst;
};

} /* namespace TC::Traits */