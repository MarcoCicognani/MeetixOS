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

#include <TC/Traits/Constant.hh>
#include <TC/Traits/IsSame.hh>

namespace TC::Traits {
namespace Details {

template<typename TQuery, typename... Ts>
struct IndexOf {
    /* Empty Body */
};

template<typename TQuery>
struct IndexOf<TQuery> : Constant<int, -1> {
    /* Empty Body */
};

template<typename TQuery, typename TFirst, typename... Ts>
struct IndexOf<TQuery, TFirst, Ts...> : Constant<int, IsSame<TQuery, TFirst> ? 0 : IndexOf<TQuery, Ts...>::value + 1> {
    /* Empty Body */
};

} /* namespace Details */

template<typename TQuery, typename... Ts>
inline constexpr int IndexOf = Details::IndexOf<TQuery, Ts...>::value;

} /* namespace TC::Traits */