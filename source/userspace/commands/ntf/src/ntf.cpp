/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * *
* MeetiX OS By MeetiX OS Project [Marco Cicognani & D. Morandi]                       *
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
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * */

#include <gui/ui.hpp>
#include <gui/notification.hpp>
#include <utils/arguments.hpp>
#include <string.h>
#include <stdio.h>
#include <string>

/**
 * show the usage of the command
 *
 * @param name:     the name of the command, usually the argv[0]
 */
void usage(char *name)
{
    printf("Send a graphic notification on the desktop\n");
    printf("\tusage: %s \"title\" \"message\"\n", name);
}

/**
 * main of the command
 */
int main(int argc, char *argv[])
{
    // check arguments
    if (argc >= 2)
    {
        // open the connection to the ui
        if (UI::open() == UI_OPEN_STATUS_SUCCESSFUL)
        {
            std::string title;
            std::string content;

            // title have the ""
            if (argv[1][0] == '"')
            {
                uint32_t length = strlen(argv[1]);
                for (int i = 1; i < length - 1; i++)
                    title += argv[1][i];
            }

            else title = argv[1];

            // content have the ""
            if (argv[2][0] == '"')
            {
                uint32_t length = strlen(argv[1]);
                for (int i = 1; i < length - 1; i++)
                    content += argv[2][i];
            }

            else content = argv[2];

            // send the notification
            Notification::send(title, content);
        }
    }

    // show usage otherwise
    else usage(argv[0]);
}
