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

#include <assert.h>
#include <ctype.h>

extern "C" long strtol(const char* str, char** endptr, int base) {
    char c;
    long i           = 0;
    bool is_negative = false;
    assert(str);
    assert(!base || (base >= 2 && base <= 36));

    /* skip leading white spaces */
    while ( str && isspace(*str) )
        ++str;

    /* optional '-' or '+' */
    if ( *str == '-' ) {
        is_negative = true;
        ++str;
    } else if ( *str == '+' ) {
        ++str;
    }

    /* check the base of the number */
    if ( (base == 16 || base == 0) && str[0] == '0' && str[1] == 'x' ) {
        str += 2;
        base = 16;
    } else if ( base == 0 && str[0] == '0' ) {
        ++str;
        base = 8;
    }

    /* default the base to 10 */
    if ( !base )
        base = 10;

    /* parse now the number */
    while ( (c = *str) ) {
        auto low_c = static_cast<char>(tolower(c));

        /* check whether the character belongs to the base dominion */
        if ( base <= 10 && (!isdigit(c) || c - '0' >= base) )
            break;
        if ( base > 10 && (!isalnum(c) || (!isdigit(c) && (low_c - 'a') >= base - 10)) )
            break;

        /* convert the number */
        if ( low_c >= 'a' )
            i = i * base + (low_c - 'a') + 10;
        else
            i = i * base + c - '0';

        /* next character */
        ++str;
    }

    /* switch the sign */
    if ( is_negative )
        i = -i;

    /* store the end pointer */
    if ( endptr )
        *endptr = const_cast<char*>(str);

    return i;
}
