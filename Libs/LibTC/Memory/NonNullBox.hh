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

#include <Api/Common.h>
#include <LibTC/DenyCopy.hh>
#include <LibTC/Functional/ErrorOr.hh>

namespace TC {
namespace Memory {

template<typename T>
class NonNullBox {
    TC_DENY_COPY(NonNullBox);

public:
    enum AdoptTag { Adopt };

public:
    /**
     * @brief Constructors
     */
    NonNullBox() = delete;
    NonNullBox(AdoptTag, T& ref);
    NonNullBox(AdoptTag, T const& ref);
    NonNullBox(NonNullBox&& rhs) noexcept;
    ~NonNullBox();

    NonNullBox& operator=(NonNullBox&& rhs) noexcept;

    /**
     * @brief Swaps this box with another
     */
    void swap(NonNullBox& rhs) noexcept;

    /**
     * @brief Access operators
     */
    A_RETURN_NONNULL T*       operator->();
    A_RETURN_NONNULL T const* operator->() const;

    T&       operator*();
    T const& operator*() const;

    /**
     * @brief Conversion operators
     */
    A_RETURN_NONNULL explicit operator T*();
    A_RETURN_NONNULL explicit operator T const*() const;

    explicit operator T&();
    explicit operator T const&() const;

    /**
     * @brief Getters
     */
    [[nodiscard]] A_RETURN_NONNULL T*       as_ptr();
    [[nodiscard]] A_RETURN_NONNULL T const* as_ptr() const;

    [[nodiscard]] T&       as_ref();
    [[nodiscard]] T const& as_ref() const;

private:
    T* m_boxed_ptr{ nullptr };
};

template<typename T, typename... Args>
inline NonNullBox<T> make_box(Args&&... args);

template<typename T, typename... Args>
inline Functional::ErrorOr<NonNullBox<T>> try_make_box(Args&&... args);

} /* namespace Memory */

using Memory::make_box;
using Memory::NonNullBox;
using Memory::try_make_box;

} /* namespace TC */

#include <LibTC/Memory/NonNullBox.hhi>