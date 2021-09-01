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

#include <gui/msgbox.hpp>
#include <gui/ui.hpp>
#include <gui/btnlist.hpp>

/*
 * screen components
 */
static Window *box;
static Label *message;

/*
 * library objects
 */
Dimension resolution;
ButtonList *btnControls;

// response for show with response
PressedButton response = BUTTON_NONE;
uint8_t waiter = true;

/*
 * convert string length in window width
 */
static inline int getWidthFromStringLength(std::string s)
{
	return s.length() * 10;
}

/*
 * garbage collector, called when show finish
 */
void garbageDelete()
{
	// set unvisible the box
	box->setVisible(false);

	// delete the components
	delete btnControls;
	delete message;
	delete box;
}

/*
 * setup components
 */
void setupComponents(std::string msg, bool complete)
{
	// get screen resolution and window width
	resolution = UI::getResolution();
	int boxWidth = getWidthFromStringLength(msg);

	// instantiate and create window
	box = Window::create();
	box->onClose(garbageDelete);
	box->setBounds(Rectangle((resolution.width / 2) - (boxWidth / 2), resolution.height / 2 - 100, boxWidth, 200));
	box->setResizable(false);

	// instantiate and add label
	message = Label::create();
	message->setBounds(Rectangle(10, 20, boxWidth, 30));

	// create okay button
	btnControls = new ButtonList();

	// if create all components instantiate also buttons
	if (complete)
	{
		btnControls->add("okay", []{ response = BUTTON_OKAY; waiter = false; });
		btnControls->configure("okay", Rectangle(boxWidth / 2 - 64, 110, 64, 30), "Okay", ARGB(180, 0, 200, 0), RGB(0, 0, 0));

		// create cancel button
		btnControls->add("cancel", []{ response = BUTTON_CANCEL; waiter = false; });
		btnControls->configure("cancel", Rectangle(boxWidth / 2, 110, 64, 30), "Cancel", RGB(200, 0, 0), RGB(0, 0, 0));
	}

	// create only the okay button
	else
	{
		btnControls->add("okay", []{ waiter = false; });
		btnControls->configure("okay", Rectangle(boxWidth / 2 - 32, 110, 64, 30), "Okay", ARGB(180, 0, 200, 0), RGB(0, 0, 0));
	}
}

/*
 * show components on screen
 */
void showComponents()
{
	// show components on screen
	box->addChild(message);
	box->setVisible(true);

	// show buttons
	btnControls->show(box);
}

/*
 * show only the message in empty window
 */
void MsgBox::show(std::string msg)
{
	// call component initialization
	setupComponents(msg, false);

	// set title of message
	message->setTitle(msg);

	// show components
	showComponents();

	// wait response from box
	AtomicBlock(&waiter);

	// deallocate components
	garbageDelete();
}

/*
 * show the message in window titled with provided title
 */
void MsgBox::show(std::string msg, std::string title)
{
	// call component initialization
	setupComponents(msg, false);

	// set title of message
	box->setTitle(title);
	message->setTitle(msg);

	// show components
	showComponents();

	// wait response from box
	AtomicBlock(&waiter);

	// deallocate components
	garbageDelete();
}

/*
 * show message with title and return pressed button
 */
PressedButton MsgBox::showAndGetResponse(std::string msg, std::string title)
{
	// call component initialization
	setupComponents(msg, true);

	// set title of message
	box->setTitle(title);
	message->setTitle(msg);

	// show components
	showComponents();

	// wait response from box
	while (response == BUTTON_NONE)
		Yield();

	// deallocate components
	garbageDelete();

	// return response
	return response;
}
