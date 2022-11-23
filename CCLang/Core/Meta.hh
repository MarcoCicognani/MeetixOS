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

/* clang-format off */

namespace Details {

template<typename T>
using AddConst = T const;

template<typename T>
using AddVolatile = T volatile;

template<typename T>
struct AddReference {
    using TLValue = T&;
    using TRValue = T&&;
};

template<class T>
struct AddConstToReference {
    using Type = T;
};
template<class T>
struct AddConstToReference<T&> {
    using Type = AddConst<T>&;
};
template<class T>
struct AddConstToReference<T&&> {
    using Type = AddConst<T>&&;
};

template<typename T>
struct RemoveConst {
    using Type = T;
};
template<typename T>
struct RemoveConst<T const> {
    using Type = T;
};

template<typename T>
struct RemovePointer {
    using Type = T;
};
template<typename T>
struct RemovePointer<T*> {
    using Type = T;
};

template<typename T>
struct RemoveReference {
    using Type = T;
};
template<typename T>
struct RemoveReference<T&> {
    using Type = T;
};
template<typename T>
struct RemoveReference<T&&> {
    using Type = T;
};

template<typename T>
struct RemoveVolatile {
    using Type = T;
};
template<typename T>
struct RemoveVolatile<T volatile> {
    using Type = T;
};

template<bool Condition, typename TTrue, typename TFalse>
struct Conditional {
    using Type = TTrue;
};
template<typename TTrue, typename TFalse>
struct Conditional<false, TTrue, TFalse> {
    using Type = TFalse;
};

template<typename T> constexpr bool is_const          = false;
template<typename T> constexpr bool is_const<T const> = true;

template<typename T> constexpr bool is_volatile             = false;
template<typename T> constexpr bool is_volatile<T volatile> = true;

template<typename T>
           constexpr bool is_integral                     = false;
template<> constexpr bool is_integral<bool>               = true;
template<> constexpr bool is_integral<unsigned char>      = true;
template<> constexpr bool is_integral<signed char>        = true;
template<> constexpr bool is_integral<char8_t>            = true;
template<> constexpr bool is_integral<char16_t>           = true;
template<> constexpr bool is_integral<char32_t>           = true;
template<> constexpr bool is_integral<unsigned short>     = true;
template<> constexpr bool is_integral<unsigned int>       = true;
template<> constexpr bool is_integral<unsigned long>      = true;
template<> constexpr bool is_integral<u8>                 = true;
template<> constexpr bool is_integral<u16>                = true;
template<> constexpr bool is_integral<u32>                = true;
template<> constexpr bool is_integral<u64>                = true;
template<> constexpr bool is_integral<usize>              = true;

template<typename T>
           constexpr bool is_floating_point              = false;
template<> constexpr bool is_floating_point<float>       = true;
template<> constexpr bool is_floating_point<double>      = true;
template<> constexpr bool is_floating_point<long double> = true;

template<typename T> constexpr bool is_pointer     = false;
template<typename T> constexpr bool is_pointer<T*> = true;

template<typename T, typename U>
                     constexpr bool is_same       = false;
template<typename T> constexpr bool is_same<T, T> = true;

template<typename T> constexpr bool is_lvalue     = false;
template<typename T> constexpr bool is_lvalue<T&> = true;

template<typename T> constexpr bool is_rvalue      = false;
template<typename T> constexpr bool is_rvalue<T&&> = true;

template<typename T>
struct MakeSigned {
    using Type = void;
};
template<>
struct MakeSigned<char> {
    using Type = signed char;
};
template<>
struct MakeSigned<unsigned char> {
    using Type = signed char;
};
template<>
struct MakeSigned<unsigned short> {
    using Type = signed short;
};
template<>
struct MakeSigned<unsigned int> {
    using Type = signed int;
};
template<>
struct MakeSigned<unsigned long> {
    using Type = signed long;
};
template<>
struct MakeSigned<unsigned long long> {
    using Type = signed long long;
};
template<>
struct MakeSigned<signed char> {
    using Type = signed char;
};
template<>
struct MakeSigned<signed short> {
    using Type = signed short;
};
template<>
struct MakeSigned<signed int> {
    using Type = signed int;
};
template<>
struct MakeSigned<signed long> {
    using Type = signed long;
};
template<>
struct MakeSigned<signed long long> {
    using Type = signed long long;
};
template<>
struct MakeSigned<u8> {
    using Type = i8;
};
template<>
struct MakeSigned<u16> {
    using Type = i16;
};
template<>
struct MakeSigned<u32> {
    using Type = i32;
};
template<>
struct MakeSigned<u64> {
    using Type = i64;
};
template<>
struct MakeSigned<usize> {
    using Type = isize;
};

template<typename T>
struct MakeUnsigned {
    using Type = void;
};
template<>
struct MakeUnsigned<char> {
    using Type = unsigned char;
};
template<>
struct MakeUnsigned<signed char> {
    using Type = unsigned char;
};
template<>
struct MakeUnsigned<signed short> {
    using Type = unsigned short;
};
template<>
struct MakeUnsigned<signed int> {
    using Type = unsigned int;
};
template<>
struct MakeUnsigned<signed long> {
    using Type = unsigned long;
};
template<>
struct MakeUnsigned<signed long long> {
    using Type = unsigned long long;
};
template<>
struct MakeUnsigned<unsigned char> {
    using Type = unsigned char;
};
template<>
struct MakeUnsigned<unsigned short> {
    using Type = unsigned short;
};
template<>
struct MakeUnsigned<unsigned int> {
    using Type = unsigned int;
};
template<>
struct MakeUnsigned<unsigned long> {
    using Type = unsigned long;
};
template<>
struct MakeUnsigned<unsigned long long> {
    using Type = unsigned long long;
};
template<>
struct MakeUnsigned<bool> {
    using Type = bool;
};
template<>
struct MakeUnsigned<i8> {
    using Type = u8;
};
template<>
struct MakeUnsigned<i16> {
    using Type = u16;
};
template<>
struct MakeUnsigned<i32> {
    using Type = u32;
};
template<>
struct MakeUnsigned<i64> {
    using Type = u64;
};
template<>
struct MakeUnsigned<isize> {
    using Type = usize;
};

} /* namespace Details */

template<bool Condition, typename TTrue, typename TFalse>
using Conditional = typename Details::Conditional<Condition, TTrue, TFalse>::Type;

template<typename T> using AddConst            = Details::AddConst<T>;
template<typename T> using AddVolatile         = Details::AddVolatile<T>;
template<typename T> using AddLValueReference  = typename Details::AddReference<T>::TLValue;
template<typename T> using AddRValueReference  = typename Details::AddReference<T>::TRValue;
template<typename T> using AddConstToReference = typename Details::AddConstToReference<T>::Type;

template<typename T> using RemoveConst         = typename Details::RemoveConst<T>::Type;
template<typename T> using RemoveVolatile      = typename Details::RemoveVolatile<T>::Type ;
template<typename T> using RemoveConstVolatile = typename Details::RemoveVolatile<RemoveConst<T>>::Type;
template<typename T> using RemovePointer       = typename Details::RemovePointer<T>::Type;
template<typename T> using RemoveReference     = typename Details::RemoveReference<T>::Type;

template<typename T> using MakeSigned   = typename Details::MakeSigned<T>::Type;
template<typename T> using MakeUnsigned = typename Details::MakeUnsigned<T>::Type;

template<typename T> constexpr bool is_const    = Details::is_const<T>;
template<typename T> constexpr bool is_volatile = Details::is_volatile<T>;

template<typename T, typename U>
constexpr bool is_same = Details::is_same<T, U>;

template<typename T> constexpr bool is_integral       = Details::is_integral<MakeUnsigned<RemoveConstVolatile<T>>>;
template<typename T> constexpr bool is_floating_point = Details::is_floating_point<RemoveConstVolatile<T>>;
template<typename T> constexpr bool is_arithmetic     = is_integral<T> || is_floating_point<T>;

template<typename T> constexpr bool is_class = __is_class(T);
template<typename T> constexpr bool is_union = __is_union(T);
template<typename T> constexpr bool is_enum  = __is_enum(T);

template<typename T> using UnderlyingType = __underlying_type(T);

template<typename TDerive, typename TBase>
constexpr bool is_derived = __is_base_of(TBase, TDerive);

template<typename T> constexpr bool is_lvalue = Details::is_lvalue<T>;
template<typename T> constexpr bool is_rvalue = Details::is_rvalue<T>;
template<typename T> constexpr bool is_value  = !is_lvalue<T> && !is_rvalue<T>;

template<typename T> constexpr bool is_trivially_copyable = __is_trivially_copyable(T);

template<typename T> constexpr bool is_signed      = is_same<T, MakeSigned<T>>;
template<typename T> constexpr bool is_unsigned    = is_same<T, MakeUnsigned<T>>;
template<typename T> constexpr bool is_void        = is_same<void, RemovePointer<RemoveConstVolatile<T>>>;
template<typename T> constexpr bool is_nullptr     = is_same<decltype(nullptr), RemoveConstVolatile<T>>;
template<typename T> constexpr bool is_fundamental = is_arithmetic<T> || is_void<T> || is_nullptr<T>;
template<typename T> constexpr bool is_pointer     = Details::is_pointer<RemoveConstVolatile<T>>;
