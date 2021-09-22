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

#ifndef MEETIX_LIBRARY_UI_NOTIFICATION
#define MEETIX_LIBRARY_UI_NOTIFICATION

#include <Api.h>
#include <graphics/color.hpp>
#include <string>

// max length of title
#define MAX_TITLE_LENGTH 512

// max legnth of single message
#define MAX_MESSAGE_LENGTH 1024

// thread identifier of notifierThread
#define NOTIFICATION_THREAD "OsmosUI::notifier"

/*
 * message animation
 */
enum class MessageAnimation_t : uint8_t
{
    DARKENING,
    SIDE_SCROLL,
};

/*
 * message descriptor
 */
typedef struct {
    // id of application that send message
    char title[MAX_TITLE_LENGTH];

    // message to show
    char message[MAX_MESSAGE_LENGTH];

    // time to display message on screen
    uint32_t displayTime;

    // color of message
    Color_t color;

    // animation type
    MessageAnimation_t animation;
} A_PACKED Notification_t;

/*
 * static class to send message notification on user interface
 */
class Notification {
private:
    Notification() = delete;

public:
    /*
     * reduced parameters version
     */
    static void send(const std::string& title, const std::string& message);
    static void send(const std::string& title, const std::string& message, uint32_t sleep);
    static void
    send(const std::string& title, const std::string& message, uint32_t sleep, Color_t color);

    /*
     * send message to notification thread of UI
     * @param title: string with title of notification
     * @param message: string to show on popup
     * @param sleep: time that the message remain on screen
     * @prama color: color of message showed
     */
    static void send(const std::string& title,
                     const std::string& message,
                     uint32_t           sleep,
                     Color_t            color,
                     MessageAnimation_t animation);
};

#endif
