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

#include <CCLang/Core/Meta.hh>
#include <CCLang/Core/Order.hh>

namespace std {

struct nothrow_t {
    explicit nothrow_t() = default;
};

enum class align_val_t : __SIZE_TYPE__ {
};

template<class T>
class initializer_list {
public:
    constexpr initializer_list() = default;

    /**
     * @brief Getters
     */
    constexpr __SIZE_TYPE__ size() const {
        return m_len;
    }
    constexpr T const* begin() const {
        return m_array;
    }
    constexpr T const* end() const {
        return begin() + size();
    }

private:
    constexpr explicit initializer_list(T const* array_begin, __SIZE_TYPE__ len)
        : m_array(array_begin)
        , m_len(len) {
    }

private:
    T const*      m_array = nullptr;
    __SIZE_TYPE__ m_len   = 0;
};

extern const nothrow_t nothrow;

using nullptr_t = decltype(nullptr);

template<typename T>
constexpr auto forward(RemoveReference<T>& param) noexcept -> T&& {
    return static_cast<T&&>(param);
}
template<typename T>
constexpr auto forward(RemoveReference<T>&& param) noexcept -> T&& {
    static_assert(!is_lvalue<T>, "Can't forward an rvalue as an lvalue.");
    return static_cast<T&&>(param);
}

template<typename T>
constexpr auto move(T&& arg) noexcept -> RemoveReference<T>&& {
    return static_cast<RemoveReference<T>&&>(arg);
}

template<typename T, typename U = T>
constexpr void swap(T& lhs, U& rhs) {
    U __tmp = std::move(lhs);
    lhs     = std::move(rhs);
    rhs     = std::move(__tmp);
}

template<typename T, typename U = T>
constexpr T exchange(T& slot, U&& value) {
    T __prev = std::move(slot);
    slot     = std::forward<U>(value);
    return __prev;
}

template<typename T>
auto declval() -> T;

} /* namespace std */

namespace Cxx {

using std::declval;
using std::exchange;
using std::forward;
using std::move;
using std::swap;

template<typename T>
using InitializerList = std::initializer_list<T>;

template<typename T, typename U>
[[nodiscard]]
constexpr auto bit_cast(U const& from) -> T {
    return __builtin_bit_cast(T, from);
}

auto memcpy(void*, void const*, usize) -> void*;
auto memmove(void*, void const*, usize) -> void*;
auto memset(void*, int, usize) -> void*;
auto memcmp(void const*, void const*, usize) -> Order;
auto strlen(char const*) -> usize;

} /* namespace Cxx */

using std::align_val_t;
using std::nothrow_t;
using std::nullptr_t;

using std::nothrow;

constexpr void* operator new(__SIZE_TYPE__, void* ptr) {
    return ptr;
}

constexpr void* operator new[](__SIZE_TYPE__, void* ptr) {
    return ptr;
}
