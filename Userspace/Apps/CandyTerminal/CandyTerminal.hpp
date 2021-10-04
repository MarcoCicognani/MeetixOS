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

#ifndef _CANDY_TERMINAL_
#define _CANDY_TERMINAL_

#include "screen.hpp"
#include "StreamStatus.hpp"

#include <Api.h>
#include <IO/Shell.hh>
#include <Tasking/Lock.hh>

class CandyTerminal;

/**
 * Information struct used to pass information to the output thread.
 */
struct OutputRoutineStartinfo {
    const char*    threadName;
    bool           errorOutput;
    CandyTerminal* terminal;
};

/**
 *
 */
class CandyTerminal {
private:
    FileHandle shellIN;
    FileHandle shellOUT;
    FileHandle shellERR;

    /**
     * Contains whether the terminal is run in headless mode or GUI mode. Only
     * one headless terminal may exist at once.
     */
    bool headless;

    /**
     * Screen that is the visual interface to the user.
     */
    Screen*       screen;
    Tasking::Lock screenLock;

    /**
     * Mode flags
     */
    bool            echo;
    IO::Shell::Mode inputMode;

    /**
     * ID of the currently controlled process
     */
    Pid currentProcess;

public:
    /**
     *
     */
    CandyTerminal(bool headless)
        : headless(headless), screen(nullptr), shellIN(FD_NONE), shellOUT(FD_NONE), shellERR(FD_NONE),
          echo(true), inputMode(IO::Shell::MODE_DEFAULT), currentProcess(-1) {
    }

    /**
     * Starts the terminal application.
     */
    void execute();

    /**
     * Initializes the terminals screen. If successful, the terminals screen property
     * is set to the new screen, otherwise it remains null.
     */
    void initializeScreen();

    /**
     *	provide a login screen if headless mode is on
     */
    void headLessLogin(bool headless);

    /**
     * Starts the shell process, mapping the in/out/error pipes to this thread.
     */
    void startShell();

    /**
     *
     */
    void writeStringToShell(std::string line);

    /**
     *
     */
    void writeShellkeyToShell(int shellkey);

    /**
     * Thread that continuously reads the keyboard input, processes and redirects
     * it to the shell.
     */
    void inputRoutine();

    /**
     * Thread that reads the output of the executing program and processes it.
     */
    static void outputRoutine(OutputRoutineStartinfo* info);

    /**
     *
     */
    void processOutputCharacter(StreamControlStatus* status, bool errorStream, char c);
    void processVT100Sequence(StreamControlStatus* status);
    static ScreenColor convertVT100ToScreenColor(int color);
    void               processTermSequence(StreamControlStatus* status);
};

#endif
