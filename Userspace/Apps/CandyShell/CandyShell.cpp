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

#include "CandyShell.hpp"

#include "GUIScreen.hpp"
#include "HeadlessScreen.hpp"

#include <Api.h>
#include <dirent.h>
#include <fstream>
#include <GUI/Application.hh>
#include <iomanip>
#include <iostream>
#include <LibIO/Keyboard.hh>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <Utils/Environment.hh>
#include <Utils/Utils.hh>
#include <vector>

using namespace std;

static vector<CandyShell*> tty;
static vector<Tid>         candyShellThreads;
static CandyShell*         activeShell = 0;

int         shellIndex = 0;
static bool headless;

static Login*   login;
static History* history;

/**
 *
 */
int main(int argc, char* argv[]) {
    // check for headless argument
    headless = (!strcmp("--headless", argv[1]));

    // headless may only run once
    if ( headless ) {
        if ( s_task_get_id("CandyText") != -1 ) {
            cerr << "error: CandyShell can only be executed once when in headless mode" << endl;
            Utils::log("error: CandyShell can only be executed once when in headless mode");
            return -1;
        }
    }
    // if not headless, init gui
    if ( !headless ) {
        UiOpenStatus status = UI::open();

        if ( status != UI_OPEN_STATUS_SUCCESSFUL ) {
            Utils::log("failed to open UI for CandyShell with status " + status);
            return -1;
        }
    }

    // initialize general settings
    CandyShell::prepare();

    // if headless start login thread
    if ( headless ) {
        login = new Login(); // instantiate object

        Tid LoginThread = s_create_thread((void*)&Login::LoginEntry); // create thread and get Tid

        s_join(LoginThread); // join to thread
    }

    // instantiate a shell
    CreateShellInfo* inf = new CreateShellInfo();
    inf->number          = shellIndex++;
    CandyShell::createShell(inf);

    // join all tty
    while ( candyShellThreads.size() > 0 ) {
        s_join(candyShellThreads[0]);
    }

    delete inf;
    delete login;
    delete history;
}

/**
 * Prepares the shell application for execution. Loads the current keyboard
 * layout and disables video s_log to avoid interfering with headless mode.
 */
void CandyShell::prepare() {
    // register as the shell task when headless
    if ( headless ) {
        s_task_register_id("CandyText");
        Utils::log("initializing headless CandyShell");
        Utils::Environment::set("SYSTEM_LEVEL", "textual");

        // disable video logging
        s_set_video_log(false);
    }

    else
        Utils::log("initializing graphic CandyShell");

    // load keyboard layout
    string initialLayout = "it-EU";
    if ( IO::Keyboard::instance().load_layout(initialLayout) )
        Utils::log("keyboard layout '" + initialLayout + "' loaded");

    else {
        Utils::log("unable to load keyboard layout '" + initialLayout + "'");
        return;
    }
}

/**
 * Creates a new shell by starting a thread.
 */
void CandyShell::createShell(CreateShellInfo* inf) {
    history = new History();

    candyShellThreads.push_back(s_create_thread_d((void*)&CandyShell::shellStartRoutine, (void*)inf));
}

/*
 * call closer for screen, deallocate pointers and kill candyshell
 */
void CandyShell::exit(Screen* dsp) {
    // closing screen
    dsp->close();

    // deleting all pointers
    delete login;
    delete history;

    // kill itself
    s_kill(s_get_pid());
}

/**
 *
 */
void CandyShell::shellStartRoutine(CreateShellInfo* inf) {
    stringstream threadName;
    threadName << "tty" << inf->number;

    string name = threadName.str();

    s_task_register_id(name.c_str());

    // create new shell
    CandyShell* shell = new CandyShell();
    tty.push_back(shell);

    // copy working directory
    if ( activeShell )
        shell->workingDirectory = activeShell->workingDirectory;

    // switch screen to new shell
    switchTo(shell);

    // run shell in this thread
    shell->run(inf);
}

/**
 *
 */
CandyShell::CandyShell()
    : inactive(false) {
    if ( headless )
        screen = new HeadlessScreen();
    else
        screen = new GUIScreen();
}

/**
 *
 */
void CandyShell::switchToNext() {
    CandyShell* next = 0;

    bool foundCurrent = false;
    for ( CandyShell* other : tty ) {
        if ( other == activeShell )
            foundCurrent = true;

        else if ( foundCurrent ) {
            next = other;
            break;
        }
    }

    if ( next == 0 )
        next = tty[0];

    switchTo(next);
}

/**
 *
 */
void CandyShell::switchTo(CandyShell* shell) {
    if ( activeShell ) {
        activeShell->inactive = true;
        activeShell->screen->deactivate();
        activeShell->readWorkingDirectory();
    }

    activeShell = shell;

    activeShell->writeWorkingDirectory();
    activeShell->inactive = false;
    activeShell->screen->activate();
    activeShell->screen->updateCursor();
}

/**
 *
 */
SetWorkingDirectoryStatus CandyShell::writeWorkingDirectory() {
    return s_set_working_directory(workingDirectory.c_str());
}

/**
 *
 */
void CandyShell::readWorkingDirectory() {
    char* buf = new char[PATH_MAX];
    s_get_working_directory(buf);

    workingDirectory = string(buf);

    delete buf;
    screen->workingDirectoryChanged(workingDirectory);
}

/**
 *
 */
void CandyShell::run(CreateShellInfo* inf) {
    hostname       = Utils::Environment::hostname();
    currentUser    = Utils::Environment::logged_user();
    currentUser[0] = static_cast<char>(toupper(currentUser[0]));
    homeDirectory  = "/Users/" + currentUser;

    s_set_working_directory(homeDirectory.c_str());
    readWorkingDirectory();

    screen->clean();
    screen->activate();

    if ( headless ) {
        if ( !inf->number ) {
            // print the logo
            ifstream logofile("/sys/graphics/CandyShell/logo.oem-us");
            if ( logofile.good() ) {
                string logo((istreambuf_iterator<char>(logofile)), istreambuf_iterator<char>());
                screen->write(logo, Graphics::Color::as_rgb(0, 200, 0));
                logofile.close();
            }

            else
                screen->write("\t\t\t\tLogo file not found", Graphics::Color::as_rgb(200, 0, 0));

            stringstream msg2;
            msg2 << endl;
            msg2 << " Enter 'cat README' for a brief introduction." << endl;
            msg2 << " Or type 'Medit -read README' for read introduction whit Medit editor" << endl << endl;
            screen->write(msg2.str(), Graphics::Color::as_rgb(255, 255, 255));
        }

        else {
            stringstream msg;
            msg << "CandyShell Session: " << inf->number << endl;
            screen->write(msg.str());
        }
    }

    string command;

    bool writeHeader = true;

    while ( true ) {
        // wait for activity
        inactive = false;
        s_atomic_block(&inactive);
        // screen->write("hey\n");
        screen->updateCursor();

        if ( writeHeader ) {
            if ( currentUser == "root" )
                screen->write(hostname, Graphics::Color::as_rgb(255, 0, 0));

            else
                screen->write(hostname, Graphics::Color::as_rgb(0, 255, 0));

            screen->write("@", Graphics::Color::as_rgb(255, 255, 255));
            screen->write(currentUser, Graphics::Color::as_rgb(0, 255, 0));
            screen->write(":", Graphics::Color::as_rgb(255, 255, 255));
            screen->write(workingDirectory, Graphics::Color::as_rgb(0, 255, 255));
            screen->write(">", Graphics::Color::as_rgb(255, 255, 255));
            screen->updateCursor();
        }

        else
            writeHeader = true;

        // read line
        ShellInputStatus status;
        command = readInput(command, screen, &status, 0);

        // handle input
        if ( status == SHELL_INPUT_STATUS_SCREEN_SWITCH ) {
            switchToNext();
            writeHeader = false;
        }

        else if ( status == SHELL_INPUT_STATUS_SCREEN_CREATE ) {
            CreateShellInfo* inf = new CreateShellInfo;
            inf->number          = shellIndex++;
            createShell(inf);
            writeHeader = false;
        }

        else if ( status == SHELL_INPUT_STATUS_EXIT ) {
            writeHeader = false;
        }

        else if ( status == SHELL_INPUT_STATUS_CLEAR ) {
            screen->clean();
            screen->activate();
        }

        else if ( status == SHELL_INPUT_STATUS_DEFAULT ) {
            screen->write("\n");
            screen->updateCursor();

            if ( !command.empty() ) {
                history->addHistory(command);

                if ( !handleBuiltin(command) )
                    runCommand(command);
            }

            command.clear();
        }
    }
}

/**
 *
 */
void CandyShell::runCommand(string command) {
    FileHandle termIn;
    FileHandle termOut;
    FileHandle termErr;
    Pid        lastPid;

    if ( !runTermCommand(command, &termIn, &termOut, &termErr, &lastPid) )
        return;

    StandardInThreadData inData;
    inData.id            = "stdInp";
    inData.continueInput = true;
    inData.stdinWriteEnd = termIn;
    inData.shell         = this;
    Tid rin              = s_create_thread_d((void*)&standardInThread, (void*)&inData);

    StandardOutThreadData outData;
    outData.id            = "stdOut";
    outData.stop          = false;
    outData.err           = false;
    outData.stdoutReadEnd = termOut;
    outData.screen        = screen;
    Tid rout              = s_create_thread_d((void*)&standardOutThread, (void*)&outData);

    StandardOutThreadData errData;
    errData.id            = "stdErrOut";
    errData.stop          = false;
    errData.err           = true;
    errData.stdoutReadEnd = termErr;
    errData.screen        = screen;
    Tid rerr              = s_create_thread_d((void*)&standardOutThread, (void*)&errData);

    inData.intPid = lastPid;

    // wait for process to exit
    s_join(lastPid);
    inData.continueInput = false;
    outData.stop         = true;
    errData.stop         = true;

    // wait for input thread before leaving
    s_join(rin);
    s_join(rout);
    s_join(rerr);
}

/**
 *
 */
bool CandyShell::runTermCommand(string      command,
                                FileHandle* termIn,
                                FileHandle* termOut,
                                FileHandle* termErr,
                                Pid*        intPid) {
    // create initial pipe
    FsPipeStatus firstPipeStat;
    FileHandle   firstPipeW;
    FileHandle   firstPipeR;
    s_pipe_s(&firstPipeW, &firstPipeR, &firstPipeStat);
    if ( firstPipeStat != FS_PIPE_SUCCESSFUL ) {
        screen->write("unable to create process pipe", Graphics::Color::as_rgb(255, 0, 0));
        return false;
    }
    *termIn               = firstPipeW;
    FileHandle lastStdout = firstPipeR;

    // split by pipes
    string         part;
    vector<string> parts;
    stringstream   commandstr(command);
    while ( getline(commandstr, part, '|') ) {
        parts.push_back(part);
    }

    int numparts = parts.size();
    for ( int i = 0; i < numparts; i++ ) {
        // split by path and arguments
        string path = Utils::trim(parts[i]);
        string args = "";

        int spacePos = path.find(' ');
        if ( spacePos != -1 ) {
            args = path.substr(spacePos + 1);
            path = path.substr(0, spacePos);
        }

        // create pipes
        FsPipeStatus pipeStat;

        FileHandle outPipeW;
        FileHandle outPipeR;
        s_pipe_s(&outPipeW, &outPipeR, &pipeStat);
        if ( pipeStat != FS_PIPE_SUCCESSFUL ) {
            screen->write("unable to create process out pipe", Graphics::Color::as_rgb(255, 0, 0));
            return false;
        }

        // execute it
        Pid        spawnedPid;
        FileHandle spawnedStdio[3];
        FileHandle inStdio[3];
        inStdio[0] = lastStdout;
        inStdio[1] = outPipeW;
        inStdio[2] = FD_NONE;

        if ( i == numparts - 1 ) { // stderr only for last
            FileHandle errPipeW;
            FileHandle errPipeR;
            s_pipe_s(&errPipeW, &errPipeR, &pipeStat);
            if ( pipeStat != FS_PIPE_SUCCESSFUL ) {
                screen->write("unable to create process err pipe", Graphics::Color::as_rgb(255, 0, 0));
                return false;
            }
            inStdio[2] = errPipeW;
            *termErr   = errPipeR;
        }

        if ( !execute(path, args, &spawnedPid, spawnedStdio, inStdio) ) {
            // closing pipe
            s_close(outPipeW);
            s_close(outPipeR);

            return false;
        }

        // close stderr in last
        if ( i == numparts - 1 )
            s_close(inStdio[2]);

        // close write end of this pipe in this process
        s_close(outPipeW);

        // close read end of previous pipe in this process
        s_close(lastStdout);

        // next one uses this as output
        lastStdout = outPipeR;

        // last is the one that's interrupted
        *intPid = spawnedPid;
    }

    // last is used as shell output
    *termOut = lastStdout;

    return true;
}

/**
 *
 */
bool CandyShell::handleBuiltin(string command) {
    if ( command == BUILTIN_COMMAND_HELP ) {
        screen->write(" The shell has the following built-in functions:\n", Graphics::Color::as_rgb(255, 255, 255));
        screen->write("\n", Graphics::Color::as_rgb(255, 255, 255));
        screen->write(" help                  prints this help screen\n", Graphics::Color::as_rgb(255, 255, 255));
        screen->write(" ls                    lists all files in the current directory\n",
                      Graphics::Color::as_rgb(255, 255, 255));
        screen->write(" cd <path>             switches to a directory\n", Graphics::Color::as_rgb(255, 255, 255));
        screen->write(" clear                 clears the screen\n", Graphics::Color::as_rgb(255, 255, 255));
        screen->write(" sleep <ms>            sleeps for the given number of milliseconds\n",
                      Graphics::Color::as_rgb(255, 255, 255));
        screen->write(" & <program>           runs a program in the background\n",
                      Graphics::Color::as_rgb(255, 255, 255));
        screen->write("\n", Graphics::Color::as_rgb(255, 255, 255));
        screen->write(" shell, ctrl+space     open a new shell\n", Graphics::Color::as_rgb(255, 255, 255));
        screen->write(" shell <num>           switches to the shell with the given number\n",
                      Graphics::Color::as_rgb(255, 255, 255));
        screen->write(" tty                   lists all tty\n", Graphics::Color::as_rgb(255, 255, 255));
        screen->write(" ctrl+tab              switches to the next shell\n", Graphics::Color::as_rgb(255, 255, 255));
        screen->write("\n", Graphics::Color::as_rgb(255, 255, 255));
        screen->write(" kb set <layout>       switches the keyboard layout\n", Graphics::Color::as_rgb(255, 255, 255));
        screen->write(" kb info               prints the keyboard layout\n", Graphics::Color::as_rgb(255, 255, 255));
        screen->write(" get time              Return hour:minute:second day/month/year\n",
                      Graphics::Color::as_rgb(255, 255, 255));
        screen->write(" uptime                Return uptime\n", Graphics::Color::as_rgb(255, 255, 255));
        screen->write(" color <color>         change background and font color\n",
                      Graphics::Color::as_rgb(255, 255, 255));
        screen->write("\n", Graphics::Color::as_rgb(255, 255, 255));
        screen->write(" logout                return to login manager\n", Graphics::Color::as_rgb(255, 255, 255));
        screen->write(" lock, crtl+l          lock the screen with current session\n",
                      Graphics::Color::as_rgb(255, 255, 255));
        screen->write("\n", Graphics::Color::as_rgb(255, 255, 255));
        screen->write(" shutdown              shutdown the system\n", Graphics::Color::as_rgb(255, 255, 255));
        screen->write(" reboot                reboot system\n", Graphics::Color::as_rgb(255, 255, 255));
        screen->write(" exit                  terminate current CandyShell\n", Graphics::Color::as_rgb(255, 255, 255));
        screen->write("\n", Graphics::Color::as_rgb(255, 255, 255));

        return true;

    }

    else if ( command.substr(0, string(BUILTIN_COMMAND_KBD_SET).length()) == BUILTIN_COMMAND_KBD_SET ) {
        command           = command.substr(string(BUILTIN_COMMAND_KBD_SET).length());
        bool layoutLoaded = IO::Keyboard::instance().load_layout(command);

        if ( layoutLoaded )
            screen->write("keyboard layout to '" + command + "' set \n", Graphics::Color::as_rgb(255, 255, 255));

        else
            screen->write("keyboard layout to '" + command + "' not found\n", Graphics::Color::as_rgb(255, 255, 255));

        return true;
    }

    else if ( command == BUILTIN_COMMAND_SCANCODE ) {
        if ( headless ) {
            while ( true ) {
                IO::Keyboard::Info key = IO::Keyboard::instance().read();
                if ( key.m_is_pressed && key.m_ctrl && key.m_key == "KEY_C" )
                    break;

                stringstream msg;
                msg << "scancode: " << (key.m_is_pressed ? "d " : "u ") << (uint32_t)key.m_scancode
                    << ", ctrl: " << key.m_ctrl << ", alt: " << key.m_alt << ", shift: " << key.m_shift << endl;
                screen->write(msg.str(), Graphics::Color::as_rgb(255, 255, 255));
            }
        }

        else
            screen->write("This command is only available in headless mode.\n", Graphics::Color::as_rgb(255, 255, 255));

        return true;
    }

    else if ( command == BUILTIN_COMMAND_KBD_INFO ) {
        screen->write("keyboard layout is '" + IO::Keyboard::instance().current_layout() + "'\n",
                      Graphics::Color::as_rgb(255, 255, 255));
        return true;

    }

    else if ( command == BUILTIN_COMMAND_GETHOUR ) {
        DateTime date_time;
        s_get_date_time(&date_time);

        // write data to stringstream
        stringstream ss_time;
        ss_time << std::setfill('0') << std::setw(2) << date_time.m_hours;
        ss_time << ':';
        ss_time << std::setfill('0') << std::setw(2) << date_time.m_minutes;
        ss_time << ':';
        ss_time << std::setfill('0') << std::setw(2) << date_time.m_seconds;

        stringstream ss_date;
        ss_date << std::setfill('0') << std::setw(2) << date_time.m_month_day;
        ss_date << ':';
        ss_date << std::setfill('0') << std::setw(2) << date_time.m_month;
        ss_date << ':';
        ss_date << std::setfill('0') << std::setw(2) << date_time.m_year;

        screen->write(ss_time.str(), Graphics::Color::as_rgb(255, 255, 255));
        screen->write(ss_date.str(), Graphics::Color::as_rgb(255, 255, 255));

        return true;
    }

    else if ( command.substr(0, string(BUILTIN_COMMAND_SET_COLOR).length()) == BUILTIN_COMMAND_SET_COLOR ) {
        command = command.substr(string(BUILTIN_COMMAND_SET_COLOR).length());

        if ( command == "-h" || command == "--help" ) {
            screen->write("red               (Red background/black font)\n", Graphics::Color::as_rgb(255, 255, 255));
            screen->write("green             (Green background/black font)\n", Graphics::Color::as_rgb(255, 255, 255));
            screen->write("black             (Black background/gray font)\n", Graphics::Color::as_rgb(255, 255, 255));
            screen->write("white             (white background/black font (default)\n",
                          Graphics::Color::as_rgb(255, 255, 255));
        }

        else if ( !screen->setColor(command) ) {
            screen->write("Color '" + command + "' not found\n", Graphics::Color::as_rgb(255, 255, 255));
            screen->write("Try color --help\n", Graphics::Color::as_rgb(255, 255, 255));
        }

        return true;
    }

    else if ( command.substr(0, string(BUILTIN_COMMAND_BACKGROUND).length()) == BUILTIN_COMMAND_BACKGROUND ) {
        command = command.substr(string(BUILTIN_COMMAND_BACKGROUND).length());

        // TODO parse arguments
        string path = command;
        string args = "";

        int spacePos = path.find(' ');
        if ( spacePos != -1 ) {
            args = path.substr(spacePos + 1);
            path = path.substr(0, spacePos);
        }

        Pid        pid;
        FileHandle out_stdio[3];
        FileHandle inStdio[3];
        inStdio[0] = FD_NONE;
        inStdio[1] = FD_NONE;
        inStdio[2] = FD_NONE;
        execute(path, args, &pid, out_stdio, inStdio);

        stringstream msg;
        msg << "process " << pid << "\n";
        screen->write(msg.str(), Graphics::Color::as_rgb(255, 255, 255));
        return true;

    }

    else if ( command.substr(0, string(BUILTIN_COMMAND_CD).length()) == BUILTIN_COMMAND_CD ) {
        string typedDir = command.substr(string(BUILTIN_COMMAND_CD).length());
        if ( typedDir == "" )
            workingDirectory = homeDirectory;
        else
            workingDirectory = typedDir;

        SetWorkingDirectoryStatus stat = writeWorkingDirectory();

        if ( stat == SET_WORKING_DIRECTORY_NOT_A_FOLDER )
            screen->write("selected file not a folder\n", Graphics::Color::as_rgb(255, 0, 0));
        else if ( stat == SET_WORKING_DIRECTORY_NOT_FOUND )
            screen->write("directory not found\n", Graphics::Color::as_rgb(255, 0, 0));
        else if ( stat == SET_WORKING_DIRECTORY_ERROR )
            screen->write("unable to switch to the selected directory\n", Graphics::Color::as_rgb(255, 0, 0));

        readWorkingDirectory();
        return true;

    }

    else if ( command.substr(0, string(BUILTIN_COMMAND_SLEEP).length()) == BUILTIN_COMMAND_SLEEP ) {
        string       sleepTime = command.substr(string(BUILTIN_COMMAND_SLEEP).length());
        long         ms;
        stringstream scrn;
        scrn << sleepTime;
        scrn >> ms;

        if ( ms > 0 && ms < 100000 ) {
            stringstream msg;
            msg << "sleeping for " << ms << "ms... ";
            screen->write(msg.str(), Graphics::Color::as_rgb(255, 255, 255));
            s_sleep(ms);
            screen->write("awake!\n", Graphics::Color::as_rgb(255, 255, 255));
        }

        else {
            stringstream msg;
            msg << "can't sleep this long (" << ms << "ms)\n";
            screen->write(msg.str(), Graphics::Color::as_rgb(255, 255, 255));
        }
        return true;

    }

    else if ( command == BUILTIN_COMMAND_CLEAR ) {
        screen->clean();
        screen->activate();

        return true;
    }

    else if ( command == BUILTIN_COMMAND_TERMS ) {
        stringstream msg;
        msg << tty.size();
        msg << " CandyShells active\n";
        screen->write(msg.str(), Graphics::Color::as_rgb(255, 255, 255));
        return true;

    }

    else if ( command.substr(0, string(BUILTIN_COMMAND_TERM_P).length()) == BUILTIN_COMMAND_TERM_P ) {
        string       screenNum = command.substr(string(BUILTIN_COMMAND_TERM_P).length());
        int          pos;
        stringstream scrn;
        scrn << screenNum;
        scrn >> pos;

        if ( pos < tty.size() && pos >= 0 ) {
            switchTo(tty[pos]);
        }

        else {
            stringstream msg;
            msg << pos;
            msg << " is not a valid screen index\n";
            screen->write(msg.str(), Graphics::Color::as_rgb(255, 255, 255));
        }
        return true;

    }

    else if ( command == BUILTIN_COMMAND_TERM ) {
        CreateShellInfo* inf = new CreateShellInfo;
        inf->number          = shellIndex++;

        createShell(inf);
        delete inf;

        return true;

    }

    else if ( command == BUILTIN_COMMAND_LOGOUT ) {
        Login::LoginEntry();

        CreateShellInfo* inf = new CreateShellInfo;
        inf->number          = shellIndex++;

        createShell(inf);
        delete inf;

        return true;
    }

    else if ( command == BUILTIN_COMMAND_LOCK ) {
        login->lockLogin();
        screen->clean();
        screen->activate();

        return true;
    }

    else if ( command == BUILTIN_COMMAND_REBOOT ) {
        screen->write("rebooting MeetiX OS", Graphics::Color::as_rgb(255, 255, 255));

        return true;
    }

    else if ( command == BUILTIN_COMMAND_SHUTDOWN ) {
        screen->write("Shutting down MeetiX OS", Graphics::Color::as_rgb(255, 255, 255));

        return true;
    }

    else if ( command == BUILTIN_COMMAND_EXIT ) {
        if ( !headless )
            exit(screen);

        return true;
    }

    else if ( command == BUILTIN_COMMAND_GET_UPTIME ) {
        stringstream ss;
        ss << s_millis() << " seconds " << endl;

        screen->write(ss.str(), Graphics::Color::as_rgb(255, 255, 255));

        return true;
    }

    return false;
}

/**
 *
 */
void CandyShell::standardOutThread(StandardOutThreadData* data) {
    s_task_register_id(data->id);
    int   buflen = 1024;
    char* buf    = new char[buflen];

    while ( true ) {
        FsReadStatus stat;
        int          read = s_read_s(data->stdoutReadEnd, buf, buflen, &stat);

        if ( stat == FS_READ_SUCCESSFUL ) {
            Graphics::Color::ArgbGradient localColor
                = (data->err ? Graphics::Color::as_rgb(255, 0, 0) : Graphics::Color::as_rgb(255, 255, 255));
            for ( int i = 0; i < read; i++ ) {
                char c = buf[i];
                if ( c == '\r' )
                    continue;

                else if ( c == '\t' ) {
                    data->screen->writeChar(' ', localColor);
                    data->screen->writeChar(' ', localColor);
                    data->screen->writeChar(' ', localColor);
                    data->screen->writeChar(' ', localColor);
                }

                else
                    data->screen->writeChar(c, localColor);
            }

            data->screen->updateCursor();
        }

        else
            break;
    }

    data->screen->updateCursor();

    delete buf;
}

/**
 *
 */
void CandyShell::standardInThread(StandardInThreadData* data) {
    s_task_register_id(data->id);
    string line;

    while ( data->continueInput ) {
        ShellInputStatus stat;

        // we add up the line because it might contain content already
        line = data->shell->readInput(line, data->shell->screen, &stat, &data->continueInput);

        if ( stat == SHELL_INPUT_STATUS_EXIT ) {
            if ( data->intPid != -1 )
                s_raise_signal(data->intPid, SIGINT);
        }

        else if ( stat == SHELL_INPUT_STATUS_SCREEN_SWITCH ) {
            data->shell->switchToNext();
            s_atomic_block(&data->shell->inactive);
        }

        else if ( stat == SHELL_INPUT_STATUS_DEFAULT ) {
            line = line + "\n";

            const char* lineContent = line.c_str();
            int         lineLength  = strlen(lineContent);

            int written = 0;
            int len     = 0;
            while ( written < lineLength ) {
                len = write(data->stdinWriteEnd, &lineContent[written], lineLength - written);

                if ( len <= 0 )
                    break;

                written = written + len;
            }

            line = "";
        }
    }
}

/**
 *
 */
bool CandyShell::fileExists(string path) {
    FILE* f;
    if ( (f = fopen(path.c_str(), "r")) != NULL ) {
        fclose(f);
        return true;
    }
    return false;
}

/**
 *
 */
bool CandyShell::findExecutable(string name, string& out) {
    out = name;

    return true;
}

/**
 *
 */
bool CandyShell::execute(string shortpath, string args, Pid* outPid, FileHandle outStdio[3], FileHandle inStdio[3]) {
    // check for command in path
    string realpath;
    if ( !findExecutable(shortpath, realpath) ) {
        screen->write(shortpath + ": command not found\n");
        return false;
    }

    // spawn binary
    SpawnStatus status = s_spawn_poi(realpath.c_str(),
                                     args.c_str(),
                                     workingDirectory.c_str(),
                                     SECURITY_LEVEL_APPLICATION,
                                     outPid,
                                     outStdio,
                                     inStdio);

    if ( status == SPAWN_STATUS_SUCCESSFUL ) {
        return true;
    }

    stringstream msg;
    msg << "failed to execute binary '" << realpath << "', code: " << status << "\n";
    screen->write(msg.str(), Graphics::Color::as_rgb(255, 0, 0));
    return false;
}

/**
 *
 */
string
CandyShell::readInput(string there, Screen* screen, ShellInputStatus* outStatus, bool* continueInput, bool visible) {
    string line = there;
    while ( continueInput == 0 || *continueInput ) {
        IO::Keyboard::Info key = screen->readInput(continueInput);

        if ( key.m_is_pressed ) {
            if ( (key.m_ctrl && key.m_key == "KEY_C") || (key.m_key == "KEY_ESC") ) {
                *outStatus = SHELL_INPUT_STATUS_EXIT;
                break;
            }

            else if ( key.m_ctrl && key.m_key == "KEY_L" ) {
                *outStatus = SHELL_INPUT_STATUS_CLEAR;
                break;
            }

            else if ( key.m_ctrl && key.m_key == "KEY_SPACE" ) {
                *outStatus = SHELL_INPUT_STATUS_SCREEN_CREATE;
                break;
            }

            else if ( key.m_ctrl && key.m_key == "KEY_TAB" ) {
                *outStatus = SHELL_INPUT_STATUS_SCREEN_SWITCH;
                break;
            }

            else if ( key.m_key == "KEY_ENTER" ) {
                *outStatus = SHELL_INPUT_STATUS_DEFAULT;
                break;
            }

            else if ( key.m_key == "KEY_BACKSPACE" ) {
                if ( line.length() > 0 ) {
                    line = line.substr(0, line.length() - 1);
                    screen->backspace();
                    screen->updateCursor();
                }
            }

            else if ( key.m_key == "KEY_TAB" ) {
                // open current dir
                DIR*    dir = opendir(workingDirectory.c_str());
                dirent* ent;

                // check if text contains a whitespace, find everything after that
                int    wspos     = line.find(' ');
                string search    = (wspos == -1) ? line : line.substr(wspos + 1);
                int    searchlen = search.length();

                // find a dirent that starts with the entered text
                while ( (ent = readdir(dir)) != 0 ) {
                    if ( searchlen < ent->d_namlen && strncmp(search.c_str(), ent->d_name, search.length()) == 0 ) {
                        stringstream appendix;
                        for ( int i = search.length(); i < ent->d_namlen; i++ ) {
                            appendix << ent->d_name[i];
                        }

                        line = line + appendix.str();
                        screen->write(appendix.str(), Graphics::Color::as_rgb(255, 255, 255));
                        break;
                    }
                }

                // deleting pointer
                delete dir;
                delete ent;
            }

            // show history
            else if ( key.m_key == "KEY_PAD_8" ) {
                string output;

                screen->cleanLine(line.length());

                line = history->getHistory();
                screen->write(line, Graphics::Color::as_rgb(255, 255, 255));
            }

            else {
                char chr = IO::Keyboard::instance().char_for_key(key);
                if ( chr != -1 ) {
                    line = line + chr;

                    string out;
                    out = out + chr;
                    screen->write(out, Graphics::Color::as_rgb(255, 255, 255), visible);
                }
            }

            screen->updateCursor();
        }
    }

    return line;
}
