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

#define OOCommonIntegerWrapperImpl$(WrapperName)                                                                                                               \
    auto WrapperName::try_left_shift(WrapperName const& rhs) const->ErrorOr<WrapperName> {                                                                     \
        if ( rhs >= bit_count().as<WrapperName>() )                                                                                                            \
            return Error::new_from_code(ErrorCode::ShiftOverflow);                                                                                             \
        else                                                                                                                                                   \
            return WrapperName{ static_cast<CCIntegerType>(m_value << rhs.m_value) };                                                                          \
    }                                                                                                                                                          \
    auto WrapperName::left_shift(WrapperName const& rhs) const->WrapperName {                                                                                  \
        if constexpr ( cclang_debugging ) {                                                                                                                    \
            return must$(try_left_shift(rhs));                                                                                                                 \
        } else {                                                                                                                                               \
            return static_cast<CCIntegerType>(m_value << rhs.m_value);                                                                                         \
        }                                                                                                                                                      \
    }                                                                                                                                                          \
    auto WrapperName::try_left_shift_assign(WrapperName const& rhs)->ErrorOr<void> {                                                                           \
        if ( rhs >= bit_count().as<WrapperName>() )                                                                                                            \
            return Error::new_from_code(ErrorCode::ShiftOverflow);                                                                                             \
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
    auto WrapperName::try_right_shift(WrapperName const& rhs) const->ErrorOr<WrapperName> {                                                                    \
        if ( rhs >= bit_count().as<WrapperName>() )                                                                                                            \
            return Error::new_from_code(ErrorCode::ShiftOverflow);                                                                                             \
        else                                                                                                                                                   \
            return WrapperName{ static_cast<CCIntegerType>(m_value >> rhs.m_value) };                                                                          \
    }                                                                                                                                                          \
    auto WrapperName::right_shift(WrapperName const& rhs) const->WrapperName {                                                                                 \
        if constexpr ( cclang_debugging ) {                                                                                                                    \
            return must$(try_right_shift(rhs));                                                                                                                \
        } else {                                                                                                                                               \
            return static_cast<CCIntegerType>(m_value >> rhs.m_value);                                                                                         \
        }                                                                                                                                                      \
    }                                                                                                                                                          \
    auto WrapperName::try_right_shift_assign(WrapperName const& rhs)->ErrorOr<void> {                                                                          \
        if ( rhs >= bit_count().as<WrapperName>() )                                                                                                            \
            return Error::new_from_code(ErrorCode::ShiftOverflow);                                                                                             \
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
    auto WrapperName::try_add(WrapperName const& rhs) const->ErrorOr<WrapperName> {                                                                            \
        WrapperName::CCIntegerType __value;                                                                                                                    \
        if ( __builtin_add_overflow(m_value, rhs.m_value, &__value) )                                                                                          \
            return Error::new_from_code(ErrorCode::IntOverflow);                                                                                               \
        else                                                                                                                                                   \
            return WrapperName{ __value };                                                                                                                     \
    }                                                                                                                                                          \
    auto WrapperName::add(WrapperName const& rhs) const->WrapperName {                                                                                         \
        if constexpr ( cclang_debugging ) {                                                                                                                    \
            return must$(try_add(rhs));                                                                                                                        \
        } else {                                                                                                                                               \
            return m_value + rhs.m_value;                                                                                                                      \
        }                                                                                                                                                      \
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
    auto WrapperName::try_sub(WrapperName const& rhs) const->ErrorOr<WrapperName> {                                                                            \
        WrapperName::CCIntegerType __value;                                                                                                                    \
        if ( __builtin_sub_overflow(m_value, rhs.m_value, &__value) )                                                                                          \
            return Error::new_from_code(ErrorCode::IntOverflow);                                                                                               \
        else                                                                                                                                                   \
            return WrapperName{ __value };                                                                                                                     \
    }                                                                                                                                                          \
    auto WrapperName::sub(WrapperName const& rhs) const->WrapperName {                                                                                         \
        if constexpr ( cclang_debugging ) {                                                                                                                    \
            return must$(try_sub(rhs));                                                                                                                        \
        } else {                                                                                                                                               \
            return m_value - rhs.m_value;                                                                                                                      \
        }                                                                                                                                                      \
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
    auto WrapperName::try_mul(WrapperName const& rhs) const->ErrorOr<WrapperName> {                                                                            \
        WrapperName::CCIntegerType __value;                                                                                                                    \
        if ( __builtin_mul_overflow(m_value, rhs.m_value, &__value) )                                                                                          \
            return Error::new_from_code(ErrorCode::IntOverflow);                                                                                               \
        else                                                                                                                                                   \
            return WrapperName{ __value };                                                                                                                     \
    }                                                                                                                                                          \
    auto WrapperName::mul(WrapperName const& rhs) const->WrapperName {                                                                                         \
        if constexpr ( cclang_debugging ) {                                                                                                                    \
            must$(try_mul(rhs));                                                                                                                               \
        } else {                                                                                                                                               \
            return m_value * rhs.m_value;                                                                                                                      \
        }                                                                                                                                                      \
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
    auto WrapperName::try_div(WrapperName const& rhs) const->ErrorOr<WrapperName> {                                                                            \
        if ( rhs == 0 )                                                                                                                                        \
            return Error::new_from_code(ErrorCode::DivisionByZero);                                                                                            \
        else                                                                                                                                                   \
            return WrapperName{ static_cast<CCIntegerType>(m_value / rhs.m_value) };                                                                           \
    }                                                                                                                                                          \
    auto WrapperName::div(WrapperName const& rhs) const->WrapperName {                                                                                         \
        if constexpr ( cclang_debugging ) {                                                                                                                    \
            return must$(try_div(rhs));                                                                                                                        \
        } else {                                                                                                                                               \
            return m_value / rhs.m_value;                                                                                                                      \
        }                                                                                                                                                      \
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
    auto WrapperName::atomic_load(MemOrder mem_order) volatile->WrapperName {                                                                                  \
        CCIntegerType __value;                                                                                                                                 \
        __atomic_load(&m_value, &__value, static_cast<UnderlyingType<MemOrder>>(mem_order));                                                                   \
        return __value;                                                                                                                                        \
    }                                                                                                                                                          \
    auto WrapperName::atomic_store(WrapperName rhs, MemOrder mem_order) volatile->void {                                                                       \
        __atomic_store(&m_value, &rhs.m_value, static_cast<UnderlyingType<MemOrder>>(mem_order));                                                              \
    }                                                                                                                                                          \
    auto WrapperName::atomic_add(WrapperName rhs, MemOrder mem_order) volatile->void { (void)atomic_fetch_add(rhs, mem_order); }                               \
    auto WrapperName::atomic_sub(WrapperName rhs, MemOrder mem_order) volatile->void { (void)atomic_fetch_sub(rhs, mem_order); }                               \
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
    }

#define OOUnsignedIntegerWrapperImpl$(WrapperName)                                                                                                             \
    auto WrapperName::try_bit_at(WrapperName index) const->ErrorOr<bool> {                                                                                     \
        if ( index > bit_count().as<WrapperName>() )                                                                                                           \
            return Error::new_from_code(ErrorCode::IndexOutOfRange);                                                                                           \
        else                                                                                                                                                   \
            return (m_value & (1 << index.unwrap())) == (1 << index.unwrap());                                                                                 \
    }                                                                                                                                                          \
    auto WrapperName::bit_at(WrapperName index) const->bool { return must$(try_bit_at(index)); }                                                               \
    auto WrapperName::try_set_bit(WrapperName index, bool value)->ErrorOr<void> {                                                                              \
        if ( index > bit_count().as<WrapperName>() )                                                                                                           \
            return Error::new_from_code(ErrorCode::IndexOutOfRange);                                                                                           \
                                                                                                                                                               \
        if ( value )                                                                                                                                           \
            m_value |= 1 << index.unwrap();                                                                                                                    \
        else                                                                                                                                                   \
            m_value &= ~(1 << index.unwrap());                                                                                                                 \
        return {};                                                                                                                                             \
    }                                                                                                                                                          \
    auto WrapperName::set_bit(WrapperName index, bool value)->void { must$(try_set_bit(index, value)); }                                                       \
    auto WrapperName::count_zeroes() const->usize { return 0; /* TODO implement */ }                                                                           \
    auto WrapperName::count_ones() const->usize { return 0; /* TODO implement */ }                                                                             \
    OOCommonIntegerWrapperImpl$(WrapperName)

OOUnsignedIntegerWrapperImpl$(u8);
OOUnsignedIntegerWrapperImpl$(u16);
OOUnsignedIntegerWrapperImpl$(u32);
OOUnsignedIntegerWrapperImpl$(u64);
OOUnsignedIntegerWrapperImpl$(usize);

OOCommonIntegerWrapperImpl$(i8);
OOCommonIntegerWrapperImpl$(i16);
OOCommonIntegerWrapperImpl$(i32);
OOCommonIntegerWrapperImpl$(i64);
OOCommonIntegerWrapperImpl$(isize);
