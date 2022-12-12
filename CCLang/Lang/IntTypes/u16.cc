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

#include <CCLang/Lang/IntTypes/u16.hh>

#include <CCLang/Core/ErrorOr.hh>
#include <CCLang/Lang/Cxx.hh>
#include <CCLang/Lang/IntTypes/usize.hh>
#include <CCLang/Lang/Must.hh>
#include <CCLang/Lang/Range.hh>
#include <CCLang/Lang/Try.hh>

auto u16::max(u16 const& lhs, u16 const& rhs) -> u16 {
    if ( lhs > rhs ) {
        return lhs;
    } else {
        return rhs;
    }
}

auto u16::min(u16 const& lhs, u16 const& rhs) -> u16 {
    if ( lhs < rhs ) {
        return lhs;
    } else {
        return rhs;
    }
}

auto u16::range(u16 const& begin, u16 const& end) -> Range<u16> {
    return Range<u16>(begin, end);
}

auto u16::range_inclusive(u16 const& begin, u16 const& last) -> RangeInclusive<u16> {
    return RangeInclusive<u16>(begin, last);
}

u16::u16(__UINT16_TYPE__ value)
    : m_value(value) {
}

u16::u16(u16&& rhs)
    : m_value(Cxx::exchange(rhs.m_value, 0)) {
}

auto u16::operator=(__UINT16_TYPE__ value) -> u16& {
    u16 integer = value;
    swap(integer);
    return *this;
}

auto u16::operator=(u16 const& rhs) -> u16& {
    u16 integer = rhs;
    swap(integer);
    return *this;
}

auto u16::operator=(u16&& rhs) -> u16& {
    u16 integer = Cxx::move(rhs);
    swap(integer);
    return *this;
}

auto u16::swap(u16& rhs) -> void {
    Cxx::swap(m_value, rhs.m_value);
}

auto u16::is_signed() const -> bool {
    return false;
}

auto u16::try_bit_at(usize index) const -> ErrorOr<bool> {
    if ( index > bit_count() ) {
        return Error::from_code(ErrorCode::IndexOutOfRange);
    } else {
        return (m_value & (1 << index.unwrap())) == (1 << index.unwrap());
    }
}

auto u16::bit_at(usize index) const -> bool {
    return must$(try_bit_at(index));
}

auto u16::try_set_bit(usize index, bool value) -> ErrorOr<void> {
    if ( index > bit_count() ) {
        return Error::from_code(ErrorCode::IndexOutOfRange);
    }

    if ( value ) {
        m_value |= 1 << index.unwrap();
    } else {
        m_value &= ~(1 << index.unwrap());
    }
    return {};
}

auto u16::set_bit(usize index, bool value) -> void {
    must$(try_set_bit(index, value));
}

auto u16::count_zeroes() const -> usize {
    return m_value; /* TODO implement */
}

auto u16::count_ones() const -> usize {
    return m_value; /* TODO implement */
}

auto u16::bit_count() const -> usize {
    return sizeof(__UINT16_TYPE__) * 8;
}

auto u16::unwrap() const -> __UINT16_TYPE__ {
    return m_value;
}

auto u16::equal(u16 const& rhs) const -> bool {
    return m_value == rhs.m_value;
}

auto u16::operator==(u16 const& rhs) const -> bool {
    return equal(rhs);
}

auto u16::not_equal(u16 const& rhs) const -> bool {
    return m_value != rhs.m_value;
}

auto u16::operator!=(u16 const& rhs) const -> bool {
    return not_equal(rhs);
}

auto u16::greater(u16 const& rhs) const -> bool {
    return m_value > rhs.m_value;
}

auto u16::operator>(u16 const& rhs) const -> bool {
    return greater(rhs);
}

auto u16::greater_equal(u16 const& rhs) const -> bool {
    return m_value >= rhs.m_value;
}

auto u16::operator>=(u16 const& rhs) const -> bool {
    return greater_equal(rhs);
}

auto u16::less(u16 const& rhs) const -> bool {
    return m_value < rhs.m_value;
}

auto u16::operator<(u16 const& rhs) const -> bool {
    return less(rhs);
}

auto u16::less_equal(u16 const& rhs) const -> bool {
    return m_value <= rhs.m_value;
}

auto u16::operator<=(u16 const& rhs) const -> bool {
    return less_equal(rhs);
}

auto u16::bit_one_complement() const -> u16 {
    return ~m_value;
}

auto u16::operator~() const -> u16 {
    return bit_one_complement();
}

auto u16::bit_and(u16 const& rhs) const -> u16 {
    return m_value & rhs.m_value;
}

auto u16::operator&(u16 const& rhs) const -> u16 {
    return bit_and(rhs);
}

auto u16::bit_and_assign(u16 const& rhs) -> void {
    m_value &= rhs.m_value;
}

auto u16::operator&=(u16 const& rhs) -> u16& {
    bit_and_assign(rhs);
    return *this;
}

auto u16::bit_or(u16 const& rhs) const -> u16 {
    return m_value | rhs.m_value;
}

auto u16::operator|(u16 const& rhs) const -> u16 {
    return bit_or(rhs);
}

auto u16::bit_or_assign(u16 const& rhs) -> void {
    m_value |= rhs.m_value;
}

auto u16::operator|=(u16 const& rhs) -> u16& {
    bit_or_assign(rhs);
    return *this;
}

auto u16::bit_xor(u16 const& rhs) const -> u16 {
    return m_value ^ rhs.m_value;
}

auto u16::operator^(u16 const& rhs) const -> u16 {
    return bit_xor(rhs);
}

auto u16::bit_xor_assign(u16 const& rhs) -> void {
    m_value ^= rhs.m_value;
}

auto u16::operator^=(u16 const& rhs) -> u16& {
    bit_xor_assign(rhs);
    return *this;
}

auto u16::try_left_shift(u16 const& rhs) const -> ErrorOr<u16> {
    if ( rhs >= bit_count().as<u16>() ) {
        return Error::from_code(ErrorCode::ShiftOverflow);
    } else {
        return u16(static_cast<CCIntegerType>(m_value << rhs.m_value));
    }
}

auto u16::left_shift(u16 const& rhs) const -> u16 {
    if constexpr ( CCLangSafeIntegerOperations ) {
        return must$(try_left_shift(rhs));
    } else {
        return static_cast<CCIntegerType>(m_value << rhs.m_value);
    }
}

auto u16::operator<<(u16 const& rhs) const -> u16 {
    return left_shift(rhs);
}

auto u16::try_left_shift_assign(u16 const& rhs) -> ErrorOr<void> {
    if ( rhs >= bit_count().as<u16>() ) {
        return Error::from_code(ErrorCode::ShiftOverflow);
    } else {
        m_value <<= rhs.m_value;
        return {};
    }
}

auto u16::left_shift_assign(u16 const& rhs) -> void {
    if constexpr ( CCLangSafeIntegerOperations ) {
        must$(try_left_shift_assign(rhs));
    } else {
        m_value <<= rhs.m_value;
    }
}

auto u16::operator<<=(u16 const& rhs) -> u16& {
    left_shift_assign(rhs);
    return *this;
}

auto u16::try_right_shift(u16 const& rhs) const -> ErrorOr<u16> {
    if ( rhs >= bit_count().as<u16>() ) {
        return Error::from_code(ErrorCode::ShiftOverflow);
    } else {
        return u16(static_cast<CCIntegerType>(m_value >> rhs.m_value));
    }
}

auto u16::right_shift(u16 const& rhs) const -> u16 {
    if constexpr ( CCLangSafeIntegerOperations ) {
        return must$(try_right_shift(rhs));
    } else {
        return static_cast<CCIntegerType>(m_value >> rhs.m_value);
    }
}

auto u16::operator>>(u16 const& rhs) const -> u16 {
    return right_shift(rhs);
}

auto u16::try_right_shift_assign(u16 const& rhs) -> ErrorOr<void> {
    if ( rhs >= bit_count().as<u16>() ) {
        return Error::from_code(ErrorCode::ShiftOverflow);
    } else {
        m_value >>= rhs.m_value;
        return {};
    }
}

auto u16::right_shift_assign(u16 const& rhs) -> void {
    if constexpr ( CCLangSafeIntegerOperations ) {
        must$(try_right_shift_assign(rhs));
    } else {
        m_value >>= rhs.m_value;
    }
}

auto u16::operator>>=(u16 const& rhs) -> u16& {
    right_shift_assign(rhs);
    return *this;
}

auto u16::operator++() -> u16& {
    add_assign(1);
    return *this;
}

auto u16::operator++(int) -> u16 {
    auto __prev = *this;
    add_assign(1);
    return __prev;
}

auto u16::operator--() -> u16& {
    sub_assign(1);
    return *this;
}

auto u16::operator--(int) -> u16 {
    auto __prev = *this;
    sub_assign(1);
    return __prev;
}

auto u16::try_add(u16 const& rhs) const -> ErrorOr<u16> {
    u16::CCIntegerType __value;
    if ( __builtin_add_overflow(m_value, rhs.m_value, &__value) ) {
        return Error::from_code(ErrorCode::IntOverflow);
    } else {
        return u16(__value);
    }
}

auto u16::add(u16 const& rhs) const -> u16 {
    if constexpr ( CCLangSafeIntegerOperations ) {
        return must$(try_add(rhs));
    } else {
        return m_value + rhs.m_value;
    }
}

auto u16::operator+(u16 const& rhs) const -> u16 {
    return add(rhs);
}

auto u16::try_add_assign(u16 const& rhs) -> ErrorOr<void> {
    m_value = try$(try_add(rhs)).m_value;
    return {};
}

auto u16::add_assign(u16 const& rhs) -> void {
    if constexpr ( CCLangSafeIntegerOperations ) {
        must$(try_add_assign(rhs));
    } else {
        m_value += rhs.m_value;
    }
}

auto u16::operator+=(u16 const& rhs) -> u16& {
    add_assign(rhs);
    return *this;
}

auto u16::try_sub(u16 const& rhs) const -> ErrorOr<u16> {
    u16::CCIntegerType __value;
    if ( __builtin_sub_overflow(m_value, rhs.m_value, &__value) ) {
        return Error::from_code(ErrorCode::IntOverflow);
    } else {
        return u16(__value);
    }
}

auto u16::sub(u16 const& rhs) const -> u16 {
    if constexpr ( CCLangSafeIntegerOperations ) {
        return must$(try_sub(rhs));
    } else {
        return m_value - rhs.m_value;
    }
}

auto u16::operator-(u16 const& rhs) const -> u16 {
    return sub(rhs);
}

auto u16::try_sub_assign(u16 const& rhs) -> ErrorOr<void> {
    m_value = try$(try_sub(rhs)).m_value;
    return {};
}

auto u16::sub_assign(u16 const& rhs) -> void {
    if constexpr ( CCLangSafeIntegerOperations ) {
        must$(try_sub_assign(rhs));
    } else {
        m_value -= rhs.m_value;
    }
}

auto u16::operator-=(u16 const& rhs) -> u16& {
    sub_assign(rhs);
    return *this;
}

auto u16::try_mul(u16 const& rhs) const -> ErrorOr<u16> {
    u16::CCIntegerType __value;
    if ( __builtin_mul_overflow(m_value, rhs.m_value, &__value) ) {
        return Error::from_code(ErrorCode::IntOverflow);
    } else {
        return u16(__value);
    }
}

auto u16::mul(u16 const& rhs) const -> u16 {
    if constexpr ( CCLangSafeIntegerOperations ) {
        return must$(try_mul(rhs));
    } else {
        return m_value * rhs.m_value;
    }
}

auto u16::operator*(u16 const& rhs) const -> u16 {
    return mul(rhs);
}

auto u16::try_mul_assign(u16 const& rhs) -> ErrorOr<void> {
    m_value = try$(try_mul(rhs)).m_value;
    return {};
}

auto u16::mul_assign(u16 const& rhs) -> void {
    if constexpr ( CCLangSafeIntegerOperations ) {
        must$(try_mul_assign(rhs));
    } else {
        m_value *= rhs.m_value;
    }
}

auto u16::operator*=(u16 const& rhs) -> u16& {
    mul_assign(rhs);
    return *this;
}

auto u16::try_div(u16 const& rhs) const -> ErrorOr<u16> {
    if ( rhs == 0 ) {
        return Error::from_code(ErrorCode::DivisionByZero);
    } else {
        return u16(static_cast<CCIntegerType>(m_value / rhs.m_value));
    }
}

auto u16::div(u16 const& rhs) const -> u16 {
    if constexpr ( CCLangSafeIntegerOperations ) {
        return must$(try_div(rhs));
    } else {
        return m_value / rhs.m_value;
    }
}

auto u16::operator/(u16 const& rhs) const -> u16 {
    return div(rhs);
}

auto u16::try_div_assign(u16 const& rhs) -> ErrorOr<void> {
    m_value = try$(try_div(rhs)).m_value;
    return {};
}

auto u16::div_assign(u16 const& rhs) -> void {
    if constexpr ( CCLangSafeIntegerOperations ) {
        must$(try_div_assign(rhs));
    } else {
        m_value /= rhs.m_value;
    }
}

auto u16::operator/=(u16 const& rhs) -> u16& {
    div_assign(rhs);
    return *this;
}

auto u16::operator%(u16 const& rhs) const -> u16 {
    return m_value % rhs.m_value;
}

auto u16::operator%=(u16 const& rhs) -> u16& {
    m_value %= rhs.m_value;
    return *this;
}

auto u16::atomic_load(MemOrder mem_order) volatile -> u16 {
    CCIntegerType __value;
    __atomic_load(&m_value, &__value, static_cast<UnderlyingType<MemOrder>>(mem_order));
    return __value;
}

auto u16::atomic_store(u16 rhs, MemOrder mem_order) volatile -> void {
    __atomic_store(&m_value, &rhs.m_value, static_cast<UnderlyingType<MemOrder>>(mem_order));
}

auto u16::atomic_add(u16 rhs, MemOrder mem_order) volatile -> void {
    (void)atomic_fetch_add(rhs, mem_order);
}

auto u16::atomic_sub(u16 rhs, MemOrder mem_order) volatile -> void {
    (void)atomic_fetch_sub(rhs, mem_order);
}

auto u16::atomic_fetch_add(u16 rhs, MemOrder mem_order) volatile -> u16 {
    return __atomic_fetch_add(&m_value, rhs.m_value, static_cast<UnderlyingType<MemOrder>>(mem_order));
}

auto u16::atomic_fetch_sub(u16 rhs, MemOrder mem_order) volatile -> u16 {
    return __atomic_fetch_sub(&m_value, rhs.m_value, static_cast<UnderlyingType<MemOrder>>(mem_order));
}

auto u16::atomic_add_fetch(u16 rhs, MemOrder mem_order) volatile -> u16 {
    return __atomic_add_fetch(&m_value, rhs.m_value, static_cast<UnderlyingType<MemOrder>>(mem_order));
}

auto u16::atomic_sub_fetch(u16 rhs, MemOrder mem_order) volatile -> u16 {
    return __atomic_sub_fetch(&m_value, rhs.m_value, static_cast<UnderlyingType<MemOrder>>(mem_order));
}

auto u16::hash_code() const -> usize {
    auto hash_key = as<usize>();
    hash_key += ~(hash_key << 15);
    hash_key ^= (hash_key >> 10);
    hash_key += (hash_key << 3);
    hash_key ^= (hash_key >> 6);
    hash_key += ~(hash_key << 11);
    hash_key ^= (hash_key >> 16);
    return hash_key;
}

auto operator<<(__UINT16_TYPE__ lhs, u16 rhs) -> u16 {
    return u16(lhs) << rhs;
}

auto operator>>(__UINT16_TYPE__ lhs, u16 rhs) -> u16 {
    return u16(lhs) >> rhs;
}