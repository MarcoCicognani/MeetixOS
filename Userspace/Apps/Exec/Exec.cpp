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

#include <Api.h>
#include <GUI/Application.hh>
#include <GUI/Component/Button.hh>
#include <GUI/Component/Label.hh>
#include <GUI/Component/TextField.hh>
#include <GUI/Component/Window.hh>
#include <GUI/Listener/ActionListener.hh>
#include <GUI/MessageBox.hh>
#include <sstream>
#include <LibUtils/Environment.hh>

using namespace std;

// main window
static Window* window;

// textfield where write command
static Textfield* exec;

// exec command
static Button* launch;

// locker
static bool lock = true;

/*
 * action handler for exec button
 */
class ExecButtonHandler : public ActionListener {
public:
    virtual void handle_action() {
        // get textfield content
        string content = exec->title();

        // continue only if there is characters
        if ( !content.empty() ) {
            // check arguments
            uint32_t space = content.find(' ');
            if ( space != -1 ) {
                // split content
                string app = content.substr(0, space);
                string arg = content.substr(space + 1);

                // exec
                if ( s_spawn(app.c_str(), arg.c_str(), "/", SECURITY_LEVEL_APPLICATION) != SPAWN_STATUS_SUCCESSFUL )
                    MessageBox::show("Unable to exec " + app, "Exec");
            }

            // only exec provided name
            else if ( s_spawn(content.c_str(), "", "/", SECURITY_LEVEL_APPLICATION) != SPAWN_STATUS_SUCCESSFUL )
                MessageBox::show("Unable to exec " + content, "Exec");

            // set title
            window->set_title(content);
        }
    }
};

/*
 *	main of application
 */
int main(int argc, char* argv[]) {
    // open channel to windowserver
    if ( UI::open() == UI_OPEN_STATUS_SUCCESSFUL ) {
        // create components
        auto resolution = UI::screen_dimension();

        // configuring window
        window = Window::create();
        window->set_bounds(
            Graphics::Metrics::Rectangle(resolution.width() / 2 - 125, resolution.height() / 2 - 75, 273, 132));
        window->set_title("Exec");
        window->on_close([] { lock = false; });

        // configuring textfield
        exec = Textfield::create();
        exec->set_bounds(Graphics::Metrics::Rectangle(0, 10, 250, 30));
        exec->set_title_alignment(Graphics::Text::Alignment::CENTER);
        exec->set_ghost_title("Enter app name");
        window->add_child(exec);

        // configuring button
        launch = Button::create();
        launch->set_bounds(Graphics::Metrics::Rectangle(0, 50, 250, 30));
        launch->set_title("Run");
        launch->set_color(Graphics::Color::as_argb(180, 0, 200, 0), Graphics::Color::as_rgb(0, 0, 0));
        launch->set_action_listener(new ExecButtonHandler());
        window->add_child(launch);

        // show window
        window->set_visible(true);

        // event mode
        s_atomic_block(&lock);

        // remove components
        delete launch;
        delete exec;
        delete window;

        // close communication
        UI::close();
    }

    return 0;
}
