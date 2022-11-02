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

#include <LibTC/Core/Concept.hh>

/* LibTC.Alloc */

template<typename T>
class Box;

template<typename T>
class List;

template<typename K, typename T, typename KTraits = TypeTraits<K>, bool IsOrdered = false>
class Map;

template<typename K, typename T, typename KTraits = TypeTraits<K>>
using OrderedMap = Map<K, T, KTraits, true>;

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

/* LibTC.Alloc.Text */

class FormatLexer;

class FormatParser;

template<typename, typename = void>
class Formatter;

class Lexer;

/* LibTC.Core */

class Error;

template<typename T>
struct NumericLimits;

class SourceLocation;

template<typename T>
struct TypeTraits;

/* LibTC.Core.Enums */

/*
enum class CaseSensitivity;
enum class InsertResult;
enum class IntBase;
enum class KeepEmpty;
enum class KeepStorageCapacity;
enum class OnExistingEntry;
enum class ParseMode;
enum class TrimMode;
*/
/* LibTC.Lang */

template<typename>
class Function;

template<typename TReturn, typename... TArgs>
class Function<TReturn(TArgs...)>;

template<typename T>
class Option;

template<Integral T>
class Range;

template<Integral T>
class RangeInclusive;

template<typename T, typename E>
class Result;

class StringView;

template<typename T>
using ErrorOr = Result<T, Error>;