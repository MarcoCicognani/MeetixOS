/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *  Ghost, a micro-kernel based operating system for the x86 architecture    *
 *  Copyright (C) 2015, Max Schlüssel <lokoxe@gmail.com>                     *
 *                                                                           *
 *  This program is free software: you can redistribute it and/or modify     *
 *  it under the terms of the GNU General Public License as published by     *
 *  the Free Software Foundation, either version 3 of the License, or        *
 *  (at your option) any later version.                                      *
 *                                                                           *
 *  This program is distributed in the hope that it will be useful,          *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
 *  GNU General Public License for more details.                             *
 *                                                                           *
 *  You should have received a copy of the GNU General Public License        *
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.    *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "assert.h"
#include "ctype.h"
#include "stddef.h"
#include "string.h"
#include "stdbool.h"

/**
 *
 */
long strtol(const char *str, char **endptr, int base)
{
	char c;
	long i = 0;
	uint8_t neg = false;
	assert(str);
	assert(!base || (base >= 2 && base <= 36));

	// skip leading whitespace
	while(isspace(*str))
		str++;

	// optional '-' or '+'
	if (*str == '-')
	{
		neg = true;
		str++;
	}

    else if (*str == '+') str++;

	// determine base
	if ((base == 16 || base == 0) && str[0] == '0' && str[1] == 'x')
	{
		str += 2;
		base = 16;
	}

    else if (base == 0 && str[0] == '0')
	{
		str++;
		base = 8;
	}

    if (!base) base = 10;

	// read number
	while ((c = *str))
	{
		char lc = tolower(c);

		// check if its a digit that belongs to the number in specified base
		if (base <= 10 && (!isdigit(c) || c - '0' >= base)) break;
		if (base > 10 && (!isalnum(c) || (!isdigit(c) && (lc - 'a') >= base - 10))) break;
		if (lc >= 'a') i = i * base + (lc - 'a') + 10;
		else i = i * base + c - '0';

        str++;
	}

	// switch sign?
	if (neg) i = -i;

    // store end
	if (endptr) *endptr = (char*)str;

    return i;
}
