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

#include "interpreter.hpp"

#include <iostream>
#include <LibUtils/Utils.hh>

using namespace std;

/*
 * interpret the parsed script
 */
void MXinterpreter::interpret(LsDocument* script) {
    // interpret script
    for ( LsStatement* stat : script->statements ) {
        string command = stat->pairs[0]->key;

        if ( command == "log" )
            log(stat);
        else if ( command == "spawn_driver" )
            spawn_driver(stat);
        else if ( command == "spawn_app" )
            spawn_app(stat);
        else if ( command == "wait_for_id" )
            wait_for_id(stat);
        else if ( command == "sleep" )
            sleep(stat);
        else if ( command == "if" )
            if_paradigm(stat);
        else if ( command == "else" )
            else_paradigm(stat);
        else if ( command == "while" )
            while_paradigm(stat);
        else if ( command == "export" )
            setUpVariable(stat);
        else if ( command == "def" )
            defineNewFunction(stat);
    }
}

/**
 *	print instruction, log on COM1 and std_out
 */
void MXinterpreter::log(LsStatement* stat) {
    cout << stat->pairs[0]->value << endl;
    Utils::log(stat->pairs[0]->value.c_str());
}

/**
 *	spawn_app application with spawn_driver security level
 */
void MXinterpreter::spawn_driver(LsStatement* stat) {
    string path = stat->pairs[0]->value;
    string args = findParam(stat, "args", "");
    execWithSpawner(path, args, SECURITY_LEVEL_DRIVER);
}

/**
 *	script spawn_app
 */
void MXinterpreter::spawn_app(LsStatement* stat) {
    string path = stat->pairs[0]->value;
    string args = findParam(stat, "args", "");
    execWithSpawner(path, args, SECURITY_LEVEL_APPLICATION);
}

/**
 *	line spawn_app
 */
void MXinterpreter::exec(string path, string args) {
    string pp = variables->getVariable("PATH") + path;

    execWithSpawner(pp, args, SECURITY_LEVEL_APPLICATION);
}

/**
 *	wait_for_id while task registering
 */
void MXinterpreter::wait_for_id(LsStatement* stat) {
    string identifier = stat->pairs[0]->value;
    Utils::log("Waiting for task which register '%s'", identifier.c_str());

    while ( s_task_get_id(identifier.c_str()) < 0 )
        s_yield();

    Utils::log("ID '%s' registered", identifier.c_str());
}

/**
 *	lock application for n milliseconds
 */
void MXinterpreter::sleep(LsStatement* stat) {
    int time;

    stringstream ss;
    ss << stat->pairs[0]->value;
    ss >> time;

    Utils::log("Sleeping for %d", time);

    s_sleep(time);
}

/**
 *	if paradigm
 */
void MXinterpreter::if_paradigm(LsStatement* stat) {
}

/**
 *	else paradigm
 */
void MXinterpreter::else_paradigm(LsStatement* stat) {
}

/**
 *	iteration while paradigm
 */
void MXinterpreter::while_paradigm(LsStatement* stat) {
}

/**
 *	create local variable in script
 */
void MXinterpreter::setUpVariable(LsStatement* stat) {
    string s = stat->pairs[0]->value;

    int index = s.find('=');

    if ( index != -1 ) {
        string key   = s.substr(0, index);
        string value = s.substr(index + 1);

        variables->setVariable(key, value);
    }

    else
        variables->setVariable(s, "0");
}

/**
 *	define new function
 */
void MXinterpreter::defineNewFunction(LsStatement* stat) {
}

/**
 *	find parameters in instruction line
 */
string MXinterpreter::findParam(LsStatement* stat, string key, string def) {
    for ( auto p : stat->pairs ) {
        if ( p->key == key )
            return p->value;
    }
    return def;
}

/**
 *	spawn_app application with provided security level
 */
Pid MXinterpreter::execWithSpawner(string path, string args, SecurityLevel slvl) {
    auto pid          = -1;
    auto spawn_status = s_spawn_p(path.c_str(), args.c_str(), "/", slvl, &pid);

    if ( spawn_status != SPAWN_STATUS_SUCCESSFUL ) {
        std::stringstream ss;
        ss << "failed to s_spawn '" << path << "' with code " << spawn_status;

        Utils::log(ss.str().c_str());
        cout << ss.str() << endl;
    }

    return pid;
}
