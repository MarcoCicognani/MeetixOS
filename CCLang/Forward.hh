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

#ifndef NDEBUG
static constexpr bool cclang_debugging = true;
#else
static constexpr bool cclang_debugging = false;
#endif

/* CCLang.Lang */

class u8;
class u16;
class u32;
class u64;
class usize;

class i8;
class i16;
class i32;
class i64;
class isize;

template<typename>
class Function;

template<typename TReturn, typename... TArgs>
class Function<TReturn(TArgs...)>;

template<typename T>
class Option;

template<typename T>
class Range;

template<typename T>
class RangeInclusive;

template<typename T, typename E>
class Result;

class StringView;


/* CCLang.Core */

class Error;

template<typename T>
struct NumericLimits;

class SourceLocation;

template<typename T>
struct TypeTraits;

template<typename T>
using ErrorOr = Result<T, Error>;

/* CCLang.Alloc */

template<typename T>
class Box;

template<typename T>
class List;

template<typename K, typename T, typename KTraits = TypeTraits<K>, bool IsOrdered = false>
class Map;

template<typename K, typename T, typename KTraits = TypeTraits<K>>
using OrderedMap = Map<K, T, KTraits, true>;

class RefCounted;

template<typename T>
class NonNullRef;

template<typename T, typename TTraits = TypeTraits<T>, bool IsOrdered = false>
class Set;

template<typename T, typename TTraits = TypeTraits<T>>
using OrderedSet = Set<T, TTraits, true>;

class String;

class StringBuilder;

class StringStorage;

template<typename T>
class Vector;

/* CCLang.Alloc.Text */

class FormatLexer;

class FormatParser;

template<typename, typename = void>
class Formatter;

class Lexer;