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
#include <TC/Assertions.hh>
#include <TC/Cxx.hh>
#include <TC/Functional/ErrorOr.hh>

namespace TC {
namespace Memory {

template<typename T>
class NonNullRef {
public:
    enum AdoptTag { Adopt };

public:
    /**
     * @brief Constructors
     */
    NonNullRef() = delete;
    NonNullRef(AdoptTag, T& ref);
    NonNullRef(AdoptTag, T const& ref);
    NonNullRef(NonNullRef const& rhs);
    NonNullRef(NonNullRef&& rhs) noexcept;
    ~NonNullRef();

    NonNullRef& operator=(T& ref);
    NonNullRef& operator=(T const& ref);
    NonNullRef& operator=(NonNullRef const& rhs);
    NonNullRef& operator=(NonNullRef&& rhs) noexcept;

    /**
     * @brief Swaps this ref with another
     */
    void swap(NonNullRef& rhs) noexcept;

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
    T* m_shared_ptr{ nullptr };
};

template<typename T, typename... Args>
inline NonNullRef<T> make_ref(Args&&... args);

template<typename T, typename... Args>
inline ErrorOr<NonNullRef<T>> try_make_ref(Args&&... args);

} /* namespace Memory */

using Memory::make_ref;
using Memory::NonNullRef;
using Memory::try_make_ref;

} /* namespace TC */

#include <../LibTC/Memory/NonNullRef.hhi>