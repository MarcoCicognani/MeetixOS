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

#include "mx.hpp"
#include "parser.hpp"

#include <eva/utils/local.hpp>
#include <io/shell.hpp>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string.h>
#include <string>

// copy of environment object
Environment* environ;

/**
 *
 */
bool readInputLine(std::string& line) {
    Shell::setMode(SHELL_MODE_RAW);
    Shell::setEcho(false);

    int caret = 0;

    while ( true ) {
        int c = Shell::getChar();
        if ( c == -1 )
            return false;

        if ( c == SHELLKEY_BACKSPACE ) {
            if ( line.size() > 0 && caret > 0 ) {
                char deleted = line.at(caret - 1);

                auto pos        = Shell::getCursor();
                auto afterCaret = line.substr(caret);
                line            = line.substr(0, caret - 1) + afterCaret;
                caret--;

                if ( deleted == '\t' ) {
                    pos.x -= 4;
                    afterCaret += "    ";
                }

                else {
                    pos.x--;
                    afterCaret += ' ';
                }
                Shell::setCursor(pos);
                for ( char c : afterCaret )
                    std::cout << c;
                Shell::setCursor(pos);
            }

        }

        else if ( c == '\t' || c == SHELLKEY_STAB /* TODO implement completion */ ) {
            auto pos = Shell::getCursor();
            std::cout << "    ";

            auto afterCaret = line.substr(caret);
            line            = line.substr(0, caret) + '\t' + afterCaret;
            caret++;

            pos.x += 4;
            Shell::setCursor(pos);
            for ( char c : afterCaret ) {
                std::cout << c;
            }
            Shell::setCursor(pos);

        }

        else if ( c == SHELLKEY_ENTER ) {
            std::cout << '\n';
            break;
        }

        else if ( c == SHELLKEY_LEFT ) {
            if ( caret > 0 ) {
                char beforeCaret = line.at(caret - 1);

                caret--;
                if ( beforeCaret == '\t' )
                    Shell::moveCursorBack(4);
                else
                    Shell::moveCursorBack(1);
            }
        }

        else if ( c == SHELLKEY_RIGHT ) {
            if ( caret < line.size() ) {
                char atCaret = line.at(caret);
                caret++;
                if ( atCaret == '\t' )
                    Shell::moveCursorForward(4);
                else
                    Shell::moveCursorForward(1);
            }
        }

        else if ( c < 0x100 ) {
            auto pos = Shell::getCursor();
            std::cout << (char)c;

            auto afterCaret = line.substr(caret);
            line            = line.substr(0, caret) + (char)c + afterCaret;
            caret++;

            pos.x++;
            Shell::setCursor(pos);
            for ( char c : afterCaret )
                std::cout << c;
            Shell::setCursor(pos);
        }
    }

    return true;
}

/**
 *
 */
bool fileExists(std::string path) {
    File_t file;
    if ( (file = Open(path.c_str())) != -1 ) {
        Close(file);
        return true;
    }
    return false;
}

/**
 *
 */
std::string findProgram(std::string cwd, std::string name) {
    // check for match with cwd
    std::string path = cwd + "/" + name;
    if ( fileExists(path) )
        return path;

    // check for full path
    if ( fileExists(name) )
        return name;

    // check for /applications folder
    path = environ->getVariable("PATH") + name;
    if ( fileExists(path) )
        return path;

    // nothing found
    return name;
}

/**
 *
 */
bool handleBuiltin(std::string cwd, ProgramCall* call) {
    if ( call->program == "cd" ) {
        if ( call->arguments.size() == 1 ) {
            auto newdir = call->arguments.at(0);
            SetWorkingDirectory(newdir.c_str());
        } else
            std::cerr << "Usage:\tcd /path/to/target" << std::endl;
        return true;
    }

    if ( call->program == "clear" ) {
        Shell::clear();
        ShellCursorPosition pos;
        pos.x = 0;
        pos.y = 0;
        Shell::setCursor(pos);
        return true;
    }

    if ( call->program == "which" ) {
        if ( call->arguments.size() == 1 )
            std::cout << findProgram(cwd, call->arguments.at(0)) << std::endl;
        else
            std::cerr << "Usage:\twhich command" << std::endl;
        return true;
    }

    return false;
}

/**
 *
 */
void MXShell::shellMode(Environment* env) {
    Shell::setCursor(ShellCursorPosition(0, 0));
    char* cwdbuf = new char[PATH_MAX];

    environ = env;
    environ->setVariable("USER", "user");
    std::string user = environ->getVariable("USER");
    std::string host = environ->getVariable("HOSTNAME");
    std::string dir  = "/Users/" + user;
    SetWorkingDirectory(dir.c_str());

    while ( true ) {
        // print host, user and cwd
        std::cout << (char)27 << "[31m" << host << (char)27 << "[0m";
        std::cout << '@' << (char)27 << "[33m" << user << (char)27 << "[0m" << ':';
        if ( GetWorkingDirectory(cwdbuf) == GET_WORKING_DIRECTORY_SUCCESSFUL )
            std::cout << (char)27 << "[35m" << cwdbuf << (char)27 << "[0m";
        else
            std::cout << "?";

        std::string cwd(cwdbuf);
        std::cout << '>';
        std::cout << (char)27 << "[0m";
        std::flush(std::cout);

        Shell::setCursor(Shell::getCursor());

        std::string line;
        if ( !readInputLine(line) )
            break;

        // switch to normal input mode
        Shell::setMode(SHELL_MODE_DEFAULT);
        Shell::setEcho(true);

        Parser          cmdparser(line);
        PipeExpression* pipeexpr;
        if ( !cmdparser.pipeExpression(&pipeexpr) )
            continue;

        // perform spawning
        File_t previousOutPipeR = -1;
        Pid    firstProcessID   = -1;
        Pid    lastProcessID    = -1;
        bool   success          = false;

        auto numCalls = pipeexpr->calls.size();
        for ( int c = 0; c < numCalls; c++ ) {
            ProgramCall* call = pipeexpr->calls[c];

            // builtin calls (only allowed as single call)
            if ( numCalls == 1 && c == 0 && handleBuiltin(cwd, call) )
                break;

            // concatenate arguments to one argument string
            std::stringstream argstream;
            bool              first = true;
            for ( auto arg : call->arguments ) {
                if ( first )
                    first = false;
                else
                    argstream << (char)CLIARGS_SEPARATOR;
                argstream << arg;
            }

            // create out pipe if necessary
            File_t outPipeW;
            File_t outPipeR;
            if ( numCalls > 1 && c < numCalls - 1 ) {
                FsPipeStatus pipeStat;
                PipeS(&outPipeW, &outPipeR, &pipeStat);

                if ( pipeStat != FS_PIPE_SUCCESSFUL ) {
                    std::cerr << "failed to create output pipe when spawning '" << call->program
                              << "'" << std::endl;
                    // TODO clean up pipes?
                    success = false;
                    break;
                }
            }
            // decide how to set in/out/err file descriptors
            File_t inStdio[3];

            // stderr is always the same
            inStdio[2] = STDERR_FILENO;

            // stdin must be chosen
            if ( c == 0 )
                inStdio[0] = STDIN_FILENO;
            else
                inStdio[0] = previousOutPipeR;

            // IDstdout must be chosen
            if ( (numCalls == 1 && c == 0) || c == numCalls - 1 )
                inStdio[1] = STDOUT_FILENO;
            else
                inStdio[1] = outPipeW;

            // do spawning
            Pid         outPid;
            File_t      outStdio[3];
            SpawnStatus status = SpawnPOI(findProgram(cwd, call->program).c_str(),
                                          argstream.str().c_str(),
                                          cwdbuf,
                                          SECURITY_LEVEL_APPLICATION,
                                          &outPid,
                                          outStdio,
                                          inStdio);

            // check result
            if ( status == SPAWN_STATUS_SUCCESSFUL ) {
                if ( firstProcessID == -1 )
                    firstProcessID = outPid;
                lastProcessID = outPid;
                success       = true;

                // close write end in this process
                Close(outPipeW);

                if ( previousOutPipeR != -1 ) {
                    // close read end of previous pipe in this process
                    Close(previousOutPipeR);
                }

                // remember for next process
                previousOutPipeR = outPipeR;
            }

            else {
                success = false;
                // error during one spawn
                // TODO clean up pipes
                std::cout << (char)27 << "[31m";
                if ( status == SPAWN_STATUS_FORMAT_ERROR )
                    std::cout << call->program << ": invalid binary format" << std::endl;
                else if ( status == SPAWN_STATUS_IO_ERROR )
                    std::cout << call->program << ": command not found" << std::endl;
                else
                    std::cout << call->program << ": unknown error during program execution"
                              << std::endl;
                std::cout << (char)27 << "[0m";
                std::flush(std::cout);
                break;
            }
        }

        if ( success ) {
            // join into the last process
            Shell::setControlProcess(lastProcessID);
            Join(lastProcessID);
            Shell::setControlProcess(0);
        }
    }

    delete cwdbuf;
}
