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

#ifndef __MEETIX_LIBC_STRING__
#define __MEETIX_LIBC_STRING__

#include <eva/common.h>
#include <stddef.h>

__BEGIN_C

/**
 * Copies <num> bytes from <src> to <dest>.
 *
 * @param dest:     destination memory location
 * @param src:      source memory location
 * @param num:      number of bytes to copy
 * @return <dest> is returned
 */
void* memcpy(void* dest, const void* src, size_t num);

/**
 * Copies <num> bytes from <src> to <dest>, allowing the
 * memory areas of source and destination to overlap.
 *
 * @param dest:     destination memory location
 * @param src:      source memory location
 * @param num:      number of bytes to copy
 * @return <dest> is returned
 */
void* memmove(void* dest, const void* src, size_t num);

/**
 * set <len> bytes of <mem> with value on <value>
 *
 * @param mem:      memory location destination
 * @param value:    value to set on each location field
 * @param len:      number of bytes to set
 * @return <mem> is returned
 */
void* memset(void* mem, int value, size_t len);

/**
 * compare the two memory pointer provided for <len> bytes
 *
 * @param mem_a:    the first memory location to compare
 * @param mem_b:    the second memory location to compare
 * @param len:      bytes count to compare
 * @return 0 if two areas are equals
 */
int memcmp(const void* mem_a, const void* mem_b, size_t len);

/**
 * Searches within the <num> bytes of <mem> for the first occurrence of <value>
 * and return it
 *
 * @param mem:      the memory location where search
 * @param value:    the value to search
 * @param num:      length of mem
 * @return the first occurrence of <value> on <mem>, NULL otherwise
 */
void* memchr(const void* mem, int value, size_t num);

/**
 * copy <src> string into <dest> string
 *
 * @param dest:     a valid char buffer
 * @param src:      the string to copy to
 * @return <dest>
 */
char* strcpy(char* dest, const char* src);

/**
 * copy <src> string into <dest> string for <max> bytes
 *
 * @param dest:     a valid char buffer
 * @param src:      the string to copy to
 * @param max:      bytes count to copy
 * @return <dest>
 */
char* strncpy(char* dest, const char* src, size_t max);

/**
 * append <src> to <dest> and return it
 *
 * @param dest:     the string where function append
 * @param src:      the string to append
 * @return <dest>
 */
char* strcat(char* dest, const char* src);

/**
 * append <src> to <dest> for <max> bytes and return it
 *
 * @param dest:     the string where function append
 * @param src:      the string to append
 * @param max:      length of src
 * @return <dest>
 */
char* strncat(char* dest, const char* src, size_t max);

/**
 * compare <str_a> with <str_b> and return a value
 *
 * @param str_a:    the first string to compare
 * @param str_b:    the second string to compare
 * @return 0 if <str_a> and <str_b> are equals
 */
int strcmp(const char* str_a, const char* str_b);

/**
 * compare <str_a> with <str_b> for <max> bytes and return a value
 *
 * @param str_a:    the first string to compare
 * @param str_b:    the second string to compare
 * @param max:      the number of bytes to compare
 * @return 0 if <str_a> and <str_b> are equals
 */
int strncmp(const char* str_a, const char* str_b, size_t max);

/**
 * compare <str_a> with <str_b> with current locale
 *
 * @param str_a:    the first string to compare
 * @param str_b:    the second string to compare
 * @return 0 if <str_a> and <str_b> are equals
 */
int strcoll(const char* str_a, const char* str_b);

/**
 * transform <src> string for <num> bytes in current locale and copy it on <dest>
 *
 * @param dest:     the destination buffer
 * @param src:      the string to transform
 * @param num:      the number of bytes to be copied into dest
 * @return the length of the transformed string,
 */
size_t strxfrm(char* dest, const char* src, size_t num);

/**
 * return the first occurrence of <value> into <str>
 *
 * @param str:      the string where search
 * @param value:    the value to search
 * @return the str at the occurrence requested
 */
char* strchr(const char* str, int value);

/**
 * return the last occurrence of <value> into <str> at
 *
 * @param str:      the string where search
 * @param value:    the value to search
 * @return the str at the occurrence requested
 */
char* strrchr(const char* str, int value);

/**
 * return the number of character read on <str_a> before the first
 * occurrence of <str_b>
 *
 * @param str_a:    the first string
 * @param str_b:    the second string
 * @return the number of bytes readed on <str_a>
 */
size_t strcspn(const char* str_a, const char* str_b);

/**
 * returns the length of the initial portion of <str_a>
 * which consists only of characters that are part of <str_b>
 *
 * @param str_a:    the first string
 * @param str_b:    the second string
 * @return the length of the initial portion of <str_a>
 */
size_t strspn(const char* str_a, const char* str_b);

/**
 * returns a pointer to the first occurrence in <str_a> of any
 * of the characters that are part of <str_b>,
 * or a null pointer if there are no matches
 *
 * @param str_a:    the first string
 * @param str_b:    the second string
 * @return the occurrence on <str_a>
 */
char* strpbrk(const char* str_a, const char* str_b);

/**
 * returns a pointer to the first occurrence of str2 in str1,
 * or a null pointer if str2 is not part of str1.
 *
 * @param str_a:    the first string
 * @param str_b:    the second string
 * @return the occurrence on <str_a>
 */
char* strstr(const char* str_a, const char* str_b);

/**
 * truncate <dest> string with characters contained into <src>
 * and return the occourence at the first truncated character
 *
 * @param dest:     the string to truncate
 * @param src:      the string with the characters to be delimit the truncate
 * @return the <dest> occurrence
 */
char* strtok(char* dest, const char* src);

/**
 * return the length of the <str>
 *
 * @param str:      the string to measure
 * @return the length of the string
 */
size_t strlen(const char* str);

/**
 * duplicate <s> creating a malloc allocated copy
 *
 * @param s:    the string to duplicate
 * @return the duplicate string
 */
char* strdup(const char* s);

/**
 * return the human readable string from errno value
 *
 * @param errno:    errno value
 * @return the error as cstring
 */
char* strerror(int errno);

__END_C

#endif
