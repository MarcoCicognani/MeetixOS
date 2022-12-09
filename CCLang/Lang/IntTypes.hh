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

#include <CCLang/Forward.hh>

#include <CCLang/Core/Assertions.hh>
#include <CCLang/Core/Concept.hh>
#include <CCLang/Lang/Cxx.hh>
#include <CCLang/Lang/MemOrder.hh>
#include <CCLang/Lang/Must.hh>
#include <CCLang/Lang/Range.hh>

#define OOSignedIntegerWrapper$(WrapperName, real_integer_type, max_value, min_value)                                                                          \
    class WrapperName final {                                                                                                                                  \
    public:                                                                                                                                                    \
        using CCIntegerType = real_integer_type;                                                                                                               \
                                                                                                                                                               \
    public:                                                                                                                                                    \
        static auto max() -> WrapperName {                                                                                                                     \
            return max_value;                                                                                                                                  \
        }                                                                                                                                                      \
        static auto min() -> WrapperName {                                                                                                                     \
            return min_value;                                                                                                                                  \
        }                                                                                                                                                      \
                                                                                                                                                               \
        static auto max(WrapperName const&, WrapperName const&) -> WrapperName;                                                                                \
        static auto min(WrapperName const&, WrapperName const&) -> WrapperName;                                                                                \
                                                                                                                                                               \
        static auto range(WrapperName const&, WrapperName const&) -> Range<WrapperName>;                                                                       \
        static auto range_inclusive(WrapperName const&, WrapperName const&) -> RangeInclusive<WrapperName>;                                                    \
                                                                                                                                                               \
        explicit(false) WrapperName() = default;                                                                                                               \
        explicit(false) WrapperName(real_integer_type);                                                                                                        \
        explicit(false) WrapperName(WrapperName const&);                                                                                                       \
        explicit(false) WrapperName(WrapperName&&);                                                                                                            \
                                                                                                                                                               \
        ~WrapperName() = default;                                                                                                                              \
                                                                                                                                                               \
        auto operator=(real_integer_type) -> WrapperName&;                                                                                                     \
        auto operator=(WrapperName const&) -> WrapperName&;                                                                                                    \
        auto operator=(WrapperName&&) -> WrapperName&;                                                                                                         \
                                                                                                                                                               \
        auto swap(WrapperName&) -> void;                                                                                                                       \
                                                                                                                                                               \
        auto bit_count() const -> usize;                                                                                                                       \
        auto is_signed() const -> bool;                                                                                                                        \
                                                                                                                                                               \
        template<typename TInteger>                                                                                                                            \
        auto as() const -> TInteger {                                                                                                                          \
            return TInteger(static_cast<typename TInteger::CCIntegerType>(m_value));                                                                           \
        }                                                                                                                                                      \
                                                                                                                                                               \
        auto unwrap() const -> real_integer_type;                                                                                                              \
                                                                                                                                                               \
        auto equal(WrapperName const&) const -> bool;                                                                                                          \
        auto operator==(WrapperName const&) const -> bool;                                                                                                     \
                                                                                                                                                               \
        auto not_equal(WrapperName const&) const -> bool;                                                                                                      \
        auto operator!=(WrapperName const&) const -> bool;                                                                                                     \
                                                                                                                                                               \
        auto greater(WrapperName const&) const -> bool;                                                                                                        \
        auto operator>(WrapperName const&) const -> bool;                                                                                                      \
                                                                                                                                                               \
        auto greater_equal(WrapperName const&) const -> bool;                                                                                                  \
        auto operator>=(WrapperName const&) const -> bool;                                                                                                     \
                                                                                                                                                               \
        auto less(WrapperName const&) const -> bool;                                                                                                           \
        auto operator<(WrapperName const&) const -> bool;                                                                                                      \
                                                                                                                                                               \
        auto less_equal(WrapperName const&) const -> bool;                                                                                                     \
        auto operator<=(WrapperName const&) const -> bool;                                                                                                     \
                                                                                                                                                               \
        auto bit_one_complement() const -> WrapperName;                                                                                                        \
        auto operator~() const -> WrapperName;                                                                                                                 \
                                                                                                                                                               \
        auto bit_and(WrapperName const&) const -> WrapperName;                                                                                                 \
        auto operator&(WrapperName const&) const -> WrapperName;                                                                                               \
                                                                                                                                                               \
        auto bit_and_assign(WrapperName const&) -> void;                                                                                                       \
        auto operator&=(WrapperName const&) -> WrapperName&;                                                                                                   \
                                                                                                                                                               \
        auto bit_or(WrapperName const&) const -> WrapperName;                                                                                                  \
        auto operator|(WrapperName const&) const -> WrapperName;                                                                                               \
                                                                                                                                                               \
        auto bit_or_assign(WrapperName const&) -> void;                                                                                                        \
        auto operator|=(WrapperName const&) -> WrapperName&;                                                                                                   \
                                                                                                                                                               \
        auto bit_xor(WrapperName const&) const -> WrapperName;                                                                                                 \
        auto operator^(WrapperName const&) const -> WrapperName;                                                                                               \
                                                                                                                                                               \
        auto bit_xor_assign(WrapperName const&) -> void;                                                                                                       \
        auto operator^=(WrapperName const&) -> WrapperName&;                                                                                                   \
                                                                                                                                                               \
        auto try_left_shift(WrapperName const&) const -> ErrorOr<WrapperName>;                                                                                 \
        auto left_shift(WrapperName const&) const -> WrapperName;                                                                                              \
        auto operator<<(WrapperName const&) const -> WrapperName;                                                                                              \
                                                                                                                                                               \
        auto try_left_shift_assign(WrapperName const&) -> ErrorOr<void>;                                                                                       \
        auto left_shift_assign(WrapperName const&) -> void;                                                                                                    \
        auto operator<<=(WrapperName const&) -> WrapperName&;                                                                                                  \
                                                                                                                                                               \
        auto try_right_shift(WrapperName const&) const -> ErrorOr<WrapperName>;                                                                                \
        auto right_shift(WrapperName const&) const -> WrapperName;                                                                                             \
        auto operator>>(WrapperName const&) const -> WrapperName;                                                                                              \
                                                                                                                                                               \
        auto try_right_shift_assign(WrapperName const&) -> ErrorOr<void>;                                                                                      \
        auto right_shift_assign(WrapperName const&) -> void;                                                                                                   \
        auto operator>>=(WrapperName const&) -> WrapperName&;                                                                                                  \
                                                                                                                                                               \
        auto operator++() -> WrapperName&;                                                                                                                     \
        auto operator++(int) -> WrapperName;                                                                                                                   \
                                                                                                                                                               \
        auto operator--() -> WrapperName&;                                                                                                                     \
        auto operator--(int) -> WrapperName;                                                                                                                   \
                                                                                                                                                               \
        auto try_add(WrapperName const&) const -> ErrorOr<WrapperName>;                                                                                        \
        auto add(WrapperName const&) const -> WrapperName;                                                                                                     \
        auto operator+(WrapperName const&) const -> WrapperName;                                                                                               \
                                                                                                                                                               \
        auto try_add_assign(WrapperName const&) -> ErrorOr<void>;                                                                                              \
        auto add_assign(WrapperName const&) -> void;                                                                                                           \
        auto operator+=(WrapperName const&) -> WrapperName&;                                                                                                   \
                                                                                                                                                               \
        auto try_sub(WrapperName const&) const -> ErrorOr<WrapperName>;                                                                                        \
        auto sub(WrapperName const&) const -> WrapperName;                                                                                                     \
        auto operator-(WrapperName const&) const -> WrapperName;                                                                                               \
                                                                                                                                                               \
        auto try_sub_assign(WrapperName const&) -> ErrorOr<void>;                                                                                              \
        auto sub_assign(WrapperName const&) -> void;                                                                                                           \
        auto operator-=(WrapperName const&) -> WrapperName&;                                                                                                   \
                                                                                                                                                               \
        auto try_mul(WrapperName const&) const -> ErrorOr<WrapperName>;                                                                                        \
        auto mul(WrapperName const&) const -> WrapperName;                                                                                                     \
        auto operator*(WrapperName const&) const -> WrapperName;                                                                                               \
                                                                                                                                                               \
        auto try_mul_assign(WrapperName const&) -> ErrorOr<void>;                                                                                              \
        auto mul_assign(WrapperName const&) -> void;                                                                                                           \
        auto operator*=(WrapperName const&) -> WrapperName&;                                                                                                   \
                                                                                                                                                               \
        auto try_div(WrapperName const&) const -> ErrorOr<WrapperName>;                                                                                        \
        auto div(WrapperName const&) const -> WrapperName;                                                                                                     \
        auto operator/(WrapperName const&) const -> WrapperName;                                                                                               \
                                                                                                                                                               \
        auto try_div_assign(WrapperName const&) -> ErrorOr<void>;                                                                                              \
        auto div_assign(WrapperName const&) -> void;                                                                                                           \
        auto operator/=(WrapperName const&) -> WrapperName&;                                                                                                   \
                                                                                                                                                               \
        auto operator%(WrapperName const& rhs) const -> WrapperName;                                                                                           \
        auto operator%=(WrapperName const& rhs) -> WrapperName&;                                                                                               \
                                                                                                                                                               \
        [[nodiscard]]                                                                                                                                          \
        auto atomic_load(MemOrder = MemOrder::Total) volatile -> WrapperName;                                                                                  \
        auto atomic_store(WrapperName, MemOrder = MemOrder::Total) volatile -> void;                                                                           \
                                                                                                                                                               \
        auto atomic_add(WrapperName, MemOrder = MemOrder::Total) volatile -> void;                                                                             \
        auto atomic_sub(WrapperName, MemOrder = MemOrder::Total) volatile -> void;                                                                             \
                                                                                                                                                               \
        [[nodiscard]]                                                                                                                                          \
        auto atomic_fetch_add(WrapperName, MemOrder = MemOrder::Total) volatile -> WrapperName;                                                                \
        [[nodiscard]]                                                                                                                                          \
        auto atomic_fetch_sub(WrapperName, MemOrder = MemOrder::Total) volatile -> WrapperName;                                                                \
                                                                                                                                                               \
        [[nodiscard]]                                                                                                                                          \
        auto atomic_add_fetch(WrapperName, MemOrder = MemOrder::Total) volatile -> WrapperName;                                                                \
        [[nodiscard]]                                                                                                                                          \
        auto atomic_sub_fetch(WrapperName, MemOrder = MemOrder::Total) volatile -> WrapperName;                                                                \
                                                                                                                                                               \
        [[nodiscard]]                                                                                                                                          \
        auto hash_code() const -> usize;                                                                                                                       \
                                                                                                                                                               \
    private:                                                                                                                                                   \
        real_integer_type m_value = 0;                                                                                                                         \
    };                                                                                                                                                         \
    static_assert(sizeof(WrapperName) == sizeof(real_integer_type));                                                                                           \
                                                                                                                                                               \
    namespace Cxx {                                                                                                                                            \
    auto swap(WrapperName& lhs, WrapperName& rhs) -> void {                                                                                                    \
        lhs.swap(rhs);                                                                                                                                         \
    }                                                                                                                                                          \
    }

#define OOUnsignedIntegerWrapper$(WrapperName, real_integer_type, max_value)                                                                                   \
    class WrapperName final {                                                                                                                                  \
    public:                                                                                                                                                    \
        using CCIntegerType = real_integer_type;                                                                                                               \
                                                                                                                                                               \
    public:                                                                                                                                                    \
        static auto max() -> WrapperName {                                                                                                                     \
            return max_value;                                                                                                                                  \
        }                                                                                                                                                      \
        static auto min() -> WrapperName {                                                                                                                     \
            return 0;                                                                                                                                          \
        }                                                                                                                                                      \
                                                                                                                                                               \
        static auto max(WrapperName const&, WrapperName const&) -> WrapperName;                                                                                \
        static auto min(WrapperName const&, WrapperName const&) -> WrapperName;                                                                                \
                                                                                                                                                               \
        static auto range(WrapperName const&, WrapperName const&) -> Range<WrapperName>;                                                                       \
        static auto range_inclusive(WrapperName const&, WrapperName const&) -> RangeInclusive<WrapperName>;                                                    \
                                                                                                                                                               \
        explicit(false) WrapperName() = default;                                                                                                               \
        explicit(false) WrapperName(real_integer_type);                                                                                                        \
        explicit(false) WrapperName(WrapperName const&);                                                                                                       \
        explicit(false) WrapperName(WrapperName&&);                                                                                                            \
                                                                                                                                                               \
        ~WrapperName() = default;                                                                                                                              \
                                                                                                                                                               \
        auto operator=(real_integer_type) -> WrapperName&;                                                                                                     \
        auto operator=(WrapperName const&) -> WrapperName&;                                                                                                    \
        auto operator=(WrapperName&&) -> WrapperName&;                                                                                                         \
                                                                                                                                                               \
        auto swap(WrapperName&) -> void;                                                                                                                       \
                                                                                                                                                               \
        auto is_signed() const -> bool;                                                                                                                        \
                                                                                                                                                               \
        auto try_bit_at(WrapperName) const -> ErrorOr<bool>;                                                                                                   \
        auto bit_at(WrapperName) const -> bool;                                                                                                                \
                                                                                                                                                               \
        auto try_set_bit(WrapperName, bool) -> ErrorOr<void>;                                                                                                  \
        auto set_bit(WrapperName, bool) -> void;                                                                                                               \
                                                                                                                                                               \
        auto count_zeroes() const -> usize;                                                                                                                    \
        auto count_ones() const -> usize;                                                                                                                      \
                                                                                                                                                               \
        auto bit_count() const -> usize;                                                                                                                       \
                                                                                                                                                               \
        template<typename TInteger>                                                                                                                            \
        auto as() const -> TInteger {                                                                                                                          \
            return TInteger{ static_cast<typename TInteger::CCIntegerType>(m_value) };                                                                         \
        }                                                                                                                                                      \
                                                                                                                                                               \
        auto unwrap() const -> real_integer_type;                                                                                                              \
                                                                                                                                                               \
        auto equal(WrapperName const&) const -> bool;                                                                                                          \
        auto operator==(WrapperName const&) const -> bool;                                                                                                     \
                                                                                                                                                               \
        auto not_equal(WrapperName const&) const -> bool;                                                                                                      \
        auto operator!=(WrapperName const&) const -> bool;                                                                                                     \
                                                                                                                                                               \
        auto greater(WrapperName const&) const -> bool;                                                                                                        \
        auto operator>(WrapperName const&) const -> bool;                                                                                                      \
                                                                                                                                                               \
        auto greater_equal(WrapperName const&) const -> bool;                                                                                                  \
        auto operator>=(WrapperName const&) const -> bool;                                                                                                     \
                                                                                                                                                               \
        auto less(WrapperName const&) const -> bool;                                                                                                           \
        auto operator<(WrapperName const&) const -> bool;                                                                                                      \
                                                                                                                                                               \
        auto less_equal(WrapperName const&) const -> bool;                                                                                                     \
        auto operator<=(WrapperName const&) const -> bool;                                                                                                     \
                                                                                                                                                               \
        auto bit_one_complement() const -> WrapperName;                                                                                                        \
        auto operator~() const -> WrapperName;                                                                                                                 \
                                                                                                                                                               \
        auto bit_and(WrapperName const&) const -> WrapperName;                                                                                                 \
        auto operator&(WrapperName const&) const -> WrapperName;                                                                                               \
                                                                                                                                                               \
        auto bit_and_assign(WrapperName const&) -> void;                                                                                                       \
        auto operator&=(WrapperName const&) -> WrapperName&;                                                                                                   \
                                                                                                                                                               \
        auto bit_or(WrapperName const&) const -> WrapperName;                                                                                                  \
        auto operator|(WrapperName const&) const -> WrapperName;                                                                                               \
                                                                                                                                                               \
        auto bit_or_assign(WrapperName const&) -> void;                                                                                                        \
        auto operator|=(WrapperName const&) -> WrapperName&;                                                                                                   \
                                                                                                                                                               \
        auto bit_xor(WrapperName const&) const -> WrapperName;                                                                                                 \
        auto operator^(WrapperName const&) const -> WrapperName;                                                                                               \
                                                                                                                                                               \
        auto bit_xor_assign(WrapperName const&) -> void;                                                                                                       \
        auto operator^=(WrapperName const&) -> WrapperName&;                                                                                                   \
                                                                                                                                                               \
        auto try_left_shift(WrapperName const&) const -> ErrorOr<WrapperName>;                                                                                 \
        auto left_shift(WrapperName const&) const -> WrapperName;                                                                                              \
        auto operator<<(WrapperName const&) const -> WrapperName;                                                                                              \
                                                                                                                                                               \
        auto try_left_shift_assign(WrapperName const&) -> ErrorOr<void>;                                                                                       \
        auto left_shift_assign(WrapperName const&) -> void;                                                                                                    \
        auto operator<<=(WrapperName const&) -> WrapperName&;                                                                                                  \
                                                                                                                                                               \
        auto try_right_shift(WrapperName const&) const -> ErrorOr<WrapperName>;                                                                                \
        auto right_shift(WrapperName const&) const -> WrapperName;                                                                                             \
        auto operator>>(WrapperName const&) const -> WrapperName;                                                                                              \
                                                                                                                                                               \
        auto try_right_shift_assign(WrapperName const&) -> ErrorOr<void>;                                                                                      \
        auto right_shift_assign(WrapperName const&) -> void;                                                                                                   \
        auto operator>>=(WrapperName const&) -> WrapperName&;                                                                                                  \
                                                                                                                                                               \
        auto operator++() -> WrapperName&;                                                                                                                     \
        auto operator++(int) -> WrapperName;                                                                                                                   \
                                                                                                                                                               \
        auto operator--() -> WrapperName&;                                                                                                                     \
        auto operator--(int) -> WrapperName;                                                                                                                   \
                                                                                                                                                               \
        auto try_add(WrapperName const&) const -> ErrorOr<WrapperName>;                                                                                        \
        auto add(WrapperName const&) const -> WrapperName;                                                                                                     \
        auto operator+(WrapperName const&) const -> WrapperName;                                                                                               \
                                                                                                                                                               \
        auto try_add_assign(WrapperName const&) -> ErrorOr<void>;                                                                                              \
        auto add_assign(WrapperName const&) -> void;                                                                                                           \
        auto operator+=(WrapperName const&) -> WrapperName&;                                                                                                   \
                                                                                                                                                               \
        auto try_sub(WrapperName const&) const -> ErrorOr<WrapperName>;                                                                                        \
        auto sub(WrapperName const&) const -> WrapperName;                                                                                                     \
        auto operator-(WrapperName const&) const -> WrapperName;                                                                                               \
                                                                                                                                                               \
        auto try_sub_assign(WrapperName const&) -> ErrorOr<void>;                                                                                              \
        auto sub_assign(WrapperName const&) -> void;                                                                                                           \
        auto operator-=(WrapperName const&) -> WrapperName&;                                                                                                   \
                                                                                                                                                               \
        auto try_mul(WrapperName const&) const -> ErrorOr<WrapperName>;                                                                                        \
        auto mul(WrapperName const&) const -> WrapperName;                                                                                                     \
        auto operator*(WrapperName const&) const -> WrapperName;                                                                                               \
                                                                                                                                                               \
        auto try_mul_assign(WrapperName const&) -> ErrorOr<void>;                                                                                              \
        auto mul_assign(WrapperName const&) -> void;                                                                                                           \
        auto operator*=(WrapperName const&) -> WrapperName&;                                                                                                   \
                                                                                                                                                               \
        auto try_div(WrapperName const&) const -> ErrorOr<WrapperName>;                                                                                        \
        auto div(WrapperName const&) const -> WrapperName;                                                                                                     \
        auto operator/(WrapperName const&) const -> WrapperName;                                                                                               \
                                                                                                                                                               \
        auto try_div_assign(WrapperName const&) -> ErrorOr<void>;                                                                                              \
        auto div_assign(WrapperName const&) -> void;                                                                                                           \
        auto operator/=(WrapperName const&) -> WrapperName&;                                                                                                   \
                                                                                                                                                               \
        auto operator%(WrapperName const&) const -> WrapperName;                                                                                               \
        auto operator%=(WrapperName const&) -> WrapperName&;                                                                                                   \
                                                                                                                                                               \
        [[nodiscard]]                                                                                                                                          \
        auto atomic_load(MemOrder = MemOrder::Total) volatile -> WrapperName;                                                                                  \
        auto atomic_store(WrapperName, MemOrder = MemOrder::Total) volatile -> void;                                                                           \
                                                                                                                                                               \
        auto atomic_add(WrapperName, MemOrder = MemOrder::Total) volatile -> void;                                                                             \
        auto atomic_sub(WrapperName, MemOrder = MemOrder::Total) volatile -> void;                                                                             \
                                                                                                                                                               \
        [[nodiscard]]                                                                                                                                          \
        auto atomic_fetch_add(WrapperName, MemOrder = MemOrder::Total) volatile -> WrapperName;                                                                \
        [[nodiscard]]                                                                                                                                          \
        auto atomic_fetch_sub(WrapperName, MemOrder = MemOrder::Total) volatile -> WrapperName;                                                                \
                                                                                                                                                               \
        [[nodiscard]]                                                                                                                                          \
        auto atomic_add_fetch(WrapperName, MemOrder = MemOrder::Total) volatile -> WrapperName;                                                                \
        [[nodiscard]]                                                                                                                                          \
        auto atomic_sub_fetch(WrapperName, MemOrder = MemOrder::Total) volatile -> WrapperName;                                                                \
                                                                                                                                                               \
        [[nodiscard]]                                                                                                                                          \
        auto hash_code() const -> usize;                                                                                                                       \
                                                                                                                                                               \
    private:                                                                                                                                                   \
        real_integer_type m_value = 0;                                                                                                                         \
    };                                                                                                                                                         \
    static_assert(sizeof(WrapperName) == sizeof(real_integer_type));                                                                                           \
                                                                                                                                                               \
    auto operator<<(real_integer_type lhs, WrapperName rhs)->WrapperName {                                                                                     \
        return WrapperName(lhs) << rhs;                                                                                                                        \
    }                                                                                                                                                          \
    auto operator>>(real_integer_type lhs, WrapperName rhs)->WrapperName {                                                                                     \
        return WrapperName(lhs) >> rhs;                                                                                                                        \
    }                                                                                                                                                          \
                                                                                                                                                               \
    namespace Cxx {                                                                                                                                            \
    auto swap(WrapperName& lhs, WrapperName& rhs) -> void {                                                                                                    \
        lhs.swap(rhs);                                                                                                                                         \
    }                                                                                                                                                          \
    }

OOUnsignedIntegerWrapper$(usize, __SIZE_TYPE__, __SIZE_MAX__); /* First because is used by bit_count() */
template<typename T>
auto operator+(T* ptr, usize offset) -> T* {
    return ptr + offset.unwrap();
}
template<typename T>
auto operator-(T* ptr, usize offset) -> T* {
    return ptr - offset.unwrap();
}

OOUnsignedIntegerWrapper$(u8, __UINT8_TYPE__, __UINT8_MAX__);
OOUnsignedIntegerWrapper$(u16, __UINT16_TYPE__, __UINT16_MAX__);
OOUnsignedIntegerWrapper$(u32, __UINT32_TYPE__, __UINT32_MAX__);
OOUnsignedIntegerWrapper$(u64, __UINT64_TYPE__, __UINT64_MAX__);

OOSignedIntegerWrapper$(i8, __INT8_TYPE__, __INT8_MAX__, -__INT8_MAX__ - 1);
OOSignedIntegerWrapper$(i16, __INT16_TYPE__, __INT16_MAX__, -__INT16_MAX__ - 1);
OOSignedIntegerWrapper$(i32, __INT32_TYPE__, __INT32_MAX__, -__INT32_MAX__ - 1);
OOSignedIntegerWrapper$(i64, __INT64_TYPE__, __INT64_MAX__, -__INT64_MAX__ - 1);
OOSignedIntegerWrapper$(isize, __PTRDIFF_TYPE__, __PTRDIFF_MAX__, -__PTRDIFF_MAX__ - 1);

#ifndef IN_KERNEL
using f32 = float;
using f64 = double;
using f80 = long double;
#endif

template<WrappedIntegral T>
struct TypeTraits<T> final : public Details::TypeTraits<T> {
    static auto hash(T const& value) -> usize {
        return value.hash_code();
    }

    static constexpr auto is_trivial() -> bool {
        return true;
    }
};

template<NativeIntegral T>
struct TypeTraits<T> final : public Details::TypeTraits<T> {
    static auto hash(T const& value) -> usize {
        return usize(static_cast<usize::CCIntegerType>(value)).hash_code();
    }

    static constexpr auto is_trivial() -> bool {
        return true;
    }
};

template<Pointer T>
struct TypeTraits<T> final : public Details::TypeTraits<T> {
    static auto hash(T value) -> usize {
        return usize(static_cast<usize::CCIntegerType>(value)).hash_code();
    }

    static constexpr auto is_trivial() -> bool {
        return true;
    }
};