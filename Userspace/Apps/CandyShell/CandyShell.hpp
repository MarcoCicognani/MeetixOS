/*
 * MeetiX OS By MeetiX OS Project [Marco Cicognani & D. Morandi]
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (char *argumentat your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef __CANDY_SHELL__
#define __CANDY_SHELL__

#include "history.hpp"
#include "login.hpp"
#include "screen.hpp"
#include "standardIO.hpp"

#include <eva.h>
#include <stdint.h>
#include <vector>

using namespace std;

/**
 *
 */
typedef int            ShellInputStatus;
const ShellInputStatus SHELL_INPUT_STATUS_DEFAULT       = 0;
const ShellInputStatus SHELL_INPUT_STATUS_EXIT          = 1;
const ShellInputStatus SHELL_INPUT_STATUS_SCREEN_SWITCH = 2;
const ShellInputStatus SHELL_INPUT_STATUS_SCREEN_CREATE = 3;
const ShellInputStatus SHELL_INPUT_STATUS_LOCK          = 4;
const ShellInputStatus SHELL_INPUT_STATUS_CLEAR         = 5;

#define BUILTIN_COMMAND_CD         "cd "
#define BUILTIN_COMMAND_SLEEP      "sleep "
#define BUILTIN_COMMAND_CLEAR      "clear"
#define BUILTIN_COMMAND_TERM       "shell"
#define BUILTIN_COMMAND_TERM_P     "shell "
#define BUILTIN_COMMAND_TERMS      "tty"
#define BUILTIN_COMMAND_BACKGROUND "& "
#define BUILTIN_COMMAND_KBD_SET    "kb set "
#define BUILTIN_COMMAND_KBD_INFO   "kb info"
#define BUILTIN_COMMAND_GETHOUR    "get time"
#define BUILTIN_COMMAND_SET_COLOR  "color "
#define BUILTIN_COMMAND_SCANCODE   "scancode"
#define BUILTIN_COMMAND_LOGOUT     "logout"
#define BUILTIN_COMMAND_LOCK       "lock"
#define BUILTIN_COMMAND_HELP       "help"
#define BUILTIN_COMMAND_SHUTDOWN   "shutdown"
#define BUILTIN_COMMAND_REBOOT     "reboot"
#define BUILTIN_COMMAND_EXIT       "exit"
#define BUILTIN_COMMAND_GET_UPTIME "uptime"

/**
 *
 */
typedef struct {
    int number;
} CreateShellInfo;

extern int shellIndex;

/**
 *
 */
class CandyShell {
public:
    CandyShell();

    /**
     * Sets up the shell, loading the default keyboard layout
     */
    static void prepare();

    /**
     * Creates a shell.
     */
    static void createShell(CreateShellInfo* inf);

    /**
     * Main loop of the shell. Reads the entered commands, tries to
     * handle them as built-in commands and otherwise tries to execute the
     * executable in the current directory.
     */
    void run(CreateShellInfo* inf);

    /**
     * Tries to handle the command as a built-in command.
     *
     * @param command
     * 		the command to handle
     *
     * @return whether the command was a builtin command
     */
    bool handleBuiltin(string command);

    /**
     *
     */
    void runCommand(string command);
    bool
    runTermCommand(string command, File_t* termIn, File_t* termOut, File_t* termErr, Pid* intPid);

    /**
     *
     */
    bool execute(string shortpath, string args, Pid* outPid, File_t outStdio[3], File_t inStdio[3]);

    SetWorkingDirectoryStatus writeWorkingDirectory();
    void                      readWorkingDirectory();

    static void switchToNext();
    static void switchTo(CandyShell* shell);
    string      readInput(string            there,
                          Screen*           screen,
                          ShellInputStatus* outStatus,
                          bool*             continueInput,
                          bool              visible = true);

    static void standardInThread(StandardInThreadData* data);
    static void standardOutThread(StandardOutThreadData* data);

    bool fileExists(string path);
    bool findExecutable(string path, string& out);

    static void exit(Screen* dsp);

private:
    string  homeDirectory;
    string  workingDirectory;
    Screen* screen;
    uint8_t inactive;

    string hostname;
    string currentUser;

    /**
     * Function used as the main entry point for a shell instance.
     */
    static void shellStartRoutine(CreateShellInfo* inf);

    static Screen* addScreen();
};

#endif
