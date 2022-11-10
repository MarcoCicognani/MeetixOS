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
#include <CCLang/Lang/IntTypes.hh>

namespace std {

struct nothrow_t {
    explicit nothrow_t() = default;
};

enum class align_val_t : usize {
};

template<class T>
class initializer_list {
public:
    constexpr initializer_list()
        : m_array(0)
        , m_len(0) {
    }

    /**
     * @brief Getters
     */
    constexpr usize size() const {
        return m_len;
    }
    constexpr T const* begin() const {
        return m_array;
    }
    constexpr T const* end() const {
        return begin() + size();
    }

private:
    constexpr explicit initializer_list(T const* array_begin, usize len)
        : m_array(array_begin)
        , m_len(len) {
    }

private:
    T const* m_array;
    usize    m_len;
};

extern const nothrow_t nothrow;

using nullptr_t = decltype(nullptr);

template<typename T>
constexpr auto forward(RemoveReference<T>& param) noexcept -> T&& {
    return static_cast<T&&>(param);
}

template<typename T>
constexpr auto forward(RemoveReference<T>&& param) noexcept -> T&& {
    static_assert(!IsLValue<T>, "Can't forward an rvalue as an lvalue.");
    return static_cast<T&&>(param);
}

template<typename T>
constexpr auto move(T&& arg) noexcept -> RemoveReference<T>&& {
    return static_cast<RemoveReference<T>&&>(arg);
}

template<typename T, typename U>
inline void swap(T& lhs, U& rhs) {
    if ( &lhs == &rhs )
        return;
    U tmp = move(lhs);
    lhs   = move(rhs);
    rhs   = move(tmp);
}

template<typename T, typename U = T>
constexpr T exchange(T& slot, U&& value) {
    T old_value = std::move(slot);
    slot        = std::forward<U>(value);
    return old_value;
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

} /* namespace Cxx */

using std::align_val_t;
using std::nothrow_t;
using std::nullptr_t;

using std::nothrow;

constexpr void* operator new(usize, void* ptr) {
    return ptr;
}

constexpr void* operator new[](usize, void* ptr) {
    return ptr;
}