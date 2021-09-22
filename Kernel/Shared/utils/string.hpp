/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * *
 * MeetiX OS By MeetiX OS Project [Marco Cicognani]                                    *
 *                                                                                     *
 *         DERIVED FROM THE GHOST OPERATING SYSTEM                                     *
 *         This software is derived from the Ghost operating system project,           *
 *         written by Max Schlüssel <lokoxe@gmail.com>. Copyright 2012-2017            *
 *         https://ghostkernel.org/                                                    *
 *         https://github.com/maxdev1/ghost                                            *
 *                                                                                     *
 * This program is free software; you can redistribute it and/or                       *
 * modify it under the terms of the GNU General Public License                         *
 * as published by the Free Software Foundation; either version 2                      *
 * of the License, or (char *argumentat your option) any later version.                *
 *                                                                                     *
 * This program is distributed in the hope that it will be useful,                     *
 * but WITHout ANY WARRANTY; without even the implied warranty of                      *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                       *
 * GNU General Public License for more details.                                        *
 *                                                                                     *
 * You should have received a copy of the GNU General Public License                   *
 * along with this program; if not, write to the Free Software                         *
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA      *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * */

#ifndef EVA_SHARED_UTILS_STRING
#define EVA_SHARED_UTILS_STRING

#include "Api/StdInt.h"

#include <stddef.h>

/**
 * macros for conversion base
 */
#define BASE_BIN     2
#define BASE_OCTA    8
#define BASE_DECIMAL 10
#define BASE_EXA     16

/**
 * macro for unvalid index
 */
#define ND_INV -1

/**
 * static class to manage strings
 */
class String {
public:
    /**
     * concats two strings and copy the result on out
     *
     * @param a:	firt part of out string
     * @param b:	second part of out string
     */
    static void concat(const char* a, const char* b, char* out);

    /**
     * copy source into target
     *
     * @param target:	string were copy store source
     */
    static void copy(char* target, const char* source);

    /**
     * return the length of the str
     *
     * @param str:		the string that we would know the length
     * @return the string length value
     */
    static uint32_t length(const char* str);

    /**
     * return the index of c into str starting from begin
     *
     * @param str:		the string where we search
     * @param c:		the character to search
     * @return the index of the character if exist, -1 otherwise
     */
    static int32_t indexOf(const char* str, char c);

    /**
     * return the last index of c into str starting from end
     *
     * @param str:		the string where we search
     * @param c:		the character to search
     * @return the index of the character if exist, -1 otherwise
     */
    static int32_t lastIndexOf(const char* str, char c);

    /**
     * return if the provided strings are equals
     *
     * @param stra:		first string to compare
     * @param strb:		second string to compare
     * @return true if strings are equals, false otherwise
     */
    static bool equals(const char* stra, const char* strb);

    /**
     * Replaces all occurences of char a with char b in the given cstring
     *
     * @param str:				string where we replace
     * @param character:		character to replace
     * @param replacement:		character to substitute to character
     */
    static void replace(char* str, char character, char replacement);

    /**
     * return the filename from a pathname
     *
     * @param pathname:		string with path
     * @return return the string basename
     */
    static char* basename(char* pathname);

    /**
     * return the string on the index of c
     *
     * @param str:		the string where we search
     * @param c:		character to search
     * @return the string pointed to the provided character to search
     */
    static char* getOccurenceOf(const char* str, char c);

    /**
     * reverse the content of the provided string for length bytes
     *
     * @param str:		the string to reverse
     * @param length:	length of string to reverse
     */
    static void reverse(char* str, uint32_t length);

    /**
     * convert the provided string into a integer value
     *
     * @param source:		string that contains the integer expression
     * @return the source converted to integer
     */
    static int32_t toInt(const char* source);

    /**
     * convert the provided integer into string
     *
     * @param source:	integer variable to convert into string
     * @param base:		base to convert source
     *
     * @return duplicate of vector where the function store converted source
     */
    static const char* fromInt(int32_t source, uint32_t base);

    /**
     * duplicate the provided string (available only in kernel context)
     *
     * @param source:	string to duplicate
     * @return the duplicated string
     */
    static char* duplicate(const char* source);
};

#endif
