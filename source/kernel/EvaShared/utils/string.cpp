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

#include <memory/memory.hpp>
#include <utils/string.hpp>

/**
 * concats two strings and copy the result on out
 *
 * @param a:	firt part of out string
 * @param b:	second part of out string
 */
void String::concat(const char *a, const char *b, char *out)
{
	// get the lengths of the strings
	int lenA = String::length(a);
	int lenB = String::length(b);

	// copy the strings on out buffer
	Memory::copy(out, a, lenA);
	Memory::copy(&out[lenA], b, lenB);

	// terminate the buffer
	out[lenA + lenB] = '\0';
}

/**
 * copy source into target
 *
 * @param target:	string were copy store source
 */
void String::copy(char *target, const char *source)
{
	uint32_t len = length(source);
	Memory::copy(target, source, len);
	target[len] = '\0';
}

/**
 * return the length of the str
 *
 * @param str:		the string that we would know the length
 * @return the string length value
 */
uint32_t String::length(const char *str)
{
	uint32_t size = 0;
	while (*str++)
		++size;

	return size;
}

/**
 * return the index of c into str starting from begin
 *
 * @param str:		the string where we search
 * @param c:		the character to search
 * @return the index of the character if exist, -1 otherwise
 */
int32_t String::indexOf(const char *str, char c)
{
	int32_t pos = 0;
	while (*str)
	{
		if (*str == c) return pos;

		++pos;
		++str;
	}
	return ND_INV;
}

/**
 * return the last index of c into str starting from end
 *
 * @param str:		the string where we search
 * @param c:		the character to search
 * @return the index of the character if exist, -1 otherwise
 */
int32_t String::lastIndexOf(const char *str, char c)
{
	int32_t pos = String::length(str) - 1;
	while (str[pos])
	{
		if (str[pos] == c) return pos;
		--pos;
	}

	return ND_INV;
}

/**
 * return if the provided strings are equals
 *
 * @param stra:		first string to compare
 * @param strb:		second string to compare
 * @return true if strings are equals, false otherwise
 */
bool String::equals(const char *stra, const char *strb)
{
	// check if is the same strings
	if (stra == strb) return true;

	// get lengths of strings
	int alen = length(stra);
	int blen = length(strb);

	// immediate return if lengths isn't equal
	if (alen != blen) return false;

	// check all characters
	while (alen-- > 0)
		if (stra[alen] != strb[alen]) return false;

	return true;
}

/**
 * Replaces all occurences of char a with char b in the given cstring
 *
 * @param str:				string where we replace
 * @param character:		character to replace
 * @param replacement:		character to substitute to character
 */
void String::replace(char *str, char character, char replacement)
{
	// check character validity
	while (*str)
	{
		// if character correspond replace it
		if (*str == character) *str = replacement;
		++str;
	}
}

/**
 * return the filename from a pathname
 *
 * @param pathname:		string with path
 * @return return the string basename
 */
char *String::basename(char *pathname)
{
	if (!pathname || !pathname[0]) return (char*)".";
	if ((pathname[0] == '.' || pathname[0] == '/') && pathname[1] == '\0') return pathname;
	if (pathname[0] == '.' && pathname[1] == '.' && pathname[2] == '\0') return pathname;

	size_t len = length(pathname);
	while (len > 0 && pathname[len - 1] == '/')
		pathname[--len] = '\0';

	while (len > 0 && pathname[len - 1] != '/')
		len--;

	return pathname + len;
}

/**
 * return the string on the index of c
 *
 * @param str:		the string where we search
 * @param c:		character to search
 * @return the string pointed to the provided character to search
 */
char *String::getOccurenceOf(const char *str, char c)
{
	while (*str)
		if (*str++ == c) return (char*)(str - 1);

	return nullptr;
}

/**
 * reverse the content of the provided string for length
 *
 * @param str:		the string to reverse
 * @param length:	length of string to reverse
 */
void String::reverse(char *str, uint32_t length)
{
	uint32_t start = 0;
    uint32_t end = length -1;
    while (start < end)
    {
		char c = str[start];
		str[start] = str[end];
		str[end] = c;
        start++;
        end--;
    }
}

/**
 * convert the provided string into a integer value
 *
 * @param source:		string that contains the integer expression
 * @return the source converted to integer
 */
int32_t String::toInt(const char *source)
{
	// check source validiy
	if (!*source) return 0;

    int res = 0;  // Initialize result
    int sign = 1;  // Initialize sign as positive
    int i = 0;  // Initialize index of first digit

    // If number is negative, then update sign
    if (source[0] == '-')
    {
        sign = -1;
        i++;  // Also update index of first digit
    }

    // Iterate through all digits of input string and update result
    while (source[i] != '\0')
    {
        if (!(source[0] >= '0' && source[i] <='9')) return 0;
        res = res * 10 + source[i] - '0';
		i++;
    }

    // Return result with sign
    return sign * res;
}

/**
 * convert the provided integer into string
 *
 * @param source:	integer variable to convert into string
 * @param base:		base to convert source
 *
 * @return duplicate of vector where the function store converted source
 */
const char *String::fromInt(int32_t source, uint32_t base)
{
	int i = 0;
	bool isNegative = false;
	char str[1024];

	// Handle 0 explicitely, otherwise empty string is printed for 0
	if (!source)
	{
		str[i++] = '0';
		str[i] = '\0';
		return duplicate(str);
	}

	// negative sourcebers are handled only with base 10. Otherwise numbers are considered unsigned.
	if (source < 0 && base == 10)
	{
		isNegative = true;
		source = -source;
	}

	// Process individual digits
	while (source)
	{
		int rem = source % base;
		str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
		source /= base;
	}

	// If number is negative, append '-'
	if (isNegative)
		str[i++] = '-';

	// Append string terminator
	str[i] = '\0';

	// Reverse the string
	reverse(str, i);

	// return a duplicate
	return duplicate(str);
}
