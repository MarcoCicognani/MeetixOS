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

#pragma clang diagnostic push
#pragma ide diagnostic   ignored "modernize-use-trailing-return-type"

#include "lib.hh"

#include <LibC/errno.h>
#include <LibC/limits.h>
#include <LibC/stdlib.h>
#include <LibC/string.h>

#include <LibApi/Api/User.h>

#include <CCLang/Core/Assertions.hh>
#include <CCLang/Core/CharTypes.hh>
#include <CCLang/Core/Concept.hh>
#include <CCLang/Lang/StringView.hh>

static u64 g_seed;

static auto swap(u8* x, u8* y, usize element_size) -> void {
    auto a = x;
    auto b = y;
    u8   c;
    while ( element_size-- ) {
        c    = *a;
        *a++ = *b;
        *b++ = c;
    }
}

static auto sort(u8* arr, usize el_sz, Callable<i32, void const*, void const*> auto comparator, usize begin, usize end) -> void {
    if ( end > begin ) {
        auto pivot        = arr + begin;
        auto begin_offset = begin + el_sz;
        auto end_offset   = end;

        while ( begin_offset < end_offset ) {
            if ( comparator(arr + begin_offset, pivot) <= 0 )
                begin_offset += el_sz;
            else if ( comparator(arr + end_offset, pivot) > 0 )
                end_offset -= el_sz;
            else if ( begin_offset < end_offset )
                swap(arr + begin_offset, arr + end_offset, el_sz);
        }

        begin_offset -= el_sz;

        swap(arr + begin, arr + begin_offset, el_sz);
        sort(arr, el_sz, comparator, begin, begin_offset);
        sort(arr, el_sz, comparator, end_offset, end);
    }
}

static auto int_base_from(StringView string_view, i32 base) -> IntBase {
    IntBase int_base;
    switch ( base ) {
        case 0:
            if ( string_view.starts_with('b') )
                int_base = IntBase::Binary;
            else if ( string_view.starts_with('0') )
                int_base = IntBase::Octal;
            else if ( string_view.starts_with("0x"sv, CaseSensitivity::Insensitive) )
                int_base = IntBase::Hex;
            else
                int_base = IntBase::Decimal;
            break;
        case 2:
            int_base = IntBase::Binary;
            break;
        case 8:
            int_base = IntBase::Octal;
            break;
        case 10:
            int_base = IntBase::Decimal;
            break;
        case 16:
            int_base = IntBase::Hex;
            break;
        default:
            verify_not_reached$();
    }
    return int_base;
}

static auto is_digit_for_base(IntBase int_base, char c) -> bool {
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
            verify_not_reached$();
    }
}

extern "C" {

double atof(const char* str) {
    return static_cast<double>(strtof(str, nullptr));
}

int atoi(const char* str) {
    auto long_int = strtol(str, nullptr, 10);

    if ( long_int > INT_MAX ) {
        errno = ERANGE;
        return INT_MAX;
    }
    if ( long_int < INT_MIN ) {
        errno = ERANGE;
        return INT_MIN;
    }

    return static_cast<int>(long_int);
}

long atol(const char* str) {
    return strtol(str, nullptr, 10);
}

long long atoll(const char* str) {
    return strtol(str, nullptr, 10);
}

float strtof(const char*, char**) {
    __NOT_IMPLEMENTED(strtof);
    return 0;
}

double strtod(const char*, char**) {
    __NOT_IMPLEMENTED(strtod);
    return 0;
}

long double strtold(const char*, char**) {
    __NOT_IMPLEMENTED(strtold);
    return 0;
}

long strtol(const char* str, char** endptr, int base) {
    auto const string_view = StringView::construct_from_cstr(str);
    auto const int_base    = int_base_from(string_view, base);

    /* convert the string to integer */
    auto error_or_int_value = string_view.as_int<long>(int_base, ParseMode::TrimWhitesAndBegin);
    if ( error_or_int_value.is_error() ) {
        errno = error_or_int_value.error().code();
        return 0;
    }

    /* find the first invalid digit index to set the <endptr> */
    if ( endptr != nullptr ) {
        auto const first_non_valid_digit_index
            = string_view.find_last_if([int_base](char c) { return !is_digit_for_base(int_base, c); }).unwrap_or(string_view.len());

        *endptr = const_cast<char*>(str + first_non_valid_digit_index);
    }
    return error_or_int_value.value();
}

unsigned long strtoul(const char* str, char** endptr, int base) {
    auto const string_view = StringView::construct_from_cstr(str);
    auto const int_base    = int_base_from(string_view, base);

    /* convert the string to integer */
    auto error_or_int_value = string_view.as_uint<unsigned long>(int_base, ParseMode::TrimWhitesAndBegin);
    if ( error_or_int_value.is_error() ) {
        errno = error_or_int_value.error().code();
        return 0;
    }

    /* find the first invalid digit index to set the <endptr> */
    if ( endptr != nullptr ) {
        auto const first_non_valid_digit_index
            = string_view.find_last_if([int_base](char c) { return !is_digit_for_base(int_base, c); }).unwrap_or(string_view.len());

        *endptr = const_cast<char*>(str + first_non_valid_digit_index);
    }
    return error_or_int_value.value();
}

long long strtoll(const char* str, char** endptr, int base) {
    auto const string_view = StringView::construct_from_cstr(str);
    auto const int_base    = int_base_from(string_view, base);

    /* convert the string to integer */
    auto error_or_int_value = string_view.as_int<long long>(int_base, ParseMode::TrimWhitesAndBegin);
    if ( error_or_int_value.is_error() ) {
        errno = error_or_int_value.error().code();
        return 0;
    }

    /* find the first invalid digit index to set the <endptr> */
    if ( endptr != nullptr ) {
        auto const first_non_valid_digit_index
            = string_view.find_last_if([int_base](char c) { return !is_digit_for_base(int_base, c); }).unwrap_or(string_view.len());

        *endptr = const_cast<char*>(str + first_non_valid_digit_index);
    }
    return error_or_int_value.value();
}

unsigned long long strtoull(const char* str, char** endptr, int base) {
    auto const string_view = StringView::construct_from_cstr(str);
    auto const int_base    = int_base_from(string_view, base);

    /* convert the string to integer */
    auto error_or_int_value = string_view.as_uint<unsigned long long>(int_base, ParseMode::TrimWhitesAndBegin);
    if ( error_or_int_value.is_error() ) {
        errno = error_or_int_value.error().code();
        return 0;
    }

    /* find the first invalid digit index to set the <endptr> */
    if ( endptr != nullptr ) {
        auto const first_non_valid_digit_index
            = string_view.find_last_if([int_base](char c) { return !is_digit_for_base(int_base, c); }).unwrap_or(string_view.len());

        *endptr = const_cast<char*>(str + first_non_valid_digit_index);
    }
    return error_or_int_value.value();
}

void srand(int s) {
    g_seed = s - 1;
}

int rand(void) {
    return static_cast<int>((g_seed = 9418247712843950125ULL * g_seed + 1) >> 33);
}

void abort(void) {
    s_exit(EXIT_FAILURE);
    __builtin_unreachable();
}

void exit(int code) {
    libc_fini();
    s_exit(code);
    __builtin_unreachable();
}

void _Exit(int code) {
    s_exit(code);
    __builtin_unreachable();
}

void quick_exit(int code) {
    _Exit(code);
}

int atexit(void (*)(void)) {
    __NOT_IMPLEMENTED(atexit);
    return 0;
}

int at_quick_exit(void (*)(void)) {
    __NOT_IMPLEMENTED(at_quick_exit);
    return 0;
}

char* getenv(const char*) {
    __NOT_IMPLEMENTED(getenv);
    return 0;
}

int setenv(const char*, const char*, int) {
    __NOT_IMPLEMENTED(setenv);
    return 0;
}

int system(const char*) {
    __NOT_IMPLEMENTED(system);
    return 0;
}

void* bsearch(const void*, const void*, usize, usize, int (*)(const void*, const void*)) {
    __NOT_IMPLEMENTED(bsearch);
    return nullptr;
}

void qsort(void* array, usize num_elements, usize element_size, int (*comparator)(const void*, const void*)) {
    sort(reinterpret_cast<u8*>(array), element_size, comparator, 0, num_elements * element_size);
}

int abs(int v) {
    if ( v < 0 )
        return -v;
    else
        return v;
}

long labs(long v) {
    if ( v < 0 )
        return -v;
    else
        return v;
}

long long llabs(long long v) {
    if ( v < 0 )
        return -v;
    else
        return v;
}

div_t div(int numer, int denom) {
    div_t result;
    result.quot = numer / denom;
    result.rem  = numer % denom;
    return result;
}

ldiv_t ldiv(long int numer, long int denom) {
    ldiv_t result;
    result.quot = numer / denom;
    result.rem  = numer % denom;
    return result;
}

lldiv_t lldiv(long long int numer, long long int denom) {
    lldiv_t result;
    result.quot = numer / denom;
    result.rem  = numer % denom;
    return result;
}

int mblen(const char*, usize) {
    __NOT_IMPLEMENTED(mblen);
    return 0;
}

int mbtowc(wchar_t*, const char*, usize) {
    __NOT_IMPLEMENTED(mbtowc);
    return 0;
}

int wctomb(char*, wchar_t) {
    __NOT_IMPLEMENTED(wctomb);
    return 0;
}

usize mbstowcs(wchar_t*, const char*, usize) {
    __NOT_IMPLEMENTED(mbstowcs);
    return 0;
}

usize wcstombs(char*, const wchar_t*, usize) {
    __NOT_IMPLEMENTED(wcstombs);
    return 0;
}

char* mktemp(char* templ) {
    if ( !templ )
        return templ;

    auto templ_len = strlen(templ);
    if ( templ_len < 6 ) {
        templ[0] = '\0';
        return templ;
    }

    for ( auto i = templ_len - 6; i < templ_len; ++i ) {
        if ( templ[i] != 'X' ) {
            errno    = EINVAL;
            templ[0] = '\0';
            return templ;
        }

        templ[i] = static_cast<char>('a' + rand() % ('z' - 'a'));
    }
    return templ;
}

} /* extern "C" */

#pragma clang diagnostic pop
