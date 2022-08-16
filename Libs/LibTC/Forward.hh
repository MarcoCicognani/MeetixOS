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

#include <LibTC/Concept.hh>

namespace TC {

template<typename T>
struct TypeTraits;

template<typename T>
struct NumericLimits;

class Error;
class SourceLocation;

namespace Collection {
namespace Enums {

enum class CaseSensitivity;
enum class InsertResult;
enum class IntBase;
enum class KeepEmpty;
enum class KeepStorageCapacity;
enum class OnExistingEntry;
enum class ParseMode;
enum class TrimMode;

} /* namespace Enums */

template<typename>
class Function;

template<typename TReturn, typename... TArgs>
class Function<TReturn(TArgs...)>;

template<typename T>
class List;

template<typename K, typename T, typename KTraits = TypeTraits<K>, bool IsOrdered = false>
class Map;

template<typename K, typename T, typename KTraits = TypeTraits<K>>
using OrderedMap = Map<K, T, KTraits, true>;

template<Integral T>
class Range;

template<Integral T>
class RangeInclusive;

template<typename T, typename TTraits = TypeTraits<T>, bool IsOrdered = false>
class Set;

template<typename T, typename TTraits = TypeTraits<T>>
using OrderedSet = Set<T, TTraits, true>;

class String;
class StringBuilder;
class StringStorage;
class StringView;

template<typename T>
class Vector;

} /* namespace Collection */

namespace Memory {

template<typename T>
class Box;

template<typename T>
class NonNullRef;

} /* namespace Memory */

namespace Functional {

template<typename T>
class Option;

template<typename T, typename E>
class Result;

} /* namespace Functional */

namespace Text {

class Lexer;
class FormatLexer;
class FormatParser;

template<typename, typename = void>
class Formatter;

} /* namespace Text */
} /* namespace TC */

using namespace TC;