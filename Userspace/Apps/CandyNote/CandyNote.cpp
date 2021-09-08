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

#include "CandyNote.hpp"

/**
 *
 */
int main(int argc, char* argv[]) {
    CandyNote_t* editor = new CandyNote_t();

    editor->processArgument(argc, argv);
    editor->mainLoop();

    uint8_t s = true;
    AtomicBlock(&s);
    delete editor;
}

/**
 *
 */
CandyNote_t::CandyNote_t() {
    if ( initialize() )
        screen = new GraphicFrontEnd_t();

    else
        screen = new TextFrontEnd_t();
}

CandyNote_t::~CandyNote_t() {
    if ( screen )
        delete screen;
}

/**
 *
 */
bool CandyNote_t::initialize() {
    UiOpenStatus openStat = UI::open();

    if ( openStat == UI_OPEN_STATUS_SUCCESSFUL )
        return true;

    else
        return false;
}

/**
 *
 */
void CandyNote_t::processArgument(int argc, char* argv[]) {
    if ( argc > 1 ) {
        string arg = argv[1];

        if ( arg == "-new" ) {
            state = STATE_NEW;

            if ( argc > 2 ) {
                string newFile = argv[2];

                workingFile.open(newFile, ios_base::in);
            }

            else {
                screen->write("file name not specified, creat newfile.txt", "Warning");
                workingFile.open("newFile.txt", ios_base::in);
            }
        }

        else if ( arg == "-open" ) {
            state = STATE_OPEN;

            if ( argc > 2 ) {
                string openFile = argv[2];
                workingFile.open(openFile, ios_base::out | ios_base::app);

                if ( !workingFile.is_open() ) {
                    screen->write("unable to open file", "Error");
                    state = STATE_ERROR;
                }
            }

            else {
                screen->write("unspecified path file", "Error");
                state = STATE_ERROR;
            }
        }

        else if ( arg == "-read" ) {
            state = STATE_READ;

            if ( argc > 2 ) {
                string readFile = argv[2];
                workingFile.open(readFile, ios_base::out);

                if ( !workingFile.is_open() ) {
                    screen->write("unable to open file", "Error");
                    state = STATE_ERROR;
                }
            }

            else {
                screen->write("unspecified path file", "Error");
                state = STATE_ERROR;
            }
        }

        else if ( arg == "-trunc" ) {
            state = STATE_TRUNC;

            if ( argc > 2 ) {
                string truncFile = argv[2];
                workingFile.open(truncFile, ios_base::trunc | ios_base::in);

                if ( !workingFile.is_open() ) {
                    screen->write("unable to open file", "Error");
                    state = STATE_ERROR;
                }
            }

            else {
                screen->write("unspecified path file", "Error");
                state = STATE_ERROR;
            }
        }
    }

    else {
        state = STATE_NEW;

        screen->write("No argument provided, creating newfile", "Warning");
        workingFile.open("newFile.txt", ios_base::in);
    }
}

/**
 *
 */
void CandyNote_t::mainLoop() {
    screen->clean();

    switch ( state ) {
        case STATE_OPEN:
            readFile();
            break;

        case STATE_READ:
            readFile();
            break;

        default:
            break;
    }

    while ( true ) {
        AtomicBlock(&waiting);
        readInput();

        switch ( state ) {
            case STATE_NEW_LINE:
                text << line << endl;
                line.clear();
                break;

            case STATE_SAVE:
                workingFile << text.str();
                line.clear();
                text.clear();
                break;

            case STATE_CLOSE:
                workingFile << text.str();
                workingFile.close();
                break;

            case STATE_EXIT:
                screen->write("closing file and exiting", "info");
                workingFile.close();
                exit = true;
                break;
        }

        if ( exit )
            break;
    }
}

/**
 *
 */
void CandyNote_t::readFile() {
    string out;

    while ( !workingFile.eof() ) {
        workingFile >> out;
        screen->write(out, "out");
    }
}

/**
 *
 */
void CandyNote_t::readInput() {
    screen->readInput();
}
