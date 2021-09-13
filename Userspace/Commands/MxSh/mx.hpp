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

#ifndef _MX_SHELL_
#define _MX_SHELL_

#include "interpreter.hpp"
#include "mxscript.hpp"

#include <string>
#include <vector>

/*
 *	Shell Version
 */
#define VERSION_MAJOR "2."
#define VERSION_MINOR ".0"
#define VERSION_PATCH ".0"

/*
 *	enumeration class that contain operative mode
 */
enum OperativeMode
{
    SHELL_MODE_SHELL,
    SHELL_MODE_SHELL_SHORT,

    SHELL_MODE_SCRIPT,
    SHELL_MODE_SCRIPT_SHORT,

    SHELL_MODE_ENVIRONMENT_GET,
    SHELL_MODE_ENVIRONMENT_GET_SHORT,

    SHELL_MODE_ENVIRONMENT_SET,
    SHELL_MODE_ENVIRONMENT_SET_SHORT,

    SHELL_MODE_COMMAND,
    SHELL_MODE_COMMAND_SHORT,

    SHELL_MODE_VERSION,
    SHELL_MODE_VERSION_SHORT,

    SHELL_MODE_HELP,
    SHELL_MODE_HELP_SHORT,

    SHELL_MODE_NULL,
};

/*
 * Main class of MeetiX OS Shell
 */
class MXShell {
private:
    // operation mode flag
    OperativeMode mode;

    // list that contain supported command
    std::vector<std::string> commands;

    // command provided from launcher
    std::string command;

    // argument provided from launcher
    std::string argument;

    // object to environment class
    Environment* environment;

    // object to script interpreter
    MXinterpreter* interpreter;

    // object to script parser
    MXScriptParser* scriptParser;

public:
    // constructor
    MXShell() {
        commands = { "--shell",   "-sh", "--script",  "-s", "--getenv", "-e", "--setenv", "-se",
                     "--command", "-c",  "--version", "-v", "--help",   "-h" };
    }

    // destructor
    ~MXShell() {
        delete environment;
        delete interpreter;
        delete scriptParser;
    }

    /*
     *	load the environment and parse arguments
     */
    void initialize(int argc, char* argv[]);

    /*
     * load the environments variables from environment file "/etc/var/env"
     */
    void loadEnvironment();

    /*
     * parse arguments and return operative mode
     */
    OperativeMode parseArguments(int argc, char* argv[]);

    /*
     *	start shell
     */
    void startRoutine();

    /*
     *	effective shell (shell.cpp)
     */
    void shellMode(Environment* env);

    /*
     * script mode
     */
    void scriptMode();

    /*
     * environment mode
     */
    void environmentMode(std::string mode);

    /*
     * single command
     */
    void commandMode();

    /*
     * write help to screen
     */
    void explainHelp();
};

#endif
