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

#include <CCLang/Core/ErrorOr.hh>
#include <CCLang/Core/Meta.hh>
#include <CCLang/Lang/IntTypes.hh>
#include <CCLang/Lang/Try.hh>

#define OOCommonIntegerWrapperImpl$(WrapperName, real_integer_type)                                                                                            \
    auto WrapperName::max(WrapperName const& lhs, WrapperName const& rhs)->WrapperName {                                                                       \
        if ( lhs > rhs ) {                                                                                                                                     \
            return lhs;                                                                                                                                        \
        } else {                                                                                                                                               \
            return rhs;                                                                                                                                        \
        }                                                                                                                                                      \
    }                                                                                                                                                          \
    auto WrapperName::min(WrapperName const& lhs, WrapperName const& rhs)->WrapperName {                                                                       \
        if ( lhs < rhs ) {                                                                                                                                     \
            return lhs;                                                                                                                                        \
        } else {                                                                                                                                               \
            return rhs;                                                                                                                                        \
        }                                                                                                                                                      \
    }                                                                                                                                                          \
    auto WrapperName::range(WrapperName const& begin, WrapperName const& end)->Range<WrapperName> {                                                            \
        return Range<WrapperName>(begin, end);                                                                                                                 \
    }                                                                                                                                                          \
    auto WrapperName::range_inclusive(WrapperName const& begin, WrapperName const& last)->RangeInclusive<WrapperName> {                                        \
        return RangeInclusive<WrapperName>(begin, last);                                                                                                       \
    }                                                                                                                                                          \
    WrapperName::WrapperName(real_integer_type value)                                                                                                          \
        : m_value(value) {                                                                                                                                     \
    }                                                                                                                                                          \
    WrapperName::WrapperName(WrapperName const& rhs)                                                                                                           \
        : m_value(rhs.m_value) {                                                                                                                               \
    }                                                                                                                                                          \
    WrapperName::WrapperName(WrapperName&& rhs)                                                                                                                \
        : m_value(Cxx::exchange(rhs.m_value, 0)) {                                                                                                             \
    }                                                                                                                                                          \
    auto WrapperName::operator=(real_integer_type value)->WrapperName& {                                                                                       \
        WrapperName integer = value;                                                                                                                           \
        swap(integer);                                                                                                                                         \
        return *this;                                                                                                                                          \
    }                                                                                                                                                          \
    auto WrapperName::operator=(WrapperName const& rhs)->WrapperName& {                                                                                        \
        auto integer = rhs;                                                                                                                                    \
        swap(integer);                                                                                                                                         \
        return *this;                                                                                                                                          \
    }                                                                                                                                                          \
    auto WrapperName::operator=(WrapperName&& rhs)->WrapperName& {                                                                                             \
        auto integer = Cxx::move(rhs);                                                                                                                         \
        swap(integer);                                                                                                                                         \
        return *this;                                                                                                                                          \
    }                                                                                                                                                          \
    auto WrapperName::swap(WrapperName& rhs)->void {                                                                                                           \
        Cxx::swap(m_value, rhs.m_value);                                                                                                                       \
    }                                                                                                                                                          \
    auto WrapperName::bit_count() const->usize {                                                                                                               \
        return sizeof(real_integer_type) * 8;                                                                                                                  \
    }                                                                                                                                                          \
    auto WrapperName::unwrap() const->real_integer_type {                                                                                                      \
        return m_value;                                                                                                                                        \
    }                                                                                                                                                          \
    auto WrapperName::equal(WrapperName const& rhs) const->bool {                                                                                              \
        return m_value == rhs.m_value;                                                                                                                         \
    }                                                                                                                                                          \
    auto WrapperName::operator==(WrapperName const& rhs) const->bool {                                                                                         \
        return equal(rhs);                                                                                                                                     \
    }                                                                                                                                                          \
    auto WrapperName::not_equal(WrapperName const& rhs) const->bool {                                                                                          \
        return m_value != rhs.m_value;                                                                                                                         \
    }                                                                                                                                                          \
    auto WrapperName::operator!=(WrapperName const& rhs) const->bool {                                                                                         \
        return not_equal(rhs);                                                                                                                                 \
    }                                                                                                                                                          \
    auto WrapperName::greater(WrapperName const& rhs) const->bool {                                                                                            \
        return m_value > rhs.m_value;                                                                                                                          \
    }                                                                                                                                                          \
    auto WrapperName::operator>(WrapperName const& rhs) const->bool {                                                                                          \
        return greater(rhs);                                                                                                                                   \
    }                                                                                                                                                          \
    auto WrapperName::greater_equal(WrapperName const& rhs) const->bool {                                                                                      \
        return m_value >= rhs.m_value;                                                                                                                         \
    }                                                                                                                                                          \
    auto WrapperName::operator>=(WrapperName const& rhs) const->bool {                                                                                         \
        return greater_equal(rhs);                                                                                                                             \
    }                                                                                                                                                          \
    auto WrapperName::less(WrapperName const& rhs) const->bool {                                                                                               \
        return m_value < rhs.m_value;                                                                                                                          \
    }                                                                                                                                                          \
    auto WrapperName::operator<(WrapperName const& rhs) const->bool {                                                                                          \
        return less(rhs);                                                                                                                                      \
    }                                                                                                                                                          \
    auto WrapperName::less_equal(WrapperName const& rhs) const->bool {                                                                                         \
        return m_value <= rhs.m_value;                                                                                                                         \
    }                                                                                                                                                          \
    auto WrapperName::operator<=(WrapperName const& rhs) const->bool {                                                                                         \
        return less_equal(rhs);                                                                                                                                \
    }                                                                                                                                                          \
    auto WrapperName::bit_one_complement() const->WrapperName {                                                                                                \
        return ~m_value;                                                                                                                                       \
    }                                                                                                                                                          \
    auto WrapperName::operator~() const->WrapperName {                                                                                                         \
        return bit_one_complement();                                                                                                                           \
    }                                                                                                                                                          \
    auto WrapperName::bit_and(WrapperName const& rhs) const->WrapperName {                                                                                     \
        return m_value & rhs.m_value;                                                                                                                          \
    }                                                                                                                                                          \
    auto WrapperName::operator&(WrapperName const& rhs) const->WrapperName {                                                                                   \
        return bit_and(rhs);                                                                                                                                   \
    }                                                                                                                                                          \
    auto WrapperName::bit_and_assign(WrapperName const& rhs)->void {                                                                                           \
        m_value &= rhs.m_value;                                                                                                                                \
    }                                                                                                                                                          \
    auto WrapperName::operator&=(WrapperName const& rhs)->WrapperName& {                                                                                       \
        bit_and_assign(rhs);                                                                                                                                   \
        return *this;                                                                                                                                          \
    }                                                                                                                                                          \
    auto WrapperName::bit_or(WrapperName const& rhs) const->WrapperName {                                                                                      \
        return m_value | rhs.m_value;                                                                                                                          \
    }                                                                                                                                                          \
    auto WrapperName::operator|(WrapperName const& rhs) const->WrapperName {                                                                                   \
        return bit_or(rhs);                                                                                                                                    \
    }                                                                                                                                                          \
    auto WrapperName::bit_or_assign(WrapperName const& rhs)->void {                                                                                            \
        m_value |= rhs.m_value;                                                                                                                                \
    }                                                                                                                                                          \
    auto WrapperName::operator|=(WrapperName const& rhs)->WrapperName& {                                                                                       \
        bit_or_assign(rhs);                                                                                                                                    \
        return *this;                                                                                                                                          \
    }                                                                                                                                                          \
    auto WrapperName::bit_xor(WrapperName const& rhs) const->WrapperName {                                                                                     \
        return m_value ^ rhs.m_value;                                                                                                                          \
    }                                                                                                                                                          \
    auto WrapperName::operator^(WrapperName const& rhs) const->WrapperName {                                                                                   \
        return bit_xor(rhs);                                                                                                                                   \
    }                                                                                                                                                          \
    auto WrapperName::bit_xor_assign(WrapperName const& rhs)->void {                                                                                           \
        m_value ^= rhs.m_value;                                                                                                                                \
    }                                                                                                                                                          \
    auto WrapperName::operator^=(WrapperName const& rhs)->WrapperName& {                                                                                       \
        bit_xor_assign(rhs);                                                                                                                                   \
        return *this;                                                                                                                                          \
    }                                                                                                                                                          \
    auto WrapperName::try_left_shift(WrapperName const& rhs) const->ErrorOr<WrapperName> {                                                                     \
        if ( rhs >= bit_count().as<WrapperName>() )                                                                                                            \
            return Error::from_code(ErrorCode::ShiftOverflow);                                                                                                 \
        else                                                                                                                                                   \
            return WrapperName(static_cast<CCIntegerType>(m_value << rhs.m_value));                                                                            \
    }                                                                                                                                                          \
    auto WrapperName::left_shift(WrapperName const& rhs) const->WrapperName {                                                                                  \
        if constexpr ( cclang_debugging ) {                                                                                                                    \
            return must$(try_left_shift(rhs));                                                                                                                 \
        } else {                                                                                                                                               \
            return static_cast<CCIntegerType>(m_value << rhs.m_value);                                                                                         \
        }                                                                                                                                                      \
    }                                                                                                                                                          \
    auto WrapperName::operator<<(WrapperName const& rhs) const->WrapperName {                                                                                  \
        return left_shift(rhs);                                                                                                                                \
    }                                                                                                                                                          \
    auto WrapperName::try_left_shift_assign(WrapperName const& rhs)->ErrorOr<void> {                                                                           \
        if ( rhs >= bit_count().as<WrapperName>() )                                                                                                            \
            return Error::from_code(ErrorCode::ShiftOverflow);                                                                                                 \
        else {                                                                                                                                                 \
            m_value <<= rhs.m_value;                                                                                                                           \
            return {};                                                                                                                                         \
        }                                                                                                                                                      \
    }                                                                                                                                                          \
    auto WrapperName::left_shift_assign(WrapperName const& rhs)->void {                                                                                        \
        if constexpr ( cclang_debugging ) {                                                                                                                    \
            must$(try_left_shift_assign(rhs));                                                                                                                 \
        } else {                                                                                                                                               \
            m_value <<= rhs.m_value;                                                                                                                           \
        }                                                                                                                                                      \
    }                                                                                                                                                          \
    auto WrapperName::operator<<=(WrapperName const& rhs)->WrapperName& {                                                                                      \
        left_shift_assign(rhs);                                                                                                                                \
        return *this;                                                                                                                                          \
    }                                                                                                                                                          \
    auto WrapperName::try_right_shift(WrapperName const& rhs) const->ErrorOr<WrapperName> {                                                                    \
        if ( rhs >= bit_count().as<WrapperName>() )                                                                                                            \
            return Error::from_code(ErrorCode::ShiftOverflow);                                                                                                 \
        else                                                                                                                                                   \
            return WrapperName(static_cast<CCIntegerType>(m_value >> rhs.m_value));                                                                            \
    }                                                                                                                                                          \
    auto WrapperName::right_shift(WrapperName const& rhs) const->WrapperName {                                                                                 \
        if constexpr ( cclang_debugging ) {                                                                                                                    \
            return must$(try_right_shift(rhs));                                                                                                                \
        } else {                                                                                                                                               \
            return static_cast<CCIntegerType>(m_value >> rhs.m_value);                                                                                         \
        }                                                                                                                                                      \
    }                                                                                                                                                          \
    auto WrapperName::operator>>(WrapperName const& rhs) const->WrapperName {                                                                                  \
        return right_shift(rhs);                                                                                                                               \
    }                                                                                                                                                          \
    auto WrapperName::try_right_shift_assign(WrapperName const& rhs)->ErrorOr<void> {                                                                          \
        if ( rhs >= bit_count().as<WrapperName>() )                                                                                                            \
            return Error::from_code(ErrorCode::ShiftOverflow);                                                                                                 \
        else {                                                                                                                                                 \
            m_value >>= rhs.m_value;                                                                                                                           \
            return {};                                                                                                                                         \
        }                                                                                                                                                      \
    }                                                                                                                                                          \
    auto WrapperName::right_shift_assign(WrapperName const& rhs)->void {                                                                                       \
        if constexpr ( cclang_debugging ) {                                                                                                                    \
            must$(try_right_shift_assign(rhs));                                                                                                                \
        } else {                                                                                                                                               \
            m_value >>= rhs.m_value;                                                                                                                           \
        }                                                                                                                                                      \
    }                                                                                                                                                          \
    auto WrapperName::operator>>=(WrapperName const& rhs)->WrapperName& {                                                                                      \
        right_shift_assign(rhs);                                                                                                                               \
        return *this;                                                                                                                                          \
    }                                                                                                                                                          \
    auto WrapperName::operator++()->WrapperName& {                                                                                                             \
        add_assign(1);                                                                                                                                         \
        return *this;                                                                                                                                          \
    }                                                                                                                                                          \
    auto WrapperName::operator++(int)->WrapperName {                                                                                                           \
        auto __prev = *this;                                                                                                                                   \
        add_assign(1);                                                                                                                                         \
        return __prev;                                                                                                                                         \
    }                                                                                                                                                          \
                                                                                                                                                               \
    auto WrapperName::operator--()->WrapperName& {                                                                                                             \
        sub_assign(1);                                                                                                                                         \
        return *this;                                                                                                                                          \
    }                                                                                                                                                          \
    auto WrapperName::operator--(int)->WrapperName {                                                                                                           \
        auto __prev = *this;                                                                                                                                   \
        sub_assign(1);                                                                                                                                         \
        return __prev;                                                                                                                                         \
    }                                                                                                                                                          \
    auto WrapperName::try_add(WrapperName const& rhs) const->ErrorOr<WrapperName> {                                                                            \
        WrapperName::CCIntegerType __value;                                                                                                                    \
        if ( __builtin_add_overflow(m_value, rhs.m_value, &__value) )                                                                                          \
            return Error::from_code(ErrorCode::IntOverflow);                                                                                                   \
        else                                                                                                                                                   \
            return WrapperName(__value);                                                                                                                       \
    }                                                                                                                                                          \
    auto WrapperName::add(WrapperName const& rhs) const->WrapperName {                                                                                         \
        if constexpr ( cclang_debugging ) {                                                                                                                    \
            return must$(try_add(rhs));                                                                                                                        \
        } else {                                                                                                                                               \
            return m_value + rhs.m_value;                                                                                                                      \
        }                                                                                                                                                      \
    }                                                                                                                                                          \
    auto WrapperName::operator+(WrapperName const& rhs) const->WrapperName {                                                                                   \
        return add(rhs);                                                                                                                                       \
    }                                                                                                                                                          \
    auto WrapperName::try_add_assign(WrapperName const& rhs)->ErrorOr<void> {                                                                                  \
        m_value = try$(try_add(rhs)).m_value;                                                                                                                  \
        return {};                                                                                                                                             \
    }                                                                                                                                                          \
    auto WrapperName::add_assign(WrapperName const& rhs)->void {                                                                                               \
        if constexpr ( cclang_debugging ) {                                                                                                                    \
            must$(try_add_assign(rhs));                                                                                                                        \
        } else {                                                                                                                                               \
            m_value += rhs.m_value;                                                                                                                            \
        }                                                                                                                                                      \
    }                                                                                                                                                          \
    auto WrapperName::operator+=(WrapperName const& rhs)->WrapperName& {                                                                                       \
        add_assign(rhs);                                                                                                                                       \
        return *this;                                                                                                                                          \
    }                                                                                                                                                          \
    auto WrapperName::try_sub(WrapperName const& rhs) const->ErrorOr<WrapperName> {                                                                            \
        WrapperName::CCIntegerType __value;                                                                                                                    \
        if ( __builtin_sub_overflow(m_value, rhs.m_value, &__value) )                                                                                          \
            return Error::from_code(ErrorCode::IntOverflow);                                                                                                   \
        else                                                                                                                                                   \
            return WrapperName(__value);                                                                                                                       \
    }                                                                                                                                                          \
    auto WrapperName::sub(WrapperName const& rhs) const->WrapperName {                                                                                         \
        if constexpr ( cclang_debugging ) {                                                                                                                    \
            return must$(try_sub(rhs));                                                                                                                        \
        } else {                                                                                                                                               \
            return m_value - rhs.m_value;                                                                                                                      \
        }                                                                                                                                                      \
    }                                                                                                                                                          \
    auto WrapperName::operator-(WrapperName const& rhs) const->WrapperName {                                                                                   \
        return sub(rhs);                                                                                                                                       \
    }                                                                                                                                                          \
    auto WrapperName::try_sub_assign(WrapperName const& rhs)->ErrorOr<void> {                                                                                  \
        m_value = try$(try_sub(rhs)).m_value;                                                                                                                  \
        return {};                                                                                                                                             \
    }                                                                                                                                                          \
    auto WrapperName::sub_assign(WrapperName const& rhs)->void {                                                                                               \
        if constexpr ( cclang_debugging ) {                                                                                                                    \
            must$(try_sub_assign(rhs));                                                                                                                        \
        } else {                                                                                                                                               \
            m_value -= rhs.m_value;                                                                                                                            \
        }                                                                                                                                                      \
    }                                                                                                                                                          \
    auto WrapperName::operator-=(WrapperName const& rhs)->WrapperName& {                                                                                       \
        sub_assign(rhs);                                                                                                                                       \
        return *this;                                                                                                                                          \
    }                                                                                                                                                          \
    auto WrapperName::try_mul(WrapperName const& rhs) const->ErrorOr<WrapperName> {                                                                            \
        WrapperName::CCIntegerType __value;                                                                                                                    \
        if ( __builtin_mul_overflow(m_value, rhs.m_value, &__value) )                                                                                          \
            return Error::from_code(ErrorCode::IntOverflow);                                                                                                   \
        else                                                                                                                                                   \
            return WrapperName(__value);                                                                                                                       \
    }                                                                                                                                                          \
    auto WrapperName::mul(WrapperName const& rhs) const->WrapperName {                                                                                         \
        if constexpr ( cclang_debugging ) {                                                                                                                    \
            must$(try_mul(rhs));                                                                                                                               \
        } else {                                                                                                                                               \
            return m_value * rhs.m_value;                                                                                                                      \
        }                                                                                                                                                      \
    }                                                                                                                                                          \
    auto WrapperName::operator*(WrapperName const& rhs) const->WrapperName {                                                                                   \
        return mul(rhs);                                                                                                                                       \
    }                                                                                                                                                          \
    auto WrapperName::try_mul_assign(WrapperName const& rhs)->ErrorOr<void> {                                                                                  \
        m_value = try$(try_mul(rhs)).m_value;                                                                                                                  \
        return {};                                                                                                                                             \
    }                                                                                                                                                          \
    auto WrapperName::mul_assign(WrapperName const& rhs)->void {                                                                                               \
        if constexpr ( cclang_debugging ) {                                                                                                                    \
            must$(try_mul_assign(rhs));                                                                                                                        \
        } else {                                                                                                                                               \
            m_value *= rhs.m_value;                                                                                                                            \
        }                                                                                                                                                      \
    }                                                                                                                                                          \
    auto WrapperName::operator*=(WrapperName const& rhs)->WrapperName& {                                                                                       \
        mul_assign(rhs);                                                                                                                                       \
        return *this;                                                                                                                                          \
    }                                                                                                                                                          \
    auto WrapperName::try_div(WrapperName const& rhs) const->ErrorOr<WrapperName> {                                                                            \
        if ( rhs == 0 )                                                                                                                                        \
            return Error::from_code(ErrorCode::DivisionByZero);                                                                                                \
        else                                                                                                                                                   \
            return WrapperName(static_cast<CCIntegerType>(m_value / rhs.m_value));                                                                             \
    }                                                                                                                                                          \
    auto WrapperName::div(WrapperName const& rhs) const->WrapperName {                                                                                         \
        if constexpr ( cclang_debugging ) {                                                                                                                    \
            return must$(try_div(rhs));                                                                                                                        \
        } else {                                                                                                                                               \
            return m_value / rhs.m_value;                                                                                                                      \
        }                                                                                                                                                      \
    }                                                                                                                                                          \
    auto WrapperName::operator/(WrapperName const& rhs) const->WrapperName {                                                                                   \
        return div(rhs);                                                                                                                                       \
    }                                                                                                                                                          \
    auto WrapperName::try_div_assign(WrapperName const& rhs)->ErrorOr<void> {                                                                                  \
        m_value = try$(try_div(rhs)).m_value;                                                                                                                  \
        return {};                                                                                                                                             \
    }                                                                                                                                                          \
    auto WrapperName::div_assign(WrapperName const& rhs)->void {                                                                                               \
        if constexpr ( cclang_debugging ) {                                                                                                                    \
            must$(try_div_assign(rhs));                                                                                                                        \
        } else {                                                                                                                                               \
            m_value /= rhs.m_value;                                                                                                                            \
        }                                                                                                                                                      \
    }                                                                                                                                                          \
    auto WrapperName::operator/=(WrapperName const& rhs)->WrapperName& {                                                                                       \
        div_assign(rhs);                                                                                                                                       \
        return *this;                                                                                                                                          \
    }                                                                                                                                                          \
    auto WrapperName::operator%(WrapperName const& rhs) const->WrapperName {                                                                                   \
        return m_value % rhs.m_value;                                                                                                                          \
    }                                                                                                                                                          \
    auto WrapperName::operator%=(WrapperName const& rhs)->WrapperName& {                                                                                       \
        m_value %= rhs.m_value;                                                                                                                                \
        return *this;                                                                                                                                          \
    }                                                                                                                                                          \
    auto WrapperName::atomic_load(MemOrder mem_order) volatile->WrapperName {                                                                                  \
        CCIntegerType __value;                                                                                                                                 \
        __atomic_load(&m_value, &__value, static_cast<UnderlyingType<MemOrder>>(mem_order));                                                                   \
        return __value;                                                                                                                                        \
    }                                                                                                                                                          \
    auto WrapperName::atomic_store(WrapperName rhs, MemOrder mem_order) volatile->void {                                                                       \
        __atomic_store(&m_value, &rhs.m_value, static_cast<UnderlyingType<MemOrder>>(mem_order));                                                              \
    }                                                                                                                                                          \
    auto WrapperName::atomic_add(WrapperName rhs, MemOrder mem_order) volatile->void {                                                                         \
        (void)atomic_fetch_add(rhs, mem_order);                                                                                                                \
    }                                                                                                                                                          \
    auto WrapperName::atomic_sub(WrapperName rhs, MemOrder mem_order) volatile->void {                                                                         \
        (void)atomic_fetch_sub(rhs, mem_order);                                                                                                                \
    }                                                                                                                                                          \
    auto WrapperName::atomic_fetch_add(WrapperName rhs, MemOrder mem_order) volatile->WrapperName {                                                            \
        return __atomic_fetch_add(&m_value, rhs.m_value, static_cast<UnderlyingType<MemOrder>>(mem_order));                                                    \
    }                                                                                                                                                          \
    auto WrapperName::atomic_fetch_sub(WrapperName rhs, MemOrder mem_order) volatile->WrapperName {                                                            \
        return __atomic_fetch_sub(&m_value, rhs.m_value, static_cast<UnderlyingType<MemOrder>>(mem_order));                                                    \
    }                                                                                                                                                          \
    auto WrapperName::atomic_add_fetch(WrapperName rhs, MemOrder mem_order) volatile->WrapperName {                                                            \
        return __atomic_add_fetch(&m_value, rhs.m_value, static_cast<UnderlyingType<MemOrder>>(mem_order));                                                    \
    }                                                                                                                                                          \
    auto WrapperName::atomic_sub_fetch(WrapperName rhs, MemOrder mem_order) volatile->WrapperName {                                                            \
        return __atomic_sub_fetch(&m_value, rhs.m_value, static_cast<UnderlyingType<MemOrder>>(mem_order));                                                    \
    }                                                                                                                                                          \
    auto WrapperName::hash_code() const->usize {                                                                                                               \
        auto hash_key = as<usize>();                                                                                                                           \
        hash_key += ~(hash_key << 15);                                                                                                                         \
        hash_key ^= (hash_key >> 10);                                                                                                                          \
        hash_key += (hash_key << 3);                                                                                                                           \
        hash_key ^= (hash_key >> 6);                                                                                                                           \
        hash_key += ~(hash_key << 11);                                                                                                                         \
        hash_key ^= (hash_key >> 16);                                                                                                                          \
        return hash_key;                                                                                                                                       \
    }

#define OOUnsignedIntegerWrapperImpl$(WrapperName, real_integer_type)                                                                                          \
    auto WrapperName::is_signed() const->bool {                                                                                                                \
        return false;                                                                                                                                          \
    }                                                                                                                                                          \
    auto WrapperName::try_bit_at(WrapperName index) const->ErrorOr<bool> {                                                                                     \
        if ( index > bit_count().as<WrapperName>() )                                                                                                           \
            return Error::from_code(ErrorCode::IndexOutOfRange);                                                                                               \
        else                                                                                                                                                   \
            return (m_value & (1 << index.unwrap())) == (1 << index.unwrap());                                                                                 \
    }                                                                                                                                                          \
    auto WrapperName::bit_at(WrapperName index) const->bool {                                                                                                  \
        return must$(try_bit_at(index));                                                                                                                       \
    }                                                                                                                                                          \
    auto WrapperName::try_set_bit(WrapperName index, bool value)->ErrorOr<void> {                                                                              \
        if ( index > bit_count().as<WrapperName>() )                                                                                                           \
            return Error::from_code(ErrorCode::IndexOutOfRange);                                                                                               \
                                                                                                                                                               \
        if ( value )                                                                                                                                           \
            m_value |= 1 << index.unwrap();                                                                                                                    \
        else                                                                                                                                                   \
            m_value &= ~(1 << index.unwrap());                                                                                                                 \
        return {};                                                                                                                                             \
    }                                                                                                                                                          \
    auto WrapperName::set_bit(WrapperName index, bool value)->void {                                                                                           \
        must$(try_set_bit(index, value));                                                                                                                      \
    }                                                                                                                                                          \
    auto WrapperName::count_zeroes() const->usize {                                                                                                            \
        return 0; /* TODO implement */                                                                                                                         \
    }                                                                                                                                                          \
    auto WrapperName::count_ones() const->usize {                                                                                                              \
        return 0; /* TODO implement */                                                                                                                         \
    }                                                                                                                                                          \
    OOCommonIntegerWrapperImpl$(WrapperName, real_integer_type)

#define OOSignedIntegerWrapperImpl$(WrapperName, real_integer_type)                                                                                            \
    auto WrapperName::is_signed() const->bool {                                                                                                                \
        return true;                                                                                                                                           \
    }                                                                                                                                                          \
    OOCommonIntegerWrapperImpl$(WrapperName, real_integer_type)

OOUnsignedIntegerWrapperImpl$(u8, __UINT8_TYPE__);
OOUnsignedIntegerWrapperImpl$(u16, __UINT16_TYPE__);
OOUnsignedIntegerWrapperImpl$(u32, __UINT32_TYPE__);
OOUnsignedIntegerWrapperImpl$(u64, __UINT64_TYPE__);
OOUnsignedIntegerWrapperImpl$(usize, __SIZE_TYPE__);

OOSignedIntegerWrapperImpl$(i8, __INT8_TYPE__);
OOSignedIntegerWrapperImpl$(i16, __INT16_TYPE__);
OOSignedIntegerWrapperImpl$(i32, __INT32_TYPE__);
OOSignedIntegerWrapperImpl$(i64, __INT64_TYPE__);
OOSignedIntegerWrapperImpl$(isize, __PTRDIFF_TYPE__);
