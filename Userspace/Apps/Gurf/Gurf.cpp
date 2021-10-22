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

#include <GUI/Application.hh>
#include <GUI/ButtonList.hh>
#include <GUI/Component/Geoshape.hh>
#include <GUI/Component/Label.hh>
#include <GUI/Component/Window.hh>
#include <stdlib.h>
#include <Tasking/Timer.hh>
#include <Utils/TimeSpan.hh>
#include <vector>

// timer object
Tasking::Timer* timer;
Label*          level;
Window*         window;
Geoshape*       display;
ButtonList*     buttons;

// vector to fill random index of colors
std::vector<int>           spawned;
std::vector<int>::iterator it;

// vector to fill user sequences
std::vector<int> userseq;

// flag for mainloop
bool _iterate = true;

// flag for number to spawn
bool finisched = false;
int  _size     = 4;
int  _clicked  = 0;
int  _level    = 1;

// button flags color
enum ButtonColor_t
{
    RED,
    GREEN,
    BLUE,
    YELLOW,
};

/*
 * interface event for buttons
 */
class ButtonEvent : public ActionListener {
public:
    // constructor
    ButtonEvent(ButtonColor_t color) : color(color) {
    }

    // event method
    virtual void handle_action() {
        userseq.push_back(color);
        _clicked++;
    }

private:
    int color;
};

/*
 * event handler for button begin
 */
static void beginButtonHandler() {
    // begin the timer
    timer->start();
}

/*
 * event handler for button end
 */
static void endButtonHandler() {
    // end the timer
    timer->stop();

    // flush the vectors
    spawned.clear();
    userseq.clear();
}

/*
 * timer handler
 */
static void timerHandler() {
    // begin the displaying of colors
    finisched = false;

    int color = *it;

    // show color
    switch ( color ) {
        case RED:
            display->set_color(Graphics::Color::as_rgb(200, 0, 0), 0);
            break;
        case GREEN:
            display->set_color(Graphics::Color::as_rgb(0, 200, 0), 0);
            break;
        case BLUE:
            display->set_color(Graphics::Color::as_rgb(0, 0, 200), 0);
            break;
        case YELLOW:
            display->set_color(Graphics::Color::as_rgb(200, 200, 0), 0);
            break;
    }

    // set to next the iterator
    ++it;

    // finisched
    finisched = true;
}

/*
 * initialize the backend of the game
 */
void prepare() {
    // fill vector with first 4 number colors
    for ( int i = 0; i < _size; i++ )
        spawned.push_back(rand() % 4);

    // initialize timer
    timer = new Tasking::Timer(
        Utils::TimeSpan(0, 0, 0, 1, 0),
        []() { timerHandler(); },
        false);
    it = spawned.begin();
}

/*
 * initialize components and timer
 */
void initializeInterface() {
    // create window
    window = Window::create();
    window->set_bounds(Graphics::Metrics::Rectangle(500, 500, 440, 350));
    window->set_title("Gurf");
    window->set_resizable(false);
    window->on_close([] { _iterate = false; });

    // create shape that show estracted colors
    display = Geoshape::create();
    display->set_bounds(Graphics::Metrics::Rectangle(50, 100, 320, 100));
    display->set_color(Graphics::Color::as_rgb(0, 0, 0), 0);
    window->add_child(display);

    // create label that show current level
    level = Label::create();
    level->set_bounds(Graphics::Metrics::Rectangle(0, 200, 440, 30));
    level->set_title("Current level: 1");
    window->add_child(level);

    // create buttons
    buttons = new ButtonList();
    buttons->add("red", new ButtonEvent(RED));
    buttons->add("green", new ButtonEvent(GREEN));
    buttons->add("blue", new ButtonEvent(BLUE));
    buttons->add("yellow", new ButtonEvent(YELLOW));
    buttons->add("begin", beginButtonHandler);
    buttons->add("end", endButtonHandler);

    // configure buttons
    buttons->configure("red",
                       Graphics::Metrics::Rectangle(10, 10, 150, 30),
                       "RED",
                       Graphics::Color::as_rgb(200, 0, 0),
                       Graphics::Color::as_rgb(255, 255, 255));
    buttons->configure("green",
                       Graphics::Metrics::Rectangle(180, 10, 150, 30),
                       "GREEN",
                       Graphics::Color::as_rgb(0, 200, 0),
                       Graphics::Color::as_rgb(255, 255, 255));
    buttons->configure("blue",
                       Graphics::Metrics::Rectangle(10, 50, 150, 30),
                       "BLUE",
                       Graphics::Color::as_rgb(0, 0, 200),
                       Graphics::Color::as_rgb(255, 255, 255));
    buttons->configure("yellow",
                       Graphics::Metrics::Rectangle(180, 50, 150, 30),
                       "YELLOW",
                       Graphics::Color::as_rgb(200, 200, 0),
                       Graphics::Color::as_rgb(255, 255, 255));
    buttons->configure("begin",
                       Graphics::Metrics::Rectangle(350, 10, 50, 30),
                       "Begin",
                       Graphics::Color::as_rgb(126, 126, 126),
                       Graphics::Color::as_rgb(255, 255, 255));
    buttons->configure("end",
                       Graphics::Metrics::Rectangle(350, 50, 50, 30),
                       "end",
                       Graphics::Color::as_rgb(126, 126, 126),
                       Graphics::Color::as_rgb(255, 255, 255));

    // begin the game
    buttons->show(window);
    window->set_visible(true);
}

/*
 * compare the userseq to the spawned seq
 */
bool checkSequence() {
    bool equal = true;

    // compare all color codes
    for ( int i = 0; i < userseq.size(); i++ )
        if ( spawned[i] != userseq[i] )
            equal = false;

    return equal;
}

/*
 * mainloop of the game
 */
void mainloop() {
    // mainLoop
    while ( _iterate ) {
        if ( finisched ) {
            // if user have clicked the sequence
            if ( _clicked == _size ) {
                // stop the timer
                timer->stop();

                // check the sequence
                if ( checkSequence() ) {
                    // restart the timer
                    timer->start();

                    // add a new color and increase the size
                    spawned.push_back(rand() % 4);
                    _size++;

                    // set new level
                    std::stringstream lvl;
                    lvl << "Current Level" << ++_level;
                    level->set_title(lvl.str());
                }

                // player have scazzed the sequence
                else {
                    // flush the vectors
                    spawned.clear();
                    userseq.clear();
                    _size    = 4;
                    _clicked = 0;

                    // reinitialize
                    for ( int i = 0; i < _size; i++ )
                        spawned.push_back(rand() % 4);

                    // set new level
                    std::stringstream lvl;
                    lvl << "Current Level" << (_level = 1);
                    level->set_title(lvl.str());

                    // begin timer
                    timer->start();
                }
            }
        }
        s_yield();
    }

    delete timer;
    delete level;
    delete display;
    delete buttons;
    delete window;

    // clear
    UI::close();
}

/*
 * main of application Gurf
 */
int main(int argc, char* argv[]) {
    // open a communication channel with zipNET
    if ( UI::open() == UI_OPEN_STATUS_SUCCESSFUL ) {
        // initialize the game
        prepare();

        // initialize the ui
        initializeInterface();

        // play the game!
        mainloop();
    }

    return 0;
}
