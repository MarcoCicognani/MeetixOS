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

#include <LibTC/Lang/IntTypes.hh>

class AllocLayout {
public:
    /**
     * @brief Error-safe Factory functions
     */
    template<typename T>
    [[nodiscard]]
    static constexpr auto new_for_type() -> AllocLayout {
        return new_from_raw_parts(sizeof(T), alignof(T));
    }
    template<typename T>
    [[nodiscard]]
    static constexpr auto new_for_type(usize custom_len) -> AllocLayout {
        return new_from_raw_parts(custom_len, alignof(T));
    }
    template<typename T>
    [[nodiscard]]
    static constexpr auto new_for_array_of(usize len) -> AllocLayout {
        return new_from_raw_parts(sizeof(T) * len, alignof(T));
    }
    [[nodiscard]]
    static constexpr auto new_from_raw_parts(usize size, usize alignment) -> AllocLayout {
        return AllocLayout{ size, alignment };
    }

    /**
     * @brief Getters
     */
    auto size() const -> usize {
        return m_size;
    }
    auto alignment() const -> usize {
        return m_alignment;
    }

private:
    explicit constexpr AllocLayout(usize size, usize alignment)
        : m_size{ size }
        , m_alignment{ alignment } {
    }

private:
    usize m_size;
    usize m_alignment;
};
