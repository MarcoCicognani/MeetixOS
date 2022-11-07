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

#include <logger/logger.hpp>
#include <tasking/process.hpp>
#include <utils/string.hpp>

/**
 * creates a new empty process descriptor
 *
 * @param securityLevel:	the security level of the new process
 */
Process::Process(SecurityLevel securityLevel) : securityLevel(securityLevel) {
    parent = 0;
    main   = 0;

    pageDirectory = 0;

    imageStart = 0;
    imageEnd   = 0;
    heapStart  = 0;
    heapBreak  = 0;
    heapPages  = 0;

    // empty string fields
    cliArguments = 0;
    sourcePath   = 0;

    // set default working directory
    workingDirectory    = new char[PATH_MAX];
    workingDirectory[0] = '/';
    workingDirectory[1] = 0;

    tlsMasterInProcLocation = 0;
    tlsMasterCopysize       = 0;
    tlsMasterTotalsize      = 0;
    tlsMasterAlignment      = 0;

    isServer = false;
}

/**
 * process destructor
 */
Process::~Process() {
    if ( cliArguments )
        delete cliArguments;
    if ( sourcePath )
        delete sourcePath;
    if ( workingDirectory )
        delete workingDirectory;
    virtualRanges.clear();
}

/**
 * copy the provided path to the internal source path property
 *
 * @param path:		the path of the process
 */
void Process::setPath(const char* path) {
    if ( !sourcePath )
        sourcePath = new char[StringUtils::length(path) + 1];
    StringUtils::copy(sourcePath, path);
}
