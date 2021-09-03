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
#include <string.h>
#include <libgen.h>

#include <gui/ui.hpp>
#include <gui/window.hpp>
#include <gui/geoshape.hpp>
#include <gui/label.hpp>
#include <gui/btnlist.hpp>
#include <gui/notification.hpp>

#include <vector>
#include <string>

// interface objects
static Window *window;
static Label *title;
static Geoshape *pngPanel;
static ButtonList *buttons;

// vector to store backgrounds image paths
static std::vector<std::string> backgrounds;
static std::vector<std::string>::iterator it;
static std::vector<std::string>::reverse_iterator rit;
static bool finisced = false;

// lock for event mode
static uint8_t lock = true;

/*
 * scan the background directory and store the path on backgrounds list
 */
void findBackgroundsPaths()
{
	// create a base path
	std::string basepath = "/cfg/gui/background/";

	// open the background directory
	FsDirectoryIterator *iterator = OpenDirectory(basepath.c_str());

	// read all nodes
	while (true)
	{
		// read the node
		FsReadDirectoryStatus stat;
		FsDirectoryEntry *node = ReadDirectoryS(iterator, &stat);

		// only on successful read
		if (stat == FS_READ_DIRECTORY_SUCCESSFUL)
		{
			// check if node exist and it isn't a cfg file // TODO remove .directory from ramdisk!!!!!
			if (node && strcmp(node->name, ".directory") != 0) backgrounds.push_back(basepath + node->name);
		}

		// exit at the end of directory
		else if (stat == FS_READ_DIRECTORY_EOD) break;
	}

	// set flag as true, we can set the first image on png panel
	finisced = true;
}

/*
 * event method for prev button
 */
static void previousHandler()
{
	// decrement reverse_iterator
	--rit;

	// there are another image? set it
	if (rit != backgrounds.rend()) pngPanel->setPNG(*it, Point(0, 0));

	// restart circle
	else
	{
		rit = backgrounds.rbegin();
		pngPanel->setPNG(*it, Point(0, 0));
	}
}

/*
 * event method for next button
 */
static void nextHandler()
{
	// increment iterator
	++it;

	// there are another image? set it
	if (it != backgrounds.end()) pngPanel->setPNG(*it, Point(0, 0));

	// restart circle
	else
	{
		it = backgrounds.begin();
		pngPanel->setPNG(*it, Point(0, 0));
	}
}

/*
 * event method for okay button
 */
static void okayHandler()
{
	if (UI::setBackground(*it))
		Notification::send("UItweak background changer", "Successful applied " + std::string(basename((char*)(*it).c_str())) + std::string(" as background"), 5, RGB(0, 200, 0), MessageAnimation_t::SIDE_SCROLL);

	else
		Notification::send("UItweak background changer", "Unable to apply " + std::string(basename((char*)(*it).c_str())) + std::string(" as background"), 5, RGB(200, 0, 0), MessageAnimation_t::DARKENING);
}

/*
 *	Main of tweak
 */
int main(int argc, char *argv[])
{
	// open comunication to windowserver
	if (UI::open() == UI_OPEN_STATUS_SUCCESSFUL)
	{
		// run as background thread the image finder
		// (for one or two image is more slower a thread, but if images are so many thread is more faster)
		CreateThreadN((void*) &findBackgroundsPaths, "backgroundFinder");

		// initialize components
		window = Window::create();
		title = Label::create();
		pngPanel = Geoshape::create();
		buttons = new ButtonList();
		buttons->add("prev", previousHandler);
		buttons->add("next", nextHandler);
		buttons->add("okay", okayHandler);

		// add components to window
		window->addChild(title);
		window->addChild(pngPanel);
		buttons->show(window);

		// set exit event
		window->onClose([]{ lock = false; });

		// get screen resolution
		Dimension resolution = UI::getResolution();

		// set bounds
		Rectangle bounds(resolution.width / 2 - 400, resolution.height / 2 - 300, 820, 655);
		window->setBounds(bounds);
		title->setBounds(Rectangle(0, 0, 800, 30));
		pngPanel->setBounds(Rectangle(30, 30, 740, 540));

		// configure titles
		window->setTitle("OsmosUI tweak Tool");
		title->setTitle("Choose one of these images and click 'Okay' to set as background");

		// configure buttons
		buttons->configure("prev", Rectangle(0, 30, 30, 540), "<", RGB(0, 255, 255), RGB(0, 0, 0));
		buttons->configure("next", Rectangle(770, 30, 30, 540), ">", RGB(0, 255, 255), RGB(0, 0, 0));
		buttons->configure("okay", Rectangle(10, 570, 100, 30), "Okay", ARGB(120, 0, 200, 0), RGB(0, 0, 0));

		// show the window
		window->setVisible(true);
		pngPanel->setTitle("Loading images");

		// wait for thread finisching
		while (!finisced)
			Yield();

		// images loaded
		pngPanel->setTitle("");

		// set iterators with first path
		it = backgrounds.begin();
		rit = backgrounds.rbegin();
		pngPanel->setPNG(*it, Point(0, 0));

		// lock in event mode
		AtomicBlock(&lock);

		// on termination delete all interface objects
		delete buttons;
		delete pngPanel;
		delete title;
		delete window;

		// close communication
		UI::close();
	}

	return 0;
}
