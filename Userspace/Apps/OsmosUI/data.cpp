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

#include "OsmosUI.hpp"
#include "SecondaryThread.hpp"

#include <cstring>
#include <fcntl.h>
#include <fstream>
#include <GUI/About.hh>
#include <GUI/ButtonMenu.hh>
#include <IO/Keyboard.hh>
#include <list>
#include <map>
#include <new>
#include <sstream>
#include <Utils/Environment.hh>
#include <Utils/PropertyFileParser.hh>
#include <vector>

using namespace std;

// flag for pressed start menu
static bool pressed = false;

// locker of application
static bool _continue = true;

// list of asincronous threads
static vector<Tid> threads;

// object where we draw components
static Geoshape* menuTab;

// main bar of interface
static Geoshape* mainBar;

// container for all buttons on interface
static ButtonList* buttons;

// start menu button
static Button* menuButton;

// environment user
static std::string loggedUser;

// menu of start
static ButtonMenu start;

// label for display
static Label* taskLabel;
static Label* hourLabel;
static Label* memLabel;

// input container
static list<IO::Keyboard::Info> inputBuffer;
static bool                     inputBufferEmpty = true;
static Tasking::Lock            locker;

/**
 *	key listener for events
 */
class InputKeyListener : public KeyListener {
public:
    virtual void handle_key_event(KeyEvent& e) {
        locker.lock();
        inputBuffer.push_back(IO::Keyboard::instance().full_key_info(e.m_info_basic));
        inputBufferEmpty = false;
        locker.unlock();
    }
};

/*
 *	destruct class
 */
OsmosUI::~OsmosUI() {
}

/**
 *	s_open comunication to windowserver
 */
bool OsmosUI::init() {
    // create communication to windowserver
    if ( UI::open() == UI_OPEN_STATUS_SUCCESSFUL )
        return true;

    return false;
}

/**
 *	configure layout of UI with configuration file and provided resolution
 */
void OsmosUI::configureUi(std::string                  pathToConfiguration,
                          Graphics::Metrics::Dimension resolution) {
    // copy to internal resolution provided dimension
    this->resolution = resolution;

    // load keyboard layout
    IO::Keyboard::instance().load_layout("it-EU");

    // keep configuration file
    std::ifstream conf(pathToConfiguration);
    if ( !conf.good() )
        klog("file non trovato in %s", pathToConfiguration.c_str());
    // parse it
    Utils::PropertyFileParser parser(conf);
    configuration = parser.properties();

    // closing file
    conf.close();

    // creating bar
    mainBar = Geoshape::create();
    mainBar->set_color(Graphics::Color::as_argb(150, 0, 0, 0),
                       Graphics::Color::as_argb(255, 255, 255, 255));
    mainBar->set_listener(UI_COMPONENT_EVENT_TYPE_KEY, new InputKeyListener());

    if ( configuration["UiStyle"] == "GNOME" ) {
        // set bounds
        mainBar->set_bounds(Graphics::Metrics::Rectangle(0, 0, resolution.width(), 30));

        // set mode
        mode = GNOME;
        Utils::Environment::set("UISTYLE", "GNOME");
    }

    else if ( configuration["UiStyle"] == "KDE" ) {
        // set bounds
        mainBar->set_bounds(
            Graphics::Metrics::Rectangle(0, resolution.height() - 30, resolution.width(), 30));

        // if mode is kde deactivate candydock thread if is active
        if ( configuration["CandyDockThread"] == "true" )
            configuration["CandyDockThread"] = "false";

        // set mode
        mode = KDE;
        Utils::Environment::set("UISTYLE", "KDE");
    }
}

/*
 * start menu event
 */
static void startEvent() {
    if ( !pressed ) {
        pressed = true;

        if ( loggedUser == "admin" )
            menuButton->set_color(Graphics::Color::as_argb(255, 255, 10, 10),
                                  Graphics::Color::as_argb(255, 0, 0, 0));
        else
            menuButton->set_color(Graphics::Color::as_argb(255, 10, 255, 10),
                                  Graphics::Color::as_argb(255, 0, 0, 0));

        menuTab->set_visible(true);
    }

    else {
        pressed = false;
        menuButton->set_color(Graphics::Color::as_argb(255, 10, 200, 10),
                              Graphics::Color::as_argb(255, 0, 0, 0));
        menuTab->set_visible(false);
    }
}

/**
 *	create and setup start button
 */
void OsmosUI::setMenuButton() {
    // create and set menù button
    menuButton = Button::create();

    // TODO getuserID
    buttons = new ButtonList();
    buttons->add("menu", menuButton, startEvent);

    buttons->configure("menu",
                       Graphics::Metrics::Rectangle(0, 0, 72, 30),
                       "",
                       "/Apps/OsmosUI/Resources/Icons/OSLogo_ldpi.png",
                       Graphics::Metrics::Point(0, 0),
                       Graphics::Color::as_argb(255, 10, 200, 10),
                       Graphics::Color::as_argb(255, 0, 0, 0));
    buttons->show(mainBar);
}

/**
 *	initialize content of start menu
 */
void OsmosUI::setMenuTab() {
    // create menutab
    menuTab = Geoshape::create();
    menuTab->set_visible(pressed);

    if ( loggedUser == "admin" )
        menuTab->set_color(Graphics::Color::as_argb(150, 0, 0, 0),
                           Graphics::Color::as_argb(255, 255, 0, 0));
    else
        menuTab->set_color(Graphics::Color::as_argb(150, 0, 0, 0),
                           Graphics::Color::as_argb(255, 0, 255, 0));

    // get username
    loggedUser = Utils::Environment::logged_user();

    // set title of start menu
    menuTab->set_title("MeetiX OS " + Utils::Environment::get("VERSION") + " [" + loggedUser + "]");

    if ( mode == GNOME )
        menuTab->set_bounds(Graphics::Metrics::Rectangle(0, 30, 350, 350));
    else if ( mode == KDE )
        menuTab->set_bounds(Graphics::Metrics::Rectangle(0, resolution.height() - 380, 350, 350));
}

/**
 *	initialize label to display memory occupation
 */
void OsmosUI::setMemLabel() {
    memLabel = Label::create();
    memLabel->set_bounds(Graphics::Metrics::Rectangle(200, 3, 135, 30));
    memLabel->set_color(0, Graphics::Color::as_rgb(255, 255, 255));
    memLabel->set_title_alignment(Graphics::Text::Alignment::CENTER);
    menuTab->add_child(memLabel);
}

/**
 *	initialize label to display current time
 */
void OsmosUI::setHourLabel() {
    // create and set hour label
    hourLabel = Label::create();
    hourLabel->set_bounds(Graphics::Metrics::Rectangle(resolution.width() - 145, 0, 140, 30));
    hourLabel->set_color(0, Graphics::Color::as_rgb(255, 255, 255));
    hourLabel->set_title_alignment(Graphics::Text::Alignment::RIGHT);
    hourLabel->set_font("Xcelsion");
    hourLabel->set_font_size(22);
    mainBar->add_child(hourLabel);
}

/**
 *	initialize label to display windows name
 */
void OsmosUI::setTaskLabel() {
    taskLabel = Label::create();
    taskLabel->set_bounds(Graphics::Metrics::Rectangle(75, -3, resolution.width() - 155, 30));
    taskLabel->set_color(0, Graphics::Color::as_rgb(255, 255, 255));
    mainBar->add_child(taskLabel);
}

/*
 * logout from meetix, return to login manager
 */
void meetiXOSLogout() {
    // kill all threads
    for ( int tid : threads )
        s_kill(tid);

    // remove all objects
    delete buttons;
    delete menuTab;
    delete mainBar;
    delete hourLabel;
    delete taskLabel;
    delete memLabel;

    // exec login
    s_spawn("/Apps/MXLogin/Bin/MXLogin", "", "/Apps/MXLogin/", SECURITY_LEVEL_APPLICATION);

    // unlock iteration
    _continue = false;
    locker.unlock();

    // close comunication with windowserver
    UI::close();
}

/*
 * create menu configuration
 */
std::map<string, string> createMenuConfiguration() {
    // create basedir
    string basedir = "/Apps/";

    // open the directory
    FsDirectoryIterator* it = s_open_directory(basedir.c_str());

    // create a configuration map
    map<string, string> configuration;

    // find all entryes
    while ( true ) {
        // read nodes
        FsReadDirectoryStatus stat;
        FsDirectoryEntry*     node = s_read_directory_s(it, &stat);

        // if read is successful
        if ( stat == FS_READ_DIRECTORY_SUCCESSFUL ) {
            // if node exist and it isn't a system application
            if ( node && strcmp(node->m_name, "OsmosUI") != 0
                 && strcmp(node->m_name, "MXLogin") != 0 && strcmp(node->m_name, "zipNET") != 0 ) {
                // create configuration line
                string name = string(node->m_name);

                // create streamer to create argument
                stringstream arg;
                arg << basedir << node->m_name << "/Bin/" << node->m_name << '&' << basedir
                    << node->m_name << "/Resources/Icons/Desktop.png";

                // store into map
                configuration.insert(make_pair(name, arg.str()));
            }

            else
                break;
        }

        // exit at the end of reading
        else if ( stat == FS_READ_DIRECTORY_EOD )
            break;
    }

    return configuration;
}

/**
 *	create list of button menu into start space
 */
void OsmosUI::createMenu() {
    // crete and configure menu object with script file
    start.create(createMenuConfiguration(), menuTab->bounds());
    start.show(menuTab);

    // create and setup logout button
    buttons->add("logout", meetiXOSLogout);
    buttons->configure("logout",
                       Graphics::Metrics::Rectangle(0, 320, 160, 30),
                       "LogOut",
                       Graphics::Color::as_argb(120, 200, 0, 0),
                       Graphics::Color::as_rgb(255, 255, 255));
    buttons->get("logout")->set_font("consolas");

    // create and setup about button
    buttons->add("about", about_os);
    buttons->configure("about",
                       Graphics::Metrics::Rectangle(190, 320, 160, 30),
                       "About MeetiX OS",
                       Graphics::Color::as_argb(120, 0, 200, 0),
                       Graphics::Color::as_rgb(255, 255, 255));
    buttons->get("about")->set_font("consolas");

    // add this two buttons to start menu
    buttons->show("about", menuTab);
    buttons->show("logout", menuTab);
}

/**
 *	call all previous method
 */
void OsmosUI::createComponents() {
    setMenuButton(); // create and set menù button
    setMenuTab();    // create menu tab
    createMenu();    // create app button into menu
    setMemLabel();   // create label for memory usage indicator
    setHourLabel();  // create and set hour label
    setTaskLabel();  // create task label
}

/*
 *	read from KeyEvent
 */
IO::Keyboard::Info OsmosUI::readInput() {
    // wait for input
    if ( inputBuffer.size() == 0 )
        s_atomic_block(&inputBufferEmpty);

    // lock thread
    locker.lock();

    IO::Keyboard::Info result = inputBuffer.front(); // safety copy
    inputBuffer.pop_front();                         // clear list

    // unlock
    if ( inputBuffer.size() == 0 )
        inputBufferEmpty = true;

    locker.unlock();

    return result;
}

/**
 *	mainloop, start secondary thread if there are set active in configuration, than wait for
 *events
 */
void OsmosUI::mainLoop() {
    // exec asincronous thread if in configuration are enabled
    if ( configuration["TaskManagerThread"] == "true" )
        UI::register_task_manager(taskLabel, taskLabel->bounds());
    if ( configuration["HourManagerThread"] == "true" )
        threads.push_back(s_create_thread_dn((void*)&SecondaryThread::HourManagerThread,
                                             (void*)hourLabel,
                                             "hourmgr"));
    if ( configuration["MemoryUsageThread"] == "true" )
        threads.push_back(s_create_thread_dn((void*)&SecondaryThread::MemoryUsageThread,
                                             (void*)memLabel,
                                             "memusage"));
    if ( configuration["OsmosUIDockThread"] == "true" )
        threads.push_back(s_create_thread_dn((void*)&SecondaryThread::OsmosUIDockThread,
                                             (void*)&resolution,
                                             "dock"));
    if ( configuration["NotificationThread"] == "true" )
        threads.push_back(
            s_create_thread_n((void*)&SecondaryThread::NotificationThread, "notifier"));

    // main loop of gui
    while ( _continue ) {
        // read pressed keys
        IO::Keyboard::Info key = readInput();

        if ( key.m_is_pressed ) {
            // reading combination
            if ( key.m_alt && key.m_key == "KEY_T" )
                s_spawn("/Apps/CandyShell/Bin/CandyShell", "", "/", SECURITY_LEVEL_APPLICATION);
            else if ( key.m_alt && key.m_key == "KEY_S" )
                s_spawn("/Apps/CandyNote/Bin/CandyNote", "-new", "/", SECURITY_LEVEL_APPLICATION);
        }
    }
}
