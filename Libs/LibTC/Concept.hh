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

#include <LibTC/Cxx.hh>
#include <LibTC/IntTypes.hh>
#include <LibTC/Meta.hh>

namespace TC {

template<typename T>
struct TypeTraits;

namespace Concept {

template<typename T>
concept Const = IsConst<T>;

template<typename T>
concept Volatile = IsVolatile<T>;

template<typename T, typename U>
concept SameAs = IsSame<T, U>;

template<typename T>
concept Integral = IsIntegral<T>;

template<typename T>
concept FloatingPoint = IsFloatingPoint<T>;

template<typename T>
concept Arithmetic = IsArithmetic<T>;

template<typename T>
concept Class = IsClass<T>;

template<typename T>
concept Union = IsUnion<T>;

template<typename T>
concept Enum = IsEnum<T>;

template<typename TDerive, typename TBase>
concept Derived = IsDerived<TDerive, TBase>;

template<typename T>
concept LValue = IsLValue<T>;

template<typename T>
concept RValue = IsRValue<T>;

template<typename T>
concept Concrete = IsConcrete<T>;

template<typename T>
concept Signed = IsSigned<T>;

template<typename T>
concept Unsigned = IsUnsigned<T>;

template<typename T>
concept Void = IsVoid<T>;

template<typename T>
concept NullPtr = IsNullPtr<T>;

template<typename T>
concept Fundamental = IsFundamental<T>;

template<typename T>
concept Pointer = IsPointer<T>;

template<typename T, typename... TArgs>
concept ConstructibleWith = requires { ::new T{ declval<TArgs>()... }; };

template<typename T>
concept CopyConstructible = IsTriviallyCopyable<T> || ConstructibleWith<T, AddLValueReference<AddConst<T>>>;

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
                       { t(declval<TArgs>()...) } -> SameAs<TReturn>;
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
concept Tryable = requires(T t) {
                      { !t };
                      { t.unwrap() };
                      { t.backward() };
                  };

} /* namespace Concept */

using Concept::Arithmetic;
using Concept::Callable;
using Concept::Class;
using Concept::Cloneable;
using Concept::Concrete;
using Concept::Const;
using Concept::ConstructibleWith;
using Concept::Consumer;
using Concept::CopyConstructible;
using Concept::Derived;
using Concept::Enum;
using Concept::FloatingPoint;
using Concept::Fundamental;
using Concept::Hashable;
using Concept::I32Predicate;
using Concept::Integral;
using Concept::LValue;
using Concept::Mapper;
using Concept::NullPtr;
using Concept::Pointer;
using Concept::Predicate;
using Concept::RValue;
using Concept::SameAs;
using Concept::Signed;
using Concept::Tryable;
using Concept::TryCloneable;
using Concept::Union;
using Concept::Unsigned;
using Concept::Void;
using Concept::Volatile;

} /* namespace TC */