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

#define OOSignedIntegerWrapper$(WrapperName, real_integer_type, max_value, min_value)                                                                          \
    class [[gnu::packed]] WrapperName final {                                                                                                                  \
    public:                                                                                                                                                    \
        using CCIntegerType = real_integer_type;                                                                                                               \
                                                                                                                                                               \
    public:                                                                                                                                                    \
        static constexpr auto max() -> WrapperName { return max_value; }                                                                                       \
        static constexpr auto min() -> WrapperName { return min_value; }                                                                                       \
                                                                                                                                                               \
        constexpr explicit(false) WrapperName() = default;                                                                                                     \
        constexpr explicit(false) WrapperName(real_integer_type value)                                                                                         \
            : m_value{ value } {}                                                                                                                              \
        constexpr explicit(false) WrapperName(WrapperName const& rhs)                                                                                          \
            : m_value{ rhs.m_value } {}                                                                                                                        \
        constexpr explicit(false) WrapperName(WrapperName&& rhs)                                                                                               \
            : m_value{ Cxx::exchange(rhs.m_value, 0) } {}                                                                                                      \
                                                                                                                                                               \
        constexpr ~WrapperName() = default;                                                                                                                    \
                                                                                                                                                               \
        constexpr auto operator=(real_integer_type value) -> WrapperName& {                                                                                    \
            WrapperName integer{ value };                                                                                                                      \
            swap(integer);                                                                                                                                     \
            return *this;                                                                                                                                      \
        }                                                                                                                                                      \
        constexpr auto operator=(WrapperName const& rhs) -> WrapperName& {                                                                                     \
            WrapperName integer{ rhs };                                                                                                                        \
            swap(integer);                                                                                                                                     \
            return *this;                                                                                                                                      \
        }                                                                                                                                                      \
        constexpr auto operator=(WrapperName&& rhs) -> WrapperName& {                                                                                          \
            WrapperName integer{ Cxx::move(rhs) };                                                                                                             \
            swap(integer);                                                                                                                                     \
            return *this;                                                                                                                                      \
        }                                                                                                                                                      \
                                                                                                                                                               \
        constexpr auto swap(WrapperName& rhs) -> void { Cxx::swap(m_value, rhs.m_value); }                                                                     \
                                                                                                                                                               \
        constexpr auto bit_count() const -> usize { return sizeof(real_integer_type) * 8; }                                                                    \
                                                                                                                                                               \
        constexpr auto is_signed() const -> bool { return true; }                                                                                              \
                                                                                                                                                               \
        template<typename TInteger>                                                                                                                            \
        constexpr auto as() -> TInteger {                                                                                                                      \
            return TInteger{ static_cast<typename TInteger::CCIntegerType>(m_value) };                                                                         \
        }                                                                                                                                                      \
                                                                                                                                                               \
        constexpr auto unwrap() const -> real_integer_type { return m_value; }                                                                                 \
                                                                                                                                                               \
        constexpr auto equal(WrapperName const& rhs) const -> bool { return m_value == rhs.m_value; }                                                          \
        constexpr auto operator==(WrapperName const& rhs) const -> bool { return equal(rhs); }                                                                 \
                                                                                                                                                               \
        constexpr auto not_equal(WrapperName const& rhs) const -> bool { return m_value != rhs.m_value; }                                                      \
        constexpr auto operator!=(WrapperName const& rhs) const -> bool { return not_equal(rhs); }                                                             \
                                                                                                                                                               \
        constexpr auto greater(WrapperName const& rhs) const -> bool { return m_value > rhs.m_value; }                                                         \
        constexpr auto operator>(WrapperName const& rhs) const -> bool { return greater(rhs); }                                                                \
                                                                                                                                                               \
        constexpr auto greater_equal(WrapperName const& rhs) const -> bool { return m_value >= rhs.m_value; }                                                  \
        constexpr auto operator>=(WrapperName const& rhs) const -> bool { return greater_equal(rhs); }                                                         \
                                                                                                                                                               \
        constexpr auto less(WrapperName const& rhs) const -> bool { return m_value < rhs.m_value; }                                                            \
        constexpr auto operator<(WrapperName const& rhs) const -> bool { return less(rhs); }                                                                   \
                                                                                                                                                               \
        constexpr auto less_equal(WrapperName const& rhs) const -> bool { return m_value <= rhs.m_value; }                                                     \
        constexpr auto operator<=(WrapperName const& rhs) const -> bool { return less_equal(rhs); }                                                            \
                                                                                                                                                               \
        constexpr auto bit_one_complement() const -> WrapperName { return ~m_value; }                                                                          \
        constexpr auto operator~() const -> WrapperName { return bit_one_complement(); }                                                                       \
                                                                                                                                                               \
        constexpr auto bit_and(WrapperName const& rhs) const -> WrapperName { return m_value & rhs.m_value; }                                                  \
        constexpr auto operator&(WrapperName const& rhs) const -> WrapperName { return bit_and(rhs); }                                                         \
                                                                                                                                                               \
        constexpr auto bit_and_assign(WrapperName const& rhs) -> void { m_value &= rhs.m_value; }                                                              \
        constexpr auto operator&=(WrapperName const& rhs) -> WrapperName& {                                                                                    \
            bit_and_assign(rhs);                                                                                                                               \
            return *this;                                                                                                                                      \
        }                                                                                                                                                      \
                                                                                                                                                               \
        constexpr auto bit_or(WrapperName const& rhs) const -> WrapperName { return m_value | rhs.m_value; }                                                   \
        constexpr auto operator|(WrapperName const& rhs) const -> WrapperName { return bit_or(rhs); }                                                          \
                                                                                                                                                               \
        constexpr auto bit_or_assign(WrapperName const& rhs) -> void { m_value |= rhs.m_value; }                                                               \
        constexpr auto operator|=(WrapperName const& rhs) -> WrapperName& {                                                                                    \
            bit_or_assign(rhs);                                                                                                                                \
            return *this;                                                                                                                                      \
        }                                                                                                                                                      \
                                                                                                                                                               \
        constexpr auto bit_xor(WrapperName const& rhs) const -> WrapperName { return m_value ^ rhs.m_value; }                                                  \
        constexpr auto operator^(WrapperName const& rhs) const -> WrapperName { return bit_xor(rhs); }                                                         \
                                                                                                                                                               \
        constexpr auto bit_xor_assign(WrapperName const& rhs) -> void { m_value ^= rhs.m_value; }                                                              \
        constexpr auto operator^=(WrapperName const& rhs) -> WrapperName& {                                                                                    \
            bit_xor_assign(rhs);                                                                                                                               \
            return *this;                                                                                                                                      \
        }                                                                                                                                                      \
                                                                                                                                                               \
        auto try_left_shift(WrapperName const& rhs) const -> ErrorOr<WrapperName>;                                                                             \
        auto left_shift(WrapperName const& rhs) const -> WrapperName;                                                                                          \
        auto operator<<(WrapperName const& rhs) const -> WrapperName { return left_shift(rhs); }                                                               \
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
        auto operator>>(WrapperName const& rhs) const -> WrapperName { return right_shift(rhs); }                                                              \
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
            WrapperName __prev{ *this };                                                                                                                       \
            add_assign(1);                                                                                                                                     \
            return __prev;                                                                                                                                     \
        }                                                                                                                                                      \
                                                                                                                                                               \
        auto operator--() -> WrapperName& {                                                                                                                    \
            sub_assign(1);                                                                                                                                     \
            return *this;                                                                                                                                      \
        }                                                                                                                                                      \
        auto operator--(int) -> WrapperName& {                                                                                                                 \
            WrapperName __prev{ *this };                                                                                                                       \
            sub_assign(1);                                                                                                                                     \
            return __prev;                                                                                                                                     \
        }                                                                                                                                                      \
                                                                                                                                                               \
        auto try_add(WrapperName const&) const -> ErrorOr<WrapperName>;                                                                                        \
        auto add(WrapperName const& rhs) const -> WrapperName;                                                                                                 \
        auto operator+(WrapperName const& rhs) const -> WrapperName { return add(rhs); }                                                                       \
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
        auto operator-(WrapperName const& rhs) const -> WrapperName { return sub(rhs); }                                                                       \
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
        auto operator*(WrapperName const& rhs) const -> WrapperName { return mul(rhs); }                                                                       \
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
        auto operator/(WrapperName const& rhs) const -> WrapperName { return div(rhs); }                                                                       \
                                                                                                                                                               \
        auto try_div_assign(WrapperName const&) -> ErrorOr<void>;                                                                                              \
        auto div_assign(WrapperName const& rhs) -> void;                                                                                                       \
        auto operator/=(WrapperName const& rhs) -> WrapperName& {                                                                                              \
            div_assign(rhs);                                                                                                                                   \
            return *this;                                                                                                                                      \
        }                                                                                                                                                      \
                                                                                                                                                               \
        constexpr auto operator%(WrapperName const& rhs) const -> WrapperName { return m_value % rhs.m_value; }                                                \
        constexpr auto operator%=(WrapperName const& rhs) -> WrapperName& {                                                                                    \
            m_value %= rhs.m_value;                                                                                                                            \
            return *this;                                                                                                                                      \
        }                                                                                                                                                      \
                                                                                                                                                               \
    private:                                                                                                                                                   \
        real_integer_type m_value{ 0 };                                                                                                                        \
    };                                                                                                                                                         \
    static_assert(sizeof(WrapperName) == sizeof(real_integer_type));                                                                                           \
                                                                                                                                                               \
    namespace Cxx {                                                                                                                                            \
    constexpr auto swap(WrapperName& lhs, WrapperName& rhs) -> void { lhs.swap(rhs); }                                                                         \
    }

#define OOUnsignedIntegerWrapper$(WrapperName, real_integer_type, max_value)                                                                                   \
    class [[gnu::packed]] WrapperName final {                                                                                                                  \
    public:                                                                                                                                                    \
        using CCIntegerType = real_integer_type;                                                                                                               \
                                                                                                                                                               \
    public:                                                                                                                                                    \
        static constexpr auto max() -> WrapperName { return max_value; }                                                                                       \
        static constexpr auto min() -> WrapperName { return 0; }                                                                                               \
                                                                                                                                                               \
        constexpr explicit(false) WrapperName() = default;                                                                                                     \
        constexpr explicit(false) WrapperName(real_integer_type value)                                                                                         \
            : m_value{ value } {}                                                                                                                              \
        constexpr explicit(false) WrapperName(WrapperName const& rhs)                                                                                          \
            : m_value{ rhs.m_value } {}                                                                                                                        \
        constexpr explicit(false) WrapperName(WrapperName&& rhs)                                                                                               \
            : m_value{ Cxx::exchange(rhs.m_value, 0) } {}                                                                                                      \
                                                                                                                                                               \
        constexpr ~WrapperName() = default;                                                                                                                    \
                                                                                                                                                               \
        constexpr auto operator=(real_integer_type value) -> WrapperName& {                                                                                    \
            WrapperName integer{ value };                                                                                                                      \
            swap(integer);                                                                                                                                     \
            return *this;                                                                                                                                      \
        }                                                                                                                                                      \
        constexpr auto operator=(WrapperName const& rhs) -> WrapperName& {                                                                                     \
            WrapperName integer{ rhs };                                                                                                                        \
            swap(integer);                                                                                                                                     \
            return *this;                                                                                                                                      \
        }                                                                                                                                                      \
        constexpr auto operator=(WrapperName&& rhs) -> WrapperName& {                                                                                          \
            WrapperName integer{ Cxx::move(rhs) };                                                                                                             \
            swap(integer);                                                                                                                                     \
            return *this;                                                                                                                                      \
        }                                                                                                                                                      \
                                                                                                                                                               \
        constexpr auto swap(WrapperName& rhs) -> void { Cxx::swap(m_value, rhs.m_value); }                                                                     \
                                                                                                                                                               \
        constexpr auto is_signed() const -> bool { return false; }                                                                                             \
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
        constexpr auto bit_count() const -> usize { return sizeof(real_integer_type) * 8; }                                                                    \
                                                                                                                                                               \
        template<typename TInteger>                                                                                                                            \
        constexpr auto as() const -> TInteger {                                                                                                                \
            return TInteger{ static_cast<typename TInteger::CCIntegerType>(m_value) };                                                                         \
        }                                                                                                                                                      \
                                                                                                                                                               \
        constexpr auto unwrap() const -> real_integer_type { return m_value; }                                                                                 \
                                                                                                                                                               \
        constexpr auto equal(WrapperName const& rhs) const -> bool { return m_value == rhs.m_value; }                                                          \
        constexpr auto operator==(WrapperName const& rhs) const -> bool { return equal(rhs); }                                                                 \
                                                                                                                                                               \
        constexpr auto not_equal(WrapperName const& rhs) const -> bool { return m_value != rhs.m_value; }                                                      \
        constexpr auto operator!=(WrapperName const& rhs) const -> bool { return not_equal(rhs); }                                                             \
                                                                                                                                                               \
        constexpr auto greater(WrapperName const& rhs) const -> bool { return m_value > rhs.m_value; }                                                         \
        constexpr auto operator>(WrapperName const& rhs) const -> bool { return greater(rhs); }                                                                \
                                                                                                                                                               \
        constexpr auto greater_equal(WrapperName const& rhs) const -> bool { return m_value >= rhs.m_value; }                                                  \
        constexpr auto operator>=(WrapperName const& rhs) const -> bool { return greater_equal(rhs); }                                                         \
                                                                                                                                                               \
        constexpr auto less(WrapperName const& rhs) const -> bool { return m_value < rhs.m_value; }                                                            \
        constexpr auto operator<(WrapperName const& rhs) const -> bool { return less(rhs); }                                                                   \
                                                                                                                                                               \
        constexpr auto less_equal(WrapperName const& rhs) const -> bool { return m_value <= rhs.m_value; }                                                     \
        constexpr auto operator<=(WrapperName const& rhs) const -> bool { return less_equal(rhs); }                                                            \
                                                                                                                                                               \
        constexpr auto bit_one_complement() const -> WrapperName { return ~m_value; }                                                                          \
        constexpr auto operator~() const -> WrapperName { return bit_one_complement(); }                                                                       \
                                                                                                                                                               \
        constexpr auto bit_and(WrapperName const& rhs) const -> WrapperName { return m_value & rhs.m_value; }                                                  \
        constexpr auto operator&(WrapperName const& rhs) const -> WrapperName { return bit_and(rhs); }                                                         \
                                                                                                                                                               \
        constexpr auto bit_and_assign(WrapperName const& rhs) -> void { m_value &= rhs.m_value; }                                                              \
        constexpr auto operator&=(WrapperName const& rhs) -> WrapperName& {                                                                                    \
            bit_and_assign(rhs);                                                                                                                               \
            return *this;                                                                                                                                      \
        }                                                                                                                                                      \
                                                                                                                                                               \
        constexpr auto bit_or(WrapperName const& rhs) const -> WrapperName { return m_value | rhs.m_value; }                                                   \
        constexpr auto operator|(WrapperName const& rhs) const -> WrapperName { return bit_or(rhs); }                                                          \
                                                                                                                                                               \
        constexpr auto bit_or_assign(WrapperName const& rhs) -> void { m_value |= rhs.m_value; }                                                               \
        constexpr auto operator|=(WrapperName const& rhs) -> WrapperName& {                                                                                    \
            bit_or_assign(rhs);                                                                                                                                \
            return *this;                                                                                                                                      \
        }                                                                                                                                                      \
                                                                                                                                                               \
        constexpr auto bit_xor(WrapperName const& rhs) const -> WrapperName { return m_value ^ rhs.m_value; }                                                  \
        constexpr auto operator^(WrapperName const& rhs) const -> WrapperName { return bit_xor(rhs); }                                                         \
                                                                                                                                                               \
        constexpr auto bit_xor_assign(WrapperName const& rhs) -> void { m_value ^= rhs.m_value; }                                                              \
        constexpr auto operator^=(WrapperName const& rhs) -> WrapperName& {                                                                                    \
            bit_xor_assign(rhs);                                                                                                                               \
            return *this;                                                                                                                                      \
        }                                                                                                                                                      \
                                                                                                                                                               \
        auto try_left_shift(WrapperName const& rhs) const -> ErrorOr<WrapperName>;                                                                             \
        auto left_shift(WrapperName const& rhs) const -> WrapperName;                                                                                          \
        auto operator<<(WrapperName const& rhs) const -> WrapperName { return left_shift(rhs); }                                                               \
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
        auto operator>>(WrapperName const& rhs) const -> WrapperName { return right_shift(rhs); }                                                              \
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
            WrapperName __prev{ *this };                                                                                                                       \
            add_assign(1);                                                                                                                                     \
            return __prev;                                                                                                                                     \
        }                                                                                                                                                      \
                                                                                                                                                               \
        auto operator--() -> WrapperName& {                                                                                                                    \
            sub_assign(1);                                                                                                                                     \
            return *this;                                                                                                                                      \
        }                                                                                                                                                      \
        auto operator--(int) -> WrapperName& {                                                                                                                 \
            WrapperName __prev{ *this };                                                                                                                       \
            sub_assign(1);                                                                                                                                     \
            return __prev;                                                                                                                                     \
        }                                                                                                                                                      \
                                                                                                                                                               \
        auto try_add(WrapperName const&) const -> ErrorOr<WrapperName>;                                                                                        \
        auto add(WrapperName const& rhs) const -> WrapperName;                                                                                                 \
        auto operator+(WrapperName const& rhs) const -> WrapperName { return add(rhs); }                                                                       \
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
        auto operator-(WrapperName const& rhs) const -> WrapperName { return sub(rhs); }                                                                       \
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
        auto operator*(WrapperName const& rhs) const -> WrapperName { return mul(rhs); }                                                                       \
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
        auto operator/(WrapperName const& rhs) const -> WrapperName { return div(rhs); }                                                                       \
                                                                                                                                                               \
        auto try_div_assign(WrapperName const&) -> ErrorOr<void>;                                                                                              \
        auto div_assign(WrapperName const& rhs) -> void;                                                                                                       \
        auto operator/=(WrapperName const& rhs) -> WrapperName& {                                                                                              \
            div_assign(rhs);                                                                                                                                   \
            return *this;                                                                                                                                      \
        }                                                                                                                                                      \
                                                                                                                                                               \
        constexpr auto operator%(WrapperName const& rhs) const -> WrapperName { return m_value % rhs.m_value; }                                                \
        constexpr auto operator%=(WrapperName const& rhs) -> WrapperName& {                                                                                    \
            m_value %= rhs.m_value;                                                                                                                            \
            return *this;                                                                                                                                      \
        }                                                                                                                                                      \
                                                                                                                                                               \
    private:                                                                                                                                                   \
        real_integer_type m_value{ 0 };                                                                                                                        \
    };                                                                                                                                                         \
    static_assert(sizeof(WrapperName) == sizeof(real_integer_type));                                                                                           \
                                                                                                                                                               \
    auto operator<<(real_integer_type lhs, WrapperName rhs)->WrapperName { return WrapperName{ lhs } << rhs; }                                                 \
    auto operator>>(real_integer_type lhs, WrapperName rhs)->WrapperName { return WrapperName{ lhs } >> rhs; }                                                 \
                                                                                                                                                               \
    namespace Cxx {                                                                                                                                            \
    constexpr auto swap(WrapperName& lhs, WrapperName& rhs) -> void { lhs.swap(rhs); }                                                                         \
    }
