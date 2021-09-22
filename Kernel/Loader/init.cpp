/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * *
 * * * * * MeetiX OS By MeetiX OS Project [Marco Cicognani] *
 *                                                                                     *
 *         DERIVED FROM THE GHOST OPERATING SYSTEM * This software is derived
 * from the Ghost operating system project,           * written by Max Schlüssel
 * <lokoxe@gmail.com>. Copyright 2012-2017            * https://ghostkernel.org/
 * * https://github.com/maxdev1/ghost *
 *                                                                                     *
 * This program is free software; you can redistribute it and/or * modify it
 * under the terms of the GNU General Public License                         *
 * as published by the Free Software Foundation; either version 2 * of the
 * License, or (char *argumentat your option) any later version. *
 *                                                                                     *
 * This program is distributed in the hope that it will be useful, * but WITHout
 * ANY WARRANTY; without even the implied warranty of                      *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the * GNU General
 * Public License for more details.                                        *
 *                                                                                     *
 * You should have received a copy of the GNU General Public License * along
 * with this program; if not, write to the Free Software * Foundation, Inc., 51
 * Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA      *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * *
 * * * * */

#include "BuildConfig.hpp"
#include "debug/DebugInterface.hpp"
#include "loader.hpp"
#include "system/BiosDataArea.hpp"

#include <logger/logger.hpp>
#include <runtime/constructors.hpp>
#include <system/serial/SerialPort.hpp>
#include <video/ConsoleVideo.hpp>
#include <video/PrettyBoot.hpp>

/**
 * Initialization function, called from the loader assembly. Checks the
 * multiboot magic number and then passes the multiboot structure to the
 * loader for further initialization.
 *
 * @param multibootStruct:		the multiboot structure provided by GRUB
 * @param magicNumber:			the magic number provided by GRUB
 */
extern "C" void initializeLoader(MultibootInformation* multibootStruct, uint32_t magicNumber) {
    // call the arch constructors
    Constructors::call();

    // initialize COM port
    ComPortInformation comPortInfo = biosDataArea->comPortInfo;
    if ( comPortInfo.com1 > 0 ) {
        SerialPort::initializePort(comPortInfo.com1,
                                   false); // Initialize in poll mode
        Logger::enableSerialPortLogging();
        DebugInterface::initialize(comPortInfo.com1);
    }

    else
        Logger::println("%! COM1 port not available for serial debug output", "logger");

    // Clear the console and print the header colored
    if ( PRETTY_BOOT )
        PrettyBoot::enable();
    else
        ConsoleVideo::clear();

    // Clear the console and print the header colored
    ConsoleVideo::clear();
    ConsoleVideo::setColor(32);
    logInfon("Evangelion Loader");
    ConsoleVideo::setColor(0x0F);
    logInfo(" Version %i.%i.%c", L_VERSION_MAJOR, L_VERSION_MINOR, L_VERSION_PATCH);
    logInfo("");

    // check magic number and continue initialization
    logInfo("%! checking magic number", "early");
    if ( magicNumber == MULTIBOOT_BOOTLOADER_MAGIC ) {
        logInfo("%! initializing loader", "early");
        EvaLoader::initialize(multibootStruct);
    }

    else
        logInfo("%! invalid magic number in multiboot struct", "early");
}
