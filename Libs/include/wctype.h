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
* GNU General Public License for more details.									 *
*																				 *
* You should have received a copy of the GNU General Public License				 *
* along with this program; if not, write to the Free Software                    *
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA *
**********************************************************************************/

#ifndef __MEETIX_LIBC_WCTYPE__
#define __MEETIX_LIBC_WCTYPE__

#include "eva/common.h"
#include <stddef.h>

__BEGIN_C

/**
 * Represents a wide character transformation. (N1548-7.29.1-2)
 */
typedef const void* wctrans_t;
/**
 * Represents a class of wide characters. (N1548-7.29.1-2)
 */
typedef const void* wctype_t;

// (N1548-7.29.2.1.2)
int iswalpha(wint_t wc);

// (N1548-7.29.2.1.3)
int iswblank(wint_t wc);

// (N1548-7.29.2.1.4)
int iswcntrl(wint_t wc);

// (N1548-7.29.2.1.5)
int iswdigit(wint_t wc);

// (N1548-7.29.2.1.6)
int iswgraph(wint_t wc);

// (N1548-7.29.2.1.7)
int iswlower(wint_t wc);

// (N1548-7.29.2.1.8)
int iswprint(wint_t wc);

// (N1548-7.29.2.1.9)
int iswpunct(wint_t wc);

// (N1548-7.29.2.1.10)
int iswspace(wint_t wc);

// (N1548-7.29.2.1.11)
int iswupper(wint_t wc);

// (N1548-7.29.2.1.12)
int iswxdigit(wint_t wc);

// (N1548-7.29.2.2.1)
int iswctype(wint_t wc, wctype_t desc);

// (N1548-7.29.2.2.2)
wctype_t wctype(const char *property);

// (N1548-7.29.3.1.1)
wint_t towlower(wint_t wc);

// (N1548-7.29.3.1.2)
wint_t towupper(wint_t wc);

// (N1548-7.29.3.2.1)
wint_t towctrans(wint_t wc, wctrans_t desc);

// (N1548-7.29.3.2.2)
wctrans_t wctrans(const char *property);

__END_C

#endif
