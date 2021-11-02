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

#include <Api/utils/local.hpp>
#include <cstdio>
#include <IO/Shell.hh>
#include <iostream>
#include <string>

Environment*                       g_shell_env;
std::vector<std::string>           g_shell_history{};
std::vector<std::string>::iterator g_shell_history_it{};

bool read_input_line(std::string& line) {
    IO::Shell::instance().set_mode(IO::Shell::MODE_RAW);
    IO::Shell::instance().set_echo(false);

    int caret = 0;

    while ( true ) {
        int c = IO::Shell::instance().read_char();
        if ( c == -1 )
            return false;

        if ( c == SHELLKEY_BACKSPACE ) {
            if ( !line.empty() && caret > 0 ) {
                char deleted = line.at(caret - 1);

                auto pos        = IO::Shell::instance().cursor();
                auto afterCaret = line.substr(caret);
                line            = line.substr(0, caret - 1).append(afterCaret);
                caret--;

                if ( deleted == '\t' ) {
                    pos.m_x -= 4;
                    afterCaret += "    ";
                }

                else {
                    pos.m_x--;
                    afterCaret += ' ';
                }
                IO::Shell::instance().set_cursor(pos);
                for ( auto cc : afterCaret )
                    std::cout << cc;
                IO::Shell::instance().set_cursor(pos);
            }
        } else if ( c == '\t' || c == SHELLKEY_STAB /* TODO implement completion */ ) {
            auto pos = IO::Shell::instance().cursor();
            std::cout << "    ";

            auto afterCaret = line.substr(caret);
            line            = line.substr(0, caret).append("\t").append(afterCaret);
            caret++;

            pos.m_x += 4;
            IO::Shell::instance().set_cursor(pos);
            for ( auto cc : afterCaret )
                std::cout << cc;
            IO::Shell::instance().set_cursor(pos);
        } else if ( c == SHELLKEY_ENTER ) {
            std::cout << '\n';
            g_shell_history.push_back(line);
            break;
        } else if ( c == SHELLKEY_LEFT ) {
            if ( caret > 0 ) {
                char beforeCaret = line.at(caret - 1);

                caret--;
                if ( beforeCaret == '\t' )
                    IO::Shell::instance().move_cursor_back(4);
                else
                    IO::Shell::instance().move_cursor_back(1);
            }
        } else if ( c == SHELLKEY_RIGHT ) {
            if ( caret < line.size() ) {
                char atCaret = line.at(caret);
                caret++;
                if ( atCaret == '\t' )
                    IO::Shell::instance().move_cursor_forward(4);
                else
                    IO::Shell::instance().move_cursor_forward(1);
            }
        } else if ( c == SHELLKEY_UP ) {
            if ( g_shell_history_it == g_shell_history.end() )
                g_shell_history_it = g_shell_history.begin();

            line = *g_shell_history_it++;
        } else if ( c < 0x100 ) {
            auto pos = IO::Shell::instance().cursor();
            std::cout << (char)c;

            auto afterCaret = line.substr(caret);
            line            = line.substr(0, caret) + static_cast<char>(c) + afterCaret;
            caret++;

            pos.m_x++;
            IO::Shell::instance().set_cursor(pos);
            for ( auto cc : afterCaret )
                std::cout << cc;
            IO::Shell::instance().set_cursor(pos);
        }
    }

    return true;
}

/**
 *
 */
bool fileExists(const std::string& path) {
    FileHandle file;
    if ( (file = s_open(path.c_str())) != -1 ) {
        s_close(file);
        return true;
    }
    return false;
}

/**
 *
 */
std::string findProgram(const std::string_view& cwd, const std::string& name) {
    // check for match with cwd
    std::string path{ cwd };
    path += "/" + name;
    if ( fileExists(path) )
        return path;

    // check for full path
    if ( fileExists(name) )
        return name;

    // check for /applications folder
    path = g_shell_env->getVariable("PATH") + name;
    if ( fileExists(path) )
        return path;

    // nothing found
    return name;
}

/**
 *
 */
bool handleBuiltin(const std::string_view& cwd, ProgramCall* call) {
    if ( call->program == "cd" ) {
        if ( call->arguments.size() == 1 ) {
            auto set_work_dir_status = s_set_working_directory(call->arguments.at(0).c_str());
            if ( set_work_dir_status != SET_WORKING_DIRECTORY_SUCCESSFUL )
                std::cerr << "cd: Failed to set working directory" << std::endl;
        } else
            std::cerr << "Usage:\tcd /path/to/target" << std::endl;
        return true;
    }

    if ( call->program == "clear" ) {
        IO::Shell::instance().clear();
        IO::Shell::CursorPosition pos;
        IO::Shell::instance().set_cursor(pos);
        std::cout << SHELLKEY_ESC << "[H" << std::flush;
        return true;
    }

    if ( call->program == "which" ) {
        if ( call->arguments.size() == 1 )
            std::cout << findProgram(cwd, call->arguments.at(0)) << std::endl;
        else
            std::cerr << "Usage:\twhich command" << std::endl;
        return true;
    }

    if ( call->program == "exit" ) {
        exit(EXIT_SUCCESS);
    }

    return false;
}

void run_command(const std::string& line, const std::string_view& cwd) {
    Parser          cmd_parser{ line };
    PipeExpression* pipe_expression;
    if ( !cmd_parser.pipeExpression(&pipe_expression) )
        return;

    // perform spawning
    FileHandle previousOutPipeR = -1;
    Pid        firstProcessID   = -1;
    Pid        lastProcessID    = -1;
    bool       success          = false;

    auto numCalls = pipe_expression->calls.size();
    for ( int c = 0; c < numCalls; c++ ) {
        auto call = pipe_expression->calls[c];

        // builtin calls (only allowed as single call)
        if ( numCalls == 1 && c == 0 && handleBuiltin(cwd, call) )
            break;

        // concatenate arguments to one argument string
        std::stringstream args_ss;
        bool              is_first_arg = true;
        for ( auto& arg : call->arguments ) {
            if ( is_first_arg )
                is_first_arg = false;
            else
                args_ss << " ";
            // args_ss << (char)CLIARGS_SEPARATOR;

            args_ss << arg;
        }

        // create out pipe if necessary
        FileHandle outPipeW;
        FileHandle outPipeR;
        if ( numCalls > 1 && c < numCalls - 1 ) {
            FsPipeStatus pipeStat;
            s_pipe_s(&outPipeW, &outPipeR, &pipeStat);

            if ( pipeStat != FS_PIPE_SUCCESSFUL ) {
                std::cerr << "failed to create output pipe when spawning '" << call->program << "'"
                          << std::endl;
                // TODO clean up pipes?
                success = false;
                break;
            }
        }
        // decide how to set in/out/err file descriptors
        FileHandle inStdio[3];

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
        FileHandle  outStdio[3];
        SpawnStatus status = s_spawn_poi(findProgram(cwd, call->program).c_str(),
                                         args_ss.str().c_str(),
                                         cwd.data(),
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
            s_close(outPipeW);

            if ( previousOutPipeR != -1 ) {
                // close read end of previous pipe in this process
                s_close(previousOutPipeR);
            }

            // remember for next process
            previousOutPipeR = outPipeR;
        } else {
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
        IO::Shell::instance().set_control_process(lastProcessID);
        s_join(lastProcessID);
        IO::Shell::instance().set_control_process(0);
    }
}

/**
 *
 */
void MXShell::shellMode(Environment* env) {
    g_shell_env = env;

    IO::Shell::instance().set_cursor(IO::Shell::CursorPosition(0, 0));
    IO::Shell::instance().set_mode(IO::Shell::MODE_DEFAULT);
    IO::Shell::instance().set_echo(true);

    /* run the login */
    run_command({ "/Bins/Login" }, { "/" });

    /* reload the environment */
    s_sleep(3000);
    g_shell_env->load();
    auto user = g_shell_env->getVariable("USER");
    auto dir  = "/Users/" + g_shell_env->getVariable("USER");
    s_set_working_directory(dir.c_str());

    Local<char> work_dir_buffer{ new char[PATH_MAX] };
    while ( true ) {
        // print host, user and cwd
        std::cout << "\033[31m" << g_shell_env->getVariable("HOSTNAME") << "\033[0m";
        std::cout << '@' << "\033[33m" << user << "\033[0m" << ':';
        if ( s_get_working_directory(work_dir_buffer()) == GET_WORKING_DIRECTORY_SUCCESSFUL )
            std::cout << "\033[35m" << work_dir_buffer() << "\033[0m";
        else
            std::cout << "?";

        std::string_view work_dir{ work_dir_buffer() };
        std::cout << '>';
        std::cout << "\033[0m" << std::flush;

        IO::Shell::instance().set_cursor(IO::Shell::instance().cursor());

        std::string line;
        if ( !read_input_line(line) )
            break;

        // switch to normal input mode
        IO::Shell::instance().set_mode(IO::Shell::MODE_DEFAULT);
        IO::Shell::instance().set_echo(true);

        run_command(line, work_dir);
    }
}
