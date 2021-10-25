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

#ifndef __WINDOWSERVER__
#define __WINDOWSERVER__

#include "components/background.hpp"
#include "components/component.hpp"
#include "components/screen.hpp"
#include "events/EventProcessor.hpp"
#include "interface/CommandMessageResponderThread.hpp"
#include "output/VideoOutput.hpp"

#include <Graphics/Context.hh>

/**
 *
 */
class ZipNET {
public:
    VideoOutput_t*                 videoOutput;
    EventProcessor*                eventProcessor;
    Screen_t*                      screen;
    Background_t*                  background;
    CommandMessageResponderThread* m_responder_thread;
    bool                           renderAtom;
    Graphics::Context              global;

    /**
     * Sets up the windowing system by configuring a video output, setting up the
     * event processor and running the main loop. Each step of the main loop includes
     * a event handling and rendering sequence.
     */
    void launch();

    /**
     *
     */
    [[noreturn]] void mainLoop(Graphics::Metrics::Rectangle screenBounds);

    /**
     * Blits the component state.
     */
    void blit(Graphics::Context* graphics);

    /**
     * Dispatches the given event to the component.
     *
     * @return whether the event was handled
     */
    bool dispatch(Component_t* component, Event_t& event);

    /**
     * Dispatches the given event upwards the component tree.
     */
    Component_t* dispatchUpwards(Component_t* component, Event_t& event);

    /**
     * Returns the singleton instance of the window server.
     *
     * @return the instance
     */
    static ZipNET* instance();

    /**
     *
     */
    void loadCursor();

    /**
     *
     */
    void triggerRender();

};

#endif
