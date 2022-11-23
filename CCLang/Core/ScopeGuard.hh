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

#include <CCLang/Core/Concept.hh>
#include <CCLang/Lang/Cxx.hh>
#include <CCLang/Lang/DenyCopy.hh>
#include <CCLang/Lang/DenyMove.hh>

template<CallBack TCallback>
class ScopeGuard : public DenyCopy,
                   public DenyMove {
public:
    explicit constexpr ScopeGuard(TCallback callback)
        : m_callback{ Cxx::move(callback) } {
    }
    ~ScopeGuard() {
        if ( m_is_armed )
            m_callback();
    }

    void disarm() {
        m_is_armed = false;
    }

private:
    TCallback m_callback;
    bool      m_is_armed{ true };
};