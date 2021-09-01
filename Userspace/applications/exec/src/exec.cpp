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

#include <eva.h>
#include <sstream>

#include <utils/environment.hpp>
#include <gui/ui.hpp>
#include <gui/window.hpp>
#include <gui/label.hpp>
#include <gui/button.hpp>
#include <gui/textfield.hpp>
#include <gui/actionlistener.hpp>
#include <gui/msgbox.hpp>

using namespace std;

// main window
static Window *window;

// textfield where write command
static Textfield *exec;

// exec command
static Button *launch;

// locker
static uint8_t lock = true;

/*
 * action handler for exec button
 */
class ExecButtonHandler : public ActionListener
{
public:
	virtual void handleAction()
	{
		// get textfield content
		string content = exec->getTitle();

		// continue only if there is characters
		if (!content.empty())
		{
			// check arguments
			uint32_t space = content.find(' ');
			if (space != -1)
			{
				// split content
				string app = content.substr(0, space);
				string arg = content.substr(space + 1);

				// exec
				if (Spawn(app.c_str(), arg.c_str(), "/", SECURITY_LEVEL_APPLICATION) != SPAWN_STATUS_SUCCESSFUL) MsgBox::show("Unable to exec " + app, "Exec");
			}

			// only exec provided name
			else if (Spawn(content.c_str(), "", "/", SECURITY_LEVEL_APPLICATION) != SPAWN_STATUS_SUCCESSFUL) MsgBox::show("Unable to exec " + content, "Exec");

			// set title
			window->setTitle(content);
		}
	}
};

/*
 *	main of application
 */
int main(int argc, char *argv[])
{
	// open channel to windowserver
	if (UI::open() == UI_OPEN_STATUS_SUCCESSFUL)
	{
		// create components
		Dimension resolution = UI::getResolution();

		// configuring window
		window = Window::create();
		window->setBounds(Rectangle(resolution.width / 2 - 125, resolution.height / 2 - 75, 273, 132));
		window->setTitle("Exec");
		window->onClose([]{ lock = false; });

		// configuring textfield
		exec = Textfield::create();
		exec->setBounds(Rectangle(0, 10, 250, 30));
		exec->setTitleAlignment(TextAlignment::CENTER);
		exec->setGhostTitle("Enter app name");
		window->addChild(exec);

		// configuring button
		launch = Button::create();
		launch->setBounds(Rectangle(0, 50, 250, 30));
		launch->setTitle("Run");
		launch->setColor(ARGB(180, 0, 200, 0), RGB(0, 0, 0));
		launch->setActionListener(new ExecButtonHandler());
		window->addChild(launch);

		// show window
		window->setVisible(true);

		// event mode
		AtomicBlock(&lock);

		// remove components
		delete launch;
		delete exec;
		delete window;

		// close communication
		UI::close();
	}

	return 0;
}
