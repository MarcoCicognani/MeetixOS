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

#include "parser.hpp"

#include <Api/utils/local.hpp>
#include <iostream>

/**
 *
 */
Parser::Parser(std::string input) : input(input) {
    position = 0;
    current  = -1;
    step();
}

/**
 *
 */
void Parser::step() {
    if ( position < input.size() )
        current = input[position++];
    else
        current = -1;
}

/**
 *
 */
void Parser::skipWhitespace() {
    while ( isspace(current) )
        step();
}

/**
 *
 */
bool Parser::pipeExpression(PipeExpression** out) {
    Local<PipeExpression> expr(new PipeExpression());

    while ( true ) {
        skipWhitespace();

        // program call
        ProgramCall* pc;
        if ( programCall(&pc) ) {
            expr()->calls.push_back(pc);
            skipWhitespace();

            // optionally a pipe end of input
            if ( current == '|' )
                step();
            else if ( current == -1 )
                break;
            else {
                std::cout << (char)27 << "[31m";
                std::cout << "unexpected character: " << current << std::endl;
                std::cout << (char)27 << "[0m";
                return false;
            }
        }

        else {
            std::cout << (char)27 << "[31m";
            std::cerr << "syntax error: expected a program call at position: " << position
                      << std::endl;
            std::cout << (char)27 << "[0m";
            return false;
        }
    }

    *out = expr.release();
    return true;
}

/**
 *
 */
bool Parser::programCall(ProgramCall** out) {
    Local<ProgramCall> expr(new ProgramCall());

    // read until space or pipe
    std::string program = "";
    while ( current != -1 && current != '|' && !isspace(current) ) {
        program += current;
        step();
    }
    if ( program.length() == 0 )
        return false;
    expr()->program = program;

    // parse arguments
    std::string arg;
    while ( argument(arg) )
        expr()->arguments.push_back(arg);

    *out = expr.release();
    return true;
}

/**
 *
 */
bool Parser::argument(std::string out) {
    // skip whitespace
    skipWhitespace();

    // fin on end
    if ( current == -1 )
        return false;

    // start on pipe
    if ( current == '|' )
        return false;

    // parse content
    std::string arg = "";

    bool instr   = false;
    bool escaped = false;

    while ( true ) {
        // stop on end
        if ( current == -1 )
            break;
        if ( !escaped && current == '\\' )
            escaped = true;
        else if ( instr ) {
            if ( current == '"' && !escaped )
                instr = false;
            else
                arg += current;
            if ( escaped )
                escaped = false;
        } else {
            if ( !escaped && (current == '|' || current == ' ') )
                break;
            else if ( current == '"' && !escaped )
                instr = true;
            else
                arg += current;
            if ( escaped )
                escaped = false;
        }

        step();
    }

    out = arg;
    return true;
}
