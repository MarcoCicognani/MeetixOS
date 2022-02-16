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
 * GNU General Public License for more details.
 **
 *																				 *
 * You should have received a copy of the GNU General Public License				 *
 * along with this program; if not, write to the Free Software                    *
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA *
 **********************************************************************************/

#include <LibGUI/ButtonList.hh>

using namespace std;

/*
 * generic handler class for button on list
 */
class GenericHandler : public ActionListener {
public:
    // constructor, espected pointer to event function
    GenericHandler(const std::function<void(void)>& handler) : handlerFunction(handler) {
    }

    // ereditated function, called by event provider
    virtual void handle_action() {
        handlerFunction();
    }

private:
    std::function<void(void)> handlerFunction;
};

/*
 *	standard destructor, delete al buttons
 */
ButtonList::~ButtonList() {
    for ( pair<string, Button*> bnt : buttons )
        delete bnt.second;
}

/*
 * add method to add one button to list
 */
void ButtonList::add(const string& name, const std::function<void(void)>& btnEvent) {
    // add parameters to internal map
    buttons.insert({ name, Button::create() });
    handlers.insert({ name, new GenericHandler(btnEvent) });

    // set provided handler as action handler of button
    buttons[name]->set_action_listener(handlers[name]);
}

/*
 * add method to add one button to list
 */
void ButtonList::add(const string&                    name,
                     Button*                          button,
                     const std::function<void(void)>& btnEvent) {
    // add parameters to internal maps
    buttons.insert({ name, button });
    handlers.insert({ name, new GenericHandler(btnEvent) });

    // set provided handler as action handler of button
    buttons[name]->set_action_listener(handlers[name]);
}

/*
 * add method to add one button to list
 */
void ButtonList::add(const string& name, Button* button, ActionListener* listener) {
    // add parameters to internal maps
    buttons.insert({ name, button });
    handlers.insert({ name, listener });

    // set provided handler as action handler of button
    buttons[name]->set_action_listener(handlers[name]);
}

/*
 * add method to add one button to list
 */
void ButtonList::add(const string& name, ActionListener* listener) {
    // add parameters to internal map
    buttons.insert({ name, Button::create() });
    handlers.insert({ name, listener });

    // set provided handler as action handler of button
    buttons[name]->set_action_listener(handlers[name]);
}

/*
 * get method to get object reference
 */
Button* ButtonList::get(const string& name) {
    // if button exist return his object
    if ( buttons.count(name) > 0 )
        return buttons[name];

    // else return null pointer
    else
        return nullptr;
}

/*
 * get operator to get object reference
 */
Button* ButtonList::operator[](const string& name) {
    return get(name);
}

/*
 * show all buttons on provided panel
 */
void ButtonList::show(Window* where) {
    for ( pair<string, Button*> p : buttons )
        where->add_child(p.second);
}

/*
 * show all buttons on provided panel
 */
void ButtonList::show(Geoshape* where) {
    for ( pair<string, Button*> p : buttons )
        where->add_child(p.second);
}

/*
 * display on screen chosed button
 */
void ButtonList::show(const string& name, Window* where) {
    if ( buttons.count(name) > 0 )
        where->add_child(buttons[name]);
}

/*
 * display on screen chosed button
 */
void ButtonList::show(const string& name, Geoshape* where) {
    if ( buttons.count(name) > 0 )
        where->add_child(buttons[name]);
}

/*
 * setup parameters to provided button name
 */
void ButtonList::configure(const string& name, const Graphics::Metrics::Rectangle& bounds) {
    // if parameters is provided
    if ( bounds.width() > 0 && bounds.height() > 0 )
        buttons[name]->set_bounds(bounds);
}

/*
 *
 */
void ButtonList::configure(const string&                       name,
                           const Graphics::Metrics::Rectangle& bounds,
                           const string&                       title) {
    // call previous method
    configure(name, bounds);

    if ( !title.empty() )
        buttons[name]->set_title(title);
}

/*
 *
 */
void ButtonList::configure(const string&                       name,
                           const Graphics::Metrics::Rectangle& bounds,
                           const string&                       title,
                           const string&                       pathToPng,
                           const Graphics::Metrics::Point&     pngPosition) {
    // call previous method
    configure(name, bounds, title);

    if ( !pathToPng.empty() )
        buttons[name]->set_image(pathToPng, pngPosition);
}

/*
 *
 */
void ButtonList::configure(const string&                       name,
                           const Graphics::Metrics::Rectangle& bounds,
                           const string&                       title,
                           Graphics::Color::ArgbGradient       buttonColor,
                           Graphics::Color::ArgbGradient       titleColor) {
    // call previous method
    configure(name, bounds, title);

    buttons[name]->set_color(buttonColor, titleColor);
}

/*
 *
 */
void ButtonList::configure(const string&                       name,
                           const Graphics::Metrics::Rectangle& bounds,
                           const string&                       title,
                           const string&                       pathToPng,
                           const Graphics::Metrics::Point&     pngPosition,
                           Graphics::Color::ArgbGradient       buttonColor,
                           Graphics::Color::ArgbGradient       titleColor) {
    // call previous method
    configure(name, bounds, title, pathToPng, pngPosition);

    buttons[name]->set_color(buttonColor, titleColor);
}
