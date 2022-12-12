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

#include <CCLang/Lang/IntTypes/i8.hh>

#include <CCLang/Core/ErrorOr.hh>
#include <CCLang/Lang/Cxx.hh>
#include <CCLang/Lang/IntTypes/usize.hh>
#include <CCLang/Lang/Must.hh>
#include <CCLang/Lang/Range.hh>
#include <CCLang/Lang/Try.hh>

auto i8::max(i8 const& lhs, i8 const& rhs) -> i8 {
    if ( lhs > rhs ) {
        return lhs;
    } else {
        return rhs;
    }
}

auto i8::min(i8 const& lhs, i8 const& rhs) -> i8 {
    if ( lhs < rhs ) {
        return lhs;
    } else {
        return rhs;
    }
}

auto i8::range(i8 const& begin, i8 const& end) -> Range<i8> {
    return Range<i8>(begin, end);
}

auto i8::range_inclusive(i8 const& begin, i8 const& last) -> RangeInclusive<i8> {
    return RangeInclusive<i8>(begin, last);
}

i8::i8(__INT8_TYPE__ value)
    : m_value(value) {
}

auto i8::operator=(__INT8_TYPE__ value) -> i8& {
    i8 integer = value;
    swap(integer);
    return *this;
}

auto i8::swap(i8& rhs) -> void {
    Cxx::swap(m_value, rhs.m_value);
}

auto i8::is_signed() const -> bool {
    return true;
}

auto i8::bit_count() const -> usize {
    return sizeof(__INT8_TYPE__) * 8;
}

auto i8::unwrap() const -> __INT8_TYPE__ {
    return m_value;
}

auto i8::equal(i8 const& rhs) const -> bool {
    return m_value == rhs.m_value;
}

auto i8::operator==(i8 const& rhs) const -> bool {
    return equal(rhs);
}

auto i8::not_equal(i8 const& rhs) const -> bool {
    return m_value != rhs.m_value;
}

auto i8::operator!=(i8 const& rhs) const -> bool {
    return not_equal(rhs);
}

auto i8::greater(i8 const& rhs) const -> bool {
    return m_value > rhs.m_value;
}

auto i8::operator>(i8 const& rhs) const -> bool {
    return greater(rhs);
}

auto i8::greater_equal(i8 const& rhs) const -> bool {
    return m_value >= rhs.m_value;
}

auto i8::operator>=(i8 const& rhs) const -> bool {
    return greater_equal(rhs);
}

auto i8::less(i8 const& rhs) const -> bool {
    return m_value < rhs.m_value;
}

auto i8::operator<(i8 const& rhs) const -> bool {
    return less(rhs);
}

auto i8::less_equal(i8 const& rhs) const -> bool {
    return m_value <= rhs.m_value;
}

auto i8::operator<=(i8 const& rhs) const -> bool {
    return less_equal(rhs);
}

auto i8::bit_one_complement() const -> i8 {
    return ~m_value;
}

auto i8::operator~() const -> i8 {
    return bit_one_complement();
}

auto i8::bit_and(i8 const& rhs) const -> i8 {
    return m_value & rhs.m_value;
}

auto i8::operator&(i8 const& rhs) const -> i8 {
    return bit_and(rhs);
}

auto i8::bit_and_assign(i8 const& rhs) -> void {
    m_value &= rhs.m_value;
}

auto i8::operator&=(i8 const& rhs) -> i8& {
    bit_and_assign(rhs);
    return *this;
}

auto i8::bit_or(i8 const& rhs) const -> i8 {
    return m_value | rhs.m_value;
}

auto i8::operator|(i8 const& rhs) const -> i8 {
    return bit_or(rhs);
}

auto i8::bit_or_assign(i8 const& rhs) -> void {
    m_value |= rhs.m_value;
}

auto i8::operator|=(i8 const& rhs) -> i8& {
    bit_or_assign(rhs);
    return *this;
}

auto i8::bit_xor(i8 const& rhs) const -> i8 {
    return m_value ^ rhs.m_value;
}

auto i8::operator^(i8 const& rhs) const -> i8 {
    return bit_xor(rhs);
}

auto i8::bit_xor_assign(i8 const& rhs) -> void {
    m_value ^= rhs.m_value;
}

auto i8::operator^=(i8 const& rhs) -> i8& {
    bit_xor_assign(rhs);
    return *this;
}

auto i8::try_left_shift(i8 const& rhs) const -> ErrorOr<i8> {
    if ( rhs >= bit_count().as<i8>() ) {
        return Error::from_code(ErrorCode::ShiftOverflow);
    } else {
        return i8(static_cast<CCIntegerType>(m_value << rhs.m_value));
    }
}

auto i8::left_shift(i8 const& rhs) const -> i8 {
    if constexpr ( CCLangSafeIntegerOperations ) {
        return must$(try_left_shift(rhs));
    } else {
        return static_cast<CCIntegerType>(m_value << rhs.m_value);
    }
}

auto i8::operator<<(i8 const& rhs) const -> i8 {
    return left_shift(rhs);
}

auto i8::try_left_shift_assign(i8 const& rhs) -> ErrorOr<void> {
    if ( rhs >= bit_count().as<i8>() ) {
        return Error::from_code(ErrorCode::ShiftOverflow);
    } else {
        m_value <<= rhs.m_value;
        return {};
    }
}

auto i8::left_shift_assign(i8 const& rhs) -> void {
    if constexpr ( CCLangSafeIntegerOperations ) {
        must$(try_left_shift_assign(rhs));
    } else {
        m_value <<= rhs.m_value;
    }
}

auto i8::operator<<=(i8 const& rhs) -> i8& {
    left_shift_assign(rhs);
    return *this;
}

auto i8::try_right_shift(i8 const& rhs) const -> ErrorOr<i8> {
    if ( rhs >= bit_count().as<i8>() ) {
        return Error::from_code(ErrorCode::ShiftOverflow);
    } else {
        return i8(static_cast<CCIntegerType>(m_value >> rhs.m_value));
    }
}

auto i8::right_shift(i8 const& rhs) const -> i8 {
    if constexpr ( CCLangSafeIntegerOperations ) {
        return must$(try_right_shift(rhs));
    } else {
        return static_cast<CCIntegerType>(m_value >> rhs.m_value);
    }
}

auto i8::operator>>(i8 const& rhs) const -> i8 {
    return right_shift(rhs);
}

auto i8::try_right_shift_assign(i8 const& rhs) -> ErrorOr<void> {
    if ( rhs >= bit_count().as<i8>() ) {
        return Error::from_code(ErrorCode::ShiftOverflow);
    } else {
        m_value >>= rhs.m_value;
        return {};
    }
}

auto i8::right_shift_assign(i8 const& rhs) -> void {
    if constexpr ( CCLangSafeIntegerOperations ) {
        must$(try_right_shift_assign(rhs));
    } else {
        m_value >>= rhs.m_value;
    }
}

auto i8::operator>>=(i8 const& rhs) -> i8& {
    right_shift_assign(rhs);
    return *this;
}

auto i8::operator++() -> i8& {
    add_assign(1);
    return *this;
}

auto i8::operator++(int) -> i8 {
    auto __prev = *this;
    add_assign(1);
    return __prev;
}

auto i8::operator--() -> i8& {
    sub_assign(1);
    return *this;
}

auto i8::operator--(int) -> i8 {
    auto __prev = *this;
    sub_assign(1);
    return __prev;
}

auto i8::try_add(i8 const& rhs) const -> ErrorOr<i8> {
    i8::CCIntegerType __value;
    if ( __builtin_add_overflow(m_value, rhs.m_value, &__value) ) {
        return Error::from_code(ErrorCode::IntOverflow);
    } else {
        return i8(__value);
    }
}

auto i8::add(i8 const& rhs) const -> i8 {
    if constexpr ( CCLangSafeIntegerOperations ) {
        return must$(try_add(rhs));
    } else {
        return m_value + rhs.m_value;
    }
}

auto i8::operator+(i8 const& rhs) const -> i8 {
    return add(rhs);
}

auto i8::try_add_assign(i8 const& rhs) -> ErrorOr<void> {
    m_value = try$(try_add(rhs)).m_value;
    return {};
}

auto i8::add_assign(i8 const& rhs) -> void {
    if constexpr ( CCLangSafeIntegerOperations ) {
        must$(try_add_assign(rhs));
    } else {
        m_value += rhs.m_value;
    }
}

auto i8::operator+=(i8 const& rhs) -> i8& {
    add_assign(rhs);
    return *this;
}

auto i8::try_sub(i8 const& rhs) const -> ErrorOr<i8> {
    i8::CCIntegerType __value;
    if ( __builtin_sub_overflow(m_value, rhs.m_value, &__value) ) {
        return Error::from_code(ErrorCode::IntOverflow);
    } else {
        return i8(__value);
    }
}

auto i8::sub(i8 const& rhs) const -> i8 {
    if constexpr ( CCLangSafeIntegerOperations ) {
        return must$(try_sub(rhs));
    } else {
        return m_value - rhs.m_value;
    }
}

auto i8::operator-(i8 const& rhs) const -> i8 {
    return sub(rhs);
}

auto i8::try_sub_assign(i8 const& rhs) -> ErrorOr<void> {
    m_value = try$(try_sub(rhs)).m_value;
    return {};
}

auto i8::sub_assign(i8 const& rhs) -> void {
    if constexpr ( CCLangSafeIntegerOperations ) {
        must$(try_sub_assign(rhs));
    } else {
        m_value -= rhs.m_value;
    }
}

auto i8::operator-=(i8 const& rhs) -> i8& {
    sub_assign(rhs);
    return *this;
}

auto i8::try_mul(i8 const& rhs) const -> ErrorOr<i8> {
    i8::CCIntegerType __value;
    if ( __builtin_mul_overflow(m_value, rhs.m_value, &__value) ) {
        return Error::from_code(ErrorCode::IntOverflow);
    } else {
        return i8(__value);
    }
}

auto i8::mul(i8 const& rhs) const -> i8 {
    if constexpr ( CCLangSafeIntegerOperations ) {
        return must$(try_mul(rhs));
    } else {
        return m_value * rhs.m_value;
    }
}

auto i8::operator*(i8 const& rhs) const -> i8 {
    return mul(rhs);
}

auto i8::try_mul_assign(i8 const& rhs) -> ErrorOr<void> {
    m_value = try$(try_mul(rhs)).m_value;
    return {};
}

auto i8::mul_assign(i8 const& rhs) -> void {
    if constexpr ( CCLangSafeIntegerOperations ) {
        must$(try_mul_assign(rhs));
    } else {
        m_value *= rhs.m_value;
    }
}

auto i8::operator*=(i8 const& rhs) -> i8& {
    mul_assign(rhs);
    return *this;
}

auto i8::try_div(i8 const& rhs) const -> ErrorOr<i8> {
    if ( rhs == 0 ) {
        return Error::from_code(ErrorCode::DivisionByZero);
    } else {
        return i8(static_cast<CCIntegerType>(m_value / rhs.m_value));
    }
}

auto i8::div(i8 const& rhs) const -> i8 {
    if constexpr ( CCLangSafeIntegerOperations ) {
        return must$(try_div(rhs));
    } else {
        return m_value / rhs.m_value;
    }
}

auto i8::operator/(i8 const& rhs) const -> i8 {
    return div(rhs);
}

auto i8::try_div_assign(i8 const& rhs) -> ErrorOr<void> {
    m_value = try$(try_div(rhs)).m_value;
    return {};
}

auto i8::div_assign(i8 const& rhs) -> void {
    if constexpr ( CCLangSafeIntegerOperations ) {
        must$(try_div_assign(rhs));
    } else {
        m_value /= rhs.m_value;
    }
}

auto i8::operator/=(i8 const& rhs) -> i8& {
    div_assign(rhs);
    return *this;
}

auto i8::operator%(i8 const& rhs) const -> i8 {
    return m_value % rhs.m_value;
}

auto i8::operator%=(i8 const& rhs) -> i8& {
    m_value %= rhs.m_value;
    return *this;
}

auto i8::atomic_load(MemOrder mem_order) volatile -> i8 {
    CCIntegerType __value;
    __atomic_load(&m_value, &__value, static_cast<UnderlyingType<MemOrder>>(mem_order));
    return __value;
}

auto i8::atomic_store(i8 rhs, MemOrder mem_order) volatile -> void {
    __atomic_store(&m_value, &rhs.m_value, static_cast<UnderlyingType<MemOrder>>(mem_order));
}

auto i8::atomic_add(i8 rhs, MemOrder mem_order) volatile -> void {
    (void)atomic_fetch_add(rhs, mem_order);
}

auto i8::atomic_sub(i8 rhs, MemOrder mem_order) volatile -> void {
    (void)atomic_fetch_sub(rhs, mem_order);
}

auto i8::atomic_fetch_add(i8 rhs, MemOrder mem_order) volatile -> i8 {
    return __atomic_fetch_add(&m_value, rhs.m_value, static_cast<UnderlyingType<MemOrder>>(mem_order));
}

auto i8::atomic_fetch_sub(i8 rhs, MemOrder mem_order) volatile -> i8 {
    return __atomic_fetch_sub(&m_value, rhs.m_value, static_cast<UnderlyingType<MemOrder>>(mem_order));
}

auto i8::atomic_add_fetch(i8 rhs, MemOrder mem_order) volatile -> i8 {
    return __atomic_add_fetch(&m_value, rhs.m_value, static_cast<UnderlyingType<MemOrder>>(mem_order));
}

auto i8::atomic_sub_fetch(i8 rhs, MemOrder mem_order) volatile -> i8 {
    return __atomic_sub_fetch(&m_value, rhs.m_value, static_cast<UnderlyingType<MemOrder>>(mem_order));
}

auto i8::hash_code() const -> usize {
    auto hash_key = as<usize>();
    hash_key += ~(hash_key << 15);
    hash_key ^= (hash_key >> 10);
    hash_key += (hash_key << 3);
    hash_key ^= (hash_key >> 6);
    hash_key += ~(hash_key << 11);
    hash_key ^= (hash_key >> 16);
    return hash_key;
}
