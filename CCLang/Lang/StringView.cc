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

#include <CCLang/Alloc/Vector.hh>
#include <CCLang/Core/AllOf.hh>
#include <CCLang/Core/AnyOf.hh>
#include <CCLang/Core/Assertions.hh>
#include <CCLang/Core/CharTypes.hh>
#include <CCLang/Core/ErrorOr.hh>
#include <CCLang/Core/Find.hh>
#include <CCLang/Lang/Cxx.hh>
#include <CCLang/Lang/Must.hh>
#include <CCLang/Lang/StringView.hh>
#include <CCLang/Lang/Try.hh>

auto StringView::from_cstr(const char* c_str) -> StringView {
    return from_raw_parts(c_str, Cxx::strlen(c_str));
}

auto StringView::from_raw_parts(char const* c_str, usize len) -> StringView {
    return StringView(c_str, len);
}

StringView::StringView(StringView&& rhs)
    : m_chars_slice(Cxx::move(rhs.m_chars_slice)) {
}

auto StringView::operator=(StringView const& rhs) -> StringView& {
    if ( this == &rhs ) {
        return *this;
    }

    StringView string_view = rhs;
    swap(string_view);
    return *this;
}

auto StringView::operator=(StringView&& rhs) -> StringView& {
    StringView string_view = Cxx::move(rhs);
    swap(string_view);
    return *this;
}

auto StringView::swap(StringView& rhs) -> void {
    Cxx::swap(m_chars_slice, rhs.m_chars_slice);
}

auto StringView::at(usize index) const -> char const& {
    return m_chars_slice.at(index);
}

auto StringView::operator[](usize index) const -> char const& {
    return at(index);
}

auto StringView::compare(StringView rhs) const -> Order {
    if ( is_null_or_empty() ) {
        return rhs.is_null_or_empty() ? Order::Equal : Order::Less;
    }
    if ( rhs.is_null_or_empty() ) {
        return Order::Greater;
    }

    /* use the optimized mem-cmp for the minimum of the two lengths */
    auto min_len = len() < rhs.len() ? len() : rhs.len();
    auto order   = Cxx::memcmp(as_cstr(), rhs.as_cstr(), min_len);
    if ( order == Order::Equal ) {
        if ( len() < rhs.len() ) {
            return Order::Less;
        } else if ( len() == rhs.len() ) {
            return Order::Equal;
        } else {
            return Order::Greater;
        }
    }
    return order;
}

auto StringView::equals_ignore_case(StringView rhs) const -> bool {
    if ( len() != rhs.len() ) {
        return false;
    }

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
        for ( auto const i : usize::range(0, len()) ) {
            if ( sub_len == 0 ) {
                return ""sv;
            }
            if ( !chars.contains(at(i)) ) {
                break;
            }

            ++sub_start;
            --sub_len;
        }
    }

    if ( trim_mode == TrimMode::Right || trim_mode == TrimMode::Both ) {
        for ( auto i = len() - 1; i > 0; --i ) {
            if ( sub_len == 0 ) {
                return ""sv;
            }
            if ( !chars.contains(at(i)) ) {
                break;
            }

            --sub_len;
        }
    }

    return sub_string_view(sub_start, sub_len);
}

auto StringView::trim_whitespaces(TrimMode trim_mode) const -> StringView {
    return trim(" \n\t\v\f\r"sv, trim_mode);
}

auto StringView::starts_with(StringView rhs, CaseSensible case_sensitivity) const -> bool {
    if ( is_null_or_empty() ) {
        return false;
    }
    if ( rhs.is_null_or_empty() ) {
        return false;
    }
    if ( rhs.len() > len() ) {
        return false;
    }
    if ( as_cstr() == rhs.as_cstr() ) {
        return true;
    }

    if ( case_sensitivity == CaseSensible::Yes ) {
        return Cxx::memcmp(as_cstr(), rhs.as_cstr(), rhs.len()) == Order::Equal;
    } else {
        for ( auto const i : usize::range(0, rhs.len()) ) {
            if ( to_ascii_lowercase(at(i)) != to_ascii_lowercase(rhs.at(i)) ) {
                return false;
            }
        }
    }
    return true;
}

auto StringView::starts_with(char rhs, CaseSensible case_sensitivity) const -> bool {
    if ( is_null_or_empty() ) {
        return false;
    }

    if ( case_sensitivity == CaseSensible::Yes ) {
        return at(0) == rhs;
    } else {
        return to_ascii_lowercase(at(0)) == to_ascii_lowercase(rhs);
    }
}

auto StringView::ends_with(StringView rhs, CaseSensible case_sensitivity) const -> bool {
    if ( is_null_or_empty() ) {
        return false;
    }
    if ( rhs.is_null_or_empty() ) {
        return false;
    }
    if ( rhs.len() > len() ) {
        return false;
    }
    if ( as_cstr() == rhs.as_cstr() ) {
        return false;
    }

    if ( case_sensitivity == CaseSensible::Yes ) {
        return Cxx::memcmp(as_cstr() + (len() - rhs.len()), rhs.as_cstr(), rhs.len()) == Order::Equal;
    } else {
        usize str_i = len() - rhs.len();
        for ( auto const i : usize::range(0, rhs.len()) ) {
            if ( to_ascii_lowercase(at(str_i)) != to_ascii_lowercase(rhs.at(i)) ) {
                return false;
            }

            ++str_i;
        }
    }
    return true;
}

auto StringView::ends_with(char rhs, CaseSensible case_sensitivity) const -> bool {
    if ( is_null_or_empty() ) {
        return false;
    }

    if ( case_sensitivity == CaseSensible::Yes ) {
        return at(len() - 1) == rhs;
    } else {
        return to_ascii_lowercase(at(len() - 1)) == to_ascii_lowercase(rhs);
    }
}

static auto without_base_prefix(StringView::IntBase int_base, StringView string_view) -> StringView {
    if ( (int_base == StringView::IntBase::Binary && string_view[0] == 'b') || (int_base == StringView::IntBase::Octal && string_view[0] == '0') ) {
        return string_view.sub_string_view(1);
    } else if ( int_base == StringView::IntBase::Hex && string_view.starts_with("0x"sv, StringView::CaseSensible::No) ) {
        return string_view.sub_string_view(2);
    } else {
        return string_view;
    }
}
static auto is_valid_digit(StringView::IntBase int_base, char c) -> bool {
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
static auto digit_value(char c) -> T {
    if ( c >= '0' && c <= '9' ) {
        return c - '0';
    } else if ( c >= 'a' && c <= 'z' ) {
        return c - 'a' + 10;
    } else if ( c >= 'A' && c <= 'Z' ) {
        return c - 'A' + 10;
    } else {
        return 0;
    }
}
template<typename T>
static auto int_base_as_integer(StringView::IntBase int_base) -> T {
    return static_cast<typename T::NativeInt>(static_cast<UnderlyingType<StringView::IntBase>>(int_base));
}

template<typename T>
auto StringView::as_int(IntBase int_base, ParseMode parse_mode) const -> ErrorOr<T> {
    /* trim the whitespaces if requested */
    auto string_view = *this;
    if ( parse_mode == ParseMode::TrimWhitesAndBegin ) {
        string_view = string_view.trim_whitespaces(TrimMode::Left);
    } else if ( parse_mode == ParseMode::TrimWhitesAndBeginToEnd ) {
        string_view = string_view.trim_whitespaces(TrimMode::Both);
    }

    /* check for only whitespaces string */
    if ( string_view.is_empty() ) {
        return Error::from_code(ErrorCode::EmptyData);
    }

    /* extract the sign from the beginning of the string */
    T sign = 1;
    if ( is_ascii_int_sign(string_view[0]) ) {
        if ( string_view.len() == 1 ) {
            return Error::from_code(ErrorCode::BadLength);
        }

        /* for minus set the sign to negative */
        if ( string_view[0] == '-' ) {
            sign = -1;
        }

        /* remove the sign from the beginning */
        string_view = string_view.sub_string_view(1);
    }

    /* convert the string into the integer */
    T int_value = 0;
    for ( auto const c : without_base_prefix(int_base, string_view) ) {
        if ( !is_valid_digit(int_base, c) ) {
            /* according to the parse all flag we decide here if the parser must return
             * with a code or simply break the cycle returning the value parsed until now */
            if ( parse_mode == ParseMode::BeginToEnd || parse_mode == ParseMode::TrimWhitesAndBeginToEnd ) {
                return Error::from_code(ErrorCode::BadData);
            } else {
                break;
            }
        }

        try$(int_value.try_mul(int_base_as_integer<T>(int_base)));
        try$(int_value.try_add(digit_value<T>(c)));
    }

    /* apply the sign to the final value */
    int_value *= sign;
    return int_value;
}

template auto StringView::as_int(IntBase int_base, ParseMode parse_mode) const -> ErrorOr<i8>;
template auto StringView::as_int(IntBase int_base, ParseMode parse_mode) const -> ErrorOr<i16>;
template auto StringView::as_int(IntBase int_base, ParseMode parse_mode) const -> ErrorOr<i32>;
template auto StringView::as_int(IntBase int_base, ParseMode parse_mode) const -> ErrorOr<i64>;
template auto StringView::as_int(IntBase int_base, ParseMode parse_mode) const -> ErrorOr<isize>;

template<typename T>
auto StringView::as_uint(IntBase int_base, ParseMode parse_mode) const -> ErrorOr<T> {
    /* trim the whitespaces if requested */
    auto string_view = *this;
    if ( parse_mode == ParseMode::TrimWhitesAndBegin ) {
        string_view = string_view.trim_whitespaces(TrimMode::Left);
    } else if ( parse_mode == ParseMode::TrimWhitesAndBeginToEnd ) {
        string_view = string_view.trim_whitespaces(TrimMode::Both);
    }

    /* check for only whitespaces string */
    if ( string_view.is_empty() ) {
        return Error::from_code(ErrorCode::EmptyData);
    }

    /* convert the string into the integer */
    T int_value = 0;
    for ( auto const c : without_base_prefix(int_base, string_view) ) {
        if ( !is_valid_digit(int_base, c) ) {
            /* according to the parse_mode flag we decide here if the parser must return
             * with a code or simply break the cycle returning the value parsed until now */
            if ( parse_mode == ParseMode::BeginToEnd || parse_mode == ParseMode::TrimWhitesAndBeginToEnd ) {
                return Error::from_code(ErrorCode::BadData);
            } else {
                break;
            }
        }

        try$(int_value.try_mul(int_base_as_integer<T>(int_base)));
        try$(int_value.try_add(digit_value<T>(c)));
    }
    return int_value;
}

template auto StringView::as_uint(IntBase int_base, ParseMode parse_mode) const -> ErrorOr<u8>;
template auto StringView::as_uint(IntBase int_base, ParseMode parse_mode) const -> ErrorOr<u16>;
template auto StringView::as_uint(IntBase int_base, ParseMode parse_mode) const -> ErrorOr<u32>;
template auto StringView::as_uint(IntBase int_base, ParseMode parse_mode) const -> ErrorOr<u64>;
template auto StringView::as_uint(IntBase int_base, ParseMode parse_mode) const -> ErrorOr<usize>;

auto StringView::find(char needle, usize start) const -> Option<usize> {
    if ( start >= len() ) {
        return {};
    }

    for ( auto i : usize::range(0, len()) ) {
        if ( at(i) == needle ) {
            return i;
        }
    }
    return {};
}

auto StringView::find(StringView needle, usize start) const -> Option<usize> {
    if ( start >= len() ) {
        return {};
    }

    return find_in_memory(Slice<char const>::from_raw_parts(as_cstr() + start, len() - start),
                          Slice<char const>::from_raw_parts(needle.as_cstr(), needle.len()));
}

auto StringView::find_last(char needle) const -> Option<usize> {
    for ( usize i = len() - 1; i > 0; --i ) {
        if ( at(i) == needle ) {
            return i;
        }
    }
    return {};
}

auto StringView::find_all(StringView needle) const -> Vector<usize> {
    return must$(try_find_all(needle));
}

auto StringView::try_find_all(StringView needle) const -> ErrorOr<Vector<usize>> {
    auto positions = Vector<usize>::empty();

    usize current_position = 0;
    while ( current_position < len() ) {
        auto ptr_index_or_none = find_in_memory(as_slice().sub_slice(current_position), needle.as_slice());
        if ( !ptr_index_or_none.is_present() ) {
            break;
        }

        auto ptr_index      = ptr_index_or_none.unwrap();
        auto relative_index = ptr_index - usize(Cxx::bit_cast<unsigned int>(as_cstr() + current_position));

        try$(positions.try_append(current_position + relative_index));
        current_position += relative_index + 1;
    }
    return positions;
}

/* clang-format off */

static auto try_split_view_if_helper(StringView                               string_view,
                                     StringView                               separator,
                                     StringView::KeepEmpty                    keep_empty,
                                     Callable<ErrorOr<void>, StringView> auto predicate) -> ErrorOr<void> {
    if ( string_view.is_empty() ) {
        return {};
    }

    auto view_cursor   = string_view;
    auto index_or_none = string_view.find(separator);
    while ( index_or_none.is_present() ) {
        auto const separator_index     = index_or_none.unwrap();
        auto const part_with_separator = view_cursor.sub_string_view(0, separator_index + separator.len());
        if ( keep_empty == StringView::KeepEmpty::Yes || separator_index > 0 ) {
            try$(predicate(part_with_separator.sub_string_view(0, separator_index)));
        }

        /* advance the cursor to the next step */
        auto const  rem_chars = part_with_separator.as_cstr() + part_with_separator.len();
        usize const split_len = rem_chars - view_cursor.as_cstr();

        view_cursor   = StringView::from_raw_parts(rem_chars, view_cursor.len() - split_len);
        index_or_none = view_cursor.find(separator);
    }

    /* provide to the predicate the last part of the view */
    if ( keep_empty == StringView::KeepEmpty::Yes || !view_cursor.is_empty() ) {
        try$(predicate(view_cursor));
    }

    return {};
}

/* clang-format on */

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
    auto vector = Vector<StringView>::empty();
    try$(try_split_view_if_helper(*this, separator, keep_empty,[&vector](StringView string_view) -> ErrorOr<void> {
        try$(vector.try_append(string_view));
        return {};
    }));
    return vector;
}

auto StringView::operator==(StringView const& rhs) const -> bool {
    return len() == rhs.len() && compare(rhs) == Order::Equal;
}

auto StringView::operator!=(StringView const& rhs) const -> bool {
    return !operator==(rhs);
}

auto StringView::operator<(StringView const& rhs) const -> bool {
    return len() < rhs.len() || compare(rhs) == Order::Less;
}

auto StringView::operator<=(StringView const& rhs) const -> bool {
    return len() <= rhs.len() || compare(rhs) == Order::Less || compare(rhs) == Order::Equal; /* FIXME not the best solution for the efficiency */
}

auto StringView::operator>(StringView const& rhs) const -> bool {
    return len() > rhs.len() || compare(rhs) == Order::Greater;
}

auto StringView::operator>=(StringView const& rhs) const -> bool {
    return len() >= rhs.len() || compare(rhs) == Order::Greater || compare(rhs) == Order::Equal; /* FIXME not the best solution for the efficiency */
}

auto StringView::contains(StringView rhs, CaseSensible case_sensible) const -> bool {
    if ( is_null_or_empty() || rhs.is_null_or_empty() || rhs.len() > len() ) {
        return false;
    }

    if ( case_sensible == CaseSensible::Yes ) {
        return find_in_memory(as_slice(), rhs.as_slice()).is_present();
    }

    auto needle_first = to_ascii_lowercase(rhs.at(0));
    for ( usize i = 0; i < len(); ++i ) {
        if ( to_ascii_lowercase(at(i)) != needle_first ) {
            continue;
        }

        for ( usize ni = 0; i + ni < len(); ++ni ) {
            if ( to_ascii_lowercase(at(i + ni)) != to_ascii_lowercase(rhs.at(ni)) ) {
                i += ni;
                break;
            }
            if ( ni + 1 == rhs.len() ) {
                return true;
            }
        }
    }
    return false;
}

auto StringView::contains(char rhs, CaseSensible case_sensible) const -> bool {
    if ( is_null_or_empty() ) {
        return false;
    }

    return any_of<char>(begin(), end(), [rhs, case_sensible](char const c) {
        u32 c_as_rune = c;
        u32 r_as_rune = rhs;
        if ( case_sensible == CaseSensible::Yes ) {
            c_as_rune = to_ascii_lowercase(c);
            r_as_rune = to_ascii_lowercase(r_as_rune);
        }
        return c_as_rune == r_as_rune;
    });
}

auto StringView::begin() const -> StringView::ConstIterator {
    return m_chars_slice.begin();
}

auto StringView::end() const -> StringView::ConstIterator {
    return m_chars_slice.end();
}

auto StringView::rbegin() const -> StringView::ConstReverseIterator {
    return m_chars_slice.rbegin();
}

auto StringView::rend() const -> StringView::ConstReverseIterator {
    return m_chars_slice.rend();
}

auto StringView::reverse_iter() const -> StringView::ConstReverseIteratorWrapper {
    return m_chars_slice.reverse_iter();
}

auto StringView::as_slice() const -> Slice<char const> {
    return m_chars_slice;
}

auto StringView::hash_code() const -> usize {
    usize hash = 0;
    for ( auto const c : m_chars_slice ) {
        hash += usize(c);
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += hash << 3;
    hash ^= hash >> 11;
    hash += hash << 15;
    return hash;
}

auto StringView::as_cstr() const -> char const* {
    return m_chars_slice.data();
}

auto StringView::len() const -> usize {
    return m_chars_slice.len();
}

auto StringView::is_empty() const -> bool {
    return m_chars_slice.is_empty();
}

auto StringView::is_null() const -> bool {
    return m_chars_slice.is_null();
}

auto StringView::is_null_or_empty() const -> bool {
    return is_null() || is_empty();
}

StringView::StringView(char const* c_str, usize count)
    : m_chars_slice(Slice<const char>::from_raw_parts(c_str, count)) {
}
