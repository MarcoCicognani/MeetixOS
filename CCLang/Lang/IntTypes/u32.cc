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

#include <CCLang/Lang/IntTypes/u32.hh>

#include <CCLang/Core/ErrorOr.hh>
#include <CCLang/Lang/Cxx.hh>
#include <CCLang/Lang/IntTypes/usize.hh>
#include <CCLang/Lang/Must.hh>
#include <CCLang/Lang/Range.hh>
#include <CCLang/Lang/Try.hh>

auto u32::max(u32 const& lhs, u32 const& rhs) -> u32 {
   if ( lhs > rhs ) {
       return lhs;
   } else {
       return rhs;
   }
}

auto u32::min(u32 const& lhs, u32 const& rhs) -> u32 {
   if ( lhs < rhs ) {
       return lhs;
   } else {
       return rhs;
   }
}

auto u32::range(u32 const& begin, u32 const& end) -> Range<u32> {
   return Range<u32>(begin, end);
}

auto u32::range_inclusive(u32 const& begin, u32 const& last) -> RangeInclusive<u32> {
   return RangeInclusive<u32>(begin, last);
}

u32::u32(__UINT32_TYPE__ value)
   : m_value(value) {
}

auto u32::operator=(__UINT32_TYPE__ value) -> u32& {
   u32 integer = value;
   swap(integer);
   return *this;
}

auto u32::swap(u32& rhs) -> void {
   Cxx::swap(m_value, rhs.m_value);
}

auto u32::is_signed() const -> bool {
   return false;
}

auto u32::try_bit_at(usize index) const -> ErrorOr<bool> {
   if ( index > bit_count() ) {
       return Error::from_code(ErrorCode::IndexOutOfRange);
   } else {
       return (m_value & (1 << index.unwrap())) == (1 << index.unwrap());
   }
}

auto u32::bit_at(usize index) const -> bool {
   return must$(try_bit_at(index));
}

auto u32::try_set_bit(usize index, bool value) -> ErrorOr<void> {
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

auto u32::set_bit(usize index, bool value) -> void {
   must$(try_set_bit(index, value));
}

auto u32::count_zeroes() const -> usize {
   return m_value; /* TODO implement */
}

auto u32::count_ones() const -> usize {
   return m_value; /* TODO implement */
}

auto u32::bit_count() const -> usize {
   return sizeof(__UINT32_TYPE__) * 8;
}

auto u32::unwrap() const -> __UINT32_TYPE__ {
   return m_value;
}

auto u32::equal(u32 const& rhs) const -> bool {
   return m_value == rhs.m_value;
}

auto u32::operator==(u32 const& rhs) const -> bool {
   return equal(rhs);
}

auto u32::not_equal(u32 const& rhs) const -> bool {
   return m_value != rhs.m_value;
}

auto u32::operator!=(u32 const& rhs) const -> bool {
   return not_equal(rhs);
}

auto u32::greater(u32 const& rhs) const -> bool {
   return m_value > rhs.m_value;
}

auto u32::operator>(u32 const& rhs) const -> bool {
   return greater(rhs);
}

auto u32::greater_equal(u32 const& rhs) const -> bool {
   return m_value >= rhs.m_value;
}

auto u32::operator>=(u32 const& rhs) const -> bool {
   return greater_equal(rhs);
}

auto u32::less(u32 const& rhs) const -> bool {
   return m_value < rhs.m_value;
}

auto u32::operator<(u32 const& rhs) const -> bool {
   return less(rhs);
}

auto u32::less_equal(u32 const& rhs) const -> bool {
   return m_value <= rhs.m_value;
}

auto u32::operator<=(u32 const& rhs) const -> bool {
   return less_equal(rhs);
}

auto u32::bit_one_complement() const -> u32 {
   return ~m_value;
}

auto u32::operator~() const -> u32 {
   return bit_one_complement();
}

auto u32::bit_and(u32 const& rhs) const -> u32 {
   return m_value & rhs.m_value;
}

auto u32::operator&(u32 const& rhs) const -> u32 {
   return bit_and(rhs);
}

auto u32::bit_and_assign(u32 const& rhs) -> void {
   m_value &= rhs.m_value;
}

auto u32::operator&=(u32 const& rhs) -> u32& {
   bit_and_assign(rhs);
   return *this;
}

auto u32::bit_or(u32 const& rhs) const -> u32 {
   return m_value | rhs.m_value;
}

auto u32::operator|(u32 const& rhs) const -> u32 {
   return bit_or(rhs);
}

auto u32::bit_or_assign(u32 const& rhs) -> void {
   m_value |= rhs.m_value;
}

auto u32::operator|=(u32 const& rhs) -> u32& {
   bit_or_assign(rhs);
   return *this;
}

auto u32::bit_xor(u32 const& rhs) const -> u32 {
   return m_value ^ rhs.m_value;
}

auto u32::operator^(u32 const& rhs) const -> u32 {
   return bit_xor(rhs);
}

auto u32::bit_xor_assign(u32 const& rhs) -> void {
   m_value ^= rhs.m_value;
}

auto u32::operator^=(u32 const& rhs) -> u32& {
   bit_xor_assign(rhs);
   return *this;
}

auto u32::try_left_shift(u32 const& rhs) const -> ErrorOr<u32> {
   if ( rhs >= bit_count().as<u32>() ) {
       return Error::from_code(ErrorCode::ShiftOverflow);
   } else {
       return u32(static_cast<CCIntegerType>(m_value << rhs.m_value));
   }
}

auto u32::left_shift(u32 const& rhs) const -> u32 {
   if constexpr ( CCLangSafeIntegerOperations ) {
       return must$(try_left_shift(rhs));
   } else {
       return static_cast<CCIntegerType>(m_value << rhs.m_value);
   }
}

auto u32::operator<<(u32 const& rhs) const -> u32 {
   return left_shift(rhs);
}

auto u32::try_left_shift_assign(u32 const& rhs) -> ErrorOr<void> {
   if ( rhs >= bit_count().as<u32>() ) {
       return Error::from_code(ErrorCode::ShiftOverflow);
   } else {
       m_value <<= rhs.m_value;
       return {};
   }
}

auto u32::left_shift_assign(u32 const& rhs) -> void {
   if constexpr ( CCLangSafeIntegerOperations ) {
       must$(try_left_shift_assign(rhs));
   } else {
       m_value <<= rhs.m_value;
   }
}

auto u32::operator<<=(u32 const& rhs) -> u32& {
   left_shift_assign(rhs);
   return *this;
}

auto u32::try_right_shift(u32 const& rhs) const -> ErrorOr<u32> {
   if ( rhs >= bit_count().as<u32>() ) {
       return Error::from_code(ErrorCode::ShiftOverflow);
   } else {
       return u32(static_cast<CCIntegerType>(m_value >> rhs.m_value));
   }
}

auto u32::right_shift(u32 const& rhs) const -> u32 {
   if constexpr ( CCLangSafeIntegerOperations ) {
       return must$(try_right_shift(rhs));
   } else {
       return static_cast<CCIntegerType>(m_value >> rhs.m_value);
   }
}

auto u32::operator>>(u32 const& rhs) const -> u32 {
   return right_shift(rhs);
}

auto u32::try_right_shift_assign(u32 const& rhs) -> ErrorOr<void> {
   if ( rhs >= bit_count().as<u32>() ) {
       return Error::from_code(ErrorCode::ShiftOverflow);
   } else {
       m_value >>= rhs.m_value;
       return {};
   }
}

auto u32::right_shift_assign(u32 const& rhs) -> void {
   if constexpr ( CCLangSafeIntegerOperations ) {
       must$(try_right_shift_assign(rhs));
   } else {
       m_value >>= rhs.m_value;
   }
}

auto u32::operator>>=(u32 const& rhs) -> u32& {
   right_shift_assign(rhs);
   return *this;
}

auto u32::operator++() -> u32& {
   add_assign(1);
   return *this;
}

auto u32::operator++(int) -> u32 {
   auto __prev = *this;
   add_assign(1);
   return __prev;
}

auto u32::operator--() -> u32& {
   sub_assign(1);
   return *this;
}

auto u32::operator--(int) -> u32 {
   auto __prev = *this;
   sub_assign(1);
   return __prev;
}

auto u32::try_add(u32 const& rhs) const -> ErrorOr<u32> {
   u32::CCIntegerType __value;
   if ( __builtin_add_overflow(m_value, rhs.m_value, &__value) ) {
       return Error::from_code(ErrorCode::IntOverflow);
   } else {
       return u32(__value);
   }
}

auto u32::add(u32 const& rhs) const -> u32 {
   if constexpr ( CCLangSafeIntegerOperations ) {
       return must$(try_add(rhs));
   } else {
       return m_value + rhs.m_value;
   }
}

auto u32::operator+(u32 const& rhs) const -> u32 {
   return add(rhs);
}

auto u32::try_add_assign(u32 const& rhs) -> ErrorOr<void> {
   m_value = try$(try_add(rhs)).m_value;
   return {};
}

auto u32::add_assign(u32 const& rhs) -> void {
   if constexpr ( CCLangSafeIntegerOperations ) {
       must$(try_add_assign(rhs));
   } else {
       m_value += rhs.m_value;
   }
}

auto u32::operator+=(u32 const& rhs) -> u32& {
   add_assign(rhs);
   return *this;
}

auto u32::try_sub(u32 const& rhs) const -> ErrorOr<u32> {
   u32::CCIntegerType __value;
   if ( __builtin_sub_overflow(m_value, rhs.m_value, &__value) ) {
       return Error::from_code(ErrorCode::IntOverflow);
   } else {
       return u32(__value);
   }
}

auto u32::sub(u32 const& rhs) const -> u32 {
   if constexpr ( CCLangSafeIntegerOperations ) {
       return must$(try_sub(rhs));
   } else {
       return m_value - rhs.m_value;
   }
}

auto u32::operator-(u32 const& rhs) const -> u32 {
   return sub(rhs);
}

auto u32::try_sub_assign(u32 const& rhs) -> ErrorOr<void> {
   m_value = try$(try_sub(rhs)).m_value;
   return {};
}

auto u32::sub_assign(u32 const& rhs) -> void {
   if constexpr ( CCLangSafeIntegerOperations ) {
       must$(try_sub_assign(rhs));
   } else {
       m_value -= rhs.m_value;
   }
}

auto u32::operator-=(u32 const& rhs) -> u32& {
   sub_assign(rhs);
   return *this;
}

auto u32::try_mul(u32 const& rhs) const -> ErrorOr<u32> {
   u32::CCIntegerType __value;
   if ( __builtin_mul_overflow(m_value, rhs.m_value, &__value) ) {
       return Error::from_code(ErrorCode::IntOverflow);
   } else {
       return u32(__value);
   }
}

auto u32::mul(u32 const& rhs) const -> u32 {
   if constexpr ( CCLangSafeIntegerOperations ) {
       return must$(try_mul(rhs));
   } else {
       return m_value * rhs.m_value;
   }
}

auto u32::operator*(u32 const& rhs) const -> u32 {
   return mul(rhs);
}

auto u32::try_mul_assign(u32 const& rhs) -> ErrorOr<void> {
   m_value = try$(try_mul(rhs)).m_value;
   return {};
}

auto u32::mul_assign(u32 const& rhs) -> void {
   if constexpr ( CCLangSafeIntegerOperations ) {
       must$(try_mul_assign(rhs));
   } else {
       m_value *= rhs.m_value;
   }
}

auto u32::operator*=(u32 const& rhs) -> u32& {
   mul_assign(rhs);
   return *this;
}

auto u32::try_div(u32 const& rhs) const -> ErrorOr<u32> {
   if ( rhs == 0 ) {
       return Error::from_code(ErrorCode::DivisionByZero);
   } else {
       return u32(static_cast<CCIntegerType>(m_value / rhs.m_value));
   }
}

auto u32::div(u32 const& rhs) const -> u32 {
   if constexpr ( CCLangSafeIntegerOperations ) {
       return must$(try_div(rhs));
   } else {
       return m_value / rhs.m_value;
   }
}

auto u32::operator/(u32 const& rhs) const -> u32 {
   return div(rhs);
}

auto u32::try_div_assign(u32 const& rhs) -> ErrorOr<void> {
   m_value = try$(try_div(rhs)).m_value;
   return {};
}

auto u32::div_assign(u32 const& rhs) -> void {
   if constexpr ( CCLangSafeIntegerOperations ) {
       must$(try_div_assign(rhs));
   } else {
       m_value /= rhs.m_value;
   }
}

auto u32::operator/=(u32 const& rhs) -> u32& {
   div_assign(rhs);
   return *this;
}

auto u32::operator%(u32 const& rhs) const -> u32 {
   return m_value % rhs.m_value;
}

auto u32::operator%=(u32 const& rhs) -> u32& {
   m_value %= rhs.m_value;
   return *this;
}

auto u32::atomic_load(MemOrder mem_order) volatile -> u32 {
   CCIntegerType __value;
   __atomic_load(&m_value, &__value, static_cast<UnderlyingType<MemOrder>>(mem_order));
   return __value;
}

auto u32::atomic_store(u32 rhs, MemOrder mem_order) volatile -> void {
   __atomic_store(&m_value, &rhs.m_value, static_cast<UnderlyingType<MemOrder>>(mem_order));
}

auto u32::atomic_add(u32 rhs, MemOrder mem_order) volatile -> void {
   (void)atomic_fetch_add(rhs, mem_order);
}

auto u32::atomic_sub(u32 rhs, MemOrder mem_order) volatile -> void {
   (void)atomic_fetch_sub(rhs, mem_order);
}

auto u32::atomic_fetch_add(u32 rhs, MemOrder mem_order) volatile -> u32 {
   return __atomic_fetch_add(&m_value, rhs.m_value, static_cast<UnderlyingType<MemOrder>>(mem_order));
}

auto u32::atomic_fetch_sub(u32 rhs, MemOrder mem_order) volatile -> u32 {
   return __atomic_fetch_sub(&m_value, rhs.m_value, static_cast<UnderlyingType<MemOrder>>(mem_order));
}

auto u32::atomic_add_fetch(u32 rhs, MemOrder mem_order) volatile -> u32 {
   return __atomic_add_fetch(&m_value, rhs.m_value, static_cast<UnderlyingType<MemOrder>>(mem_order));
}

auto u32::atomic_sub_fetch(u32 rhs, MemOrder mem_order) volatile -> u32 {
   return __atomic_sub_fetch(&m_value, rhs.m_value, static_cast<UnderlyingType<MemOrder>>(mem_order));
}

auto u32::hash_code() const -> usize {
   auto hash_key = as<usize>();
   hash_key += ~(hash_key << 15);
   hash_key ^= (hash_key >> 10);
   hash_key += (hash_key << 3);
   hash_key ^= (hash_key >> 6);
   hash_key += ~(hash_key << 11);
   hash_key ^= (hash_key >> 16);
   return hash_key;
}

auto operator<<(__UINT32_TYPE__ lhs, u32 rhs) -> u32 {
   return u32(lhs) << rhs;
}

auto operator>>(__UINT32_TYPE__ lhs, u32 rhs) -> u32 {
   return u32(lhs) >> rhs;
}