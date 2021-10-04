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

#include "CandyTerminal.hpp"

#include "GuiScreen.hpp"
#include "HeadlessScreen.hpp"

#include <IO/Shell.hh>
#include <signal.h>
#include <unistd.h>
#include <Utils/Utils.hh>

/**
 * Main routine that initializes the terminal. The terminal can run in headless
 * and graphical mode - this is determined by the respective command line
 * argument.
 */
int main(int argc, char* argv[]) {
    // process command line arguments
    bool headless = false;

    for ( int i = 0; i < argc; i++ ) {
        if ( !strcmp("--headless", argv[i]) )
            headless = true;
    }

    CandyTerminal terminal(headless);
    terminal.execute();
}

/**
 * Executes the terminal instance. This initializes the screen, laods the
 * keyboard layout for this process and then starts the actual shell process.
 * Then control is given to the routines that process user input and program
 * output.
 */
void CandyTerminal::execute() {
    // initialize the screen
    initializeScreen();
    if ( !screen ) {
        Utils::log("CandyTerminal: Failed to initialize screen");
        return;
    }
    screen->clean();

    // load keyboard layout
    std::string initialLayout = "it-EU";
    if ( !IO::Keyboard::instance().load_layout(initialLayout) ) {
        Utils::log("CandyTerminal: Failed to load keyboard layout: " + initialLayout);
        return;
    }

    // disable video logging
    s_set_video_log(false);

    // start shell
    startShell();
    if ( shellIN == FD_NONE || shellOUT == FD_NONE || shellERR == FD_NONE )
        return;

    // start in/out routines
    auto outInfo         = new OutputRoutineStartinfo();
    outInfo->threadName  = "CandyTerminal/StandardOutput";
    outInfo->errorOutput = false;
    outInfo->terminal    = this;
    s_create_thread_d((void*)&outputRoutine, outInfo);

    auto errInfo         = new OutputRoutineStartinfo();
    errInfo->threadName  = "CandyTerminal/ErrorOutput";
    errInfo->errorOutput = true;
    errInfo->terminal    = this;
    s_create_thread_d((void*)&outputRoutine, errInfo);

    inputRoutine();
}

/**
 *
 */
void CandyTerminal::initializeScreen() {
    if ( headless ) {
        if ( s_task_get_id("CandyTermText") != -1 ) {
            fprintf(stderr, "CandyTerminal: Can only be executed once when in headless mode");
            return;
        }
        s_task_register_id("CandyTermText");

        screen = new HeadlessScreen();
        return;
    }

    auto guiScreen = new GuiScreen();
    if ( guiScreen->initialize() )
        screen = guiScreen;
    else
        Utils::log("CandyTerminal: Failed to initialize the graphical screen");
}

/**
 *
 */
void CandyTerminal::startShell() {
    // create input & output pipes
    FsPipeStatus stdinStat;
    FileHandle   shellinW;
    FileHandle   shellinR;
    s_pipe_s(&shellinW, &shellinR, &stdinStat);
    if ( stdinStat != FS_PIPE_SUCCESSFUL ) {
        Utils::log("CandyTerminal: Failed to setup stdin pipe for shell");
        return;
    }

    FsPipeStatus stdoutStat;
    FileHandle   shelloutW;
    FileHandle   shelloutR;
    s_pipe_s(&shelloutW, &shelloutR, &stdoutStat);
    if ( stdoutStat != FS_PIPE_SUCCESSFUL ) {
        Utils::log("CandyTerminal: Failed to setup stdout pipe for shell");
        return;
    }

    FsPipeStatus stderrStat;
    FileHandle   shellerrW;
    FileHandle   shellerrR;
    s_pipe_s(&shellerrW, &shellerrR, &stderrStat);
    if ( stderrStat != FS_PIPE_SUCCESSFUL ) {
        Utils::log("CandyTerminal: Failed to setup stderr pipe for shell");
        return;
    }

    // spawn binary
    Pid        outPid;
    FileHandle stdioIN[3];
    stdioIN[0] = shellinR;
    stdioIN[1] = shelloutW;
    stdioIN[2] = shellerrW;
    FileHandle  stdioTarget[3];
    SpawnStatus status = s_spawn_poi("/Bins/MxSh",
                                     "-sh",
                                     "/",
                                     SECURITY_LEVEL_APPLICATION,
                                     &outPid,
                                     stdioTarget,
                                     stdioIN);

    if ( status != SPAWN_STATUS_SUCCESSFUL ) {
        Utils::log("CandyTerminal: Failed to s_spawn mx shell process");
        return;
    }

    shellIN  = shellinW;
    shellOUT = shelloutR;
    shellERR = shellerrR;
}

/**
 *
 */
void CandyTerminal::writeStringToShell(std::string line) {
    const char* lineContent = line.c_str();
    int         lineLength  = strlen(lineContent);

    int written = 0;
    int len     = 0;
    while ( written < lineLength ) {
        len = write(shellIN, &lineContent[written], lineLength - written);

        if ( len <= 0 )
            break;

        written = written + len;
    }
}

/**
 *
 */
void CandyTerminal::writeShellkeyToShell(int shellkey) {
    char buf[3];
    buf[0] = SHELLKEY_SUB;
    buf[1] = shellkey & 0xFF;
    buf[2] = (shellkey >> 8) & 0xFF;
    write(shellIN, &buf, 3);
}

/**
 *
 */
void CandyTerminal::inputRoutine() {
    std::string buffer = "";
    while ( true ) {
        IO::Keyboard::Info readInput = screen->readInput();

        // Default line-buffered input
        if ( inputMode == IO::Shell::MODE_DEFAULT ) {
            if ( readInput.key == "KEY_ENTER" && readInput.m_is_pressed ) {
                if ( echo ) {
                    screenLock.lock();
                    screen->writeChar('\n');
                    screenLock.unlock();
                }

                buffer = buffer + '\n';
                writeStringToShell(buffer);

                buffer = "";
            }

            else if ( (readInput.m_ctrl && readInput.key == "KEY_C")
                      || (readInput.key == "KEY_ESC") ) {
                if ( currentProcess )
                    s_raise_signal(currentProcess, SIGINT);
            }

            else if ( readInput.key == "KEY_BACKSPACE" && readInput.m_is_pressed ) {
                buffer = buffer.size() > 0 ? buffer.substr(0, buffer.size() - 1) : buffer;
                screen->backspace();
            }

            else {
                char chr = IO::Keyboard::instance().char_for_key(readInput);
                if ( chr != -1 ) {
                    buffer = buffer + chr;

                    if ( echo ) {
                        screenLock.lock();
                        screen->writeChar(chr);
                        screenLock.unlock();
                    }
                }
            }

        }

        else if ( inputMode == IO::Shell::MODE_RAW ) {
            if ( readInput.key == "KEY_ENTER" && readInput.m_is_pressed ) {
                writeShellkeyToShell(SHELLKEY_ENTER);

                if ( echo ) {
                    screenLock.lock();
                    screen->writeChar('\n');
                    screenLock.unlock();
                }
            }

            else if ( readInput.key == "KEY_BACKSPACE" && readInput.m_is_pressed ) {
                writeShellkeyToShell(SHELLKEY_BACKSPACE);

                if ( echo ) {
                    screenLock.lock();
                    screen->backspace();
                    screenLock.unlock();
                }
            }

            else if ( readInput.key == "KEY_ARROW_LEFT" && readInput.m_is_pressed )
                writeShellkeyToShell(SHELLKEY_LEFT);
            else if ( readInput.key == "KEY_ARROW_RIGHT" && readInput.m_is_pressed )
                writeShellkeyToShell(SHELLKEY_RIGHT);
            else if ( readInput.key == "KEY_ARROW_UP" && readInput.m_is_pressed )
                writeShellkeyToShell(SHELLKEY_UP);
            else if ( readInput.key == "KEY_ARROW_DOWN" && readInput.m_is_pressed )
                writeShellkeyToShell(SHELLKEY_DOWN);

            else {
                char chr = IO::Keyboard::instance().char_for_key(readInput);
                if ( chr != -1 ) {
                    write(shellIN, &chr, 1);

                    if ( echo ) {
                        screenLock.lock();
                        screen->writeChar(chr);
                        screenLock.unlock();
                    }
                }
            }
        }
    }
}

/**
 *
 */
void CandyTerminal::outputRoutine(OutputRoutineStartinfo* info) {
    s_task_register_id(info->threadName);

    int   buflen = 1024;
    char* buffer = new char[buflen];

    StreamControlStatus status;

    while ( true ) {
        FsReadStatus stat;
        int r = s_read_s(info->errorOutput ? info->terminal->shellERR : info->terminal->shellOUT,
                         buffer,
                         buflen,
                         &stat);

        if ( stat == FS_READ_SUCCESSFUL ) {
            for ( int i = 0; i < r; i++ ) {
                char c = buffer[i];

                // Lock screen and set error color if required
                info->terminal->screenLock.lock();
                info->terminal->processOutputCharacter(&status, info->errorOutput, c);
                info->terminal->screenLock.unlock();
            }
        }

        else
            break;
    }

    // clean up
    delete buffer;
    delete info;
}

/**
 *
 */
void CandyTerminal::processOutputCharacter(StreamControlStatus* status, bool errorStream, char c) {
    if ( status->status == TERMINAL_STREAM_STATUS_TEXT ) {
        // Simple textual output
        if ( c == '\r' )
            return;

        else if ( c == '\t' ) {
            screen->writeChar(' ');
            screen->writeChar(' ');
            screen->writeChar(' ');
            screen->writeChar(' ');
        }

        else if ( c == 27 /* ESC */ )
            status->status = TERMINAL_STREAM_STATUS_LAST_WAS_ESC;

        else {
            int fg = screen->getColorForeground();
            if ( errorStream )
                screen->setColorForeground(SC_RED);

            screen->writeChar(c);

            if ( errorStream )
                screen->setColorForeground(fg);
        }
        // ESC was sent
    }

    else if ( status->status == TERMINAL_STREAM_STATUS_LAST_WAS_ESC ) {
        // must be followed by [ for VT100 sequence // or a Ghost terminal sequence
        if ( c == '[' )
            status->status = TERMINAL_STREAM_STATUS_WITHIN_VT100;

        // otherwise reset
        else if ( c == '{' )
            status->status = TERMINAL_STREAM_STATUS_WITHIN_TERM;

        else
            status->status = TERMINAL_STREAM_STATUS_TEXT;

        // Handle sequence parameters
    }

    else if ( status->status == TERMINAL_STREAM_STATUS_WITHIN_VT100
              || status->status == TERMINAL_STREAM_STATUS_WITHIN_TERM ) {
        // Parameter value
        if ( c >= '0' && c <= '9' ) {
            if ( status->parameterCount == 0 )
                status->parameterCount = 1;

            if ( status->parameterCount <= SHELL_STREAM_CONTROL_MAX_PARAMETERS ) {
                status->parameters[status->parameterCount - 1]
                    = status->parameters[status->parameterCount - 1] * 10;
                status->parameters[status->parameterCount - 1] += c - '0';

                // Illegal number of parameters is skipped
            }

            // Parameter seperator
        }

        else if ( c == ';' ) {
            status->parameterCount++;

            // Finish character
        }

        else {
            status->controlCharacter = c;

            if ( status->status == TERMINAL_STREAM_STATUS_WITHIN_VT100 )
                processVT100Sequence(status);

            else if ( status->status == TERMINAL_STREAM_STATUS_WITHIN_TERM )
                processTermSequence(status);

            // reset status
            status->parameterCount = 0;
            for ( int i = 0; i < SHELL_STREAM_CONTROL_MAX_PARAMETERS; i++ ) {
                status->parameters[i] = 0;
            }
            status->status = TERMINAL_STREAM_STATUS_TEXT;
        }
    }
}

/**
 *
 */
void CandyTerminal::processVT100Sequence(StreamControlStatus* status) {
    switch ( status->controlCharacter ) {
        // Cursor up
        case 'A':
            screen->moveCursor(screen->getCursorX(), screen->getCursorY() - status->parameters[0]);
            break;

            // Cursor down
        case 'B':
            screen->moveCursor(screen->getCursorX(), screen->getCursorY() + status->parameters[0]);
            break;

            // Cursor forward
        case 'C':
            screen->moveCursor(screen->getCursorX() + status->parameters[0], screen->getCursorY());
            break;

            // Cursor back
        case 'D':
            screen->moveCursor(screen->getCursorX() - status->parameters[0], screen->getCursorY());
            break;

            // Mode setting
        case 'm':
            for ( int i = 0; i < status->parameterCount; i++ ) {
                int param = status->parameters[i];

                // Reset
                if ( param == 0 ) {
                    screen->setColorBackground(SC_BLACK);
                    screen->setColorForeground(SC_WHITE);

                    // Foreground color
                }

                else if ( param >= 30 && param < 40 ) {
                    screen->setColorForeground(convertVT100ToScreenColor(param - 30));

                    // Background color
                }

                else if ( param >= 40 && param < 50 ) {
                    screen->setColorBackground(convertVT100ToScreenColor(param - 40));
                }
            }

            break;

            // Reposition cursor
        case 'f':
            screen->moveCursor(status->parameters[1], status->parameters[0]);
            break;

            // Cursor queries
        case 'n':
            // Query position
            if ( status->parameters[0] == 6 ) {
                std::stringstream response;
                response << (char)SHELLKEY_ESC << "[" << screen->getCursorY() << ";"
                         << screen->getCursorX() << "R";
                auto responseStr = response.str();
                write(shellIN, responseStr.c_str(), responseStr.size());
            }
            break;

        // Set scroll area
        case 'r':
            if ( status->parameterCount == 0 )
                screen->setScrollAreaScreen();
            else
                screen->setScrollArea(status->parameters[0], status->parameters[1]);
            break;

            // Scroll Scrolling Region Up
        case 'S':
            screen->scroll(status->parameters[0]);
            break;

            // Scroll Scrolling Region Down
        case 'T':
            screen->scroll(-status->parameters[0]);
            break;
    }
}

/**
 *
 */
void CandyTerminal::processTermSequence(StreamControlStatus* status) {
    switch ( status->controlCharacter ) {
        // Change mode
        case 'm':
            this->inputMode = (IO::Shell::Mode)status->parameters[0];
            break;

            // Change echo
        case 'e':
            this->echo = (status->parameters[0] == 1);
            break;

            // Screen info
        case 'i':
            {
                std::stringstream response;
                response << (char)SHELLKEY_ESC << "{" << screen->getWidth() << ";"
                         << screen->getHeight() << "i";
                writeStringToShell(response.str().c_str());
                break;
            }

            // Put char
        case 'p':
            screen->writeChar(status->parameters[0]);
            break;

            // Process control
        case 'c':
            currentProcess = status->parameters[0];
            break;
    }
}

/**
 *
 */
ScreenColor CandyTerminal::convertVT100ToScreenColor(int color) {
    switch ( color ) {
        case VT100_COLOR_BLACK:
            return SC_BLACK;
        case VT100_COLOR_BLUE:
            return SC_BLUE;
        case VT100_COLOR_CYAN:
            return SC_CYAN;
        case VT100_COLOR_GREEN:
            return SC_GREEN;
        case VT100_COLOR_MAGENTA:
            return SC_MAGENTA;
        case VT100_COLOR_RED:
            return SC_RED;
        case VT100_COLOR_WHITE:
            return SC_WHITE;
        case VT100_COLOR_YELLOW:
            return SC_YELLOW;
    }
    return SC_WHITE;
}
