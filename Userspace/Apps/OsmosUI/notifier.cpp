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

#include "SecondaryThread.hpp"

#include <LibGUI/Component/ComponentRegistry.hh>
#include <LibGUI/Notification.hh>
#include <LibUtils/Environment.hh>

using namespace std;

// number of popup on screen
uint32_t popupOnScreen = 0;

// popup size
uint32_t boxWidth  = 350;
uint32_t boxheight = 100;

/*
 * create animation for popup with provided animation type
 */
static void showPopupAnimation(Geoshape* popup, MessageAnimation_t animation, bool first) {
    if ( animation == MessageAnimation_t::DARKENING ) {
        // set limits with bolean flag
        int start = first ? 10 : 150;
        int end   = first ? 150 : 10;

        // do animation
        for ( int index = start; index < end; index++ )
            if ( !(index % 2) )
                popup->set_color(Graphics::Color::as_argb(index, 0, 0, 0),
                                 Graphics::Color::as_argb(255, 255, 255, 255));
    }

    else if ( animation == MessageAnimation_t::SIDE_SCROLL ) {
        // get bounds from popup
        auto bounds = popup->bounds();
        popup->set_color(Graphics::Color::as_argb(150, 0, 0, 0), Graphics::Color::as_argb(255, 255, 255, 255));

        // set limits with bolean flag
        int start = first ? bounds.x() + bounds.width() : bounds.x();
        int end   = first ? bounds.x() : bounds.x() + bounds.width();

        // do animation
        for ( int index = start; index < end; index++ )
            if ( !(index % 6) )
                popup->set_bounds({ index, bounds.y(), bounds.width(), bounds.height() });
    }
}

/*
 * show on user interface the popup with message
 */
static void showOnScreen(Notification_t* msg) {
    // create popup only if message exist and display time is major than 0
    if ( msg->message[0] && msg->displayTime ) {
        // get screen dimension
        auto resolution = UI::screen_dimension();

        // create the popup and the components
        Geoshape* popup             = Geoshape::create();
        Label*    messageContenitor = Label::create();

        // set bounds
        uint32_t xPos = resolution.width() - boxWidth - 5;
        uint32_t dist = popupOnScreen == 1 ? 35 : 5 * popupOnScreen;
        uint32_t yPos = resolution.height() - dist - boxheight * popupOnScreen;
        popup->set_bounds({ static_cast<i32>(xPos),
                            static_cast<i32>(yPos),
                            static_cast<i32>(boxWidth),
                            static_cast<i32>(boxheight) });
        messageContenitor->set_bounds({ 0, 0, static_cast<i32>(boxWidth), static_cast<i32>(boxheight) });

        // set title
        popup->set_title(msg->title);

        // set text message
        messageContenitor->set_title_alignment(Graphics::Text::Alignment::CENTER);
        messageContenitor->set_color(0, msg->color);
        messageContenitor->set_title(msg->message);
        popup->add_child(messageContenitor);

        // show popup with animation
        popup->set_visible(true);
        showPopupAnimation(popup, msg->animation, true);

        // wait for provided time
        s_sleep(msg->displayTime * 1000);

        // delete popup with animation
        showPopupAnimation(popup, msg->animation, false);

        // destroy interfaces
        delete messageContenitor;
        delete popup;
        popupOnScreen--;
    }
}

/*
 * receive and the notification from other user application
 */
void SecondaryThread::NotificationThread() {
    // create buffer for message
    size_t  length = sizeof(MessageHeader) + sizeof(Notification_t);
    uint8_t buffer[length];

    // mainloop
    while ( true ) {
        // receive incoming request
        MessageReceiveStatus stat = s_receive_message(buffer, length);

        // start notification only if there aren't errors
        if ( stat == MESSAGE_RECEIVE_STATUS_SUCCESSFUL ) {
            // read message and begin thread
            popupOnScreen++;
            Notification_t* incoming = (Notification_t*)MESSAGE_CONTENT(buffer);
            s_create_thread_d((void*)showOnScreen, incoming);
        }
    }
}
