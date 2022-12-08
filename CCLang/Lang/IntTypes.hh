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
        static constexpr auto max() -> WrapperName {                                                                                                           \
            return max_value;                                                                                                                                  \
        }                                                                                                                                                      \
        static constexpr auto max(WrapperName const& lhs, WrapperName const& rhs) -> WrapperName {                                                             \
            if ( lhs > rhs )                                                                                                                                   \
                return lhs;                                                                                                                                    \
            else                                                                                                                                               \
                return rhs;                                                                                                                                    \
        }                                                                                                                                                      \
        static constexpr auto min() -> WrapperName {                                                                                                           \
            return min_value;                                                                                                                                  \
        }                                                                                                                                                      \
        static constexpr auto min(WrapperName const& lhs, WrapperName const& rhs) -> WrapperName {                                                             \
            if ( lhs < rhs )                                                                                                                                   \
                return lhs;                                                                                                                                    \
            else                                                                                                                                               \
                return rhs;                                                                                                                                    \
        }                                                                                                                                                      \
                                                                                                                                                               \
        static constexpr auto range(WrapperName const& begin, WrapperName const& end) -> Range<WrapperName> {                                                  \
            return Range<WrapperName>(begin, end);                                                                                                             \
        }                                                                                                                                                      \
        static constexpr auto range_inclusive(WrapperName const& begin, WrapperName const& last) -> RangeInclusive<WrapperName> {                              \
            return RangeInclusive<WrapperName>(begin, last);                                                                                                   \
        }                                                                                                                                                      \
                                                                                                                                                               \
        constexpr explicit(false) WrapperName() = default;                                                                                                     \
        constexpr explicit(false) WrapperName(real_integer_type value)                                                                                         \
            : m_value(value) {                                                                                                                                 \
        }                                                                                                                                                      \
        constexpr explicit(false) WrapperName(WrapperName const& rhs)                                                                                          \
            : m_value(rhs.m_value) {                                                                                                                           \
        }                                                                                                                                                      \
        constexpr explicit(false) WrapperName(WrapperName&& rhs)                                                                                               \
            : m_value(Cxx::exchange(rhs.m_value, {})) {                                                                                                        \
        }                                                                                                                                                      \
                                                                                                                                                               \
        constexpr ~WrapperName() = default;                                                                                                                    \
                                                                                                                                                               \
        constexpr auto operator=(real_integer_type value) -> WrapperName& {                                                                                    \
            WrapperName integer = value;                                                                                                                       \
            swap(integer);                                                                                                                                     \
            return *this;                                                                                                                                      \
        }                                                                                                                                                      \
        constexpr auto operator=(WrapperName const& rhs) -> WrapperName& {                                                                                     \
            auto integer = rhs;                                                                                                                                \
            swap(integer);                                                                                                                                     \
            return *this;                                                                                                                                      \
        }                                                                                                                                                      \
        constexpr auto operator=(WrapperName&& rhs) -> WrapperName& {                                                                                          \
            auto integer = Cxx::move(rhs);                                                                                                                     \
            swap(integer);                                                                                                                                     \
            return *this;                                                                                                                                      \
        }                                                                                                                                                      \
                                                                                                                                                               \
        constexpr auto swap(WrapperName& rhs) -> void {                                                                                                        \
            Cxx::swap(m_value, rhs.m_value);                                                                                                                   \
        }                                                                                                                                                      \
                                                                                                                                                               \
        constexpr auto bit_count() const -> usize {                                                                                                            \
            return sizeof(real_integer_type) * 8;                                                                                                              \
        }                                                                                                                                                      \
                                                                                                                                                               \
        constexpr auto is_signed() const -> bool {                                                                                                             \
            return true;                                                                                                                                       \
        }                                                                                                                                                      \
                                                                                                                                                               \
        template<typename TInteger>                                                                                                                            \
        constexpr auto as() const -> TInteger {                                                                                                                \
            return TInteger{ static_cast<typename TInteger::CCIntegerType>(m_value) };                                                                         \
        }                                                                                                                                                      \
                                                                                                                                                               \
        constexpr auto unwrap() const -> real_integer_type {                                                                                                   \
            return m_value;                                                                                                                                    \
        }                                                                                                                                                      \
                                                                                                                                                               \
        constexpr auto equal(WrapperName const& rhs) const -> bool {                                                                                           \
            return m_value == rhs.m_value;                                                                                                                     \
        }                                                                                                                                                      \
        constexpr auto operator==(WrapperName const& rhs) const -> bool {                                                                                      \
            return equal(rhs);                                                                                                                                 \
        }                                                                                                                                                      \
                                                                                                                                                               \
        constexpr auto not_equal(WrapperName const& rhs) const -> bool {                                                                                       \
            return m_value != rhs.m_value;                                                                                                                     \
        }                                                                                                                                                      \
        constexpr auto operator!=(WrapperName const& rhs) const -> bool {                                                                                      \
            return not_equal(rhs);                                                                                                                             \
        }                                                                                                                                                      \
                                                                                                                                                               \
        constexpr auto greater(WrapperName const& rhs) const -> bool {                                                                                         \
            return m_value > rhs.m_value;                                                                                                                      \
        }                                                                                                                                                      \
        constexpr auto operator>(WrapperName const& rhs) const -> bool {                                                                                       \
            return greater(rhs);                                                                                                                               \
        }                                                                                                                                                      \
                                                                                                                                                               \
        constexpr auto greater_equal(WrapperName const& rhs) const -> bool {                                                                                   \
            return m_value >= rhs.m_value;                                                                                                                     \
        }                                                                                                                                                      \
        constexpr auto operator>=(WrapperName const& rhs) const -> bool {                                                                                      \
            return greater_equal(rhs);                                                                                                                         \
        }                                                                                                                                                      \
                                                                                                                                                               \
        constexpr auto less(WrapperName const& rhs) const -> bool {                                                                                            \
            return m_value < rhs.m_value;                                                                                                                      \
        }                                                                                                                                                      \
        constexpr auto operator<(WrapperName const& rhs) const -> bool {                                                                                       \
            return less(rhs);                                                                                                                                  \
        }                                                                                                                                                      \
                                                                                                                                                               \
        constexpr auto less_equal(WrapperName const& rhs) const -> bool {                                                                                      \
            return m_value <= rhs.m_value;                                                                                                                     \
        }                                                                                                                                                      \
        constexpr auto operator<=(WrapperName const& rhs) const -> bool {                                                                                      \
            return less_equal(rhs);                                                                                                                            \
        }                                                                                                                                                      \
                                                                                                                                                               \
        constexpr auto bit_one_complement() const -> WrapperName {                                                                                             \
            return ~m_value;                                                                                                                                   \
        }                                                                                                                                                      \
        constexpr auto operator~() const -> WrapperName {                                                                                                      \
            return bit_one_complement();                                                                                                                       \
        }                                                                                                                                                      \
                                                                                                                                                               \
        constexpr auto bit_and(WrapperName const& rhs) const -> WrapperName {                                                                                  \
            return m_value & rhs.m_value;                                                                                                                      \
        }                                                                                                                                                      \
        constexpr auto operator&(WrapperName const& rhs) const -> WrapperName {                                                                                \
            return bit_and(rhs);                                                                                                                               \
        }                                                                                                                                                      \
                                                                                                                                                               \
        constexpr auto bit_and_assign(WrapperName const& rhs) -> void {                                                                                        \
            m_value &= rhs.m_value;                                                                                                                            \
        }                                                                                                                                                      \
        constexpr auto operator&=(WrapperName const& rhs) -> WrapperName& {                                                                                    \
            bit_and_assign(rhs);                                                                                                                               \
            return *this;                                                                                                                                      \
        }                                                                                                                                                      \
                                                                                                                                                               \
        constexpr auto bit_or(WrapperName const& rhs) const -> WrapperName {                                                                                   \
            return m_value | rhs.m_value;                                                                                                                      \
        }                                                                                                                                                      \
        constexpr auto operator|(WrapperName const& rhs) const -> WrapperName {                                                                                \
            return bit_or(rhs);                                                                                                                                \
        }                                                                                                                                                      \
                                                                                                                                                               \
        constexpr auto bit_or_assign(WrapperName const& rhs) -> void {                                                                                         \
            m_value |= rhs.m_value;                                                                                                                            \
        }                                                                                                                                                      \
        constexpr auto operator|=(WrapperName const& rhs) -> WrapperName& {                                                                                    \
            bit_or_assign(rhs);                                                                                                                                \
            return *this;                                                                                                                                      \
        }                                                                                                                                                      \
                                                                                                                                                               \
        constexpr auto bit_xor(WrapperName const& rhs) const -> WrapperName {                                                                                  \
            return m_value ^ rhs.m_value;                                                                                                                      \
        }                                                                                                                                                      \
        constexpr auto operator^(WrapperName const& rhs) const -> WrapperName {                                                                                \
            return bit_xor(rhs);                                                                                                                               \
        }                                                                                                                                                      \
                                                                                                                                                               \
        constexpr auto bit_xor_assign(WrapperName const& rhs) -> void {                                                                                        \
            m_value ^= rhs.m_value;                                                                                                                            \
        }                                                                                                                                                      \
        constexpr auto operator^=(WrapperName const& rhs) -> WrapperName& {                                                                                    \
            bit_xor_assign(rhs);                                                                                                                               \
            return *this;                                                                                                                                      \
        }                                                                                                                                                      \
                                                                                                                                                               \
        auto try_left_shift(WrapperName const& rhs) const -> ErrorOr<WrapperName>;                                                                             \
        auto left_shift(WrapperName const& rhs) const -> WrapperName;                                                                                          \
        auto operator<<(WrapperName const& rhs) const -> WrapperName {                                                                                         \
            return left_shift(rhs);                                                                                                                            \
        }                                                                                                                                                      \
                                                                                                                                                               \
        auto try_left_shift_assign(WrapperName const& rhs) -> ErrorOr<void>;                                                                                   \
        auto left_shift_assign(WrapperName const& rhs) -> void;                                                                                                \
        auto operator<<=(WrapperName const& rhs) -> WrapperName& {                                                                                             \
            left_shift_assign(rhs);                                                                                                                            \
            return *this;                                                                                                                                      \
        }                                                                                                                                                      \
                                                                                                                                                               \
        auto try_right_shift(WrapperName const& rhs) const -> ErrorOr<WrapperName>;                                                                            \
        auto right_shift(WrapperName const& rhs) const -> WrapperName;                                                                                         \
        auto operator>>(WrapperName const& rhs) const -> WrapperName {                                                                                         \
            return right_shift(rhs);                                                                                                                           \
        }                                                                                                                                                      \
                                                                                                                                                               \
        auto try_right_shift_assign(WrapperName const& rhs) -> ErrorOr<void>;                                                                                  \
        auto right_shift_assign(WrapperName const& rhs) -> void;                                                                                               \
        auto operator>>=(WrapperName const& rhs) -> WrapperName& {                                                                                             \
            right_shift_assign(rhs);                                                                                                                           \
            return *this;                                                                                                                                      \
        }                                                                                                                                                      \
                                                                                                                                                               \
        auto operator++() -> WrapperName& {                                                                                                                    \
            add_assign(1);                                                                                                                                     \
            return *this;                                                                                                                                      \
        }                                                                                                                                                      \
        auto operator++(int) -> WrapperName {                                                                                                                  \
            auto __prev = *this;                                                                                                                               \
            add_assign(1);                                                                                                                                     \
            return __prev;                                                                                                                                     \
        }                                                                                                                                                      \
                                                                                                                                                               \
        auto operator--() -> WrapperName& {                                                                                                                    \
            sub_assign(1);                                                                                                                                     \
            return *this;                                                                                                                                      \
        }                                                                                                                                                      \
        auto operator--(int) -> WrapperName {                                                                                                                  \
            auto __prev = *this;                                                                                                                               \
            sub_assign(1);                                                                                                                                     \
            return __prev;                                                                                                                                     \
        }                                                                                                                                                      \
                                                                                                                                                               \
        auto try_add(WrapperName const&) const -> ErrorOr<WrapperName>;                                                                                        \
        auto add(WrapperName const& rhs) const -> WrapperName;                                                                                                 \
        auto operator+(WrapperName const& rhs) const -> WrapperName {                                                                                          \
            return add(rhs);                                                                                                                                   \
        }                                                                                                                                                      \
                                                                                                                                                               \
        auto try_add_assign(WrapperName const&) -> ErrorOr<void>;                                                                                              \
        auto add_assign(WrapperName const& rhs) -> void;                                                                                                       \
        auto operator+=(WrapperName const& rhs) -> WrapperName& {                                                                                              \
            add_assign(rhs);                                                                                                                                   \
            return *this;                                                                                                                                      \
        }                                                                                                                                                      \
                                                                                                                                                               \
        auto try_sub(WrapperName const& rhs) const -> ErrorOr<WrapperName>;                                                                                    \
        auto sub(WrapperName const& rhs) const -> WrapperName;                                                                                                 \
        auto operator-(WrapperName const& rhs) const -> WrapperName {                                                                                          \
            return sub(rhs);                                                                                                                                   \
        }                                                                                                                                                      \
                                                                                                                                                               \
        auto try_sub_assign(WrapperName const& rhs) -> ErrorOr<void>;                                                                                          \
        auto sub_assign(WrapperName const& rhs) -> void;                                                                                                       \
        auto operator-=(WrapperName const& rhs) -> WrapperName& {                                                                                              \
            sub_assign(rhs);                                                                                                                                   \
            return *this;                                                                                                                                      \
        }                                                                                                                                                      \
                                                                                                                                                               \
        auto try_mul(WrapperName const& rhs) const -> ErrorOr<WrapperName>;                                                                                    \
        auto mul(WrapperName const& rhs) const -> WrapperName;                                                                                                 \
        auto operator*(WrapperName const& rhs) const -> WrapperName {                                                                                          \
            return mul(rhs);                                                                                                                                   \
        }                                                                                                                                                      \
                                                                                                                                                               \
        auto try_mul_assign(WrapperName const&) -> ErrorOr<void>;                                                                                              \
        auto mul_assign(WrapperName const& rhs) -> void;                                                                                                       \
        auto operator*=(WrapperName const& rhs) -> WrapperName& {                                                                                              \
            mul_assign(rhs);                                                                                                                                   \
            return *this;                                                                                                                                      \
        }                                                                                                                                                      \
                                                                                                                                                               \
        auto try_div(WrapperName const&) const -> ErrorOr<WrapperName>;                                                                                        \
        auto div(WrapperName const& rhs) const -> WrapperName;                                                                                                 \
        auto operator/(WrapperName const& rhs) const -> WrapperName {                                                                                          \
            return div(rhs);                                                                                                                                   \
        }                                                                                                                                                      \
                                                                                                                                                               \
        auto try_div_assign(WrapperName const&) -> ErrorOr<void>;                                                                                              \
        auto div_assign(WrapperName const& rhs) -> void;                                                                                                       \
        auto operator/=(WrapperName const& rhs) -> WrapperName& {                                                                                              \
            div_assign(rhs);                                                                                                                                   \
            return *this;                                                                                                                                      \
        }                                                                                                                                                      \
                                                                                                                                                               \
        constexpr auto operator%(WrapperName const& rhs) const -> WrapperName {                                                                                \
            return m_value % rhs.m_value;                                                                                                                      \
        }                                                                                                                                                      \
        constexpr auto operator%=(WrapperName const& rhs) -> WrapperName& {                                                                                    \
            m_value %= rhs.m_value;                                                                                                                            \
            return *this;                                                                                                                                      \
        }                                                                                                                                                      \
                                                                                                                                                               \
        [[nodiscard]] auto atomic_load(MemOrder = MemOrder::Total) volatile -> WrapperName;                                                                    \
        auto               atomic_store(WrapperName, MemOrder = MemOrder::Total) volatile -> void;                                                             \
                                                                                                                                                               \
        auto atomic_add(WrapperName, MemOrder = MemOrder::Total) volatile -> void;                                                                             \
        auto atomic_sub(WrapperName, MemOrder = MemOrder::Total) volatile -> void;                                                                             \
                                                                                                                                                               \
        [[nodiscard]] auto atomic_fetch_add(WrapperName, MemOrder = MemOrder::Total) volatile -> WrapperName;                                                  \
        [[nodiscard]] auto atomic_fetch_sub(WrapperName, MemOrder = MemOrder::Total) volatile -> WrapperName;                                                  \
                                                                                                                                                               \
        [[nodiscard]] auto atomic_add_fetch(WrapperName, MemOrder = MemOrder::Total) volatile -> WrapperName;                                                  \
        [[nodiscard]] auto atomic_sub_fetch(WrapperName, MemOrder = MemOrder::Total) volatile -> WrapperName;                                                  \
                                                                                                                                                               \
        [[nodiscard]] auto hash_code() const -> usize;                                                                                                         \
                                                                                                                                                               \
    private:                                                                                                                                                   \
        real_integer_type m_value = 0;                                                                                                                         \
    };                                                                                                                                                         \
    static_assert(sizeof(WrapperName) == sizeof(real_integer_type));                                                                                           \
                                                                                                                                                               \
    namespace Cxx {                                                                                                                                            \
    constexpr auto swap(WrapperName& lhs, WrapperName& rhs) -> void {                                                                                          \
        lhs.swap(rhs);                                                                                                                                         \
    }                                                                                                                                                          \
    }

#define OOUnsignedIntegerWrapper$(WrapperName, real_integer_type, max_value)                                                                                   \
    class WrapperName final {                                                                                                                                  \
    public:                                                                                                                                                    \
        using CCIntegerType = real_integer_type;                                                                                                               \
                                                                                                                                                               \
    public:                                                                                                                                                    \
        static constexpr auto max() -> WrapperName {                                                                                                           \
            return max_value;                                                                                                                                  \
        }                                                                                                                                                      \
        static constexpr auto max(WrapperName const& lhs, WrapperName const& rhs) -> WrapperName {                                                             \
            if ( lhs > rhs )                                                                                                                                   \
                return lhs;                                                                                                                                    \
            else                                                                                                                                               \
                return rhs;                                                                                                                                    \
        }                                                                                                                                                      \
        static constexpr auto min() -> WrapperName {                                                                                                           \
            return 0;                                                                                                                                          \
        }                                                                                                                                                      \
        static constexpr auto min(WrapperName const& lhs, WrapperName const& rhs) -> WrapperName {                                                             \
            if ( lhs < rhs )                                                                                                                                   \
                return lhs;                                                                                                                                    \
            else                                                                                                                                               \
                return rhs;                                                                                                                                    \
        }                                                                                                                                                      \
                                                                                                                                                               \
        static constexpr auto range(WrapperName const& begin, WrapperName const& end) -> Range<WrapperName> {                                                  \
            return Range<WrapperName>(begin, end);                                                                                                             \
        }                                                                                                                                                      \
        static constexpr auto range_inclusive(WrapperName const& begin, WrapperName const& last) -> RangeInclusive<WrapperName> {                              \
            return RangeInclusive<WrapperName>(begin, last);                                                                                                   \
        }                                                                                                                                                      \
                                                                                                                                                               \
        constexpr explicit(false) WrapperName() = default;                                                                                                     \
        constexpr explicit(false) WrapperName(real_integer_type value)                                                                                         \
            : m_value(value) {                                                                                                                                 \
        }                                                                                                                                                      \
        constexpr explicit(false) WrapperName(WrapperName const& rhs)                                                                                          \
            : m_value(rhs.m_value) {                                                                                                                           \
        }                                                                                                                                                      \
        constexpr explicit(false) WrapperName(WrapperName&& rhs)                                                                                               \
            : m_value(Cxx::exchange(rhs.m_value, {})) {                                                                                                        \
        }                                                                                                                                                      \
                                                                                                                                                               \
        constexpr ~WrapperName() = default;                                                                                                                    \
                                                                                                                                                               \
        constexpr auto operator=(real_integer_type value) -> WrapperName& {                                                                                    \
            WrapperName integer = value;                                                                                                                       \
            swap(integer);                                                                                                                                     \
            return *this;                                                                                                                                      \
        }                                                                                                                                                      \
        constexpr auto operator=(WrapperName const& rhs) -> WrapperName& {                                                                                     \
            auto integer = rhs;                                                                                                                                \
            swap(integer);                                                                                                                                     \
            return *this;                                                                                                                                      \
        }                                                                                                                                                      \
        constexpr auto operator=(WrapperName&& rhs) -> WrapperName& {                                                                                          \
            auto integer = Cxx::move(rhs);                                                                                                                     \
            swap(integer);                                                                                                                                     \
            return *this;                                                                                                                                      \
        }                                                                                                                                                      \
                                                                                                                                                               \
        constexpr auto swap(WrapperName& rhs) -> void {                                                                                                        \
            Cxx::swap(m_value, rhs.m_value);                                                                                                                   \
        }                                                                                                                                                      \
                                                                                                                                                               \
        constexpr auto is_signed() const -> bool {                                                                                                             \
            return false;                                                                                                                                      \
        }                                                                                                                                                      \
                                                                                                                                                               \
        auto try_bit_at(WrapperName index) const -> ErrorOr<bool>;                                                                                             \
        auto bit_at(WrapperName index) const -> bool;                                                                                                          \
                                                                                                                                                               \
        auto try_set_bit(WrapperName index, bool value) -> ErrorOr<void>;                                                                                      \
        auto set_bit(WrapperName index, bool value) -> void;                                                                                                   \
                                                                                                                                                               \
        auto count_zeroes() const -> usize;                                                                                                                    \
        auto count_ones() const -> usize;                                                                                                                      \
                                                                                                                                                               \
        constexpr auto bit_count() const -> usize {                                                                                                            \
            return sizeof(real_integer_type) * 8;                                                                                                              \
        }                                                                                                                                                      \
                                                                                                                                                               \
        template<typename TInteger>                                                                                                                            \
        constexpr auto as() const -> TInteger {                                                                                                                \
            return TInteger{ static_cast<typename TInteger::CCIntegerType>(m_value) };                                                                         \
        }                                                                                                                                                      \
                                                                                                                                                               \
        constexpr auto unwrap() const -> real_integer_type {                                                                                                   \
            return m_value;                                                                                                                                    \
        }                                                                                                                                                      \
                                                                                                                                                               \
        constexpr auto equal(WrapperName const& rhs) const -> bool {                                                                                           \
            return m_value == rhs.m_value;                                                                                                                     \
        }                                                                                                                                                      \
        constexpr auto operator==(WrapperName const& rhs) const -> bool {                                                                                      \
            return equal(rhs);                                                                                                                                 \
        }                                                                                                                                                      \
                                                                                                                                                               \
        constexpr auto not_equal(WrapperName const& rhs) const -> bool {                                                                                       \
            return m_value != rhs.m_value;                                                                                                                     \
        }                                                                                                                                                      \
        constexpr auto operator!=(WrapperName const& rhs) const -> bool {                                                                                      \
            return not_equal(rhs);                                                                                                                             \
        }                                                                                                                                                      \
                                                                                                                                                               \
        constexpr auto greater(WrapperName const& rhs) const -> bool {                                                                                         \
            return m_value > rhs.m_value;                                                                                                                      \
        }                                                                                                                                                      \
        constexpr auto operator>(WrapperName const& rhs) const -> bool {                                                                                       \
            return greater(rhs);                                                                                                                               \
        }                                                                                                                                                      \
                                                                                                                                                               \
        constexpr auto greater_equal(WrapperName const& rhs) const -> bool {                                                                                   \
            return m_value >= rhs.m_value;                                                                                                                     \
        }                                                                                                                                                      \
        constexpr auto operator>=(WrapperName const& rhs) const -> bool {                                                                                      \
            return greater_equal(rhs);                                                                                                                         \
        }                                                                                                                                                      \
                                                                                                                                                               \
        constexpr auto less(WrapperName const& rhs) const -> bool {                                                                                            \
            return m_value < rhs.m_value;                                                                                                                      \
        }                                                                                                                                                      \
        constexpr auto operator<(WrapperName const& rhs) const -> bool {                                                                                       \
            return less(rhs);                                                                                                                                  \
        }                                                                                                                                                      \
                                                                                                                                                               \
        constexpr auto less_equal(WrapperName const& rhs) const -> bool {                                                                                      \
            return m_value <= rhs.m_value;                                                                                                                     \
        }                                                                                                                                                      \
        constexpr auto operator<=(WrapperName const& rhs) const -> bool {                                                                                      \
            return less_equal(rhs);                                                                                                                            \
        }                                                                                                                                                      \
                                                                                                                                                               \
        constexpr auto bit_one_complement() const -> WrapperName {                                                                                             \
            return ~m_value;                                                                                                                                   \
        }                                                                                                                                                      \
        constexpr auto operator~() const -> WrapperName {                                                                                                      \
            return bit_one_complement();                                                                                                                       \
        }                                                                                                                                                      \
                                                                                                                                                               \
        constexpr auto bit_and(WrapperName const& rhs) const -> WrapperName {                                                                                  \
            return m_value & rhs.m_value;                                                                                                                      \
        }                                                                                                                                                      \
        constexpr auto operator&(WrapperName const& rhs) const -> WrapperName {                                                                                \
            return bit_and(rhs);                                                                                                                               \
        }                                                                                                                                                      \
                                                                                                                                                               \
        constexpr auto bit_and_assign(WrapperName const& rhs) -> void {                                                                                        \
            m_value &= rhs.m_value;                                                                                                                            \
        }                                                                                                                                                      \
        constexpr auto operator&=(WrapperName const& rhs) -> WrapperName& {                                                                                    \
            bit_and_assign(rhs);                                                                                                                               \
            return *this;                                                                                                                                      \
        }                                                                                                                                                      \
                                                                                                                                                               \
        constexpr auto bit_or(WrapperName const& rhs) const -> WrapperName {                                                                                   \
            return m_value | rhs.m_value;                                                                                                                      \
        }                                                                                                                                                      \
        constexpr auto operator|(WrapperName const& rhs) const -> WrapperName {                                                                                \
            return bit_or(rhs);                                                                                                                                \
        }                                                                                                                                                      \
                                                                                                                                                               \
        constexpr auto bit_or_assign(WrapperName const& rhs) -> void {                                                                                         \
            m_value |= rhs.m_value;                                                                                                                            \
        }                                                                                                                                                      \
        constexpr auto operator|=(WrapperName const& rhs) -> WrapperName& {                                                                                    \
            bit_or_assign(rhs);                                                                                                                                \
            return *this;                                                                                                                                      \
        }                                                                                                                                                      \
                                                                                                                                                               \
        constexpr auto bit_xor(WrapperName const& rhs) const -> WrapperName {                                                                                  \
            return m_value ^ rhs.m_value;                                                                                                                      \
        }                                                                                                                                                      \
        constexpr auto operator^(WrapperName const& rhs) const -> WrapperName {                                                                                \
            return bit_xor(rhs);                                                                                                                               \
        }                                                                                                                                                      \
                                                                                                                                                               \
        constexpr auto bit_xor_assign(WrapperName const& rhs) -> void {                                                                                        \
            m_value ^= rhs.m_value;                                                                                                                            \
        }                                                                                                                                                      \
        constexpr auto operator^=(WrapperName const& rhs) -> WrapperName& {                                                                                    \
            bit_xor_assign(rhs);                                                                                                                               \
            return *this;                                                                                                                                      \
        }                                                                                                                                                      \
                                                                                                                                                               \
        auto try_left_shift(WrapperName const& rhs) const -> ErrorOr<WrapperName>;                                                                             \
        auto left_shift(WrapperName const& rhs) const -> WrapperName;                                                                                          \
        auto operator<<(WrapperName const& rhs) const -> WrapperName {                                                                                         \
            return left_shift(rhs);                                                                                                                            \
        }                                                                                                                                                      \
                                                                                                                                                               \
        auto try_left_shift_assign(WrapperName const& rhs) -> ErrorOr<void>;                                                                                   \
        auto left_shift_assign(WrapperName const& rhs) -> void;                                                                                                \
        auto operator<<=(WrapperName const& rhs) -> WrapperName& {                                                                                             \
            left_shift_assign(rhs);                                                                                                                            \
            return *this;                                                                                                                                      \
        }                                                                                                                                                      \
                                                                                                                                                               \
        auto try_right_shift(WrapperName const& rhs) const -> ErrorOr<WrapperName>;                                                                            \
        auto right_shift(WrapperName const& rhs) const -> WrapperName;                                                                                         \
        auto operator>>(WrapperName const& rhs) const -> WrapperName {                                                                                         \
            return right_shift(rhs);                                                                                                                           \
        }                                                                                                                                                      \
                                                                                                                                                               \
        auto try_right_shift_assign(WrapperName const& rhs) -> ErrorOr<void>;                                                                                  \
        auto right_shift_assign(WrapperName const& rhs) -> void;                                                                                               \
        auto operator>>=(WrapperName const& rhs) -> WrapperName& {                                                                                             \
            right_shift_assign(rhs);                                                                                                                           \
            return *this;                                                                                                                                      \
        }                                                                                                                                                      \
                                                                                                                                                               \
        auto operator++() -> WrapperName& {                                                                                                                    \
            add_assign(1);                                                                                                                                     \
            return *this;                                                                                                                                      \
        }                                                                                                                                                      \
        auto operator++(int) -> WrapperName {                                                                                                                  \
            auto __prev = *this;                                                                                                                               \
            add_assign(1);                                                                                                                                     \
            return __prev;                                                                                                                                     \
        }                                                                                                                                                      \
                                                                                                                                                               \
        auto operator--() -> WrapperName& {                                                                                                                    \
            sub_assign(1);                                                                                                                                     \
            return *this;                                                                                                                                      \
        }                                                                                                                                                      \
        auto operator--(int) -> WrapperName {                                                                                                                  \
            auto __prev = *this;                                                                                                                               \
            sub_assign(1);                                                                                                                                     \
            return __prev;                                                                                                                                     \
        }                                                                                                                                                      \
                                                                                                                                                               \
        auto try_add(WrapperName const&) const -> ErrorOr<WrapperName>;                                                                                        \
        auto add(WrapperName const& rhs) const -> WrapperName;                                                                                                 \
        auto operator+(WrapperName const& rhs) const -> WrapperName {                                                                                          \
            return add(rhs);                                                                                                                                   \
        }                                                                                                                                                      \
                                                                                                                                                               \
        auto try_add_assign(WrapperName const&) -> ErrorOr<void>;                                                                                              \
        auto add_assign(WrapperName const& rhs) -> void;                                                                                                       \
        auto operator+=(WrapperName const& rhs) -> WrapperName& {                                                                                              \
            add_assign(rhs);                                                                                                                                   \
            return *this;                                                                                                                                      \
        }                                                                                                                                                      \
                                                                                                                                                               \
        auto try_sub(WrapperName const& rhs) const -> ErrorOr<WrapperName>;                                                                                    \
        auto sub(WrapperName const& rhs) const -> WrapperName;                                                                                                 \
        auto operator-(WrapperName const& rhs) const -> WrapperName {                                                                                          \
            return sub(rhs);                                                                                                                                   \
        }                                                                                                                                                      \
                                                                                                                                                               \
        auto try_sub_assign(WrapperName const& rhs) -> ErrorOr<void>;                                                                                          \
        auto sub_assign(WrapperName const& rhs) -> void;                                                                                                       \
        auto operator-=(WrapperName const& rhs) -> WrapperName& {                                                                                              \
            sub_assign(rhs);                                                                                                                                   \
            return *this;                                                                                                                                      \
        }                                                                                                                                                      \
                                                                                                                                                               \
        auto try_mul(WrapperName const& rhs) const -> ErrorOr<WrapperName>;                                                                                    \
        auto mul(WrapperName const& rhs) const -> WrapperName;                                                                                                 \
        auto operator*(WrapperName const& rhs) const -> WrapperName {                                                                                          \
            return mul(rhs);                                                                                                                                   \
        }                                                                                                                                                      \
                                                                                                                                                               \
        auto try_mul_assign(WrapperName const&) -> ErrorOr<void>;                                                                                              \
        auto mul_assign(WrapperName const& rhs) -> void;                                                                                                       \
        auto operator*=(WrapperName const& rhs) -> WrapperName& {                                                                                              \
            mul_assign(rhs);                                                                                                                                   \
            return *this;                                                                                                                                      \
        }                                                                                                                                                      \
                                                                                                                                                               \
        auto try_div(WrapperName const&) const -> ErrorOr<WrapperName>;                                                                                        \
        auto div(WrapperName const& rhs) const -> WrapperName;                                                                                                 \
        auto operator/(WrapperName const& rhs) const -> WrapperName {                                                                                          \
            return div(rhs);                                                                                                                                   \
        }                                                                                                                                                      \
                                                                                                                                                               \
        auto try_div_assign(WrapperName const&) -> ErrorOr<void>;                                                                                              \
        auto div_assign(WrapperName const& rhs) -> void;                                                                                                       \
        auto operator/=(WrapperName const& rhs) -> WrapperName& {                                                                                              \
            div_assign(rhs);                                                                                                                                   \
            return *this;                                                                                                                                      \
        }                                                                                                                                                      \
                                                                                                                                                               \
        constexpr auto operator%(WrapperName const& rhs) const -> WrapperName {                                                                                \
            return m_value % rhs.m_value;                                                                                                                      \
        }                                                                                                                                                      \
        constexpr auto operator%=(WrapperName const& rhs) -> WrapperName& {                                                                                    \
            m_value %= rhs.m_value;                                                                                                                            \
            return *this;                                                                                                                                      \
        }                                                                                                                                                      \
                                                                                                                                                               \
        [[nodiscard]] auto atomic_load(MemOrder = MemOrder::Total) volatile -> WrapperName;                                                                    \
        auto               atomic_store(WrapperName, MemOrder = MemOrder::Total) volatile -> void;                                                             \
                                                                                                                                                               \
        auto atomic_add(WrapperName, MemOrder = MemOrder::Total) volatile -> void;                                                                             \
        auto atomic_sub(WrapperName, MemOrder = MemOrder::Total) volatile -> void;                                                                             \
                                                                                                                                                               \
        [[nodiscard]] auto atomic_fetch_add(WrapperName, MemOrder = MemOrder::Total) volatile -> WrapperName;                                                  \
        [[nodiscard]] auto atomic_fetch_sub(WrapperName, MemOrder = MemOrder::Total) volatile -> WrapperName;                                                  \
                                                                                                                                                               \
        [[nodiscard]] auto atomic_add_fetch(WrapperName, MemOrder = MemOrder::Total) volatile -> WrapperName;                                                  \
        [[nodiscard]] auto atomic_sub_fetch(WrapperName, MemOrder = MemOrder::Total) volatile -> WrapperName;                                                  \
                                                                                                                                                               \
        [[nodiscard]] auto hash_code() const -> usize;                                                                                                         \
                                                                                                                                                               \
    private:                                                                                                                                                   \
        real_integer_type m_value = 0;                                                                                                                         \
    };                                                                                                                                                         \
    static_assert(sizeof(WrapperName) == sizeof(real_integer_type));                                                                                           \
                                                                                                                                                               \
    auto operator<<(real_integer_type lhs, WrapperName rhs)->WrapperName {                                                                                     \
        return WrapperName{ lhs } << rhs;                                                                                                                      \
    }                                                                                                                                                          \
    auto operator>>(real_integer_type lhs, WrapperName rhs)->WrapperName {                                                                                     \
        return WrapperName{ lhs } >> rhs;                                                                                                                      \
    }                                                                                                                                                          \
                                                                                                                                                               \
    namespace Cxx {                                                                                                                                            \
    constexpr auto swap(WrapperName& lhs, WrapperName& rhs) -> void {                                                                                          \
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
OOUnsignedIntegerWrapper$(u64, __UINT32_TYPE__, __UINT64_MAX__);

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