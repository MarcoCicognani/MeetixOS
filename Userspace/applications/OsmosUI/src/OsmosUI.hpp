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

#ifndef _OsmosUI_
#define _OsmosUI_

#include <gui/ui.hpp>
#include <gui/about.hpp>
#include <gui/msgbox.hpp>
#include <gui/window.hpp>
#include <gui/label.hpp>
#include <gui/geoshape.hpp>
#include <gui/button.hpp>
#include <gui/textfield.hpp>
#include <gui/actionlistener.hpp>
#include <gui/btnlist.hpp>
#include <gui/keylistener.hpp>
#include <graphics/metrics/dimension.hpp>
#include <tasking/lock.hpp>
#include <math.h>
#include <map>

/*
 *
 */
class OsmosUI
{
public:
	OsmosUI() {}
	~OsmosUI();

	// initialize UI
	bool init();

	// configure ui with provided script
	void configureUi(std::string pathToConfiguration, Dimension resolution);

	// create the components of ui
	void createComponents();

	// main loop of ui
	void mainLoop();

private:
	enum UiStyle_t
	{
		GNOME,
		KDE,
	};

	// setup MeetiX menu button
	void setMenuButton();

	// setup menu tab
	void setMenuTab();

	// setup memory usage label
	void setMemLabel();

	// setup hour label
	void setHourLabel();

	// set task label
	void setTaskLabel();

	// create app button
	void createMenu();

	// read from keyboard
	Keyboard::Info readInput();

	// resolution of screen
	Dimension resolution;

	// map that contains configuration provided by .ui.cfg file
	std::map<std::string, std::string> configuration;
	UiStyle_t mode;
};

// prototype of method for MeetiX OS Logout
void meetiXOSLogout();

#endif
