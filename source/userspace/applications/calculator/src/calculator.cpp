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


#include "calculator.hpp"
#include <gui/ui.hpp>
#include <gui/btnlist.hpp>
#include <gui/window.hpp>
#include <gui/label.hpp>
#include <gui/button.hpp>
#include <gui/textfield.hpp>
#include <gui/geoshape.hpp>
#include <gui/actionlistener.hpp>

#include <algorithm>
#include <sstream>
#include <fstream>

#include <stdlib.h>

static Window *window;
static Textfield *display;
static ButtonList keyBoard;
static uint8_t blocker = true;

int totalValue = 0;
int currentValue = 0;
int previousCommand = COM_NONE;

int grid1 = 10;
int grid2 = 50;
int grid3 = 90;
int grid4 = 130;
int grid5 = 170;
int dispOff = 40;

/**
 *
 */
class NumPressActionListener : public ActionListener
{
public:
	//
	int number;

	//
	NumPressActionListener(int num) { this->number = num; }

	//
	virtual void handleAction() { padButtonPressed(number); }
};

/**
 *
 */
class CommandPressActionListener : public ActionListener
{
public:
	//
	int command;

	//
	CommandPressActionListener(int com) { this->command = com; }

	//
	virtual void handleAction() { commandPressed(command); }
};

/**
 *
 */
int main(int argc, char *argv[])
{
	UiOpenStatus openStat = UI::open();

	if (openStat == UI_OPEN_STATUS_SUCCESSFUL)
	{
		klog("PATH=%s", getenv("SYSTEM_LEVEL"));

		// configure window
		window = Window::create();
		window->setBounds(Rectangle(70, 70, 195, 300));
		window->setColor(RGB(0, 180, 0), ARGB(255, 255, 255, 255));
		window->setTitle("Calculator");
		window->setResizable(false);
		window->onClose([]{ blocker = false; });

		// configure display
		display = Textfield::create();
		display->setTitle("0");
		display->setBounds(Rectangle(10, 10, 150, 30));
		display->setColor(ARGB(120, 0, 0, 0), RGB(255, 255, 255));
		window->addChild(display);

		// add to list buttons
		keyBoard.add("0", new NumPressActionListener(0));
		keyBoard.add("1", new NumPressActionListener(1));
		keyBoard.add("2", new NumPressActionListener(2));
		keyBoard.add("3", new NumPressActionListener(3));
		keyBoard.add("4", new NumPressActionListener(4));
		keyBoard.add("5", new NumPressActionListener(5));
		keyBoard.add("6", new NumPressActionListener(6));
		keyBoard.add("7", new NumPressActionListener(7));
		keyBoard.add("8", new NumPressActionListener(8));
		keyBoard.add("9", new NumPressActionListener(9));
		keyBoard.add("C", new CommandPressActionListener(COM_CLEAR));
		keyBoard.add("+", new CommandPressActionListener(COM_PLUS));
		keyBoard.add("-", new CommandPressActionListener(COM_MINUS));
		keyBoard.add("*", new CommandPressActionListener(COM_MULT));
		keyBoard.add("/", new CommandPressActionListener(COM_DIV));
		keyBoard.add("=", new CommandPressActionListener(COM_EQ));
		keyBoard.show(window);

		// configure number buttons
		keyBoard.configure("0", Rectangle(grid2, grid5 + dispOff, 30, 30), "0", RGB(0, 200, 0), RGB(0, 0, 0));
		keyBoard.configure("1", Rectangle(grid1, grid2 + dispOff, 30, 30), "1", RGB(0, 200, 0), RGB(0, 0, 0));
		keyBoard.configure("2", Rectangle(grid2, grid2 + dispOff, 30, 30), "2", RGB(0, 200, 0), RGB(0, 0, 0));
		keyBoard.configure("3", Rectangle(grid3, grid2 + dispOff, 30, 30), "3", RGB(0, 200, 0), RGB(0, 0, 0));
		keyBoard.configure("4", Rectangle(grid1, grid3 + dispOff, 30, 30), "4", RGB(0, 200, 0), RGB(0, 0, 0));
		keyBoard.configure("5", Rectangle(grid2, grid3 + dispOff, 30, 30), "5", RGB(0, 200, 0), RGB(0, 0, 0));
		keyBoard.configure("6", Rectangle(grid3, grid3 + dispOff, 30, 30), "6", RGB(0, 200, 0), RGB(0, 0, 0));
		keyBoard.configure("7", Rectangle(grid1, grid4 + dispOff, 30, 30), "7", RGB(0, 200, 0), RGB(0, 0, 0));
		keyBoard.configure("8", Rectangle(grid2, grid4 + dispOff, 30, 30), "8", RGB(0, 200, 0), RGB(0, 0, 0));
		keyBoard.configure("9", Rectangle(grid3, grid4 + dispOff, 30, 30), "9", RGB(0, 200, 0), RGB(0, 0, 0));

		// configure control buttons
		keyBoard.configure("C", Rectangle(grid1, grid1 + dispOff, 30, 30), "C", RGB(200, 0, 0), RGB(0, 0, 0));
		keyBoard.configure("+", Rectangle(grid4, grid1 + dispOff, 30, 30), "+", RGB(200, 0, 0), RGB(0, 0, 0));
		keyBoard.configure("-", Rectangle(grid4, grid2 + dispOff, 30, 30), "-", RGB(200, 0, 0), RGB(0, 0, 0));
		keyBoard.configure("*", Rectangle(grid4, grid3 + dispOff, 30, 30), "*", RGB(200, 0, 0), RGB(0, 0, 0));
		keyBoard.configure("/", Rectangle(grid4, grid4 + dispOff, 30, 30), "/", RGB(200, 0, 0), RGB(0, 0, 0));
		keyBoard.configure("=", Rectangle(grid4, grid5 + dispOff, 30, 30), "=", RGB(200, 0, 0), RGB(0, 0, 0));

		// show the window application
		window->setVisible(true);

		// lock application, if unlock the flag ui closing and terminating
		AtomicBlock(&blocker);

		delete display;
		delete window;
		UI::close();
	}
}

/**
 *
 */
void padButtonPressed(int num)
{
	keyBoard["*"]->setEnabled(true);
	keyBoard["-"]->setEnabled(true);
	keyBoard["*"]->setEnabled(true);
	keyBoard["/"]->setEnabled(true);

	currentValue = currentValue * 10 + num;

	std::stringstream str;
	str << currentValue;
	display->setTitle(str.str());

	if (previousCommand == COM_EQ || previousCommand == COM_CLEAR || previousCommand == COM_NONE) totalValue = currentValue;
}

/**
 *
 */
void commandPressed(int command)
{
	keyBoard["*"]->setEnabled(true);
	keyBoard["-"]->setEnabled(true);
	keyBoard["*"]->setEnabled(true);
	keyBoard["/"]->setEnabled(true);

	if (previousCommand == COM_PLUS) totalValue = totalValue + currentValue;
	else if (previousCommand == COM_MINUS) totalValue = totalValue - currentValue;
	else if (previousCommand == COM_MULT) totalValue = totalValue * currentValue;
	else if (previousCommand == COM_DIV) totalValue = totalValue / currentValue;
	else if (previousCommand != COM_EQ && previousCommand != COM_CLEAR) totalValue = currentValue;


	if (command == COM_PLUS) keyBoard["*"]->setEnabled(false);
	else if (command == COM_MINUS) keyBoard["-"]->setEnabled(false);
	else if (command == COM_MULT) keyBoard["*"]->setEnabled(false);
	else if (command == COM_DIV) keyBoard["/"]->setEnabled(false);
	else if (command == COM_EQ)
	{
		std::stringstream ss;
		ss << totalValue;
		display->setTitle(ss.str());
	}

	else if (command == COM_CLEAR)
	{
		if (currentValue == 0) totalValue = 0;
		display->setTitle("0");
	}

	currentValue = 0;
	previousCommand = command;
}
