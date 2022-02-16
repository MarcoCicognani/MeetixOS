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

#ifndef _MX_INTERPRETER_
#define _MX_INTERPRETER_

#include "Environment.hpp"
#include "mxscript.hpp"

#include <Api.h>
#include <fstream>
#include <map>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <LibUtils/PropertyFileParser.hh>

/*
 * class that provide basic interpretation commands
 * for script
 */
class MXinterpreter {
public:
    MXinterpreter(Environment* env) { variables = env; }

    // interpret script, espected parsed document
    void interpret(LsDocument* script);

    // intrinsic function, script
    void log(LsStatement* stat);
    void spawn_app(LsStatement* stat);
    void spawn_driver(LsStatement* stat);
    void wait_for_id(LsStatement* stat);
    void sleep(LsStatement* stat);

    // intrisic function, line
    void exec(std::string path, std::string args);

    // control structures
    void if_paradigm(LsStatement* stat);
    void else_paradigm(LsStatement* stat);
    void while_paradigm(LsStatement* stat);

    // expansion structures
    void setUpVariable(LsStatement* stat);
    void defineNewFunction(LsStatement* stat);

private:
    // internal function
    std::string findParam(LsStatement* stat, std::string key, std::string def);
    Pid         execWithSpawner(std::string path, std::string args, SecurityLevel slvl);

    // map of environment variables
    Environment* variables;
};

#endif
