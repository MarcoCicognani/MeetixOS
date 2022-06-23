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

#include <LibTC/Assertions.hh>
#include <LibTC/BitCast.hh>
#include <LibTC/CharTypes.hh>
#include <LibTC/Collection/String.hh>
#include <LibTC/Collection/StringView.hh>
#include <LibTC/Cxx.hh>
#include <LibTC/Functional/Must.hh>
#include <LibTC/Functional/Try.hh>
#include <LibTC/Memory/Find.hh>
#include <LibTC/Trait/NumericLimits.hh>

namespace TC::Collection {

constexpr StringView::StringView(char const* str, usize count) noexcept
    : m_chars_ptr{ str }
    , m_chars_count{ count } {
}

StringView::StringView(String const& string) noexcept
    : StringView{ string.as_cstr(), string.len() } {
}

StringView::StringView(char const* str) noexcept
    : StringView{ str, str != nullptr ? __builtin_strlen(str) : 0 } {
}

StringView::StringView(StringView const& rhs) noexcept
    : StringView{ rhs.as_cstr(), rhs.len() } {
}

StringView::StringView(StringView&& rhs) noexcept
    : m_chars_ptr{ exchange(rhs.m_chars_ptr, nullptr) }
    , m_chars_count{ exchange(rhs.m_chars_count, 0) } {
}

StringView& StringView::operator=(String const& str) {
    StringView string_view{ str };
    swap(string_view);
    return *this;
}

StringView& StringView::operator=(char const* str) {
    StringView string_view{ str };
    swap(string_view);
    return *this;
}

StringView& StringView::operator=(StringView const& rhs) {
    if ( this == &rhs )
        return *this;

    StringView string_view{ rhs };
    swap(string_view);
    return *this;
}

StringView& StringView::operator=(StringView&& rhs) noexcept {
    StringView string_view{ move(rhs) };
    swap(string_view);
    return *this;
}

void StringView::swap(StringView& rhs) noexcept {
    Cxx::swap(m_chars_ptr, rhs.m_chars_ptr);
    Cxx::swap(m_chars_count, rhs.m_chars_count);
}

char const& StringView::at(usize index) const {
    VERIFY_LESS(index, len());
    return m_chars_ptr[index];
}

char const& StringView::operator[](usize index) const {
    return at(index);
}

int StringView::compare(StringView rhs) const {
    if ( is_null_or_empty() )
        return rhs.is_null_or_empty() ? 0 : -1;
    if ( rhs.is_null_or_empty() )
        return 1;

    /* use the optimized mem-cmp for the minimum of the two lengths */
    auto min_len = len() < rhs.len() ? len() : rhs.len();
    auto res     = __builtin_memcmp(as_cstr(), rhs.as_cstr(), min_len);
    if ( res == 0 ) {
        if ( len() < rhs.len() )
            return -1;
        else if ( len() == rhs.len() )
            return 0;
        else
            return 1;
    }
    return res;
}

bool StringView::equals_ignore_case(StringView rhs) const {
    if ( len() != rhs.len() )
        return false;

    for ( usize i = 0; i < len(); ++i ) {
        if ( to_ascii_lowercase(at(i)) != to_ascii_lowercase(rhs.at(i)) )
            return false;
    }
    return true;
}

StringView StringView::sub_string_view(usize start) const {
    return sub_string_view(start, len() - start);
}

StringView StringView::sub_string_view(usize start, usize count) const {
    VERIFY_LESS_EQUAL(start + count, len());
    return StringView{ as_cstr() + start, count };
}

StringView StringView::trim(StringView chars, TrimMode trim_mode) const {
    usize sub_start = 0;
    usize sub_len   = len();

    if ( trim_mode == TrimMode::Left || trim_mode == TrimMode::Both ) {
        for ( usize i = 0; i < len(); ++i ) {
            if ( sub_len == 0 )
                return "";
            if ( !chars.contains(at(i)) )
                break;

            ++sub_start;
            --sub_len;
        }
    }

    if ( trim_mode == TrimMode::Right || trim_mode == TrimMode::Both ) {
        for ( usize i = len() - 1; i > 0; --i ) {
            if ( sub_len == 0 )
                return "";
            if ( !chars.contains(at(i)) )
                break;

            --sub_len;
        }
    }

    return sub_string_view(sub_start, sub_len);
}

StringView StringView::trim_whitespaces(TrimMode trim_mode) const {
    return trim(" \n\t\v\f\r", trim_mode);
}

bool StringView::starts_with(StringView rhs, CaseSensitivity case_sensitivity) const {
    if ( is_null_or_empty() )
        return false;
    if ( rhs.is_null_or_empty() )
        return false;
    if ( rhs.len() > len() )
        return false;
    if ( as_cstr() == rhs.as_cstr() )
        return true;

    if ( case_sensitivity == CaseSensitivity::Sensitive )
        return __builtin_memcmp(as_cstr(), rhs.as_cstr(), rhs.len()) == 0;
    else {
        for ( usize i = 0; i < rhs.len(); ++i ) {
            if ( to_ascii_lowercase(at(i)) != to_ascii_lowercase(rhs.at(i)) )
                return false;
        }
    }
    return true;
}

bool StringView::starts_with(char rhs, CaseSensitivity case_sensitivity) const {
    if ( is_null_or_empty() )
        return false;

    if ( case_sensitivity == CaseSensitivity::Sensitive )
        return at(0) == rhs;
    else
        return to_ascii_lowercase(at(0)) == to_ascii_lowercase(rhs);
}

bool StringView::ends_with(StringView rhs, CaseSensitivity case_sensitivity) const {
    if ( is_null_or_empty() )
        return false;
    if ( rhs.is_null_or_empty() )
        return false;
    if ( rhs.len() > len() )
        return false;
    if ( as_cstr() == rhs.as_cstr() )
        return false;

    if ( case_sensitivity == CaseSensitivity::Sensitive )
        return __builtin_memcmp(as_cstr() + (len() - rhs.len()), rhs.as_cstr(), rhs.len()) == 0;
    else {
        usize str_i = len() - rhs.len();
        for ( usize i = 0; i < rhs.len(); ++i, ++str_i ) {
            if ( to_ascii_lowercase(at(str_i)) != to_ascii_lowercase(rhs.at(i)) )
                return false;
        }
    }
    return true;
}

bool StringView::ends_with(char rhs, CaseSensitivity case_sensitivity) const {
    if ( is_null_or_empty() )
        return false;

    if ( case_sensitivity == CaseSensitivity::Sensitive )
        return at(len() - 1) == rhs;
    else
        return to_ascii_lowercase(at(len() - 1)) == to_ascii_lowercase(rhs);
}

template<typename T>
Option<T> StringView::as_int(TrimWhitespace trim_whitespace) const {
    auto string_view = trim_whitespace == TrimWhitespace::Yes ? trim_whitespaces() : *this;
    if ( string_view.is_empty() )
        return {};

    /* extract the sign from the beginning of the string */
    T     sign = 1;
    usize i    = 0;
    if ( string_view[0] == '-' || string_view[0] == '+' ) {
        if ( string_view.len() == 1 )
            return {};

        ++i;
        if ( string_view[0] == '-' )
            sign = -1;
    }

    /* convert the string into the integer */
    T int_value = 0;
    for ( ; i < string_view.len(); ++i ) {
        if ( string_view[i] < '0' || string_view[i] > '9' )
            return {};

        if ( __builtin_mul_overflow(int_value, 10, &int_value) )
            return {};
        if ( __builtin_add_overflow(int_value, sign * (string_view[i] - '0'), &int_value) )
            return {};
    }
    return int_value;
}

template Option<i8>  StringView::as_int(TrimWhitespace trim_whitespace) const;
template Option<i16> StringView::as_int(TrimWhitespace trim_whitespace) const;
template Option<i32> StringView::as_int(TrimWhitespace trim_whitespace) const;
template Option<i64> StringView::as_int(TrimWhitespace trim_whitespace) const;

template<typename T>
Option<T> StringView::as_uint(TrimWhitespace trim_whitespace) const {
    auto string_view = trim_whitespace == TrimWhitespace::Yes ? trim_whitespaces() : *this;
    if ( string_view.is_empty() )
        return {};

    /* convert the string into the integer */
    T uint_value = 0;
    for ( auto const c : string_view ) {
        if ( c < '0' || c > '9' )
            return {};

        if ( __builtin_mul_overflow(uint_value, 10, &uint_value) )
            return {};
        if ( __builtin_add_overflow(uint_value, c - '0', &uint_value) )
            return {};
    }
    return uint_value;
}

template Option<u8>  StringView::as_uint(TrimWhitespace trim_whitespace) const;
template Option<u16> StringView::as_uint(TrimWhitespace trim_whitespace) const;
template Option<u32> StringView::as_uint(TrimWhitespace trim_whitespace) const;
template Option<u64> StringView::as_uint(TrimWhitespace trim_whitespace) const;

template<typename T>
Option<T> StringView::as_uint_from_hex(TrimWhitespace trim_whitespace) const {
    auto string_view = trim_whitespace == TrimWhitespace::Yes ? trim_whitespaces() : *this;
    if ( string_view.is_empty() )
        return {};

    /* remove base prefix */
    if ( string_view.starts_with("0x", CaseSensitivity::Insensitive) ) {
        if ( string_view.len() <= 2 )
            return {};

        string_view = string_view.sub_string_view(2);
    }

    /* convert the string into the integer */
    T    uint_value = 0;
    auto max_value  = NumericLimits<T>::max();
    for ( auto const c : string_view ) {
        if ( uint_value > (max_value >> 4) )
            return {};

        u8 value = 0;
        if ( c >= '0' && c <= '9' )
            value = c - '0';
        else if ( c >= 'a' && c <= 'f' )
            value = 10 + (c - 'a');
        else if ( c >= 'A' && c <= 'F' )
            value = 10 + (c - 'A');
        else
            return {};

        uint_value = (uint_value << 4) + value;
    }
    return uint_value;
}

template Option<u8>  StringView::as_uint_from_hex(TrimWhitespace trim_whitespace) const;
template Option<u16> StringView::as_uint_from_hex(TrimWhitespace trim_whitespace) const;
template Option<u32> StringView::as_uint_from_hex(TrimWhitespace trim_whitespace) const;
template Option<u64> StringView::as_uint_from_hex(TrimWhitespace trim_whitespace) const;

template<typename T>
Option<T> StringView::as_uint_from_octal(TrimWhitespace trim_whitespace) const {
    auto string_view = trim_whitespace == TrimWhitespace::Yes ? trim_whitespaces() : *this;
    if ( string_view.is_empty() )
        return {};

    /* convert the string into the integer */
    T    uint_value = 0;
    auto max_value  = NumericLimits<T>::max();
    for ( auto const c : string_view ) {
        if ( uint_value > max_value >> 3 )
            return {};

        u8 value = 0;
        if ( c >= '0' && c <= '7' )
            value = c - '0';
        else
            return {};

        uint_value = (uint_value << 3) + value;
    }
    return uint_value;
}

template Option<u8>  StringView::as_uint_from_octal(TrimWhitespace trim_whitespace) const;
template Option<u16> StringView::as_uint_from_octal(TrimWhitespace trim_whitespace) const;
template Option<u32> StringView::as_uint_from_octal(TrimWhitespace trim_whitespace) const;
template Option<u64> StringView::as_uint_from_octal(TrimWhitespace trim_whitespace) const;

Option<usize> StringView::find(char needle, size_t start) const {
    if ( start >= len() )
        return {};

    for ( usize i = start; i < len(); ++i ) {
        if ( at(i) == needle )
            return i;
    }
    return {};
}

Option<usize> StringView::find(StringView needle, size_t start) const {
    if ( start >= len() )
        return {};

    return find_in_memory(as_cstr() + start, len() - start, needle.as_cstr(), needle.len())
        .map<usize>([](auto ptr_pos) { return bit_cast<usize>(ptr_pos); })
        .map<usize>([this](usize result_as_usize) { return result_as_usize - bit_cast<usize>(as_cstr()); });
}

Option<usize> StringView::find_last(char needle) const {
    for ( usize i = len() - 1; i > 0; --i ) {
        if ( at(i) == needle )
            return i;
    }
    return {};
}

Vector<usize> StringView::find_all(StringView needle) const {
    return MUST(try_find_all(needle));
}

ErrorOr<Vector<usize>> StringView::try_find_all(StringView needle) const {
    Vector<usize> positions{};

    usize current_position = 0;
    while ( current_position < len() ) {
        auto ptr_pos_or_none = find_in_memory(as_cstr() + current_position, len() - current_position, needle.as_cstr(), needle.len());
        if ( !ptr_pos_or_none.is_present() )
            break;

        auto char_ptr_pos   = ptr_pos_or_none.value();
        auto relative_index = char_ptr_pos - (as_cstr() + current_position);

        TRY(positions.try_append(current_position + relative_index));
        current_position += relative_index + 1;
    }
    return positions;
}

bool StringView::operator==(StringView const& rhs) const {
    return len() == rhs.len() && compare(rhs) == 0;
}

bool StringView::operator!=(StringView const& rhs) const {
    return !operator==(rhs);
}

bool StringView::operator<(StringView const& rhs) const {
    return len() < rhs.len() || compare(rhs) < 0;
}

bool StringView::operator<=(StringView const& rhs) const {
    return len() <= rhs.len() || compare(rhs) <= 0;
}

bool StringView::operator>(StringView const& rhs) const {
    return len() > rhs.len() || compare(rhs) > 0;
}

bool StringView::operator>=(StringView const& rhs) const {
    return len() >= rhs.len() || compare(rhs) >= 0;
}

bool StringView::contains(StringView rhs, CaseSensitivity case_sensitivity) const {
    if ( is_null_or_empty() || rhs.is_null_or_empty() || rhs.len() > len() )
        return false;

    if ( case_sensitivity == CaseSensitivity::Sensitive )
        return find_in_memory(as_cstr(), len(), rhs.as_cstr(), rhs.len()).is_present();

    auto needle_first = to_ascii_lowercase(rhs.at(0));
    for ( usize i = 0; i < len(); ++i ) {
        if ( to_ascii_lowercase(at(i)) != needle_first )
            continue;

        for ( usize ni = 0; i + ni < len(); ++ni ) {
            if ( to_ascii_lowercase(at(i + ni)) != to_ascii_lowercase(rhs.at(ni)) ) {
                i += ni;
                break;
            }
            if ( ni + 1 == rhs.len() )
                return true;
        }
    }
    return false;
}

bool StringView::contains(char rhs, CaseSensitivity case_sensitivity) const {
    if ( is_null_or_empty() )
        return false;

    for ( auto const c : *this ) {
        if ( case_sensitivity == CaseSensitivity::Sensitive ) {
            if ( c == rhs )
                return true;
        } else {
            if ( to_ascii_lowercase(c) == to_ascii_lowercase(rhs) )
                return true;
        }
    }
    return false;
}

StringView::ConstIterator StringView::begin() const {
    return ConstIterator::begin(*this);
}

StringView::ConstIterator StringView::end() const {
    return ConstIterator::end(*this);
}

char const* StringView::as_cstr() const {
    return m_chars_ptr;
}

usize StringView::len() const {
    return m_chars_count;
}

usize StringView::count() const {
    return len();
}

bool StringView::is_empty() const {
    return len() == 0;
}

bool StringView::is_null() const {
    return m_chars_ptr == nullptr;
}

bool StringView::is_null_or_empty() const {
    return is_null() || is_empty();
}

} /* namespace TC::Collection */
