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

#include <CCLang/Forward.hh>

namespace ReverseIteratorSupport {

template<typename TContainer>
struct Wrapper {
    TContainer& m_container;
};

template<typename TContainer>
auto begin(Wrapper<TContainer> wrapper) -> decltype(wrapper.m_container.rbegin()) {
    return wrapper.m_container.rbegin();
}

template<typename TContainer>
auto end(Wrapper<TContainer> wrapper) -> decltype(wrapper.m_container.rend()) {
    return wrapper.m_container.rend();
}

template<typename TContainer>
auto in_reverse(TContainer& container) -> Wrapper<TContainer> {
    return { container };
}

} /* namespace ReverseIteratorSupport */
