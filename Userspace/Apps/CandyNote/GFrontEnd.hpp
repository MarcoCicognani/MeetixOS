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

#ifndef _Graphic_FrontEnd_H_
#define _Graphic_FrontEnd_H_

#include "FrontEnd.hpp"

#include <list>
#include <fstream>

#include <cairo/cairo.h>
#include <gui/window.hpp>
#include <gui/canvas.hpp>

#include <utils/utils.hpp>
#include <gui/ui.hpp>
#include <gui/label.hpp>
#include <gui/button.hpp>
#include <gui/textfield.hpp>
#include <gui/actionlistener.hpp>
#include <gui/focuslistener.hpp>
#include <gui/keylistener.hpp>
#include <gui/msgbox.hpp>
#include <tasking/lock.hpp>
#include <graphics/text/fontldr.hpp>
#include <graphics/text/font.hpp>
#include <graphics/text/textlyot.hpp>

using namespace std;

class GraphicFrontEnd_t : public FrontEnd
{
public:
	// constructor
	GraphicFrontEnd_t();

	virtual void clean();

	virtual void end();

	virtual void write(string message, string title);

	virtual Keyboard::Info readInput();

private:
	void initialize();
	void paintEntry();
	void paintLoop();
};

#endif
