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
#include <GUI/Application.hh>
#include <GUI/Component/Button.hh>
#include <GUI/Component/Geoshape.hh>
#include <GUI/Component/Label.hh>
#include <GUI/Component/TextField.hh>
#include <GUI/Component/Window.hh>
#include <GUI/Listener/ActionListener.hh>
#include <iomanip>
#include <LibUtils/Environment.hh>
#include <LibUtils/PropertyFileParser.hh>
#include <LibUtils/Utils.hh>

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
static Graphics::Metrics::Dimension resolution;

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
    virtual void handle_action() { loginToMeetiX(); }
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
        username = usernameField->title();
    else if ( mode == LOCK )
        username = usernameLabel->title();
    password = passwordField->title();

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
        loginButton->set_title("Wrong UserName or PassWord!");
        loginButton->set_color(Graphics::Color::as_argb(200, 230, 230, 230), Graphics::Color::as_argb(255, 255, 0, 0));

        if ( mode == LOGIN )
            usernameField->set_title("");
        passwordField->set_title("");

        s_sleep(1500);

        loginButton->set_color(Graphics::Color::as_argb(150, 0, 0, 0), Graphics::Color::as_argb(255, 255, 255, 255));
        loginButton->set_title("Login");
    }
}

/*
 * compare the provided username and password with username and passwords on file in /cfg/passwd/crd
 */
static bool researchAccess(string username, string password, LoginMode_t mode) {
    // open the crd file
    ifstream users_tab("/MeetiX/Configs/Credentials/Users");

    // get file properties
    Utils::PropertyFileParser parser(users_tab);
    map<string, string>       properties = parser.properties();

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
        DateTime date_time;
        s_get_date_time(&date_time);

        // write data to stringstream
        stringstream ss;
        ss << std::setfill('0') << std::setw(2) << date_time.m_hours;
        ss << ':';
        ss << std::setfill('0') << std::setw(2) << date_time.m_minutes;
        ss << ':';
        ss << std::setfill('0') << std::setw(2) << date_time.m_seconds;

        // and write in hour label
        hourInfo->set_title(ss.str());

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
    loginWindow->set_bounds(Graphics::Metrics::Rectangle(0, 0, resolution.width(), resolution.height()));
    loginWindow->set_color(Graphics::Color::as_argb(120, 180, 180, 180), Graphics::Color::as_argb(255, 0, 255, 0));
    loginWindow->set_title("Welcome in MeetiX OS");
}

/*
 *	configure the info labels
 */
void configureLabels() {
    // setting label with hostname and os version
    info->set_bounds(Graphics::Metrics::Rectangle(10, resolution.height() - 30, 200, 30));
    info->set_font("consolas");
    info->set_color(0, Graphics::Color::as_rgb(0, 0, 0));
    info->set_title("Version " + Utils::Environment::get("VERSION"));
    loginWindow->add_child(info);

    // setting hour label
    hourInfo->set_font("consolas");
    hourInfo->set_bounds(Graphics::Metrics::Rectangle(0, 10, 280, 30));
    hourInfo->set_title_alignment(Graphics::Text::Alignment::CENTER);
    hourInfo->set_font_size(20);
    hourInfo->set_color(0, Graphics::Color::as_rgb(255, 255, 255));

    // setting info labels
    info2->set_bounds(Graphics::Metrics::Rectangle(resolution.width() - 90, resolution.height() - 30, 200, 30));
    info2->set_font("consolas");
    info2->set_color(0, Graphics::Color::as_rgb(0, 0, 0));
    info2->set_title(Utils::Environment::hostname() + "//");
    loginWindow->add_child(info2);
}

/*
 *	configure the textfields
 */
void configureActionComponents() {
    if ( mode == LOGIN ) {
        // setting textfield for username
        usernameField->set_bounds(Graphics::Metrics::Rectangle(15, 50, 250, 30));
        usernameField->set_color(Graphics::Color::as_argb(20, 0, 0, 0), Graphics::Color::as_argb(255, 255, 255, 255));
        usernameField->set_ghost_title("Enter your UserName");
    } else if ( mode == LOCK ) {
        // set label that display logged usernameField
        usernameLabel->set_bounds(Graphics::Metrics::Rectangle(0, 50, 280, 30));
        usernameLabel->set_color(0, Graphics::Color::as_rgb(255, 255, 255));
        usernameLabel->set_font_size(20);
        usernameLabel->set_title_alignment(Graphics::Text::Alignment::CENTER);
        usernameLabel->set_title(Utils::Environment::get("USER"));
    }

    // setting textfield for password
    passwordField->set_bounds(Graphics::Metrics::Rectangle(15, 100, 250, 30));
    passwordField->set_color(Graphics::Color::as_argb(20, 0, 0, 0), Graphics::Color::as_argb(255, 255, 255, 255));
    passwordField->set_ghost_title("Enter your PassWord");
    passwordField->setSecure(true);

    // setting button
    loginButton->set_bounds(Graphics::Metrics::Rectangle(15, 150, 250, 30));
    loginButton->set_color(Graphics::Color::as_argb(150, 0, 0, 0), Graphics::Color::as_argb(255, 255, 255, 255));
    loginButton->set_title("Login");
    loginButton->set_action_listener(new LoginButtonActionListener());
}

/*
 * do animation with "MeetiX OS" logo
 */
void paintAnimation() {
    // create a big label with "MeetiX OS"
    animation = Label::create();
    animation->set_bounds(Graphics::Metrics::Rectangle(0, 250, resolution.width(), 200));
    animation->set_title_alignment(Graphics::Text::Alignment::CENTER);
    animation->set_font("Xcelsion");
    animation->set_font_size(100);
    animation->set_title("MeetiX OS");
    animation->set_color(0, Graphics::Color::as_rgb(20, 180, 20));
    loginWindow->add_child(animation);

    // do an animation
    for ( int yPosition = 180; yPosition >= 100; yPosition-- ) {
        // show logo animation only in even position
        if ( !(yPosition % 2) )
            animation->set_bounds(Graphics::Metrics::Rectangle(0, yPosition, resolution.width(), 200));

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
        resolution = UI::screen_dimension();

        // instantiate objects
        initializeObjects();

        // configure main geoshape
        configureMainWindow();

        // configure labels
        configureLabels();

        // setting login geoshape
        loginRectangle->set_visible(false);
        loginRectangle->set_bounds(
            Graphics::Metrics::Rectangle(resolution.width() / 2 - 140, resolution.height() / 2 - 115, 280, 190));
        loginRectangle->set_color(Graphics::Color::as_argb(120, 20, 150, 20), Graphics::Color::as_argb(255, 0, 0, 0));

        // run as a background thread the paint animation method
        s_create_thread_n((void*)&paintAnimation, "animation");

        // configure button and textfields
        configureActionComponents();

        // add to screen componets
        loginRectangle->set_visible(true);

        // exec timeThread in background
        s_create_thread_n((void*)&timeThread, "timelabel");

        // add to login rectangle components
        loginRectangle->add_child(hourInfo);
        if ( mode == LOGIN )
            loginRectangle->add_child(usernameField);
        else if ( mode == LOCK )
            loginRectangle->add_child(usernameLabel);
        loginRectangle->add_child(passwordField);
        loginRectangle->add_child(loginButton);

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
