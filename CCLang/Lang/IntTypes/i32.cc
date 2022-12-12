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

#include <CCLang/Lang/IntTypes/i32.hh>

#include <CCLang/Core/ErrorOr.hh>
#include <CCLang/Lang/Cxx.hh>
#include <CCLang/Lang/IntTypes/usize.hh>
#include <CCLang/Lang/Must.hh>
#include <CCLang/Lang/Range.hh>
#include <CCLang/Lang/Try.hh>

auto i32::max(i32 const& lhs, i32 const& rhs) -> i32 {
    if ( lhs > rhs ) {
        return lhs;
    } else {
        return rhs;
    }
}

auto i32::min(i32 const& lhs, i32 const& rhs) -> i32 {
    if ( lhs < rhs ) {
        return lhs;
    } else {
        return rhs;
    }
}

auto i32::range(i32 const& begin, i32 const& end) -> Range<i32> {
    return Range<i32>(begin, end);
}

auto i32::range_inclusive(i32 const& begin, i32 const& last) -> RangeInclusive<i32> {
    return RangeInclusive<i32>(begin, last);
}

i32::i32(__INT32_TYPE__ value)
    : m_value(value) {
}

auto i32::operator=(__INT32_TYPE__ value) -> i32& {
    i32 integer = value;
    swap(integer);
    return *this;
}

auto i32::swap(i32& rhs) -> void {
    Cxx::swap(m_value, rhs.m_value);
}

auto i32::is_signed() const -> bool {
    return true;
}

auto i32::bit_count() const -> usize {
    return sizeof(__INT32_TYPE__) * 8;
}

auto i32::unwrap() const -> __INT32_TYPE__ {
    return m_value;
}

auto i32::equal(i32 const& rhs) const -> bool {
    return m_value == rhs.m_value;
}

auto i32::operator==(i32 const& rhs) const -> bool {
    return equal(rhs);
}

auto i32::not_equal(i32 const& rhs) const -> bool {
    return m_value != rhs.m_value;
}

auto i32::operator!=(i32 const& rhs) const -> bool {
    return not_equal(rhs);
}

auto i32::greater(i32 const& rhs) const -> bool {
    return m_value > rhs.m_value;
}

auto i32::operator>(i32 const& rhs) const -> bool {
    return greater(rhs);
}

auto i32::greater_equal(i32 const& rhs) const -> bool {
    return m_value >= rhs.m_value;
}

auto i32::operator>=(i32 const& rhs) const -> bool {
    return greater_equal(rhs);
}

auto i32::less(i32 const& rhs) const -> bool {
    return m_value < rhs.m_value;
}

auto i32::operator<(i32 const& rhs) const -> bool {
    return less(rhs);
}

auto i32::less_equal(i32 const& rhs) const -> bool {
    return m_value <= rhs.m_value;
}

auto i32::operator<=(i32 const& rhs) const -> bool {
    return less_equal(rhs);
}

auto i32::bit_one_complement() const -> i32 {
    return ~m_value;
}

auto i32::operator~() const -> i32 {
    return bit_one_complement();
}

auto i32::bit_and(i32 const& rhs) const -> i32 {
    return m_value & rhs.m_value;
}

auto i32::operator&(i32 const& rhs) const -> i32 {
    return bit_and(rhs);
}

auto i32::bit_and_assign(i32 const& rhs) -> void {
    m_value &= rhs.m_value;
}

auto i32::operator&=(i32 const& rhs) -> i32& {
    bit_and_assign(rhs);
    return *this;
}

auto i32::bit_or(i32 const& rhs) const -> i32 {
    return m_value | rhs.m_value;
}

auto i32::operator|(i32 const& rhs) const -> i32 {
    return bit_or(rhs);
}

auto i32::bit_or_assign(i32 const& rhs) -> void {
    m_value |= rhs.m_value;
}

auto i32::operator|=(i32 const& rhs) -> i32& {
    bit_or_assign(rhs);
    return *this;
}

auto i32::bit_xor(i32 const& rhs) const -> i32 {
    return m_value ^ rhs.m_value;
}

auto i32::operator^(i32 const& rhs) const -> i32 {
    return bit_xor(rhs);
}

auto i32::bit_xor_assign(i32 const& rhs) -> void {
    m_value ^= rhs.m_value;
}

auto i32::operator^=(i32 const& rhs) -> i32& {
    bit_xor_assign(rhs);
    return *this;
}

auto i32::try_left_shift(i32 const& rhs) const -> ErrorOr<i32> {
    if ( rhs >= bit_count().as<i32>() ) {
        return Error::from_code(ErrorCode::ShiftOverflow);
    } else {
        return i32(static_cast<CCIntegerType>(m_value << rhs.m_value));
    }
}

auto i32::left_shift(i32 const& rhs) const -> i32 {
    if constexpr ( CCLangSafeIntegerOperations ) {
        return must$(try_left_shift(rhs));
    } else {
        return static_cast<CCIntegerType>(m_value << rhs.m_value);
    }
}

auto i32::operator<<(i32 const& rhs) const -> i32 {
    return left_shift(rhs);
}

auto i32::try_left_shift_assign(i32 const& rhs) -> ErrorOr<void> {
    if ( rhs >= bit_count().as<i32>() ) {
        return Error::from_code(ErrorCode::ShiftOverflow);
    } else {
        m_value <<= rhs.m_value;
        return {};
    }
}

auto i32::left_shift_assign(i32 const& rhs) -> void {
    if constexpr ( CCLangSafeIntegerOperations ) {
        must$(try_left_shift_assign(rhs));
    } else {
        m_value <<= rhs.m_value;
    }
}

auto i32::operator<<=(i32 const& rhs) -> i32& {
    left_shift_assign(rhs);
    return *this;
}

auto i32::try_right_shift(i32 const& rhs) const -> ErrorOr<i32> {
    if ( rhs >= bit_count().as<i32>() ) {
        return Error::from_code(ErrorCode::ShiftOverflow);
    } else {
        return i32(static_cast<CCIntegerType>(m_value >> rhs.m_value));
    }
}

auto i32::right_shift(i32 const& rhs) const -> i32 {
    if constexpr ( CCLangSafeIntegerOperations ) {
        return must$(try_right_shift(rhs));
    } else {
        return static_cast<CCIntegerType>(m_value >> rhs.m_value);
    }
}

auto i32::operator>>(i32 const& rhs) const -> i32 {
    return right_shift(rhs);
}

auto i32::try_right_shift_assign(i32 const& rhs) -> ErrorOr<void> {
    if ( rhs >= bit_count().as<i32>() ) {
        return Error::from_code(ErrorCode::ShiftOverflow);
    } else {
        m_value >>= rhs.m_value;
        return {};
    }
}

auto i32::right_shift_assign(i32 const& rhs) -> void {
    if constexpr ( CCLangSafeIntegerOperations ) {
        must$(try_right_shift_assign(rhs));
    } else {
        m_value >>= rhs.m_value;
    }
}

auto i32::operator>>=(i32 const& rhs) -> i32& {
    right_shift_assign(rhs);
    return *this;
}

auto i32::operator++() -> i32& {
    add_assign(1);
    return *this;
}

auto i32::operator++(int) -> i32 {
    auto __prev = *this;
    add_assign(1);
    return __prev;
}

auto i32::operator--() -> i32& {
    sub_assign(1);
    return *this;
}

auto i32::operator--(int) -> i32 {
    auto __prev = *this;
    sub_assign(1);
    return __prev;
}

auto i32::try_add(i32 const& rhs) const -> ErrorOr<i32> {
    i32::CCIntegerType __value;
    if ( __builtin_add_overflow(m_value, rhs.m_value, &__value) ) {
        return Error::from_code(ErrorCode::IntOverflow);
    } else {
        return i32(__value);
    }
}

auto i32::add(i32 const& rhs) const -> i32 {
    if constexpr ( CCLangSafeIntegerOperations ) {
        return must$(try_add(rhs));
    } else {
        return m_value + rhs.m_value;
    }
}

auto i32::operator+(i32 const& rhs) const -> i32 {
    return add(rhs);
}

auto i32::try_add_assign(i32 const& rhs) -> ErrorOr<void> {
    m_value = try$(try_add(rhs)).m_value;
    return {};
}

auto i32::add_assign(i32 const& rhs) -> void {
    if constexpr ( CCLangSafeIntegerOperations ) {
        must$(try_add_assign(rhs));
    } else {
        m_value += rhs.m_value;
    }
}

auto i32::operator+=(i32 const& rhs) -> i32& {
    add_assign(rhs);
    return *this;
}

auto i32::try_sub(i32 const& rhs) const -> ErrorOr<i32> {
    i32::CCIntegerType __value;
    if ( __builtin_sub_overflow(m_value, rhs.m_value, &__value) ) {
        return Error::from_code(ErrorCode::IntOverflow);
    } else {
        return i32(__value);
    }
}

auto i32::sub(i32 const& rhs) const -> i32 {
    if constexpr ( CCLangSafeIntegerOperations ) {
        return must$(try_sub(rhs));
    } else {
        return m_value - rhs.m_value;
    }
}

auto i32::operator-(i32 const& rhs) const -> i32 {
    return sub(rhs);
}

auto i32::try_sub_assign(i32 const& rhs) -> ErrorOr<void> {
    m_value = try$(try_sub(rhs)).m_value;
    return {};
}

auto i32::sub_assign(i32 const& rhs) -> void {
    if constexpr ( CCLangSafeIntegerOperations ) {
        must$(try_sub_assign(rhs));
    } else {
        m_value -= rhs.m_value;
    }
}

auto i32::operator-=(i32 const& rhs) -> i32& {
    sub_assign(rhs);
    return *this;
}

auto i32::try_mul(i32 const& rhs) const -> ErrorOr<i32> {
    i32::CCIntegerType __value;
    if ( __builtin_mul_overflow(m_value, rhs.m_value, &__value) ) {
        return Error::from_code(ErrorCode::IntOverflow);
    } else {
        return i32(__value);
    }
}

auto i32::mul(i32 const& rhs) const -> i32 {
    if constexpr ( CCLangSafeIntegerOperations ) {
        return must$(try_mul(rhs));
    } else {
        return m_value * rhs.m_value;
    }
}

auto i32::operator*(i32 const& rhs) const -> i32 {
    return mul(rhs);
}

auto i32::try_mul_assign(i32 const& rhs) -> ErrorOr<void> {
    m_value = try$(try_mul(rhs)).m_value;
    return {};
}

auto i32::mul_assign(i32 const& rhs) -> void {
    if constexpr ( CCLangSafeIntegerOperations ) {
        must$(try_mul_assign(rhs));
    } else {
        m_value *= rhs.m_value;
    }
}

auto i32::operator*=(i32 const& rhs) -> i32& {
    mul_assign(rhs);
    return *this;
}

auto i32::try_div(i32 const& rhs) const -> ErrorOr<i32> {
    if ( rhs == 0 ) {
        return Error::from_code(ErrorCode::DivisionByZero);
    } else {
        return i32(static_cast<CCIntegerType>(m_value / rhs.m_value));
    }
}

auto i32::div(i32 const& rhs) const -> i32 {
    if constexpr ( CCLangSafeIntegerOperations ) {
        return must$(try_div(rhs));
    } else {
        return m_value / rhs.m_value;
    }
}

auto i32::operator/(i32 const& rhs) const -> i32 {
    return div(rhs);
}

auto i32::try_div_assign(i32 const& rhs) -> ErrorOr<void> {
    m_value = try$(try_div(rhs)).m_value;
    return {};
}

auto i32::div_assign(i32 const& rhs) -> void {
    if constexpr ( CCLangSafeIntegerOperations ) {
        must$(try_div_assign(rhs));
    } else {
        m_value /= rhs.m_value;
    }
}

auto i32::operator/=(i32 const& rhs) -> i32& {
    div_assign(rhs);
    return *this;
}

auto i32::operator%(i32 const& rhs) const -> i32 {
    return m_value % rhs.m_value;
}

auto i32::operator%=(i32 const& rhs) -> i32& {
    m_value %= rhs.m_value;
    return *this;
}

auto i32::atomic_load(MemOrder mem_order) volatile -> i32 {
    CCIntegerType __value;
    __atomic_load(&m_value, &__value, static_cast<UnderlyingType<MemOrder>>(mem_order));
    return __value;
}

auto i32::atomic_store(i32 rhs, MemOrder mem_order) volatile -> void {
    __atomic_store(&m_value, &rhs.m_value, static_cast<UnderlyingType<MemOrder>>(mem_order));
}

auto i32::atomic_add(i32 rhs, MemOrder mem_order) volatile -> void {
    (void)atomic_fetch_add(rhs, mem_order);
}

auto i32::atomic_sub(i32 rhs, MemOrder mem_order) volatile -> void {
    (void)atomic_fetch_sub(rhs, mem_order);
}

auto i32::atomic_fetch_add(i32 rhs, MemOrder mem_order) volatile -> i32 {
    return __atomic_fetch_add(&m_value, rhs.m_value, static_cast<UnderlyingType<MemOrder>>(mem_order));
}

auto i32::atomic_fetch_sub(i32 rhs, MemOrder mem_order) volatile -> i32 {
    return __atomic_fetch_sub(&m_value, rhs.m_value, static_cast<UnderlyingType<MemOrder>>(mem_order));
}

auto i32::atomic_add_fetch(i32 rhs, MemOrder mem_order) volatile -> i32 {
    return __atomic_add_fetch(&m_value, rhs.m_value, static_cast<UnderlyingType<MemOrder>>(mem_order));
}

auto i32::atomic_sub_fetch(i32 rhs, MemOrder mem_order) volatile -> i32 {
    return __atomic_sub_fetch(&m_value, rhs.m_value, static_cast<UnderlyingType<MemOrder>>(mem_order));
}

auto i32::hash_code() const -> usize {
    auto hash_key = as<usize>();
    hash_key += ~(hash_key << 15);
    hash_key ^= (hash_key >> 10);
    hash_key += (hash_key << 3);
    hash_key ^= (hash_key >> 6);
    hash_key += ~(hash_key << 11);
    hash_key ^= (hash_key >> 16);
    return hash_key;
}
