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

#include "eva/stdint.h"
#include "EvangelionNG.hpp"

#include <BuildConfig.hpp>
#include <kernelloader/SetupInformation.hpp>
#include <logger/logger.hpp>
#include <runtime/constructors.hpp>
#include <system/serial/SerialPort.hpp>
#include <video/ConsoleVideo.hpp>
#include <video/PrettyBoot.hpp>

/**
 * Does the final loading preparation and starts the kernel.
 *
 * @param setupInformation		the setup information passed by the loader
 */
extern "C" void loadKernel(SetupInformation* setupInformation) {
    // construct constructors
    Constructors::call();

    // Set video output
    if ( PRETTY_BOOT )
        PrettyBoot::enable(false);
    else
        ConsoleVideo::clear();

    // Call the kernel
    EvaKernel::run(setupInformation);

    // Hang after execution
    logInfo("%! execution finished, halting", "EvaKernel");

    asm("cli");
    for ( ;; )
        asm("hlt");
}
