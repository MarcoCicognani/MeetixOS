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

#include <CCLang/Lang/IntTypes/usize.hh>

#include <CCLang/Core/ErrorOr.hh>
#include <CCLang/Lang/Cxx.hh>
#include <CCLang/Lang/Must.hh>
#include <CCLang/Lang/Range.hh>
#include <CCLang/Lang/Try.hh>

auto usize::max(usize const& lhs, usize const& rhs) -> usize {
    if ( lhs > rhs ) {
        return lhs;
    } else {
        return rhs;
    }
}

auto usize::min(usize const& lhs, usize const& rhs) -> usize {
    if ( lhs < rhs ) {
        return lhs;
    } else {
        return rhs;
    }
}

auto usize::range(usize const& begin, usize const& end) -> Range<usize> {
    return Range<usize>(begin, end);
}

auto usize::range_inclusive(usize const& begin, usize const& last) -> RangeInclusive<usize> {
    return RangeInclusive<usize>(begin, last);
}

usize::usize(__SIZE_TYPE__ value)
    : m_value(value) {
}

usize::usize(usize&& rhs)
    : m_value(Cxx::exchange(rhs.m_value, 0)) {
}

auto usize::operator=(__SIZE_TYPE__ value) -> usize& {
    usize integer = value;
    swap(integer);
    return *this;
}

auto usize::operator=(usize const& rhs) -> usize& {
    usize integer = rhs;
    swap(integer);
    return *this;
}

auto usize::operator=(usize&& rhs) -> usize& {
    usize integer = Cxx::move(rhs);
    swap(integer);
    return *this;
}

auto usize::swap(usize& rhs) -> void {
    Cxx::swap(m_value, rhs.m_value);
}

auto usize::is_signed() const -> bool {
    return false;
}

auto usize::try_bit_at(usize index) const -> ErrorOr<bool> {
    if ( index > bit_count() ) {
        return Error::from_code(ErrorCode::IndexOutOfRange);
    } else {
        return (m_value & (1 << index.unwrap())) == (1 << index.unwrap());
    }
}

auto usize::bit_at(usize index) const -> bool {
    return must$(try_bit_at(index));
}

auto usize::try_set_bit(usize index, bool value) -> ErrorOr<void> {
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

auto usize::set_bit(usize index, bool value) -> void {
    must$(try_set_bit(index, value));
}

auto usize::count_zeroes() const -> usize {
    return m_value; /* TODO implement */
}

auto usize::count_ones() const -> usize {
    return m_value; /* TODO implement */
}

auto usize::bit_count() const -> usize {
    return sizeof(__SIZE_TYPE__) * 8;
}

auto usize::unwrap() const -> __SIZE_TYPE__ {
    return m_value;
}

auto usize::equal(usize const& rhs) const -> bool {
    return m_value == rhs.m_value;
}

auto usize::operator==(usize const& rhs) const -> bool {
    return equal(rhs);
}

auto usize::not_equal(usize const& rhs) const -> bool {
    return m_value != rhs.m_value;
}

auto usize::operator!=(usize const& rhs) const -> bool {
    return not_equal(rhs);
}

auto usize::greater(usize const& rhs) const -> bool {
    return m_value > rhs.m_value;
}

auto usize::operator>(usize const& rhs) const -> bool {
    return greater(rhs);
}

auto usize::greater_equal(usize const& rhs) const -> bool {
    return m_value >= rhs.m_value;
}

auto usize::operator>=(usize const& rhs) const -> bool {
    return greater_equal(rhs);
}

auto usize::less(usize const& rhs) const -> bool {
    return m_value < rhs.m_value;
}

auto usize::operator<(usize const& rhs) const -> bool {
    return less(rhs);
}

auto usize::less_equal(usize const& rhs) const -> bool {
    return m_value <= rhs.m_value;
}

auto usize::operator<=(usize const& rhs) const -> bool {
    return less_equal(rhs);
}

auto usize::bit_one_complement() const -> usize {
    return ~m_value;
}

auto usize::operator~() const -> usize {
    return bit_one_complement();
}

auto usize::bit_and(usize const& rhs) const -> usize {
    return m_value & rhs.m_value;
}

auto usize::operator&(usize const& rhs) const -> usize {
    return bit_and(rhs);
}

auto usize::bit_and_assign(usize const& rhs) -> void {
    m_value &= rhs.m_value;
}

auto usize::operator&=(usize const& rhs) -> usize& {
    bit_and_assign(rhs);
    return *this;
}

auto usize::bit_or(usize const& rhs) const -> usize {
    return m_value | rhs.m_value;
}

auto usize::operator|(usize const& rhs) const -> usize {
    return bit_or(rhs);
}

auto usize::bit_or_assign(usize const& rhs) -> void {
    m_value |= rhs.m_value;
}

auto usize::operator|=(usize const& rhs) -> usize& {
    bit_or_assign(rhs);
    return *this;
}

auto usize::bit_xor(usize const& rhs) const -> usize {
    return m_value ^ rhs.m_value;
}

auto usize::operator^(usize const& rhs) const -> usize {
    return bit_xor(rhs);
}

auto usize::bit_xor_assign(usize const& rhs) -> void {
    m_value ^= rhs.m_value;
}

auto usize::operator^=(usize const& rhs) -> usize& {
    bit_xor_assign(rhs);
    return *this;
}

auto usize::try_left_shift(usize const& rhs) const -> ErrorOr<usize> {
    if ( rhs >= bit_count() ) {
        return Error::from_code(ErrorCode::ShiftOverflow);
    } else {
        return usize(static_cast<CCIntegerType>(m_value << rhs.m_value));
    }
}

auto usize::left_shift(usize const& rhs) const -> usize {
    if constexpr ( CCLangSafeIntegerOperations ) {
        return must$(try_left_shift(rhs));
    } else {
        return static_cast<CCIntegerType>(m_value << rhs.m_value);
    }
}

auto usize::operator<<(usize const& rhs) const -> usize {
    return left_shift(rhs);
}

auto usize::try_left_shift_assign(usize const& rhs) -> ErrorOr<void> {
    if ( rhs >= bit_count().as<usize>() ) {
        return Error::from_code(ErrorCode::ShiftOverflow);
    } else {
        m_value <<= rhs.m_value;
        return {};
    }
}

auto usize::left_shift_assign(usize const& rhs) -> void {
    if constexpr ( CCLangSafeIntegerOperations ) {
        must$(try_left_shift_assign(rhs));
    } else {
        m_value <<= rhs.m_value;
    }
}

auto usize::operator<<=(usize const& rhs) -> usize& {
    left_shift_assign(rhs);
    return *this;
}

auto usize::try_right_shift(usize const& rhs) const -> ErrorOr<usize> {
    if ( rhs >= bit_count().as<usize>() ) {
        return Error::from_code(ErrorCode::ShiftOverflow);
    } else {
        return usize(static_cast<CCIntegerType>(m_value >> rhs.m_value));
    }
}

auto usize::right_shift(usize const& rhs) const -> usize {
    if constexpr ( CCLangSafeIntegerOperations ) {
        return must$(try_right_shift(rhs));
    } else {
        return static_cast<CCIntegerType>(m_value >> rhs.m_value);
    }
}

auto usize::operator>>(usize const& rhs) const -> usize {
    return right_shift(rhs);
}

auto usize::try_right_shift_assign(usize const& rhs) -> ErrorOr<void> {
    if ( rhs >= bit_count().as<usize>() ) {
        return Error::from_code(ErrorCode::ShiftOverflow);
    } else {
        m_value >>= rhs.m_value;
        return {};
    }
}

auto usize::right_shift_assign(usize const& rhs) -> void {
    if constexpr ( CCLangSafeIntegerOperations ) {
        must$(try_right_shift_assign(rhs));
    } else {
        m_value >>= rhs.m_value;
    }
}

auto usize::operator>>=(usize const& rhs) -> usize& {
    right_shift_assign(rhs);
    return *this;
}

auto usize::operator++() -> usize& {
    add_assign(1);
    return *this;
}

auto usize::operator++(int) -> usize {
    auto __prev = *this;
    add_assign(1);
    return __prev;
}

auto usize::operator--() -> usize& {
    sub_assign(1);
    return *this;
}

auto usize::operator--(int) -> usize {
    auto __prev = *this;
    sub_assign(1);
    return __prev;
}

auto usize::try_add(usize const& rhs) const -> ErrorOr<usize> {
    usize::CCIntegerType __value;
    if ( __builtin_add_overflow(m_value, rhs.m_value, &__value) ) {
        return Error::from_code(ErrorCode::IntOverflow);
    } else {
        return usize(__value);
    }
}

auto usize::add(usize const& rhs) const -> usize {
    if constexpr ( CCLangSafeIntegerOperations ) {
        return must$(try_add(rhs));
    } else {
        return m_value + rhs.m_value;
    }
}

auto usize::operator+(usize const& rhs) const -> usize {
    return add(rhs);
}

auto usize::try_add_assign(usize const& rhs) -> ErrorOr<void> {
    m_value = try$(try_add(rhs)).m_value;
    return {};
}

auto usize::add_assign(usize const& rhs) -> void {
    if constexpr ( CCLangSafeIntegerOperations ) {
        must$(try_add_assign(rhs));
    } else {
        m_value += rhs.m_value;
    }
}

auto usize::operator+=(usize const& rhs) -> usize& {
    add_assign(rhs);
    return *this;
}

auto usize::try_sub(usize const& rhs) const -> ErrorOr<usize> {
    usize::CCIntegerType __value;
    if ( __builtin_sub_overflow(m_value, rhs.m_value, &__value) ) {
        return Error::from_code(ErrorCode::IntOverflow);
    } else {
        return usize(__value);
    }
}

auto usize::sub(usize const& rhs) const -> usize {
    if constexpr ( CCLangSafeIntegerOperations ) {
        return must$(try_sub(rhs));
    } else {
        return m_value - rhs.m_value;
    }
}

auto usize::operator-(usize const& rhs) const -> usize {
    return sub(rhs);
}

auto usize::try_sub_assign(usize const& rhs) -> ErrorOr<void> {
    m_value = try$(try_sub(rhs)).m_value;
    return {};
}

auto usize::sub_assign(usize const& rhs) -> void {
    if constexpr ( CCLangSafeIntegerOperations ) {
        must$(try_sub_assign(rhs));
    } else {
        m_value -= rhs.m_value;
    }
}

auto usize::operator-=(usize const& rhs) -> usize& {
    sub_assign(rhs);
    return *this;
}

auto usize::try_mul(usize const& rhs) const -> ErrorOr<usize> {
    usize::CCIntegerType __value;
    if ( __builtin_mul_overflow(m_value, rhs.m_value, &__value) ) {
        return Error::from_code(ErrorCode::IntOverflow);
    } else {
        return usize(__value);
    }
}

auto usize::mul(usize const& rhs) const -> usize {
    if constexpr ( CCLangSafeIntegerOperations ) {
        return must$(try_mul(rhs));
    } else {
        return m_value * rhs.m_value;
    }
}

auto usize::operator*(usize const& rhs) const -> usize {
    return mul(rhs);
}

auto usize::try_mul_assign(usize const& rhs) -> ErrorOr<void> {
    m_value = try$(try_mul(rhs)).m_value;
    return {};
}

auto usize::mul_assign(usize const& rhs) -> void {
    if constexpr ( CCLangSafeIntegerOperations ) {
        must$(try_mul_assign(rhs));
    } else {
        m_value *= rhs.m_value;
    }
}

auto usize::operator*=(usize const& rhs) -> usize& {
    mul_assign(rhs);
    return *this;
}

auto usize::try_div(usize const& rhs) const -> ErrorOr<usize> {
    if ( rhs == 0 ) {
        return Error::from_code(ErrorCode::DivisionByZero);
    } else {
        return usize(static_cast<CCIntegerType>(m_value / rhs.m_value));
    }
}

auto usize::div(usize const& rhs) const -> usize {
    if constexpr ( CCLangSafeIntegerOperations ) {
        return must$(try_div(rhs));
    } else {
        return m_value / rhs.m_value;
    }
}

auto usize::operator/(usize const& rhs) const -> usize {
    return div(rhs);
}

auto usize::try_div_assign(usize const& rhs) -> ErrorOr<void> {
    m_value = try$(try_div(rhs)).m_value;
    return {};
}

auto usize::div_assign(usize const& rhs) -> void {
    if constexpr ( CCLangSafeIntegerOperations ) {
        must$(try_div_assign(rhs));
    } else {
        m_value /= rhs.m_value;
    }
}

auto usize::operator/=(usize const& rhs) -> usize& {
    div_assign(rhs);
    return *this;
}

auto usize::operator%(usize const& rhs) const -> usize {
    return m_value % rhs.m_value;
}

auto usize::operator%=(usize const& rhs) -> usize& {
    m_value %= rhs.m_value;
    return *this;
}

auto usize::atomic_load(MemOrder mem_order) volatile -> usize {
    CCIntegerType __value;
    __atomic_load(&m_value, &__value, static_cast<UnderlyingType<MemOrder>>(mem_order));
    return __value;
}

auto usize::atomic_store(usize rhs, MemOrder mem_order) volatile -> void {
    __atomic_store(&m_value, &rhs.m_value, static_cast<UnderlyingType<MemOrder>>(mem_order));
}

auto usize::atomic_add(usize rhs, MemOrder mem_order) volatile -> void {
    (void)atomic_fetch_add(rhs, mem_order);
}

auto usize::atomic_sub(usize rhs, MemOrder mem_order) volatile -> void {
    (void)atomic_fetch_sub(rhs, mem_order);
}

auto usize::atomic_fetch_add(usize rhs, MemOrder mem_order) volatile -> usize {
    return __atomic_fetch_add(&m_value, rhs.m_value, static_cast<UnderlyingType<MemOrder>>(mem_order));
}

auto usize::atomic_fetch_sub(usize rhs, MemOrder mem_order) volatile -> usize {
    return __atomic_fetch_sub(&m_value, rhs.m_value, static_cast<UnderlyingType<MemOrder>>(mem_order));
}

auto usize::atomic_add_fetch(usize rhs, MemOrder mem_order) volatile -> usize {
    return __atomic_add_fetch(&m_value, rhs.m_value, static_cast<UnderlyingType<MemOrder>>(mem_order));
}

auto usize::atomic_sub_fetch(usize rhs, MemOrder mem_order) volatile -> usize {
    return __atomic_sub_fetch(&m_value, rhs.m_value, static_cast<UnderlyingType<MemOrder>>(mem_order));
}

auto usize::hash_code() const -> usize {
    auto hash_key = as<usize>();
    hash_key += ~(hash_key << 15);
    hash_key ^= (hash_key >> 10);
    hash_key += (hash_key << 3);
    hash_key ^= (hash_key >> 6);
    hash_key += ~(hash_key << 11);
    hash_key ^= (hash_key >> 16);
    return hash_key;
}

auto operator<<(__SIZE_TYPE__ lhs, usize rhs) -> usize {
    return usize(lhs) << rhs;
}

auto operator>>(__SIZE_TYPE__ lhs, usize rhs) -> usize {
    return usize(lhs) >> rhs;
}