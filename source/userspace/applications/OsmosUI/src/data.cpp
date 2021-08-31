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

#include <OsmosUI.hpp>
#include <SecondaryThread.hpp>
#include <gui/about.hpp>
#include <utils/environment.hpp>
#include <utils/fparser.hpp>
#include <utils/utils.hpp>
#include <io/keyboard.hpp>
#include <gui/menu.hpp>
#include <fcntl.h>
#include <sstream>
#include <fstream>
#include <vector>
#include <list>
#include <map>

using namespace std;

// flag for pressed start menu
static bool pressed = false;

// locker of application
static bool _continue = true;

// list of asincronous threads
static vector<Tid> threads;

// object where we draw components
static Geoshape *menuTab;

// main bar of interface
static Geoshape *mainBar;

// container for all buttons on interface
static ButtonList *buttons;

// start menu button
static Button *menuButton;

// environment user
static std::string loggedUser;

// menu of start
static ButtonMenu start;

// label for display
static Label *taskLabel;
static Label *hourLabel;
static Label *memLabel;

// input container
static list<Keyboard::Info> inputBuffer;
static uint8_t inputBufferEmpty = true;
static Lock locker;

/**
 *	key listener for events
 */
class InputKeyListener : public KeyListener
{
public:
	virtual void handleKeyEvent(KeyEvent &e)
	{
		locker.lock();
		inputBuffer.push_back(Keyboard::fullKeyInfo(e.info));
		inputBufferEmpty = false;
		locker.unlock();
	}
};

/*
 *	destruct class
 */
OsmosUI::~OsmosUI()
{
}

/**
 *	Open comunication to windowserver
 */
bool OsmosUI::init()
{
	// create communication to windowserver
	if (UI::open() == UI_OPEN_STATUS_SUCCESSFUL) return true;

	return false;
}

/**
 *	configure layout of UI with configuration file and provided resolution
 */
void OsmosUI::configureUi(std::string pathToConfiguration, Dimension resolution)
{
	// copy to internal resolution provided dimension
	this->resolution = resolution;

	// load keyboard layout
	Keyboard::loadLayout("it-EU");

	// keep configuration file
	std::ifstream conf(pathToConfiguration);
	if (!conf.good()) klog("file non trovato in %s", pathToConfiguration.c_str());
	// parse it
	PropertyFileParser parser(conf);
	configuration = parser.getProperties();

	// closing file
	conf.close();

	// creating bar
	mainBar = Geoshape::create();
	mainBar->setColor(ARGB(150, 0, 0, 0), ARGB(255, 255, 255, 255));
	mainBar->setListener(UI_COMPONENT_EVENT_TYPE_KEY, new InputKeyListener());

	if (configuration["UiStyle"] == "GNOME")
	{
		// set bounds
		mainBar->setBounds(Rectangle(0, 0, resolution.width, 30));

		// set mode
		mode = GNOME;
		Environment::set("UISTYLE", "GNOME");
	}

	else if (configuration["UiStyle"] == "KDE")
	{
		// set bounds
		mainBar->setBounds(Rectangle(0, resolution.height - 30, resolution.width, 30));

		// if mode is kde deactivate candydock thread if is active
		if (configuration["CandyDockThread"] == "true") configuration["CandyDockThread"] = "false";

		// set mode
		mode = KDE;
		Environment::set("UISTYLE", "KDE");
	}
}

/*
 * start menu event
 */
static void startEvent()
{
	if (!pressed)
	{
		pressed = true;

		if (loggedUser == "admin") menuButton->setColor(ARGB(255, 255, 10, 10), ARGB(255, 0, 0, 0));
		else menuButton->setColor(ARGB(255, 10, 255, 10), ARGB(255, 0, 0, 0));

		menuTab->setVisible(true);
	}

	else
	{
		pressed = false;
		menuButton->setColor(ARGB(255, 10, 200, 10), ARGB(255, 0, 0, 0));
		menuTab->setVisible(false);
	}
}

/**
 *	create and setup start button
 */
void OsmosUI::setMenuButton()
{
	//create and set menù button
	menuButton = Button::create();

	// TODO getuserID
	buttons = new ButtonList();
	buttons->add("menu", menuButton, startEvent);

	buttons->configure("menu", Rectangle(0, 0, 72, 30), "", "/app/OsmosUI/deps/icon/mx.png", Point(0, 0), ARGB(255, 10, 200, 10), ARGB(255, 0, 0, 0));
	buttons->show(mainBar);
}

/**
 *	initialize content of start menu
 */
void OsmosUI::setMenuTab()
{
	//create menutab
	menuTab = Geoshape::create();
	menuTab->setVisible(pressed);

	if (loggedUser == "admin") menuTab->setColor(ARGB(150, 0, 0, 0), ARGB(255, 255, 0, 0));
	else menuTab->setColor(ARGB(150, 0, 0, 0), ARGB(255, 0, 255, 0));

	// get username
	loggedUser = Environment::getLoggedUser();

	// set title of start menu
	menuTab->setTitle("MeetiX OS " + Environment::get("VERSION") + " [" + loggedUser + "]");

	if (mode == GNOME) menuTab->setBounds(Rectangle(0, 30, 350, 350));
	else if (mode == KDE) menuTab->setBounds(Rectangle(0, resolution.height - 380, 350, 350));
}

/**
 *	initialize label to display memory occupation
 */
void OsmosUI::setMemLabel()
{
	memLabel = Label::create();
	memLabel->setBounds(Rectangle(200, 3, 135, 30));
	memLabel->setColor(0, RGB(255, 255, 255));
	memLabel->setTitleAlignment(TextAlignment::CENTER);
	menuTab->addChild(memLabel);
}

/**
 *	initialize label to display current time
 */
void OsmosUI::setHourLabel()
{
	//create and set hour label
	hourLabel = Label::create();
	hourLabel->setBounds(Rectangle(resolution.width - 145, 0, 140, 30));
	hourLabel->setColor(0, RGB(255, 255, 255));
	hourLabel->setTitleAlignment(TextAlignment::RIGHT);
	hourLabel->setFont("Xcelsion");
	hourLabel->setFontSize(22);
	mainBar->addChild(hourLabel);
}

/**
 *	initialize label to display windows name
 */
void OsmosUI::setTaskLabel()
{
	taskLabel = Label::create();
	taskLabel->setBounds(Rectangle(75, -3, resolution.width - 155, 30));
	taskLabel->setColor(0, RGB(255, 255, 255));
	mainBar->addChild(taskLabel);
}

/*
 * logout from meetix, return to login manager
 */
void meetiXOSLogout()
{
	// kill all threads
	for (int tid : threads)
		Kill(tid);

	// remove all objects
	delete buttons;
	delete menuTab;
	delete mainBar;
	delete hourLabel;
	delete taskLabel;
	delete memLabel;

	// exec login
	Spawn("/app/MXLogin/bin/MXLogin", "", "/app/MXLogin/", SECURITY_LEVEL_APPLICATION);

	// unlock iteration
	_continue = false;
	locker.unlock();

	// close comunication with windowserver
	UI::close();
}

/*
 * create menu configuration
 */
std::map<string, string> createMenuConfiguration()
{
	// create basedir
	string basedir = "/app/";

	// open the directory
	FsDirectoryIterator *it = OpenDirectory(basedir.c_str());

	// create a configuration map
	map<string, string> configuration;

	// find all entryes
	while (true)
	{
		// read nodes
		FsReadDirectoryStatus stat;
		FsDirectoryEntry *node = ReadDirectoryS(it, &stat);

		// if read is successful
		if (stat == FS_READ_DIRECTORY_SUCCESSFUL)
		{
			// if node exist and it isn't a system application
			if (node && strcmp(node->name, "OsmosUI") != 0 && strcmp(node->name, "MXLogin") != 0 && strcmp(node->name, "zipNET") != 0)
			{
				// create configuration line
				string name = string(node->name);

				// create streamer to create argument
				stringstream arg;
				arg << basedir << node->name << "/bin/" << node->name << '&' << basedir << node->name << "/deps/icon/ico.png";

				// store into map
				configuration.insert(make_pair(name, arg.str()));
			}

			else break;
		}

		// exit at the end of reading
		else if (stat == FS_READ_DIRECTORY_EOD) break;
	}

	return configuration;
}

/**
 *	create list of button menu into start space
 */
void OsmosUI::createMenu()
{
	// crete and configure menu object with script file
	start.create(createMenuConfiguration(), menuTab->getBounds());
	start.show(menuTab);

	// create and setup logout button
	buttons->add("logout", meetiXOSLogout);
	buttons->configure("logout", Rectangle(0, 320, 160, 30), "LogOut", ARGB(120, 200, 0, 0), RGB(255, 255, 255));
	buttons->get("logout")->setFont("consolas");

	// create and setup about button
	buttons->add("about", aboutMeetiXOS);
	buttons->configure("about", Rectangle(190, 320, 160, 30), "About MeetiX OS", ARGB(120, 0, 200, 0), RGB(255, 255, 255));
	buttons->get("about")->setFont("consolas");

	// add this two buttons to start menu
	buttons->show("about", menuTab);
	buttons->show("logout", menuTab);
}

/**
 *	call all previous method
 */
void OsmosUI::createComponents()
{
	setMenuButton(); // create and set menù button
	setMenuTab();	// create menu tab
	createMenu();	// create app button into menu
	setMemLabel();	// create label for memory usage indicator
	setHourLabel();	// create and set hour label
	setTaskLabel();	// create task label
}

/*
 *	read from KeyEvent
 */
Keyboard::Info OsmosUI::readInput()
{
	// wait for input
	if (inputBuffer.size() == 0) AtomicBlock(&inputBufferEmpty);

	// lock thread
	locker.lock();

	Keyboard::Info result = inputBuffer.front(); // safety copy
	inputBuffer.pop_front(); // clear list

	// unlock
	if (inputBuffer.size() == 0) inputBufferEmpty = true;

	locker.unlock();

	return result;
}

/**
 *	mainloop, start secondary thread if there are set active in configuration, than wait for events
 */
void OsmosUI::mainLoop()
{
	// exec asincronous thread if in configuration are enabled
	if (configuration["TaskManagerThread"] == "true")  UI::registerTaskManager(taskLabel, taskLabel->getBounds());
	if (configuration["HourManagerThread"] == "true")  threads.push_back(CreateThreadDN((void*) &SecondaryThread::HourManagerThread, (void*) hourLabel, "hourmgr"));
	if (configuration["MemoryUsageThread"] == "true")  threads.push_back(CreateThreadDN((void*) &SecondaryThread::MemoryUsageThread, (void*) memLabel, "memusage"));
	if (configuration["OsmosUIDockThread"] == "true")  threads.push_back(CreateThreadDN((void*) &SecondaryThread::OsmosUIDockThread, (void*) &resolution, "dock"));
	if (configuration["NotificationThread"] == "true") threads.push_back(CreateThreadN((void*) &SecondaryThread::NotificationThread, "notifier"));

	// main loop of gui
	while (_continue)
	{
		// read pressed keys
		Keyboard::Info key = readInput();

		if (key.pressed)
		{
			// reading combination
			if (key.alt && key.key == "KEY_T") Spawn("/app/CandyShell/bin/CandyShell", "", "/", SECURITY_LEVEL_APPLICATION);
			else if (key.alt && key.key == "KEY_S") Spawn("/app/CandyNote/bin/CandyNote", "-new", "/", SECURITY_LEVEL_APPLICATION);
		}
	}
}
