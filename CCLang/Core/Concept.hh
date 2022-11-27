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

#include <CCLang/Core/Meta.hh>
#include <CCLang/Lang/Cxx.hh>
#include <CCLang/Lang/IntTypes.hh>

/* forward declaration here since we cannot include CCLang.Forward */
template<typename T>
struct TypeTraits;

template<typename T>
concept Const = is_const<T>;

template<typename T>
concept Volatile = is_volatile<T>;

template<typename T, typename U>
concept SameAs = is_same<T, U>;

template<typename T>
concept Integral = is_integral<T>;

template<typename T>
concept FloatingPoint = is_floating_point<T>;

template<typename T>
concept Arithmetic = is_arithmetic<T>;

template<typename T>
concept Class = is_class<T>;

template<typename T>
concept Union = is_union<T>;

template<typename T>
concept Enum = is_enum<T>;

template<typename TDerive, typename TBase>
concept Derived = is_derived<TDerive, TBase>;

template<typename T>
concept LValue = is_lvalue<T>;

template<typename T>
concept RValue = is_rvalue<T>;

template<typename T>
concept Concrete = is_value<T>;

template<typename T>
concept Signed = is_signed<T>;

template<typename T>
concept Unsigned = is_unsigned<T>;

template<typename T>
concept Void = is_void<T>;

template<typename T>
concept NullPtr = is_nullptr<T>;

template<typename T>
concept Fundamental = is_fundamental<T>;

template<typename T>
concept Pointer = is_pointer<T>;

template<typename T, typename... TArgs>
concept ConstructibleWith = requires { ::new T(Cxx::declval<TArgs>()...); };

template<typename T>
concept CopyConstructible = is_trivially_copyable<T> || ConstructibleWith<T, AddLValueReference<AddConst<T>>>;

template<typename T>
concept Cloneable = requires(T t) {
                        { t.clone() } -> SameAs<T>;
                    };

template<typename T, typename TErrorOr>
concept TryCloneable = requires(T t) {
                           { t.try_clone() } -> SameAs<TErrorOr>;
                       };

template<typename T, typename TTraits = TypeTraits<T>>
concept Hashable = requires(T value) {
                       { TTraits::hash(value) } -> SameAs<usize>;
                   };

template<typename T, typename TReturn, typename... TArgs>
concept Callable = requires(T t) {
                       { t(Cxx::declval<TArgs>()...) } -> SameAs<TReturn>;
                   };

template<typename T, typename... TArgs>
concept Predicate = Callable<T, bool, TArgs...>;

template<typename T, typename... TArgs>
concept I32Predicate = Callable<T, i32, TArgs...>;

template<typename T, typename TSource, typename TResult>
concept Mapper = Callable<T, TResult, TSource const&>;

template<typename T, typename... TArgs>
concept Consumer = Callable<T, void, TArgs...>;

template<typename T>
concept CallBack = Callable<T, void>;

template<typename T>
concept Tryable = requires(T t) {
                      { !t } -> SameAs<bool>;
                      { t.unwrap() };
                      { t.propagate_failure() };
                  };
