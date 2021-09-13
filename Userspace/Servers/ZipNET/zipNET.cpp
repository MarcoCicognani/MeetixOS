/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * *
 * MeetiX OS By MeetiX OS Project [Marco Cicognani]                                    *
 *                                                                                     *
 *         DERIVED FROM THE GHOST OPERATING SYSTEM                                     *
 *         This software is derived from the Ghost operating system project,           *
 *         written by Max Schlüssel <lokoxe@gmail.com>. Copyright 2012-2017            *
 *         https://ghostkernel.org/                                                    *
 *         https://github.com/maxdev1/ghost                                            *
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

#include "zipNET.hpp"

#include "components/background.hpp"
#include "components/button.hpp"
#include "components/checkbox.hpp"
#include "components/cursor.hpp"
#include "components/scrollbar.hpp"
#include "components/scrollpane.hpp"
#include "components/text/TextField.hpp"
#include "components/window.hpp"
#include "events/event.hpp"
#include "events/locatable.hpp"
#include "input/InputReceiver.hpp"
#include "interface/ComponentRegistry.hpp"
#include "interface/RegistrationThread.hpp"
#include "layout/FlowLayoutManager.hpp"
#include "layout/GridLayoutManager.hpp"
#include "output/VbeVideoOutput.hpp"

#include <cairo/cairo.h>
#include <iostream>
#include <map>
#include <tasking/lock.hpp>
#include <utils/environment.hpp>
#include <utils/utils.hpp>

// global instance of ZipNET class
static ZipNET* server;

// thread locker
static Lock dispatchLock;

// flag with last render time
static uint64_t renderStart;

/**
 *
 */
int main(int argc, char* argv[]) {
    server = new ZipNET();
    server->launch();
    return 0;
}

/**
 *
 */
void ZipNET::launch() {
    // disable video log
    SetVideoLog(false);

    // initialize the video output
    videoOutput = new VbeVideoOutput_t();
    if ( !videoOutput->initialize() ) {
        std::cerr << "failed to initialize video mode" << std::endl;
        Utils::log("failed to initialize video mode");
        return;
    }

    // set up event handling
    eventProcessor = new EventProcessor_t();
    InputReceiver_t::initialize();

    std::string keyLayout = "it-EU";
    Utils::log("loading keyboard layout '%s'", keyLayout.c_str());
    if ( !Keyboard::loadLayout(keyLayout) )
        Utils::log(
            ("failed to load keyboard layout '" + keyLayout + "', no keyboard input available")
                .c_str());

    // create the cursor
    loadCursor();

    // get resolution
    Dimension resolution = videoOutput->getResolution();
    Rectangle screenBounds(0, 0, resolution.width, resolution.height);

    // instantiate screen object with bounds
    screen = new Screen_t();
    screen->setBounds(screenBounds);

    // add background to screen
    background = new Background_t(screenBounds);
    screen->addChild(background);

    // start registration interface
    RegistrationThread_t* registrationThread = new RegistrationThread_t();
    registrationThread->start();

    // start responder interface
    responderThread = new CommandMessageResponderThread_t();
    responderThread->start();

    // execute the main loop
    mainLoop(screenBounds);
}

/**
 *
 */
void lockCheck() {
    while ( true ) {
        if ( Millis() - renderStart > 6000 )
            Utils::log("window server has frozen");
        Sleep(1000);
    }
}

/**
 *
 */
void ZipNET::mainLoop(Rectangle screenBounds) {
    global.resize(screenBounds.width, screenBounds.height);
    Environment::set("SYSTEM_LEVEL", "interactive");

    CreateThreadN((void*)lockCheck, "lockCheck");

    Cursor_t::nextPosition = Point(screenBounds.width / 2, screenBounds.height / 2);

    // intially set rendering atom
    renderAtom = true;

    uint64_t renderTime;

    while ( true ) {
        renderStart = Millis();
        eventProcessor->processMouseState();
        eventProcessor->process();

        // make the root component resolve all requirements
        screen->resolveRequirement(COMPONENT_REQUIREMENT_UPDATE);
        screen->resolveRequirement(COMPONENT_REQUIREMENT_LAYOUT);
        screen->resolveRequirement(COMPONENT_REQUIREMENT_PAINT);

        // blit the root component to the buffer
        screen->blit(&global, screenBounds, Point(0, 0));

        // paint the cursor
        Cursor_t::paint(&global);

        // blit output
        blit(&global);

        // limit to 60 fps
        renderTime = Millis() - renderStart;
        if ( renderTime < (1000 / 60) )
            Sleep((1000 / 60) - renderTime);

        // wait for next rendering
        AtomicLock(&renderAtom);
    }
}

/**
 *
 */
void ZipNET::blit(Graphics* graphics) {
    Dimension resolution = videoOutput->getResolution();
    Rectangle screenBounds(0, 0, resolution.width, resolution.height);
    Color_t*  buffer = (Color_t*)cairo_image_surface_get_data(graphics->getSurface());

    // get invalid output
    Rectangle invalid = screen->grabInvalid();

    if ( !invalid.width && !invalid.height )
        return;

    videoOutput->blit(invalid, screenBounds, buffer);
}

/**
 *
 */
void ZipNET::loadCursor() {
    if ( !Cursor_t::load("/cfg/gui/cursor/default.cursor") )
        Utils::log("Unable to load default cursor");
    if ( !Cursor_t::load("/cfg/gui/cursor/text.cursor") )
        Utils::log("Unable to load text cursor");
    if ( !Cursor_t::load("/cfg/gui/cursor/resize-ns.cursor") )
        Utils::log("Unable to load resize-ns cursor");
    if ( !Cursor_t::load("/cfg/gui/cursor/resize-ew.cursor") )
        Utils::log("Unable to load resize-ew cursor");
    if ( !Cursor_t::load("/cfg/gui/cursor/resize-nesw.cursor") )
        Utils::log("Unable to load resize-nesw cursor");
    if ( !Cursor_t::load("/cfg/gui/cursor/resize-nwes.cursor") )
        Utils::log("Unable to load resize-nwes cursor");

    Cursor_t::set("default");
    Cursor_t::focusedComponent = screen;
}

/**
 *
 */
Component_t* ZipNET::dispatchUpwards(Component_t* component, Event_t& event) {
    // store when dispatching to parents
    Point        initialPosition;
    Locatable_t* locatable = dynamic_cast<Locatable_t*>(&event);
    if ( locatable )
        initialPosition = locatable->position;

    // check upwards until someone accepts the event
    Component_t* acceptor = component;
    while ( !dispatch(acceptor, event) ) {
        acceptor = acceptor->getParent();
        if ( !acceptor )
            break;

        // restore position on locatable events
        if ( locatable )
            locatable->position = initialPosition;
    }
    return acceptor;
}

/**
 *
 */
bool ZipNET::dispatch(Component_t* component, Event_t& event) {
    dispatchLock.lock();

    bool handled = false;

    if ( component->canHandleEvents() ) {
        Locatable_t* locatable = dynamic_cast<Locatable_t*>(&event);
        if ( locatable != 0 ) {
            Point locationOnScreen = component->getLocationOnScreen();
            locatable->position.x -= locationOnScreen.x;
            locatable->position.y -= locationOnScreen.y;
        }

        handled = component->handle(event);
    }

    dispatchLock.unlock();
    return handled;
}

/**
 *
 */
ZipNET* ZipNET::instance() {
    return server;
}

/**
 *
 */
void ZipNET::triggerRender() {
    renderAtom = false;
}

/**
 *
 */
bool ZipNET::cleanup(Pid process) {
    klog("removing components for process %i", process);

    // get components mapped for process
    std::map<int, Component_t*>* components = ComponentRegistry_t::getProcessMap(process);

    // if component exist set false visible to clear screen
    if ( components ) {
        // remove all process components and set unvisible
        for ( std::pair<const int, Component_t*>& entry : *components ) {
            Component_t* current = entry.second;
            if ( current ) {
                current->setVisible(false);
                ComponentRegistry_t::removeComponent(process, entry.first);
            }
        }

        ComponentRegistry_t::removeProcessMap(process);

        return true;
    }

    return false;
}
