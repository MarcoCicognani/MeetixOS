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

#ifndef _LOGIN_HPP_
#define _LOGIN_HPP_

#include "screen.hpp"
#include <string>

#define logoPath "/sys/graphics/CandyShell/logologin.oem-us"

using namespace std;

class Login
{
public:
	/**
	 *  constructor
	 */
	Login();

	/**
	 *  main of login
	 */
	static void LoginEntry();

	/**
	 *  login loop
	 */
	void loginLoop();

	/**
 	 *  lock screen
 	 */
	void lockLogin();

private:

	/**
	 *  clear screen, write logo
	 */
	void createAmbient();

	/**
	 *	write logo
	 */
	void loadLogo(const char *path);

	/**
	 *	control credential from login
	 */
	bool researchCredential(const string &username, const string &password);

	/**
	 *	create loggin file
	 */
	void logged(const string &loggedUser);

	// screen object
	Screen *out;

	string rootUser;
	string rootPassword;
	string user;
	string passwd;
};

#endif
