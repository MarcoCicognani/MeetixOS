/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * *
* MeetiX OS By MeetiX OS Project [Marco Cicognani & D. Morandi]                       *
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

#ifdef __cplusplus

#ifndef EVA_SHARED_UTILS_LOCAL
#define EVA_SHARED_UTILS_LOCAL

#include "eva/common.h"

/**
 * Class that smart manage a malloched / newed
 * pointer of an object and delete it when it gone out of scope
 */
template<typename T> class Local
{
public:

	/**
	 * filled constructor
	 *
	 * @param value:	the pointer to the object/buffer to contains
	 */
	Local(T *value) : value(value) {}

	/**
	 * default constructor
	 */
	Local() : value(0) {}

	/**
	 * destructor
	 */
	~Local() {
		if (value) delete value;
	}

	/**
	 * invalidate copy constructor
	 */
	Local(const Local &rhs) = delete;

	/**
	 * invalidate copy operator
	 */
	Local &operator=(const Local &rhs) = delete;

	/**
	 * overload the () operator
	 */
	T *operator()() const { return value; }

	/**
	 * release the previourly stored buffer
	 */
	T *release()
	{
		T *releasedValue = value;
		value = 0;
		return releasedValue;
	}

private:
	T *value;
};

#endif

#endif
