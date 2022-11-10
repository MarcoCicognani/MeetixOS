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

#include <LibLS/Alloc/Vector.hh>
#include <LibLS/Core/AllOf.hh>
#include <LibLS/Core/AnyOf.hh>
#include <LibLS/Core/Assertions.hh>
#include <LibLS/Core/CharTypes.hh>
#include <LibLS/Core/ErrorOr.hh>
#include <LibLS/Core/Find.hh>
#include <LibLS/Lang/Cxx.hh>
#include <LibLS/Lang/Must.hh>
#include <LibLS/Lang/StringView.hh>
#include <LibLS/Lang/Try.hh>

auto StringView::new_from_cstr(const char* c_str) -> StringView {
    return new_from_raw_parts(c_str, __builtin_strlen(c_str));
}

auto StringView::operator=(StringView const& rhs) -> StringView& {
    if ( this == &rhs )
        return *this;

    StringView string_view{ rhs };
    swap(string_view);
    return *this;
}

auto StringView::operator=(StringView&& rhs) -> StringView& {
    StringView string_view{ Cxx::move(rhs) };
    swap(string_view);
    return *this;
}

auto StringView::swap(StringView& rhs) -> void {
    Cxx::swap(m_chars_ptr, rhs.m_chars_ptr);
    Cxx::swap(m_chars_count, rhs.m_chars_count);
}

auto StringView::at(usize index) const -> char const& {
    verify_less$(index, len());
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

    usize i = 0;
    return all_of<char>(begin(), end(), [&rhs, &i](char const c) { return to_ascii_lowercase(c) == to_ascii_lowercase(rhs.at(i++)); });
}

auto StringView::sub_string_view(usize start) const -> StringView {
    return sub_string_view(start, len() - start);
}

auto StringView::sub_string_view(usize start, usize count) const -> StringView {
    verify_less_equal$(start + count, len());
    return StringView{ as_cstr() + start, count };
}

auto StringView::trim(StringView chars, TrimMode trim_mode) const -> StringView {
    usize sub_start = 0;
    usize sub_len   = len();

    if ( trim_mode == TrimMode::Left || trim_mode == TrimMode::Both ) {
        for ( auto const i : Range{ 0u, len() } ) {
            if ( sub_len == 0 )
                return ""sv;
            if ( !chars.contains(at(i)) )
                break;

            ++sub_start;
            --sub_len;
        }
    }

    if ( trim_mode == TrimMode::Right || trim_mode == TrimMode::Both ) {
        for ( auto i = len() - 1; i > 0; --i ) {
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

auto StringView::starts_with(StringView rhs, CaseSensible case_sensitivity) const -> bool {
    if ( is_null_or_empty() )
        return false;
    if ( rhs.is_null_or_empty() )
        return false;
    if ( rhs.len() > len() )
        return false;
    if ( as_cstr() == rhs.as_cstr() )
        return true;

    if ( case_sensitivity == CaseSensible::Yes )
        return __builtin_memcmp(as_cstr(), rhs.as_cstr(), rhs.len()) == 0;
    else {
        for ( auto const i : Range{ 0u, rhs.len() } ) {
            if ( to_ascii_lowercase(at(i)) != to_ascii_lowercase(rhs.at(i)) )
                return false;
        }
    }
    return true;
}

auto StringView::starts_with(char rhs, CaseSensible case_sensitivity) const -> bool {
    if ( is_null_or_empty() )
        return false;

    if ( case_sensitivity == CaseSensible::Yes )
        return at(0) == rhs;
    else
        return to_ascii_lowercase(at(0)) == to_ascii_lowercase(rhs);
}

auto StringView::ends_with(StringView rhs, CaseSensible case_sensitivity) const -> bool {
    if ( is_null_or_empty() )
        return false;
    if ( rhs.is_null_or_empty() )
        return false;
    if ( rhs.len() > len() )
        return false;
    if ( as_cstr() == rhs.as_cstr() )
        return false;

    if ( case_sensitivity == CaseSensible::Yes )
        return __builtin_memcmp(as_cstr() + (len() - rhs.len()), rhs.as_cstr(), rhs.len()) == 0;
    else {
        usize str_i = len() - rhs.len();
        for ( auto const i : Range{ 0u, rhs.len() } ) {
            if ( to_ascii_lowercase(at(str_i)) != to_ascii_lowercase(rhs.at(i)) )
                return false;

            ++str_i;
        }
    }
    return true;
}

auto StringView::ends_with(char rhs, CaseSensible case_sensitivity) const -> bool {
    if ( is_null_or_empty() )
        return false;

    if ( case_sensitivity == CaseSensible::Yes )
        return at(len() - 1) == rhs;
    else
        return to_ascii_lowercase(at(len() - 1)) == to_ascii_lowercase(rhs);
}

static constexpr auto without_base_prefix(StringView::IntBase int_base, StringView string_view) -> StringView {
    if ( (int_base == StringView::IntBase::Binary && string_view[0] == 'b') || (int_base == StringView::IntBase::Octal && string_view[0] == '0') )
        return string_view.sub_string_view(1);
    else if ( int_base == StringView::IntBase::Hex && string_view.starts_with("0x"sv, StringView::CaseSensible::No) )
        return string_view.sub_string_view(2);
    else
        return string_view;
}
static constexpr auto is_valid_digit(StringView::IntBase int_base, char c) -> bool {
    switch ( int_base ) {
        case StringView::IntBase::Binary:
            return is_ascii_binary_digit(c);
        case StringView::IntBase::Octal:
            return is_ascii_octal_digit(c);
        case StringView::IntBase::Decimal:
            return is_ascii_digit(c);
        case StringView::IntBase::Hex:
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
        return Error::new_from_code(ErrorCode::EmptyData);

    /* extract the sign from the beginning of the string */
    T sign = 1;
    if ( is_ascii_int_sign(string_view[0]) ) {
        if ( string_view.len() == 1 )
            return Error::new_from_code(ErrorCode::BadLength);

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
             * with a code or simply break the cycle returning the value parsed until now */
            if ( parse_mode == ParseMode::BeginToEnd || parse_mode == ParseMode::TrimWhitesAndBeginToEnd )
                return Error::new_from_code(ErrorCode::BadData);
            else
                break;
        }

        if ( __builtin_mul_overflow(int_value, static_cast<T>(int_base), &int_value) )
            return Error::new_from_code(ErrorCode::IntOverflow);
        if ( __builtin_add_overflow(int_value, digit_value<T>(c), &int_value) )
            return Error::new_from_code(ErrorCode::IntOverflow);
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
        return Error::new_from_code(ErrorCode::EmptyData);

    /* convert the string into the integer */
    T int_value = 0;
    for ( auto const c : without_base_prefix(int_base, string_view) ) {
        if ( !is_valid_digit(int_base, c) ) {
            /* according to the parse_mode flag we decide here if the parser must return
             * with a code or simply break the cycle returning the value parsed until now */
            if ( parse_mode == ParseMode::BeginToEnd || parse_mode == ParseMode::TrimWhitesAndBeginToEnd )
                return Error::new_from_code(ErrorCode::BadData);
            else
                break;
        }

        if ( __builtin_mul_overflow(int_value, static_cast<T>(int_base), &int_value) )
            return Error::new_from_code(ErrorCode::IntOverflow);
        if ( __builtin_add_overflow(int_value, digit_value<T>(c), &int_value) )
            return Error::new_from_code(ErrorCode::IntOverflow);
    }
    return int_value;
}

template auto StringView::as_uint(IntBase int_base, ParseMode parse_mode) const -> ErrorOr<unsigned char>;
template auto StringView::as_uint(IntBase int_base, ParseMode parse_mode) const -> ErrorOr<unsigned short>;
template auto StringView::as_uint(IntBase int_base, ParseMode parse_mode) const -> ErrorOr<unsigned int>;
template auto StringView::as_uint(IntBase int_base, ParseMode parse_mode) const -> ErrorOr<unsigned long>;
template auto StringView::as_uint(IntBase int_base, ParseMode parse_mode) const -> ErrorOr<unsigned long long>;

auto StringView::find(char needle, usize start) const -> Option<usize> {
    if ( start >= len() )
        return {};

    for ( auto const i : Range{ start, len() } ) {
        if ( at(i) == needle )
            return i;
    }
    return {};
}

auto StringView::find(StringView needle, usize start) const -> Option<usize> {
    if ( start >= len() )
        return {};

    return find_in_memory(as_cstr() + start, len() - start, needle.as_cstr(), needle.len())
        .map<usize>([this](auto ptr_pos) -> usize { return Cxx::bit_cast<usize>(ptr_pos) - Cxx::bit_cast<usize>(as_cstr()); });
}

auto StringView::find_last(char needle) const -> Option<usize> {
    for ( usize i = len() - 1; i > 0; --i ) {
        if ( at(i) == needle )
            return i;
    }
    return {};
}

auto StringView::find_all(StringView needle) const -> Vector<usize> {
    return must$(try_find_all(needle));
}

auto StringView::try_find_all(StringView needle) const -> ErrorOr<Vector<usize>> {
    auto positions = Vector<usize>::new_empty();

    usize current_position = 0;
    while ( current_position < len() ) {
        auto ptr_pos_or_none = find_in_memory(as_cstr() + current_position, len() - current_position, needle.as_cstr(), needle.len());
        if ( !ptr_pos_or_none.is_present() )
            break;

        auto char_ptr_pos   = ptr_pos_or_none.value();
        auto relative_index = char_ptr_pos - (as_cstr() + current_position);

        try$(positions.try_append(current_position + relative_index));
        current_position += relative_index + 1;
    }
    return positions;
}

static auto try_split_view_if_helper(StringView string_view,
                                     StringView separator,
                                     StringView::KeepEmpty keep_empty,
                                     Callable<ErrorOr<void>, StringView> auto predicate) -> ErrorOr<void> {
    if ( string_view.is_empty() )
        return {};

    auto view_cursor   = string_view;
    auto index_or_none = string_view.find(separator);
    while ( index_or_none.is_present() ) {
        auto const separator_index     = index_or_none.value();
        auto const part_with_separator = view_cursor.sub_string_view(0, separator_index + separator.len());
        if ( keep_empty == StringView::KeepEmpty::Yes || separator_index > 0 )
            try$(predicate(part_with_separator.sub_string_view(0, separator_index)));

        /* advance the cursor to the next step */
        auto const remaining_chars = part_with_separator.as_cstr() + part_with_separator.len();

        view_cursor   = StringView::new_from_raw_parts(remaining_chars, view_cursor.len() - (remaining_chars - view_cursor.as_cstr()));
        index_or_none = view_cursor.find(separator);
    }

    /* provide to the predicate the last part of the view */
    if ( keep_empty == StringView::KeepEmpty::Yes || !view_cursor.is_empty() )
        try$(predicate(view_cursor));

    return {};
}

auto StringView::split_view(char separator, KeepEmpty keep_empty) const -> Vector<StringView> {
    return split_view(StringView{ &separator, 1 }, keep_empty);
}

auto StringView::split_view(StringView separator, KeepEmpty keep_empty) const -> Vector<StringView> {
    return must$(try_split_view(separator, keep_empty));
}

auto StringView::try_split_view(char separator, KeepEmpty keep_empty) const -> ErrorOr<Vector<StringView>> {
    return try_split_view(StringView{ &separator, 1 }, keep_empty);
}

auto StringView::try_split_view(StringView separator, KeepEmpty keep_empty) const -> ErrorOr<Vector<StringView>> {
    auto vector = Vector<StringView>::new_empty();
    try$(try_split_view_if_helper(*this, separator, keep_empty, [&vector](StringView string_view) -> ErrorOr<void> {
        try$(vector.try_append(string_view));
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

auto StringView::contains(StringView rhs, CaseSensible case_sensible) const -> bool {
    if ( is_null_or_empty() || rhs.is_null_or_empty() || rhs.len() > len() )
        return false;

    if ( case_sensible == CaseSensible::Yes )
        return find_in_memory(as_cstr(), len(), rhs.as_cstr(), rhs.len()).is_present();

    auto needle_first = to_ascii_lowercase(rhs.at(0));
    for ( auto i = 0u; i < len(); ++i ) {
        if ( to_ascii_lowercase(at(i)) != needle_first )
            continue;

        for ( auto ni = 0u; i + ni < len(); ++ni ) {
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

auto StringView::contains(char rhs, CaseSensible case_sensible) const -> bool {
    if ( is_null_or_empty() )
        return false;

    return any_of<char>(begin(), end(), [rhs, case_sensible](char const c) {
        auto unicode_c = u32{ static_cast<u8>(c) };
        auto unicode_rhs = u32{ static_cast<u8>(rhs) };
        if ( case_sensible == CaseSensible::Yes ) {
            unicode_c   = to_ascii_lowercase(c);
            unicode_rhs = to_ascii_lowercase(unicode_rhs);
        }

        return unicode_c == unicode_rhs;
    });
}

auto StringView::begin() const -> StringView::ConstIterator {
    return ConstIterator::new_from_begin(this);
}

auto StringView::end() const -> StringView::ConstIterator {
    return ConstIterator::new_from_end(this);
}

auto StringView::rbegin() const -> StringView::ConstReverseIterator {
    return ConstReverseIterator::new_from_rbegin(this);
}

auto StringView::rend() const -> StringView::ConstReverseIterator {
    return ConstReverseIterator::new_from_rend(this);
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

auto StringView::is_empty() const -> bool {
    return len() == 0;
}

auto StringView::is_null() const -> bool {
    return m_chars_ptr == nullptr;
}

auto StringView::is_null_or_empty() const -> bool {
    return is_null() || is_empty();
}
