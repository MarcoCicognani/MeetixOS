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

#include <LibTC/Core/Concept.hh>

template<typename T, typename TIterator>
auto all_of(TIterator begin, TIterator end, Predicate<T const&> auto predicate) -> bool {
    for ( auto it = begin; it != end; ++it ) {
        if ( !predicate(*it) )
            return false;
    }
    return true;
}
