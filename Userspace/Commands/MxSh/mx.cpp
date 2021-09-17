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

#include "mx.hpp"

#include <iostream>
#include <map>
#include <utils/utils.hpp>

using namespace std;

/*
 *	main of shell
 */
int main(int argc, char* argv[]) {
    // create object to class
    MXShell* shell = new MXShell();

    // call general initialization
    shell->initialize(argc, argv);

    // initialize routine
    shell->startRoutine();

    // deleting pointer
    delete shell;
    return 0;
}

/*
 *	load the environment and parse arguments
 */
void MXShell::initialize(int argc, char* argv[]) {
    loadEnvironment();
    mode = parseArguments(argc, argv);
}

/**
 *	load environment variables from file env
 */
void MXShell::loadEnvironment() {
    // instantiate object and provide path
    environment = new Environment("/MeetiX/Configs/Env/Global");

    // load file to variables
    environment->load();

    // instantiate interpreter
    interpreter = new MXinterpreter(environment);
}

/*
 *	parse arguments and return mode
 */
OperativeMode MXShell::parseArguments(int argc, char* argv[]) {
    // if there aren't arguments return no mode
    if ( argc < 1 )
        return SHELL_MODE_NULL;

    // get command argument
    if ( argv[1] )
        command = argv[1];

    // get argument of command
    if ( argv[2] )
        argument = argv[2];

    for ( int index = 0; index < commands.size(); index++ ) {
        // if provided command is supported return index of command as OperativeMode
        if ( command == commands[index] )
            return (OperativeMode)index;
    }

    return SHELL_MODE_NULL;
}

/*
 * start routine of shell from mode provided and argument passed
 */
void MXShell::startRoutine() {
    // run selected mode from arguments
    if ( mode == SHELL_MODE_SHELL || mode == SHELL_MODE_SHELL_SHORT )
        shellMode(environment);
    else if ( mode == SHELL_MODE_SCRIPT || mode == SHELL_MODE_SCRIPT_SHORT )
        scriptMode();
    else if ( mode == SHELL_MODE_ENVIRONMENT_GET || mode == SHELL_MODE_ENVIRONMENT_GET_SHORT )
        environmentMode("get");
    else if ( mode == SHELL_MODE_ENVIRONMENT_SET || mode == SHELL_MODE_ENVIRONMENT_SET_SHORT )
        environmentMode("set");
    else if ( mode == SHELL_MODE_COMMAND || mode == SHELL_MODE_COMMAND_SHORT )
        commandMode();
    else if ( mode == SHELL_MODE_HELP || mode == SHELL_MODE_HELP_SHORT )
        explainHelp();
    else if ( mode == SHELL_MODE_VERSION || mode == SHELL_MODE_VERSION_SHORT )
        cout << "MeetiX OS Shell Version: " << VERSION_MAJOR << VERSION_MINOR << VERSION_PATCH
             << endl;
    else if ( mode == SHELL_MODE_NULL )
        cerr << "Invalid mode argument provided" << endl;
}

/*
 * script mode
 */
void MXShell::scriptMode() {
    // check if there are arguments provided
    if ( !argument.empty() ) {
        // load given script
        FILE* scriptfile = fopen(argument.c_str(), "r");

        // check if file exist
        if ( scriptfile == nullptr ) {
            Utils::log("%s: file not found", argument.c_str());
            cerr << argument << ": file not found" << endl;
            return;
        }

        // parse the script
        scriptParser         = new MXScriptParser(scriptfile);
        LsDocument* document = scriptParser->document();

        // call interpreter
        interpreter->interpret(document);

        // deleting pointer
        delete document;

        // closing file
        fclose(scriptfile);
    }

    // show error if no file provided
    else {
        Utils::log("no filename provided", argument.c_str());
        cerr << "no filename provided" << endl;
    }
}

/*
 * environment mode
 */
void MXShell::environmentMode(string mode) {
    // if getenv requested
    if ( mode == "get" ) {
        // if argument string is filled
        if ( !argument.empty() ) {
            // find variable with argument
            string value = environment->getVariable(argument);

            // if exist write it
            if ( !value.empty() )
                cout << value + '\n';

            // else show error
            else
                cerr << "Unable to find requested environment variable" << endl;
        }

        // else show all variables
        else {
            map<string, string> vars = environment->getAllVariables();
            for ( pair<string, string> element : vars ) {
                cout << element.first << "=" << element.second << endl;
            }
        }
    }

    // if setenv requested
    else if ( mode == "set" ) {
        // if argument string is filled
        if ( !argument.empty() ) {
            // get index of ':' separator
            int pos = argument.find('=');
            if ( pos != -1 ) {
                // separate value and key from argument and call manager
                string key   = argument.substr(0, pos);
                string value = argument.substr(pos + 1);
                environment->setVariable(key, value);
            }
        }
    }
}

/*
 * single command
 */
void MXShell::commandMode() {
    // if there is command provided
    if ( !argument.empty() ) {
        string args;

        // if there is third argument assign it to args
        // if (argv[3]) args = argv[3];

        // exec program
        interpreter->exec(argument, args);
    }
}

/*
 * show help on screen
 */
void MXShell::explainHelp() {
    cerr << "use: mx [long option/option] ..." << endl;
    cerr << "     mx [long option/option] fileScript.sh" << endl << endl;
    cerr << "Long option:           Short option:" << endl;
    cerr << "     --shell                     -sh" << endl;
    cerr << "     --script                    -s" << endl;
    cerr << "     --getenv                    -e" << endl;
    cerr << "     --command                   -c" << endl;
    cerr << "     --version                   -v" << endl;
    cerr << "     --help                      -h" << endl;
}
