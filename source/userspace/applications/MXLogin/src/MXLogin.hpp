/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * *
* MeetiX OS By MeetiX OS Project [Marco Cicognani]                                    *
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

#ifndef _LOGIN_SCREEN_
#define _LOGIN_SCREEN_

#include <string>

/*
 * mode for login
 */
enum LoginMode_t
{
	LOGIN,
	LOCK,
};

/*
 * last textfield selected
 */
enum LastFocusedTextField_t
{
	USERNAME,
	PASSWORD,
};

/*
 *	this method is called when enter or button login is pressed
 */
static void loginToMeetiX();

/*
 * compare the provided username and password with username and passwords on file in /cfg/passwd/crd
 */
static bool researchAccess(std::string username, std::string password, LoginMode_t mode);

#endif
