/*********************************************************************************
 * MeetiX OS By MeetiX OS Project [Marco Cicognani]                               *
 * 																			     *
 * This program is free software; you can redistribute it and/or                  *
 * modify it under the terms of the GNU General Public License                    *
 * as published by the Free Software Foundation; either version 2				 *
 * of the License, or (char *argumentat your option) any later version.			 *
 *																				 *
 * This program is distributed in the hope that it will be useful,				 *
 * but WITHout ANY WARRANTY; without even the implied warranty of                 *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 				 *
 * GNU General Public License for more details.
 **
 *																				 *
 * You should have received a copy of the GNU General Public License				 *
 * along with this program; if not, write to the Free Software                    *
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA *
 **********************************************************************************/

#ifndef __MEETIX_LIBC_CTYPE__
#define __MEETIX_LIBC_CTYPE__

#include <eva/common.h>

__BEGIN_C

#define _U (1 << 0) // Upper case
#define _L (1 << 1) // Lower case
#define _N (1 << 2) // Digit
#define _S (1 << 3) // Space
#define _P (1 << 4) // Punctuator
#define _C (1 << 5) // Control character
#define _B (1 << 6) // Blank
#define _X (1 << 7) // Hexadecimal digit

/**
 * check if the given character is ASCII
 *
 * @param c:    character to check
 * @return 1 if <c> is ascii, otherwise 0
 */
int isascii(int c);

/**
 * check if the given character is utf8
 *
 * @param c:    character to check
 * @return 1 if <c> is ascii, otherwise 0
 */
int isutf8(int c);

/**
 * Checks if the given character is alphanumeric. (N1548-7.4.1.1)
 *
 * @param c:    character to check
 * @return 1 if <c> is alphanumeric, otherwise 0
 */
int isalnum(int c);

/**
 * Checks if the given character is alphabetic. (N1548-7.4.1.2)
 *
 * @param c:    character to check
 * @return 1 if <c> is alphabetic, otherwise 0
 */
int isalpha(int c);

/**
 * Checks if the given character is a blank character (space ' ' and
 * horizontal tab '\t'). (N1548-7.4.1.3)
 *
 * @param c:    character to check
 * @return 1 if <c> is blank, otherwise 0
 */
int isblank(int c);

/**
 * Checks if the given character is a control character. (N1548-7.4.1.4)
 *
 * @param c:    character to check
 * @return 1 if <c> is a control character, otherwise 0
 */
int iscntrl(int c);

/**
 * Checks if the given character is a digit. (N1548-7.4.1.5)
 *
 * @param c:    character to check
 * @return 1 if <c> is a digit, otherwise 0
 */
int isdigit(int c);

/**
 * Checks if the given character is any printing character except space. (N1548-7.4.1.6)
 *
 * @param c:    character to check
 * @return 1 if <c> is a graph, otherwise 0
 */
int isgraph(int c);

/**
 * Checks if the given character is a lowercase letter. (N1548-7.4.1.7)
 *
 * @param c:    character to check
 * @return 1 if <c> is lowercase, otherwise 0
 */
int islower(int c);

/**
 * Checks if the given character is any printing character including space. (N1548-7.4.1.8)
 *
 * @param c:    character to check
 * @return 1 if <c> is printable, otherwise 0
 */
int isprint(int c);

/**
 * Checks if the given character is a punctuator. (N1548-7.4.1.9)
 *
 * @param c:    character to check
 * @return 1 if <c> is a punctuator, otherwise 0
 */
int ispunct(int c);

/**
 * Checks if the given character is any standard white-space character (space ' ',
 * form feed '\f', new line '\n', carriage return '\r', horizontal tab '\t' and
 * vertical tab '\v'). (N1548-7.4.1.10)
 *
 * @param c:    character to check
 * @return 1 if <c> is whitespace, otherwise 0
 */
int isspace(int c);

/**
 * Checks if the given character is an uppercase letter. (N1548-7.4.1.11)
 *
 * @param c:    character to check
 * @return 1 if <c> is uppercase, otherwise 0
 */
int isupper(int c);

/**
 * Checks if the given character is one of the hexadecimal-digit characters. (N1548-7.4.1.12)
 *
 * @param c:    character to check
 * @return 1 if <c> is a hexadecimal-digit character, otherwise 0
 */
int isxdigit(int c);

/**
 * Converts an uppercase letter to its corresponding lowercase letter. (N1548-7.4.2.1)
 *
 * @param c:    character to convert
 * @return the corresponding lowercase letter
 */
int tolower(int c);

/**
 * Converts a lowercase letter to its corresponding uppercase letter. (N1548-7.4.2.2)
 *
 * @param c:    character to convert
 * @return the corresponding uppercase letter
 */
int toupper(int c);

__END_C

#endif
