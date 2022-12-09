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

#include <CCLang/Lang/IntTypes/i64.hh>

#include <CCLang/Core/ErrorOr.hh>
#include <CCLang/Lang/Cxx.hh>
#include <CCLang/Lang/IntTypes/usize.hh>
#include <CCLang/Lang/Must.hh>
#include <CCLang/Lang/Range.hh>
#include <CCLang/Lang/Try.hh>

auto i64::max(i64 const& lhs, i64 const& rhs) -> i64 {
    if ( lhs > rhs ) {
        return lhs;
    } else {
        return rhs;
    }
}

auto i64::min(i64 const& lhs, i64 const& rhs) -> i64 {
    if ( lhs < rhs ) {
        return lhs;
    } else {
        return rhs;
    }
}

auto i64::range(i64 const& begin, i64 const& end) -> Range<i64> {
    return Range<i64>(begin, end);
}

auto i64::range_inclusive(i64 const& begin, i64 const& last) -> RangeInclusive<i64> {
    return RangeInclusive<i64>(begin, last);
}

i64::i64(__INT64_TYPE__ value)
    : m_value(value) {
}

i64::i64(i64&& rhs)
    : m_value(Cxx::exchange(rhs.m_value, 0)) {
}

auto i64::operator=(__INT64_TYPE__ value) -> i64& {
    i64 integer = value;
    swap(integer);
    return *this;
}

auto i64::operator=(i64 const& rhs) -> i64& {
    auto integer = rhs;
    swap(integer);
    return *this;
}

auto i64::operator=(i64&& rhs) -> i64& {
    auto integer = Cxx::move(rhs);
    swap(integer);
    return *this;
}

auto i64::swap(i64& rhs) -> void {
    Cxx::swap(m_value, rhs.m_value);
}

auto i64::is_signed() const -> bool {
    return true;
}

auto i64::bit_count() const -> usize {
    return sizeof(__INT64_TYPE__) * 8;
}

auto i64::unwrap() const -> __INT64_TYPE__ {
    return m_value;
}

auto i64::equal(i64 const& rhs) const -> bool {
    return m_value == rhs.m_value;
}

auto i64::operator==(i64 const& rhs) const -> bool {
    return equal(rhs);
}

auto i64::not_equal(i64 const& rhs) const -> bool {
    return m_value != rhs.m_value;
}

auto i64::operator!=(i64 const& rhs) const -> bool {
    return not_equal(rhs);
}

auto i64::greater(i64 const& rhs) const -> bool {
    return m_value > rhs.m_value;
}

auto i64::operator>(i64 const& rhs) const -> bool {
    return greater(rhs);
}

auto i64::greater_equal(i64 const& rhs) const -> bool {
    return m_value >= rhs.m_value;
}

auto i64::operator>=(i64 const& rhs) const -> bool {
    return greater_equal(rhs);
}

auto i64::less(i64 const& rhs) const -> bool {
    return m_value < rhs.m_value;
}

auto i64::operator<(i64 const& rhs) const -> bool {
    return less(rhs);
}

auto i64::less_equal(i64 const& rhs) const -> bool {
    return m_value <= rhs.m_value;
}

auto i64::operator<=(i64 const& rhs) const -> bool {
    return less_equal(rhs);
}

auto i64::bit_one_complement() const -> i64 {
    return ~m_value;
}

auto i64::operator~() const -> i64 {
    return bit_one_complement();
}

auto i64::bit_and(i64 const& rhs) const -> i64 {
    return m_value & rhs.m_value;
}

auto i64::operator&(i64 const& rhs) const -> i64 {
    return bit_and(rhs);
}

auto i64::bit_and_assign(i64 const& rhs) -> void {
    m_value &= rhs.m_value;
}

auto i64::operator&=(i64 const& rhs) -> i64& {
    bit_and_assign(rhs);
    return *this;
}

auto i64::bit_or(i64 const& rhs) const -> i64 {
    return m_value | rhs.m_value;
}

auto i64::operator|(i64 const& rhs) const -> i64 {
    return bit_or(rhs);
}

auto i64::bit_or_assign(i64 const& rhs) -> void {
    m_value |= rhs.m_value;
}

auto i64::operator|=(i64 const& rhs) -> i64& {
    bit_or_assign(rhs);
    return *this;
}

auto i64::bit_xor(i64 const& rhs) const -> i64 {
    return m_value ^ rhs.m_value;
}

auto i64::operator^(i64 const& rhs) const -> i64 {
    return bit_xor(rhs);
}

auto i64::bit_xor_assign(i64 const& rhs) -> void {
    m_value ^= rhs.m_value;
}

auto i64::operator^=(i64 const& rhs) -> i64& {
    bit_xor_assign(rhs);
    return *this;
}

auto i64::try_left_shift(i64 const& rhs) const -> ErrorOr<i64> {
    if ( rhs >= bit_count().as<i64>() ) {
        return Error::from_code(ErrorCode::ShiftOverflow);
    } else {
        return i64(static_cast<CCIntegerType>(m_value << rhs.m_value));
    }
}

auto i64::left_shift(i64 const& rhs) const -> i64 {
    if constexpr ( CCLangSafeIntegerOperations ) {
        return must$(try_left_shift(rhs));
    } else {
        return static_cast<CCIntegerType>(m_value << rhs.m_value);
    }
}

auto i64::operator<<(i64 const& rhs) const -> i64 {
    return left_shift(rhs);
}

auto i64::try_left_shift_assign(i64 const& rhs) -> ErrorOr<void> {
    if ( rhs >= bit_count().as<i64>() ) {
        return Error::from_code(ErrorCode::ShiftOverflow);
    } else {
        m_value <<= rhs.m_value;
        return {};
    }
}

auto i64::left_shift_assign(i64 const& rhs) -> void {
    if constexpr ( CCLangSafeIntegerOperations ) {
        must$(try_left_shift_assign(rhs));
    } else {
        m_value <<= rhs.m_value;
    }
}

auto i64::operator<<=(i64 const& rhs) -> i64& {
    left_shift_assign(rhs);
    return *this;
}

auto i64::try_right_shift(i64 const& rhs) const -> ErrorOr<i64> {
    if ( rhs >= bit_count().as<i64>() ) {
        return Error::from_code(ErrorCode::ShiftOverflow);
    } else {
        return i64(static_cast<CCIntegerType>(m_value >> rhs.m_value));
    }
}

auto i64::right_shift(i64 const& rhs) const -> i64 {
    if constexpr ( CCLangSafeIntegerOperations ) {
        return must$(try_right_shift(rhs));
    } else {
        return static_cast<CCIntegerType>(m_value >> rhs.m_value);
    }
}

auto i64::operator>>(i64 const& rhs) const -> i64 {
    return right_shift(rhs);
}

auto i64::try_right_shift_assign(i64 const& rhs) -> ErrorOr<void> {
    if ( rhs >= bit_count().as<i64>() ) {
        return Error::from_code(ErrorCode::ShiftOverflow);
    } else {
        m_value >>= rhs.m_value;
        return {};
    }
}

auto i64::right_shift_assign(i64 const& rhs) -> void {
    if constexpr ( CCLangSafeIntegerOperations ) {
        must$(try_right_shift_assign(rhs));
    } else {
        m_value >>= rhs.m_value;
    }
}

auto i64::operator>>=(i64 const& rhs) -> i64& {
    right_shift_assign(rhs);
    return *this;
}

auto i64::operator++() -> i64& {
    add_assign(1);
    return *this;
}

auto i64::operator++(int) -> i64 {
    auto __prev = *this;
    add_assign(1);
    return __prev;
}

auto i64::operator--() -> i64& {
    sub_assign(1);
    return *this;
}

auto i64::operator--(int) -> i64 {
    auto __prev = *this;
    sub_assign(1);
    return __prev;
}

auto i64::try_add(i64 const& rhs) const -> ErrorOr<i64> {
    i64::CCIntegerType __value;
    if ( __builtin_add_overflow(m_value, rhs.m_value, &__value) ) {
        return Error::from_code(ErrorCode::IntOverflow);
    } else {
        return i64(__value);
    }
}

auto i64::add(i64 const& rhs) const -> i64 {
    if constexpr ( CCLangSafeIntegerOperations ) {
        return must$(try_add(rhs));
    } else {
        return m_value + rhs.m_value;
    }
}

auto i64::operator+(i64 const& rhs) const -> i64 {
    return add(rhs);
}

auto i64::try_add_assign(i64 const& rhs) -> ErrorOr<void> {
    m_value = try$(try_add(rhs)).m_value;
    return {};
}

auto i64::add_assign(i64 const& rhs) -> void {
    if constexpr ( CCLangSafeIntegerOperations ) {
        must$(try_add_assign(rhs));
    } else {
        m_value += rhs.m_value;
    }
}

auto i64::operator+=(i64 const& rhs) -> i64& {
    add_assign(rhs);
    return *this;
}

auto i64::try_sub(i64 const& rhs) const -> ErrorOr<i64> {
    i64::CCIntegerType __value;
    if ( __builtin_sub_overflow(m_value, rhs.m_value, &__value) ) {
        return Error::from_code(ErrorCode::IntOverflow);
    } else {
        return i64(__value);
    }
}

auto i64::sub(i64 const& rhs) const -> i64 {
    if constexpr ( CCLangSafeIntegerOperations ) {
        return must$(try_sub(rhs));
    } else {
        return m_value - rhs.m_value;
    }
}

auto i64::operator-(i64 const& rhs) const -> i64 {
    return sub(rhs);
}

auto i64::try_sub_assign(i64 const& rhs) -> ErrorOr<void> {
    m_value = try$(try_sub(rhs)).m_value;
    return {};
}

auto i64::sub_assign(i64 const& rhs) -> void {
    if constexpr ( CCLangSafeIntegerOperations ) {
        must$(try_sub_assign(rhs));
    } else {
        m_value -= rhs.m_value;
    }
}

auto i64::operator-=(i64 const& rhs) -> i64& {
    sub_assign(rhs);
    return *this;
}

auto i64::try_mul(i64 const& rhs) const -> ErrorOr<i64> {
    i64::CCIntegerType __value;
    if ( __builtin_mul_overflow(m_value, rhs.m_value, &__value) ) {
        return Error::from_code(ErrorCode::IntOverflow);
    } else {
        return i64(__value);
    }
}

auto i64::mul(i64 const& rhs) const -> i64 {
    if constexpr ( CCLangSafeIntegerOperations ) {
        must$(try_mul(rhs));
    } else {
        return m_value * rhs.m_value;
    }
}

auto i64::operator*(i64 const& rhs) const -> i64 {
    return mul(rhs);
}

auto i64::try_mul_assign(i64 const& rhs) -> ErrorOr<void> {
    m_value = try$(try_mul(rhs)).m_value;
    return {};
}

auto i64::mul_assign(i64 const& rhs) -> void {
    if constexpr ( CCLangSafeIntegerOperations ) {
        must$(try_mul_assign(rhs));
    } else {
        m_value *= rhs.m_value;
    }
}

auto i64::operator*=(i64 const& rhs) -> i64& {
    mul_assign(rhs);
    return *this;
}

auto i64::try_div(i64 const& rhs) const -> ErrorOr<i64> {
    if ( rhs == 0 ) {
        return Error::from_code(ErrorCode::DivisionByZero);
    } else {
        return i64(static_cast<CCIntegerType>(m_value / rhs.m_value));
    }
}

auto i64::div(i64 const& rhs) const -> i64 {
    if constexpr ( CCLangSafeIntegerOperations ) {
        return must$(try_div(rhs));
    } else {
        return m_value / rhs.m_value;
    }
}

auto i64::operator/(i64 const& rhs) const -> i64 {
    return div(rhs);
}

auto i64::try_div_assign(i64 const& rhs) -> ErrorOr<void> {
    m_value = try$(try_div(rhs)).m_value;
    return {};
}

auto i64::div_assign(i64 const& rhs) -> void {
    if constexpr ( CCLangSafeIntegerOperations ) {
        must$(try_div_assign(rhs));
    } else {
        m_value /= rhs.m_value;
    }
}

auto i64::operator/=(i64 const& rhs) -> i64& {
    div_assign(rhs);
    return *this;
}

auto i64::operator%(i64 const& rhs) const -> i64 {
    return m_value % rhs.m_value;
}

auto i64::operator%=(i64 const& rhs) -> i64& {
    m_value %= rhs.m_value;
    return *this;
}

auto i64::atomic_load(MemOrder mem_order) volatile -> i64 {
    CCIntegerType __value;
    __atomic_load(&m_value, &__value, static_cast<UnderlyingType<MemOrder>>(mem_order));
    return __value;
}

auto i64::atomic_store(i64 rhs, MemOrder mem_order) volatile -> void {
    __atomic_store(&m_value, &rhs.m_value, static_cast<UnderlyingType<MemOrder>>(mem_order));
}

auto i64::atomic_add(i64 rhs, MemOrder mem_order) volatile -> void {
    (void)atomic_fetch_add(rhs, mem_order);
}

auto i64::atomic_sub(i64 rhs, MemOrder mem_order) volatile -> void {
    (void)atomic_fetch_sub(rhs, mem_order);
}

auto i64::atomic_fetch_add(i64 rhs, MemOrder mem_order) volatile -> i64 {
    return __atomic_fetch_add(&m_value, rhs.m_value, static_cast<UnderlyingType<MemOrder>>(mem_order));
}

auto i64::atomic_fetch_sub(i64 rhs, MemOrder mem_order) volatile -> i64 {
    return __atomic_fetch_sub(&m_value, rhs.m_value, static_cast<UnderlyingType<MemOrder>>(mem_order));
}

auto i64::atomic_add_fetch(i64 rhs, MemOrder mem_order) volatile -> i64 {
    return __atomic_add_fetch(&m_value, rhs.m_value, static_cast<UnderlyingType<MemOrder>>(mem_order));
}

auto i64::atomic_sub_fetch(i64 rhs, MemOrder mem_order) volatile -> i64 {
    return __atomic_sub_fetch(&m_value, rhs.m_value, static_cast<UnderlyingType<MemOrder>>(mem_order));
}

auto i64::hash_code() const -> usize {
    auto hash_key = as<usize>();
    hash_key += ~(hash_key << 15);
    hash_key ^= (hash_key >> 10);
    hash_key += (hash_key << 3);
    hash_key ^= (hash_key >> 6);
    hash_key += ~(hash_key << 11);
    hash_key ^= (hash_key >> 16);
    return hash_key;
}
