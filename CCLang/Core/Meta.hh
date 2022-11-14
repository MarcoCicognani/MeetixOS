/**
 * @brief
 * This file is part of the MeetiX Operating System.
 * Copyright (c) 2017-2021, Marco Cicognani (marco.cicognani@meetixos.org)
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

template<typename T> constexpr bool IsConst          = false;
template<typename T> constexpr bool IsConst<T const> = true;

template<typename T> constexpr bool IsVolatile             = false;
template<typename T> constexpr bool IsVolatile<T volatile> = true;

template<typename T>
           constexpr bool IsIntegral                     = false;
template<> constexpr bool IsIntegral<bool>               = true;
template<> constexpr bool IsIntegral<unsigned char>      = true;
template<> constexpr bool IsIntegral<signed char>        = true;
template<> constexpr bool IsIntegral<char8_t>            = true;
template<> constexpr bool IsIntegral<char16_t>           = true;
template<> constexpr bool IsIntegral<char32_t>           = true;
template<> constexpr bool IsIntegral<unsigned short>     = true;
template<> constexpr bool IsIntegral<unsigned int>       = true;
template<> constexpr bool IsIntegral<unsigned long>      = true;
template<> constexpr bool IsIntegral<unsigned long long> = true;

template<typename T>
           constexpr bool IsFloatingPoint              = false;
template<> constexpr bool IsFloatingPoint<float>       = true;
template<> constexpr bool IsFloatingPoint<double>      = true;
template<> constexpr bool IsFloatingPoint<long double> = true;

template<typename T> constexpr bool IsPointer     = false;
template<typename T> constexpr bool IsPointer<T*> = true;

template<typename T, typename U>
                     constexpr bool IsSame       = false;
template<typename T> constexpr bool IsSame<T, T> = true;

template<typename T> constexpr bool IsLValue     = false;
template<typename T> constexpr bool IsLValue<T&> = true;

template<typename T> constexpr bool IsRValue      = false;
template<typename T> constexpr bool IsRValue<T&&> = true;

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

} /* namespace Details */

template<typename T>
class FriendOf {
protected:
    friend T;

    constexpr FriendOf() = default;

    FriendOf(FriendOf<T> const&) = delete;
    auto operator=(FriendOf<T> const&) -> FriendOf<T>& = delete;

    FriendOf(FriendOf<T> &&) = delete;
    auto operator=(FriendOf<T> &&) -> FriendOf<T>& = delete;
};

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

template<typename T> constexpr bool IsConst    = Details::IsConst<T>;
template<typename T> constexpr bool IsVolatile = Details::IsVolatile<T>;

template<typename T, typename U>
constexpr bool IsSame = Details::IsSame<T, U>;

template<typename T> constexpr bool IsIntegral      = Details::IsIntegral<MakeUnsigned<RemoveConstVolatile<T>>>;
template<typename T> constexpr bool IsFloatingPoint = Details::IsFloatingPoint<RemoveConstVolatile<T>>;
template<typename T> constexpr bool IsArithmetic    = IsIntegral<T> || IsFloatingPoint<T>;

template<typename T> constexpr bool IsClass = __is_class(T);
template<typename T> constexpr bool IsUnion = __is_union(T);
template<typename T> constexpr bool IsEnum  = __is_enum(T);

template<typename T> using UnderlyingType = __underlying_type(T);

template<typename TDerive, typename TBase>
constexpr bool IsDerived = __is_base_of(TBase, TDerive);

template<typename T> constexpr bool IsLValue   = Details::IsLValue<T>;
template<typename T> constexpr bool IsRValue   = Details::IsRValue<T>;
template<typename T> constexpr bool IsConcrete = !IsLValue<T> && !IsRValue<T>;

template<typename T> constexpr bool IsTriviallyCopyable = __is_trivially_copyable(T);

template<typename T> constexpr bool IsSigned      = IsSame<T, MakeSigned<T>>;
template<typename T> constexpr bool IsUnsigned    = IsSame<T, MakeUnsigned<T>>;
template<typename T> constexpr bool IsVoid        = IsSame<void, RemovePointer<RemoveConstVolatile<T>>>;
template<typename T> constexpr bool IsNullPtr     = IsSame<decltype(nullptr), RemoveConstVolatile<T>>;
template<typename T> constexpr bool IsFundamental = IsArithmetic<T> || IsVoid<T> || IsNullPtr<T>;
template<typename T> constexpr bool IsPointer     = Details::IsPointer<RemoveConstVolatile<T>>;
