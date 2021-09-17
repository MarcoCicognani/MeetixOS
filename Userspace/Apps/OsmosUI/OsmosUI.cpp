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

#include "OsmosUI.hpp"

#include <utils/environment.hpp>
#include <utils/utils.hpp>

/**
 *	main of application
 */
int main(int argc, char* argv[]) {
    // creating object
    OsmosUI* ui = new OsmosUI();

    // initialize graphic comunication
    if ( ui->init() ) {
        // get resolution from windowserver
        Dimension resolution = UI::getResolution();

        auto user_name = Environment::getLoggedUser();
        user_name[0] = static_cast<char>(toupper(user_name[0]));

        // configure ui with script
        ui->configureUi("/Users/" + user_name + "/Desktop/.ui.cfg", resolution);
        ui->createComponents();

        // main loop
        ui->mainLoop();
    }

    // print error message on log
    else
        Utils::log("CandyGui: Unable to comunicate with zipNET");

    // delete object
    delete ui;

    return 0;
}
