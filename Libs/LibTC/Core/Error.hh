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

#include <LibC/errno.h> /* TODO Remove */

#include <LibTC/Forward.hh>

#include <LibTC/Core/SourceLocation.hh>
#include <LibTC/Lang/StringView.hh>

class Error final {
public:
    enum class FromSyscall : bool {
        Yes,
        No
    };

public:
    /**
     * @brief Error safe factory functions
     */
    [[nodiscard]]
    static auto new_from_code(u32,
                                    FromSyscall    = FromSyscall::No,
                                    SourceLocation = SourceLocation::new_from_here()) -> Error;
    [[nodiscard]]
    static auto new_from_literal(StringView,
                                       FromSyscall    = FromSyscall::No,
                                       SourceLocation = SourceLocation::new_from_here()) -> Error;

    /**
     * @brief Getters
     */
    [[nodiscard]]
    auto code() const -> u32;
    [[nodiscard]]
    auto string_literal() const -> StringView;
    [[nodiscard]]
    auto is_from_syscall() const -> FromSyscall;
    [[nodiscard]]
    auto source_location() const -> SourceLocation;

    [[nodiscard]]
    auto operator==(u32 const&) const -> bool;
    [[nodiscard]]
    auto operator==(StringView) const -> bool;

private:
    explicit Error(u32 code, StringView string_literal, FromSyscall from_syscall, SourceLocation error_location);

private:
    u32            m_code;
    StringView     m_string_literal;
    FromSyscall    m_from_syscall;
    SourceLocation m_error_location;
};
