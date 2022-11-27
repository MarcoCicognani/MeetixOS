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

#include <CCLang/Lang/DenyCopy.hh>
#include <CCLang/Lang/DenyMove.hh>
#include <CCLang/Lang/IntTypes.hh>

struct RefCounted : public DenyCopy, public DenyMove {
public:
    auto add_strong_ref() const -> void;
    auto remove_strong_ref() const -> void;

    /**
     * @brief Getters
     */
    [[nodiscard]]
    auto strong_ref_count() const -> usize;

protected:
    constexpr RefCounted() = default;
    ~RefCounted();

private:
    mutable usize m_strong_ref_count = 1;
};
