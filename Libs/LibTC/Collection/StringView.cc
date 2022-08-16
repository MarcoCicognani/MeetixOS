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
#include <LibTC/Collection/StringView.hh>
#include <LibTC/Cxx.hh>
#include <LibTC/Functional/Must.hh>
#include <LibTC/Functional/Try.hh>
#include <LibTC/Memory/Find.hh>
#include <LibTC/NumericLimits.hh>

namespace TC::Collection {

auto StringView::operator=(StringView const& rhs) -> StringView& {
    if ( this == &rhs )
        return *this;

    StringView string_view{ rhs };
    swap(string_view);
    return *this;
}

auto StringView::operator=(StringView&& rhs) noexcept -> StringView& {
    StringView string_view{ Cxx::move(rhs) };
    swap(string_view);
    return *this;
}

auto StringView::swap(StringView& rhs) noexcept -> void {
    Cxx::swap(m_chars_ptr, rhs.m_chars_ptr);
    Cxx::swap(m_chars_count, rhs.m_chars_count);
}

auto StringView::at(usize index) const -> char const& {
    VERIFY_LESS(index, len());
    return m_chars_ptr[index];
}

auto StringView::operator[](usize index) const -> char const& {
    return at(index);
}

auto StringView::compare(StringView rhs) const -> int {
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

auto StringView::equals_ignore_case(StringView rhs) const -> bool {
    if ( len() != rhs.len() )
        return false;

    for ( usize i : Range{ 0u, len() } ) {
        if ( to_ascii_lowercase(at(i)) != to_ascii_lowercase(rhs.at(i)) )
            return false;
    }
    return true;
}

auto StringView::sub_string_view(usize start) const -> StringView {
    return sub_string_view(start, len() - start);
}

auto StringView::sub_string_view(usize start, usize count) const -> StringView {
    VERIFY_LESS_EQUAL(start + count, len());
    return StringView{ as_cstr() + start, count };
}

auto StringView::trim(StringView chars, TrimMode trim_mode) const -> StringView {
    usize sub_start = 0;
    usize sub_len   = len();

    if ( trim_mode == TrimMode::Left || trim_mode == TrimMode::Both ) {
        for ( usize i : Range{ 0u, len() } ) {
            if ( sub_len == 0 )
                return ""sv;
            if ( !chars.contains(at(i)) )
                break;

            ++sub_start;
            --sub_len;
        }
    }

    if ( trim_mode == TrimMode::Right || trim_mode == TrimMode::Both ) {
        for ( usize i = len() - 1; i > 0; --i ) {
            if ( sub_len == 0 )
                return ""sv;
            if ( !chars.contains(at(i)) )
                break;

            --sub_len;
        }
    }

    return sub_string_view(sub_start, sub_len);
}

auto StringView::trim_whitespaces(TrimMode trim_mode) const -> StringView {
    return trim(" \n\t\v\f\r"sv, trim_mode);
}

auto StringView::starts_with(StringView rhs, CaseSensitivity case_sensitivity) const -> bool {
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
        for ( usize i : Range{ 0u, rhs.len() } ) {
            if ( to_ascii_lowercase(at(i)) != to_ascii_lowercase(rhs.at(i)) )
                return false;
        }
    }
    return true;
}

auto StringView::starts_with(char rhs, CaseSensitivity case_sensitivity) const -> bool {
    if ( is_null_or_empty() )
        return false;

    if ( case_sensitivity == CaseSensitivity::Sensitive )
        return at(0) == rhs;
    else
        return to_ascii_lowercase(at(0)) == to_ascii_lowercase(rhs);
}

auto StringView::ends_with(StringView rhs, CaseSensitivity case_sensitivity) const -> bool {
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
        for ( usize i : Range{ 0u, rhs.len() } ) {
            if ( to_ascii_lowercase(at(str_i)) != to_ascii_lowercase(rhs.at(i)) )
                return false;

            ++str_i;
        }
    }
    return true;
}

auto StringView::ends_with(char rhs, CaseSensitivity case_sensitivity) const -> bool {
    if ( is_null_or_empty() )
        return false;

    if ( case_sensitivity == CaseSensitivity::Sensitive )
        return at(len() - 1) == rhs;
    else
        return to_ascii_lowercase(at(len() - 1)) == to_ascii_lowercase(rhs);
}

static constexpr auto without_base_prefix(IntBase int_base, StringView string_view) -> StringView {
    if ( (int_base == IntBase::Binary && string_view[0] == 'b') || (int_base == IntBase::Octal && string_view[0] == '0') )
        return string_view.sub_string_view(1);
    else if ( int_base == IntBase::Hex && string_view.starts_with("0x"sv, CaseSensitivity::Insensitive) )
        return string_view.sub_string_view(2);
    else
        return string_view;
}
static constexpr auto is_valid_digit(IntBase int_base, char c) -> bool {
    switch ( int_base ) {
        case IntBase::Binary:
            return is_ascii_binary_digit(c);
        case IntBase::Octal:
            return is_ascii_octal_digit(c);
        case IntBase::Decimal:
            return is_ascii_digit(c);
        case IntBase::Hex:
            return is_ascii_hex_digit(c);
        default:
            return false;
    }
}
template<typename T>
static constexpr auto digit_value(char c) -> T {
    if ( c >= '0' && c <= '9' )
        return c - '0';
    else if ( c >= 'a' && c <= 'z' )
        return c - 'a' + 10;
    else if ( c >= 'A' && c <= 'Z' )
        return c - 'A' + 10;
    else
        return 0;
}

template<typename T>
auto StringView::as_int(IntBase int_base, ParseMode parse_mode) const -> ErrorOr<T> {
    /* trim the whitespaces if requested */
    auto string_view = *this;
    if ( parse_mode == ParseMode::TrimWhitesAndBegin )
        string_view = string_view.trim_whitespaces(TrimMode::Left);
    else if ( parse_mode == ParseMode::TrimWhitesAndBeginToEnd )
        string_view = string_view.trim_whitespaces(TrimMode::Both);

    /* check for only whitespaces string */
    if ( string_view.is_empty() )
        return Error{ EINVAL };

    /* extract the sign from the beginning of the string */
    T sign = 1;
    if ( is_ascii_int_sign(string_view[0]) ) {
        if ( string_view.len() == 1 )
            return Error{ EINVAL };

        /* for minus set the sign to negative */
        if ( string_view[0] == '-' )
            sign = -1;

        /* remove the sign from the beginning */
        string_view = string_view.sub_string_view(1);
    }

    /* convert the string into the integer */
    T int_value = 0;
    for ( auto const c : without_base_prefix(int_base, string_view) ) {
        if ( !is_valid_digit(int_base, c) ) {
            /* according to the parse all flag we decide here if the parser must return
             * with an error or simply break the cycle returning the value parsed until now */
            if ( parse_mode == ParseMode::BeginToEnd || parse_mode == ParseMode::TrimWhitesAndBeginToEnd )
                return Error{ EINVAL };
            else
                break;
        }

        if ( __builtin_mul_overflow(int_value, static_cast<T>(int_base), &int_value) )
            return Error{ EOVERFLOW };
        if ( __builtin_add_overflow(int_value, digit_value<T>(c), &int_value) )
            return Error{ EOVERFLOW };
    }

    /* apply the sign to the final value */
    int_value *= sign;
    return int_value;
}

template auto StringView::as_int(IntBase int_base, ParseMode parse_mode) const -> ErrorOr<signed char>;
template auto StringView::as_int(IntBase int_base, ParseMode parse_mode) const -> ErrorOr<signed short>;
template auto StringView::as_int(IntBase int_base, ParseMode parse_mode) const -> ErrorOr<signed int>;
template auto StringView::as_int(IntBase int_base, ParseMode parse_mode) const -> ErrorOr<signed long>;
template auto StringView::as_int(IntBase int_base, ParseMode parse_mode) const -> ErrorOr<signed long long>;

template<typename T>
auto StringView::as_uint(IntBase int_base, ParseMode parse_mode) const -> ErrorOr<T> {
    /* trim the whitespaces if requested */
    auto string_view = *this;
    if ( parse_mode == ParseMode::TrimWhitesAndBegin )
        string_view = string_view.trim_whitespaces(TrimMode::Left);
    else if ( parse_mode == ParseMode::TrimWhitesAndBeginToEnd )
        string_view = string_view.trim_whitespaces(TrimMode::Both);

    /* check for only whitespaces string */
    if ( string_view.is_empty() )
        return Error{ EINVAL };

    /* convert the string into the integer */
    T int_value = 0;
    for ( auto const c : without_base_prefix(int_base, string_view) ) {
        if ( !is_valid_digit(int_base, c) ) {
            /* according to the parse all flag we decide here if the parser must return
             * with an error or simply break the cycle returning the value parsed until now */
            if ( parse_mode == ParseMode::BeginToEnd || parse_mode == ParseMode::TrimWhitesAndBeginToEnd )
                return Error{ EINVAL };
            else
                break;
        }

        if ( __builtin_mul_overflow(int_value, static_cast<T>(int_base), &int_value) )
            return Error{ EOVERFLOW };
        if ( __builtin_add_overflow(int_value, digit_value<T>(c), &int_value) )
            return Error{ EOVERFLOW };
    }
    return int_value;
}

template auto StringView::as_uint(IntBase int_base, ParseMode parse_mode) const -> ErrorOr<unsigned char>;
template auto StringView::as_uint(IntBase int_base, ParseMode parse_mode) const -> ErrorOr<unsigned short>;
template auto StringView::as_uint(IntBase int_base, ParseMode parse_mode) const -> ErrorOr<unsigned int>;
template auto StringView::as_uint(IntBase int_base, ParseMode parse_mode) const -> ErrorOr<unsigned long>;
template auto StringView::as_uint(IntBase int_base, ParseMode parse_mode) const -> ErrorOr<unsigned long long>;

auto StringView::find(char needle, size_t start) const -> Option<usize> {
    if ( start >= len() )
        return {};

    for ( usize i : Range{ start, len() } ) {
        if ( at(i) == needle )
            return i;
    }
    return {};
}

auto StringView::find(StringView needle, size_t start) const -> Option<usize> {
    if ( start >= len() )
        return {};

    return find_in_memory(as_cstr() + start, len() - start, needle.as_cstr(), needle.len())
        .map<usize>([](auto ptr_pos) -> usize { return bit_cast<usize>(ptr_pos); })
        .map<usize>([this](usize result_as_usize) -> usize { return result_as_usize - bit_cast<usize>(as_cstr()); });
}

auto StringView::find_last(char needle) const -> Option<usize> {
    for ( usize i = len() - 1; i > 0; --i ) {
        if ( at(i) == needle )
            return i;
    }
    return {};
}

auto StringView::find_all(StringView needle) const -> Vector<usize> {
    return MUST(try_find_all(needle));
}

auto StringView::try_find_all(StringView needle) const -> ErrorOr<Vector<usize>> {
    auto positions = Vector<usize>::construct_empty();

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

auto StringView::split_view(char separator, KeepEmpty keep_empty) const -> Vector<StringView> {
    return split_view(StringView{ &separator, 1 }, keep_empty);
}

auto StringView::split_view(StringView separator, KeepEmpty keep_empty) const -> Vector<StringView> {
    return MUST(try_split_view(separator, keep_empty));
}

auto StringView::try_split_view(char separator, KeepEmpty keep_empty) const -> ErrorOr<Vector<StringView>> {
    return try_split_view(StringView{ &separator, 1 }, keep_empty);
}

auto StringView::try_split_view(StringView separator, KeepEmpty keep_empty) const -> ErrorOr<Vector<StringView>> {
    auto vector = Vector<StringView>::construct_empty();
    TRY(split_view(separator, keep_empty, [&vector](StringView string_view) -> ErrorOr<void> {
        TRY(vector.try_append(string_view));
        return {};
    }));
    return vector;
}

auto StringView::operator==(StringView const& rhs) const -> bool {
    return len() == rhs.len() && compare(rhs) == 0;
}

auto StringView::operator!=(StringView const& rhs) const -> bool {
    return !operator==(rhs);
}

auto StringView::operator<(StringView const& rhs) const -> bool {
    return len() < rhs.len() || compare(rhs) < 0;
}

auto StringView::operator<=(StringView const& rhs) const -> bool {
    return len() <= rhs.len() || compare(rhs) <= 0;
}

auto StringView::operator>(StringView const& rhs) const -> bool {
    return len() > rhs.len() || compare(rhs) > 0;
}

auto StringView::operator>=(StringView const& rhs) const -> bool {
    return len() >= rhs.len() || compare(rhs) >= 0;
}

auto StringView::contains(StringView rhs, CaseSensitivity case_sensitivity) const -> bool {
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

auto StringView::contains(char rhs, CaseSensitivity case_sensitivity) const -> bool {
    if ( is_null_or_empty() )
        return false;

    for ( char const c : *this ) {
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

auto StringView::begin() const -> StringView::ConstIterator {
    return ConstIterator::begin(*this);
}

auto StringView::end() const -> StringView::ConstIterator {
    return ConstIterator::end(*this);
}

auto StringView::rbegin() const -> StringView::ConstReverseIterator {
    return ConstReverseIterator::rbegin(*this);
}

auto StringView::rend() const -> StringView::ConstReverseIterator {
    return ConstReverseIterator::rend(*this);
}

auto StringView::reverse_iter() const -> StringView::ConstReverseIteratorWrapper {
    return ReverseIteratorSupport::in_reverse(*this);
}

auto StringView::as_cstr() const -> char const* {
    return m_chars_ptr;
}

auto StringView::len() const -> usize {
    return m_chars_count;
}

auto StringView::count() const -> usize {
    return len();
}

auto StringView::is_empty() const -> bool {
    return len() == 0;
}

auto StringView::is_null() const -> bool {
    return m_chars_ptr == nullptr;
}

auto StringView::is_null_or_empty() const -> bool {
    return is_null() || is_empty();
}

} /* namespace TC::Collection */
