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

#include "MXLogin.hpp"

#include <cstring>
#include <fstream>
#include <gui/actionlistener.hpp>
#include <gui/button.hpp>
#include <gui/geoshape.hpp>
#include <gui/label.hpp>
#include <gui/textfield.hpp>
#include <gui/ui.hpp>
#include <gui/window.hpp>
#include <Utils/Environment.hh>
#include <Utils/PropertyFileParser.hh>
#include <Utils/Time.hh>
#include <Utils/Utils.hh>

using namespace std;

// main graphic form
static Geoshape* loginWindow;

// centre of login
static Geoshape*  loginRectangle;
static Textfield* usernameField;
static Label*     usernameLabel;
static Textfield* passwordField;
static Button*    loginButton;

// info labels
static Label* info;
static Label* info2;
static Label* hourInfo;
static Label* animation;

// resolution of screen
static Dimension resolution;

// locker for event mode
static bool lock = true;

// login application mode
LoginMode_t mode = LOGIN;

/*
 *	listener of login button
 */
class LoginButtonActionListener : public ActionListener {
public:
    /**
     *  virtual method of ActionListener that is called when button is cliked
     */
    virtual void handleAction() {
        loginToMeetiX();
    }
};

/*
 *	this method is called when enter or button login is pressed
 */
static void loginToMeetiX() {
    // creating string variables for isereted username and password
    string username;
    string password;

    // get username and password
    if ( mode == LOGIN )
        username = usernameField->getTitle();
    else if ( mode == LOCK )
        username = usernameLabel->getTitle();
    password = passwordField->getTitle();

    // test credential
    if ( researchAccess(username, password, mode) ) {
        // only in login mode read the script
        if ( mode == LOGIN ) {
            // log the logged username
            Utils::log("Logging as: %s", username.c_str());

            // exec mx shell to start ui components
            s_spawn("/Bins/MxSh",
                    "-s AfterLogin.sh",
                    "/MeetiX/Configs/Startup/Interactive/",
                    SECURITY_LEVEL_APPLICATION);
        }

        // unlock event mode
        lock = false;
    }

    else {
        loginButton->setTitle("Wrong UserName or PassWord!");
        loginButton->setColor(ARGB(200, 230, 230, 230), ARGB(255, 255, 0, 0));

        if ( mode == LOGIN )
            usernameField->setTitle("");
        passwordField->setTitle("");

        s_sleep(1500);

        loginButton->setColor(ARGB(150, 0, 0, 0), ARGB(255, 255, 255, 255));
        loginButton->setTitle("Login");
    }
}

/*
 * compare the provided username and password with username and passwords on file in /cfg/passwd/crd
 */
static bool researchAccess(string username, string password, LoginMode_t mode) {
    // open the crd file
    ifstream users_tab("/MeetiX/Configs/Credentials/Users");

    // get file properties
    Utils::PropertyFileParser  parser(users_tab);
    map<string, string> properties = parser.getProperties();

    // always close file
    users_tab.close();

    // check all username and passwords
    for ( auto& current : properties ) {
        // compare
        if ( username == current.first && password == current.second ) {
            if ( mode != LoginMode_t::LOCK )
                Utils::Environment::set("USER", username);
            return true;
        }
    }

    // username or password not found
    return false;
}

/*
 *	secondary thread: responsible of time label
 */
[[noreturn]] void timeThread() {
    // registering name of task
    s_task_register_id("timer");

    // infinite iteration, end when login terminate
    while ( true ) {
        // call timedriver to fill time struct
        Utils::Time::Current time;
        Utils::Time::current(time);

        // pack data
        stringstream hours;
        hours << time.m_hour << ":" << time.m_minute << ":" << time.m_second;

        // and write in hour label
        hourInfo->setTitle(hours.str());

        // wait one second
        s_sleep(1000);
    }
}

/*
 *	instantiate objects
 */
void initializeObjects() {
    // creating geoshape objects
    loginWindow    = Geoshape::create();
    loginRectangle = Geoshape::create();

    // creating labels objects
    info     = Label::create();
    info2    = Label::create();
    hourInfo = Label::create();

    // creating textfields objects
    if ( mode == LOGIN )
        usernameField = Textfield::create();
    else if ( mode == LOCK )
        usernameLabel = Label::create();
    passwordField = Textfield::create();

    // creting buttons
    loginButton = Button::create();
}

/*
 * configure main window
 */
void configureMainWindow() {
    // setting main geoshape with dimension of screen
    loginWindow->setBounds(Rectangle(0, 0, resolution.width, resolution.height));
    loginWindow->setColor(ARGB(120, 180, 180, 180), ARGB(255, 0, 255, 0));
    loginWindow->setTitle("Welcome in MeetiX OS");
}

/*
 *	configure the info labels
 */
void configureLabels() {
    // setting label with hostname and os version
    info->setBounds(Rectangle(10, resolution.height - 30, 200, 30));
    info->setFont("consolas");
    info->setColor(0, RGB(0, 0, 0));
    info->setTitle("Version " + Utils::Environment::get("VERSION"));
    loginWindow->addChild(info);

    // setting hour label
    hourInfo->setFont("consolas");
    hourInfo->setBounds(Rectangle(0, 10, 280, 30));
    hourInfo->setTitleAlignment(TextAlignment::CENTER);
    hourInfo->setFontSize(20);
    hourInfo->setColor(0, RGB(255, 255, 255));

    // setting info labels
    info2->setBounds(Rectangle(resolution.width - 90, resolution.height - 30, 200, 30));
    info2->setFont("consolas");
    info2->setColor(0, RGB(0, 0, 0));
    info2->setTitle(Utils::Environment::getHostname() + "//");
    loginWindow->addChild(info2);
}

/*
 *	configure the textfields
 */
void configureActionComponents() {
    if ( mode == LOGIN ) {
        // setting textfield for username
        usernameField->setBounds(Rectangle(15, 50, 250, 30));
        usernameField->setColor(ARGB(20, 0, 0, 0), ARGB(255, 255, 255, 255));
        usernameField->setGhostTitle("Enter your UserName");
    } else if ( mode == LOCK ) {
        // set label that display logged usernameField
        usernameLabel->setBounds(Rectangle(0, 50, 280, 30));
        usernameLabel->setColor(0, RGB(255, 255, 255));
        usernameLabel->setFontSize(20);
        usernameLabel->setTitleAlignment(TextAlignment::CENTER);
        usernameLabel->setTitle(Utils::Environment::get("USER"));
    }

    // setting textfield for password
    passwordField->setBounds(Rectangle(15, 100, 250, 30));
    passwordField->setColor(ARGB(20, 0, 0, 0), ARGB(255, 255, 255, 255));
    passwordField->setGhostTitle("Enter your PassWord");
    passwordField->setSecure(true);

    // setting button
    loginButton->setBounds(Rectangle(15, 150, 250, 30));
    loginButton->setColor(ARGB(150, 0, 0, 0), ARGB(255, 255, 255, 255));
    loginButton->setTitle("Login");
    loginButton->setActionListener(new LoginButtonActionListener());
}

/*
 * do animation with "MeetiX OS" logo
 */
void paintAnimation() {
    // create a big label with "MeetiX OS"
    animation = Label::create();
    animation->setBounds(Rectangle(0, 250, resolution.width, 200));
    animation->setTitleAlignment(TextAlignment::CENTER);
    animation->setFont("Xcelsion");
    animation->setFontSize(100);
    animation->setTitle("MeetiX OS");
    animation->setColor(0, RGB(20, 180, 20));
    loginWindow->addChild(animation);

    // do an animation
    for ( int yPosition = 180; yPosition >= 100; yPosition-- ) {
        // show logo animation only in even position
        if ( !(yPosition % 2) )
            animation->setBounds(Rectangle(0, yPosition, resolution.width, 200));

        // sleep
        s_sleep(5);
    }
}

/*
 *	MXLogin, graphical login of MeetiX OS
 */
int main(int argc, char* argv[]) {
    // open communication to windowserver
    if ( UI::open() == UI_OPEN_STATUS_SUCCESSFUL ) {
        // parse args, if mode is lock get the first user name
        if ( argc == 2 && !strcmp(argv[1], "lock") )
            mode = LOCK;

        // get resolution of screen
        resolution = UI::getResolution();

        // instantiate objects
        initializeObjects();

        // configure main geoshape
        configureMainWindow();

        // configure labels
        configureLabels();

        // setting login geoshape
        loginRectangle->setVisible(false);
        loginRectangle->setBounds(
            Rectangle(resolution.width / 2 - 140, resolution.height / 2 - 115, 280, 190));
        loginRectangle->setColor(ARGB(120, 20, 150, 20), ARGB(255, 0, 0, 0));

        // run as a background thread the paint animation method
        s_create_thread_n((void*)&paintAnimation, "animation");

        // configure button and textfields
        configureActionComponents();

        // add to screen componets
        loginRectangle->setVisible(true);

        // exec timeThread in background
        s_create_thread_n((void*)&timeThread, "timelabel");

        // add to login rectangle components
        loginRectangle->addChild(hourInfo);
        if ( mode == LOGIN )
            loginRectangle->addChild(usernameField);
        else if ( mode == LOCK )
            loginRectangle->addChild(usernameLabel);
        loginRectangle->addChild(passwordField);
        loginRectangle->addChild(loginButton);

        // event mode
        s_atomic_block(&lock);

        // delete interface objects
        delete usernameField;
        delete usernameLabel;
        delete passwordField;
        delete loginButton;
        delete info;
        delete info2;
        delete hourInfo;
        delete animation;
        delete loginWindow;
        delete loginRectangle;

        // close interface connection
        UI::close();
    } else {
        Utils::log("unable to open login GUI");
    }
    return 0;
}
