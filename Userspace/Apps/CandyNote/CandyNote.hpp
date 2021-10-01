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

#ifndef _CandyNote_H_
#define _CandyNote_H_

#include "FrontEnd.hpp"
#include "GFrontEnd.hpp"
#include "TFrontEnd.hpp"

#include <Api.h>
#include <fstream>
#include <io/keyboard.hpp>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

typedef int           state_of_editor;
const state_of_editor STATE_NEW      = 0;
const state_of_editor STATE_OPEN     = 1;
const state_of_editor STATE_READ     = 2;
const state_of_editor STATE_TRUNC    = 3;
const state_of_editor STATE_SAVE     = 4;
const state_of_editor STATE_ERROR    = 5;
const state_of_editor STATE_NEW_LINE = 6;
const state_of_editor STATE_EXIT     = 7;
const state_of_editor STATE_CLOSE    = 8;

class CandyNote_t {
public:
    CandyNote_t();
    ~CandyNote_t();

    void processArgument(int argc, char* argv[]);
    void mainLoop();

private:
    bool initialize();
    void readFile();
    void readInput();

    FrontEnd* screen;

    state_of_editor state;
    bool            waiting;
    fstream         workingFile;

    string       line;
    stringstream text;

    bool exit = false;
};

#endif
