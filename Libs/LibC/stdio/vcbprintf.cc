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

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#define LENGTH_DEFAULT 0
#define LENGTH_hh      1
#define LENGTH_h       2
#define LENGTH_l       3
#define LENGTH_ll      4
#define LENGTH_j       5
#define LENGTH_z       6
#define LENGTH_t       7
#define LENGTH_L       8

#define CHOMP_CHAR()                                                                                                   \
    if ( !*++s )                                                                                                       \
        return written;

extern "C" {

static int string_to_number(const char** s) {
    char* end_ptr;
    auto  res = static_cast<int>(strtol(*s, &end_ptr, 10));
    *s        = end_ptr;
    return res;
}

static usize integer_to_string(char* out, uintmax_t value, uintmax_t base, const char* digits) {
    auto count = 1;

    /* check how many characters are required */
    auto temp = uintmax_t{ value };
    while ( temp >= base ) {
        temp /= base;
        ++count;
    }

    /* convert to digits */
    for ( auto i = count; i != 0; --i ) {
        out[i - 1] = digits[value % base];
        value /= base;
    }
    return count;
}

int vcbprintf(void* param, isize (*callback)(void*, const char*, usize), const char* format, va_list arg_list) {
    auto s       = format;
    auto written = 0;

    char number_buffer[24];
    char precision_buffer[24];

    /* iterate through format characters */
    while ( *s ) {
        /* enter format */
        if ( *s == '%' ) {
            CHOMP_CHAR();

            /* early exit on '%' */
            if ( *s == '%' ) {
                if ( callback(param, s, 1) != 1 )
                    return -1;
                ++written;
                CHOMP_CHAR();
                continue;
            }

            /* flags */
            bool flag_left_justify                   = false;
            bool flag_always_prepend_sign            = false;
            bool flag_always_prepend_space_plus_sign = false;
            bool flag_force_0x_or_dec                = false;
            bool flag_left_pad_zeroes                = false;

            bool flag_found;
            do {
                flag_found = false;
                switch ( *s ) {
                    case '-':
                        flag_left_justify = true;
                        flag_found        = true;
                        break;
                    case '+':
                        flag_always_prepend_sign = true;
                        flag_found               = true;
                        break;
                    case ' ':
                        flag_always_prepend_space_plus_sign = true;
                        flag_found                          = true;
                        break;
                    case '#':
                        flag_force_0x_or_dec = true;
                        flag_found           = true;
                        break;
                    case '0':
                        flag_left_pad_zeroes = true;
                        flag_found           = true;
                        break;
                }

                /* eat the character */
                if ( flag_found )
                    CHOMP_CHAR();
            } while ( flag_found );

            /* width */
            auto width = 0;
            if ( *s == '*' ) {
                /* take from argument list */
                width = va_arg(arg_list, int);
                CHOMP_CHAR();
            } else if ( *s >= '0' && *s <= '9' ) {
                /* take from format,
                 * NOTE: string_to_number already chomp the characters */
                width = string_to_number(&s);
            }

            /* precision */
            auto precision          = 6;
            bool explicit_precision = false;
            if ( *s == '.' ) {
                CHOMP_CHAR();
                explicit_precision = true;
                if ( *s == '*' ) {
                    /* take from argument list */
                    precision = va_arg(arg_list, int);
                    CHOMP_CHAR();

                } else if ( *s >= '0' && *s <= '9' ) {
                    /* take from format,
                     * NOTE: string_to_number already chomp the characters */
                    precision = string_to_number(&s);
                }
            }

            /* length of the format */
            auto length = LENGTH_DEFAULT;
            if ( *s == 'h' ) {
                CHOMP_CHAR();

                if ( *s == 'h' ) {
                    length = LENGTH_hh;
                    CHOMP_CHAR();
                } else {
                    length = LENGTH_h;
                }
            } else if ( *s == 'l' ) {
                CHOMP_CHAR();

                if ( *s == 'l' ) {
                    length = LENGTH_ll;
                    CHOMP_CHAR();
                } else {
                    length = LENGTH_l;
                }
            } else if ( *s == 'j' ) {
                CHOMP_CHAR();
                length = LENGTH_j;
            } else if ( *s == 'z' ) {
                CHOMP_CHAR();
                length = LENGTH_z;
            } else if ( *s == 't' ) {
                CHOMP_CHAR();
                length = LENGTH_t;
            } else if ( *s == 'L' ) {
                CHOMP_CHAR();
                length = LENGTH_L;
            }

            /* use specifier and length to get value of argument */
            auto specifier = *s;
            switch ( *s ) {
                case 'u':
                case 'o':
                case 'x':
                case 'X':
                case 'd':
                case 'i':
                case 'p': {
                    uintmax_t value;
                    bool      is_signed;
                    if ( specifier == 'd' || specifier == 'i' ) {
                        is_signed = true;
                        switch ( length ) {
                            case LENGTH_DEFAULT:
                                value = va_arg(arg_list, int);
                                break;
                            case LENGTH_hh:
                                value = (uintmax_t)((char)va_arg(arg_list, int));
                                break;
                            case LENGTH_h:
                                value = (short int)va_arg(arg_list, int);
                                break;
                            case LENGTH_l:
                                value = va_arg(arg_list, long int);
                                break;
                            case LENGTH_ll:
                                value = va_arg(arg_list, long long int);
                                break;
                            case LENGTH_j:
                                value = va_arg(arg_list, intmax_t);
                                break;
                            case LENGTH_z:
                                value = va_arg(arg_list, size_t);
                                break;
                            case LENGTH_t:
                                value = va_arg(arg_list, ptrdiff_t);
                                break;
                            default:
                                errno = EINVAL;
                                return -1;
                        }
                    } else {
                        is_signed = false;

                        switch ( length ) {
                            case LENGTH_DEFAULT:
                                value = va_arg(arg_list, unsigned int);
                                break;
                            case LENGTH_hh:
                                value = (unsigned char)va_arg(arg_list, int);
                                break;
                            case LENGTH_h:
                                value = (unsigned short int)va_arg(arg_list, int);
                                break;
                            case LENGTH_l:
                                value = va_arg(arg_list, unsigned long int);
                                break;
                            case LENGTH_ll:
                                value = va_arg(arg_list, unsigned long long int);
                                break;
                            case LENGTH_j:
                                value = va_arg(arg_list, uintmax_t);
                                break;
                            case LENGTH_z:
                                value = va_arg(arg_list, size_t);
                                break;
                            case LENGTH_t:
                                value = va_arg(arg_list, ptrdiff_t);
                                break;
                            default:
                                errno = EINVAL;
                                return -1;
                        }
                    }

                    /* write number in temporary buffer */
                    const char* digits;
                    int         base;
                    bool        is_negative = (is_signed && ((intmax_t)value) < 0);

                    /* pointers are printed as numbers */
                    if ( specifier == 'p' ) {
                        specifier            = 'x';
                        flag_force_0x_or_dec = true;
                    }

                    /* prepare base characters */
                    if ( specifier == 'x' ) {
                        base   = 16;
                        digits = "0123456789abcdef";
                    } else if ( specifier == 'X' ) {
                        base   = 16;
                        digits = "0123456789ABCDEF";
                    } else if ( specifier == 'o' ) {
                        base   = 8;
                        digits = "012345678";
                    } else {
                        base   = 10;
                        digits = "0123456789";
                    }

                    auto len
                        = integer_to_string(number_buffer, (is_negative ? -((intmax_t)value) : value), base, digits);

                    /* check for additional */
                    auto additional_len = 0;
                    if ( flag_always_prepend_sign || flag_always_prepend_space_plus_sign || is_negative ) {
                        ++additional_len; /* +/-/space */
                    }
                    if ( flag_force_0x_or_dec && (specifier == 'x' || specifier == 'X') ) {
                        additional_len += 2; /* 0x/0X */
                    }
                    if ( flag_force_0x_or_dec && specifier == 'o' ) {
                        ++additional_len; /* 0 */
                    }

                    /* adjust width to pad correctly */
                    if ( width < len + additional_len ) {
                        width = len + additional_len;
                    }

                    /* left padding with spaces */
                    if ( !flag_left_justify && !flag_left_pad_zeroes ) {
                        for ( int i = 0; i < width - len - additional_len; ++i ) {
                            if ( callback(param, " ", 1) != 1 )
                                return -1;
                            ++written;
                        }
                    }

                    /* sign */
                    if ( is_negative ) {
                        if ( callback(param, "-", 1) != 1 )
                            return -1;
                        ++written;
                    } else if ( flag_always_prepend_sign ) {
                        if ( callback(param, "+", 1) != 1 )
                            return -1;
                        ++written;
                    } else if ( flag_always_prepend_space_plus_sign ) {
                        if ( callback(param, " ", 1) != 1 )
                            return -1;
                        ++written;
                    }

                    /* left padding with zeroes */
                    if ( !flag_left_justify && flag_left_pad_zeroes ) {
                        for ( int i = 0; i < width - len - additional_len; ++i ) {
                            if ( callback(param, "0", 1) != 1 )
                                return -1;
                            ++written;
                        }
                    }

                    /* additional */
                    if ( flag_force_0x_or_dec && (specifier == 'x' || specifier == 'X') ) {
                        if ( callback(param, "0", 1) != 1 )
                            return -1;
                        ++written;

                        if ( specifier == 'x' ) {
                            if ( callback(param, "x", 1) != 1 )
                                return -1;
                        } else {
                            if ( callback(param, "X", 1) != 1 )
                                return -1;
                        }
                        ++written;
                    } else if ( flag_force_0x_or_dec && specifier == 'o' ) {
                        if ( callback(param, "0", 1) != 1 )
                            return -1;
                        ++written;
                    }

                    /* write number */
                    if ( callback(param, number_buffer, len) != len )
                        return -1;
                    written += len;

                    /* right padding */
                    if ( flag_left_justify ) {
                        for ( int i = 0; i < width - len - additional_len; ++i ) {
                            if ( callback(param, " ", 1) != 1 )
                                return -1;
                            ++written;
                        }
                    }

                    break;
                }
                case 'f':
                case 'F':
                case 'e':
                case 'E':
                case 'g':
                case 'G':
                case 'a':
                case 'A': {
                    long double value;
                    switch ( length ) {
                        case LENGTH_DEFAULT:
                            value = va_arg(arg_list, double);
                            break;
                        case LENGTH_L:
                            value = va_arg(arg_list, long double);
                            break;
                        default:
                            errno = EINVAL;
                            return -1;
                    }

                    /* TODO this implementation only does simple printing, printing */
                    /* floating point numbers is much more complex, but okay for now */

                    /* TODO scientific and hexadecimal numbers are converted as normal */
                    /* decimals here. long doubles won't work properly as well. */

                    /* store negativity and make value positive */
                    bool is_negative = value < 0;
                    if ( is_negative )
                        value = -value;

                    /* get before comma stuff */
                    uintmax_t value_integer = (uintmax_t)value;

                    /* get after comma stuff */
                    double value_fractional_d = value - ((intmax_t)value);
                    while ( value_fractional_d - ((intmax_t)value_fractional_d) != 0 )
                        value_fractional_d *= 10;

                    uintmax_t value_fractional = (uintmax_t)value_fractional_d;

                    /* write number in temporary buffer */
                    usize  len_int   = integer_to_string(number_buffer, value_integer, 10, "0123456789");
                    size_t len_fract = integer_to_string(precision_buffer, value_fractional, 10, "0123456789");
                    if ( len_fract > precision )
                        len_fract = precision;

                    /* check for additional */
                    int additional_len = 0;
                    if ( flag_always_prepend_sign || flag_always_prepend_space_plus_sign || is_negative ) {
                        ++additional_len; /* +/-/space */
                    }

                    if ( flag_force_0x_or_dec ) {
                        ++additional_len; /* decimal dot */
                    } else {
                        /* check if the characters that will be printed after the decimal dot
                         * will be other than zero, otherwise it is neglected */
                        for ( int i = 0; i < len_fract; ++i ) {
                            if ( precision_buffer[i] != '0' ) {
                                ++additional_len; /* we'll need a decimal dot */
                                break;
                            }
                        }
                    }

                    /* adjust width to pad correctly */
                    if ( width < len_int + additional_len + precision ) {
                        width = len_int + additional_len + precision;
                    }

                    /* left padding with spaces */
                    if ( !flag_left_justify && !flag_left_pad_zeroes ) {
                        for ( int i = 0; i < width - len_int - additional_len - precision; ++i ) {
                            if ( callback(param, " ", 1) != 1 )
                                return -1;
                            ++written;
                        }
                    }

                    /* sign */
                    if ( is_negative ) {
                        if ( callback(param, "-", 1) != 1 )
                            return -1;
                        ++written;
                    } else if ( flag_always_prepend_sign ) {
                        if ( callback(param, "+", 1) != 1 )
                            return -1;
                        ++written;
                    } else if ( flag_always_prepend_space_plus_sign ) {
                        if ( callback(param, " ", 1) != 1 )
                            return -1;
                        ++written;
                    }

                    /* left padding with zeroes */
                    if ( !flag_left_justify && !flag_left_pad_zeroes ) {
                        for ( int i = 0; i < width - len_int - additional_len - precision; ++i ) {
                            if ( callback(param, "0", 1) != 1 )
                                return -1;
                            ++written;
                        }
                    }

                    /* write integer part */
                    if ( callback(param, number_buffer, len_int) != len_int )
                        return -1;
                    written += len_int;

                    if ( flag_force_0x_or_dec || value_fractional != 0 ) {
                        if ( callback(param, ".", 1) != 1 )
                            return -1;
                        ++written;

                        /* write fractional part */
                        if ( callback(param, precision_buffer, len_fract) != len_fract )
                            return -1;
                        written += len_fract;

                        /* write precision filling zeroes */
                        for ( size_t i = 0; i < (len_fract == 0 ? 1 : precision - len_fract); ++i ) {
                            if ( callback(param, "0", 1) != 1 )
                                return -1;
                            ++written;
                        }
                    }

                    /* right padding */
                    if ( flag_left_justify ) {
                        for ( int i = 0; i < width - len_int - additional_len - precision; ++i ) {
                            if ( callback(param, " ", 1) != 1 )
                                return -1;
                            ++written;
                        }
                    }

                    break;
                }
                case 'c': {
                    char value;
                    switch ( length ) {
                        case LENGTH_DEFAULT:
                            value = va_arg(arg_list, int);
                            break;
                        case LENGTH_l:
                            value = va_arg(arg_list, wint_t);
                            break;
                        default:
                            errno = EINVAL;
                            return -1;
                    }

                    /* left padding */
                    if ( !flag_left_justify ) {
                        for ( int i = 0; i < width - 1; ++i ) {
                            if ( callback(param, " ", 1) != 1 )
                                return -1;
                            ++written;
                        }
                    }

                    /* print character TODO wchar_t? */
                    if ( callback(param, &value, 1) != 1 )
                        return -1;
                    ++written;

                    /* right padding */
                    if ( flag_left_justify ) {
                        for ( int i = 0; i < width - 1; ++i ) {
                            if ( callback(param, " ", 1) != 1 )
                                return -1;
                            ++written;
                        }
                    }
                    break;
                }
                case 's': {
                    char* value;
                    switch ( length ) {
                        case LENGTH_DEFAULT:
                            value = (char*)va_arg(arg_list, char*);
                            break;
                        case LENGTH_l:
                            value = (char*)va_arg(arg_list, wchar_t*);
                            break;
                        default:
                            errno = EINVAL;
                            return -1;
                    }

                    /* get length */
                    auto len = strlen(value);
                    if ( width < len )
                        width = len;

                    /* left padding */
                    if ( !flag_left_justify ) {
                        for ( int i = 0; i < width - len; ++i ) {
                            if ( callback(param, " ", 1) != 1 )
                                return -1;
                            ++written;
                        }
                    }

                    /* limit the output if a precision was explicitly set */
                    if ( explicit_precision && len > precision )
                        len = precision;

                    /* write string TODO wchar_t? */
                    if ( callback(param, value, len) != len )
                        return -1;
                    written += len;

                    /* expand output if a precision was explicitly set */
                    if ( explicit_precision && len < precision ) {
                        for ( int i = 0; i < precision - len; ++i ) {
                            if ( callback(param, " ", 1) != 1 )
                                return -1;
                            ++written;
                        }
                    }

                    /* right padding */
                    if ( flag_left_justify ) {
                        for ( int i = 0; i < width - len; ++i ) {
                            if ( callback(param, " ", 1) != 1 )
                                return -1;
                            ++written;
                        }
                    }
                    break;
                }
                case 'n': {
                    auto value = reinterpret_cast<signed int*>(va_arg(arg_list, void*));
                    *value     = written;
                    break;
                }
                default: {
                    errno = EINVAL;
                    return -1;
                }
            }
        } else {
            if ( callback(param, s, 1) != 1 )
                return -1;
            ++written;
        }

        CHOMP_CHAR();
    }

    return static_cast<int>(written);
}
}