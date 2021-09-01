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

#ifndef __MEETIX_LIBC_ASSERT__
#define __MEETIX_LIBC_ASSERT__

#include "eva.h"

__BEGIN_C

// use pretty function from g++
#if defined(__cplusplus) && defined(__GNUC__)
#define __mxassert_function		__PRETTY_FUNCTION__

// use __func__ in C99
#elif __STDC_VERSION__ >= 199901L
#define __mxassert_function		__func__

// otherwise unsupported
#else
#define __mxassert_function		((char *) 0)
#endif


// (N1548-7.2-1) redefining
#undef assert

// if the macro NDEBUG is defined, assert must result in nothing (N1548-7.2-1)
#if defined(NDEBUG)
#define assert(ignore)		((void) 0)
#endif

// macro declaration of assert (N1548-7.2-2)
#if !defined(NDEBUG)
#define assert(expr)		((expr) ? (void) 0 : mxassert(__FILE__, __LINE__, __mxassert_function, #expr))
#endif

#if __HAS_STDC11
// remove existing declaration
#undef static_assert
// macro declaration of static_assert (N1548-7.2-3)
#define static_assert		_Static_assert
#endif


// assert function
void mxassert(const char *file, int line, const char *function, const char *expr);


__END_C

#endif
