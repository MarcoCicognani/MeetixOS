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

#ifndef MEETIX_LIBRARY_UI_MESSAGEBOX
#define MEETIX_LIBRARY_UI_MESSAGEBOX

#include <string>

#include <gui/window.hpp>
#include <gui/label.hpp>
#include <gui/button.hpp>
#include <gui/actionlistener.hpp>

/*
 * pressed button
 */
enum PressedButton
{
	BUTTON_NONE,
	BUTTON_OKAY,
	BUTTON_CANCEL,
};

/*
 * message box class
 */
class MsgBox
{
public:
	/*
	 * basic message on window
	 */
	static void show(std::string msg);
	static void show(std::string msg, std::string title);

	/*
	 * message with return of chosen button
	 */
	static PressedButton showAndGetResponse(std::string msg, std::string title);
};

#endif
