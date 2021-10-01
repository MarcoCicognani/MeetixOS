/*********************************************************************************
 * MeetiX OS By MeetiX OS Project [Marco Cicognani]                               *
 * 																			     *
 * This program is free software; you can redistribute it and/or                  *
 * modify it under the terms of the GNU General Public License                    *
 * as published by the Free Software Foundation; either version 2				 *
 * of the License, or (char *argumentat your option) any later version.			 *
 *																				 *
 * This program is distributed in the hope that it will be useful,				 *
 * but WITHout ANY WARRANTY; without even the implied warranty of                 *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 				 *
 * GNU General Public License for more details.
 **
 *																				 *
 * You should have received a copy of the GNU General Public License				 *
 * along with this program; if not, write to the Free Software                    *
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA *
 **********************************************************************************/

#include <Api.h>
#include <io/shell.hpp>
#include <iostream>
#include <string.h>
#include <Tasking/Lock.hh>
#include <unistd.h>
#include <Utils/Utils.hh>

/**
 * Use to buffer stray input when responses to shell requests are read.
 */
static int*          buffer        = nullptr;
static int           bufferedChars = 0;
static int           bufferSize    = 64;
static Tasking::Lock bufferLock;

/**
 * read unbuffered the characters from stdin
 *
 * @return the readed character value
 */
int Shell::readUnbuffered() {
    int c = getc(stdin);

    // Escaped sequences
    if ( c == SHELLKEY_SUB ) {
        int b1 = getc(stdin);
        if ( b1 == -1 )
            return -1;

        int b2 = getc(stdin);
        if ( b2 == -1 )
            return -1;

        return ((b2 << 8) | b1);
    }

    return c;
}

/**
 * store to buffer the provided character
 *
 * @param c:	the character to store to
 */
void Shell::bufferChar(int c) {
    bufferLock.lock();

    // Create buffer
    if ( !buffer ) {
        buffer = new int[bufferSize];
        if ( !buffer ) {
            Utils::log("failed to buffer char in shell client, could not allocate buffer");
            return;
        }

        bufferedChars = 0;
    }

    // Put char in buffer
    if ( bufferedChars < bufferSize )
        buffer[bufferedChars++] = c;

    bufferLock.unlock();
}

/**
 * reads each character from stdin until the escaping character
 * and buffer each character to the global buffer
 */
void Shell::readAndBufferUntilESC() {
    int c;
    while ( (c = readUnbuffered()) != SHELLKEY_ESC )
        bufferChar(c);
    // now ESC was read
}

/**
 * read the escaped parameter value from the provided array
 *
 * @param parameters:	the provided escaped parameters
 * @return the last escaped parameter value
 */
int Shell::readEscapedParameters(int* parameters) {
    int parameterCount = 0;

    int c;
    while ( true ) {
        c = readUnbuffered();

        if ( c >= '0' && c <= '9' ) {
            if ( parameterCount == 0 )
                parameterCount = 1;
            if ( parameterCount <= SHELL_STREAM_CONTROL_MAX_PARAMETERS ) {
                parameters[parameterCount - 1] = parameters[parameterCount - 1] * 10;
                parameters[parameterCount - 1] = parameters[parameterCount - 1] + c - '0';
            }
        }

        else if ( c == ';' )
            parameterCount++;
        else
            break;
    }

    return c;
}

/**
 * set the echo of the Shell
 *
 * @param echo:		the activation flag
 */
void Shell::setEcho(bool echo) {
    std::cout << (char)SHELLKEY_ESC << "{" << (echo ? "1" : "0") << "e";
    std::flush(std::cout);
}

/**
 * set the mode of the shell
 *
 * @param mode:		the mode to set
 */
void Shell::setMode(ShellMode mode) {
    std::cout << (char)SHELLKEY_ESC << "{" << mode << "m";
    std::flush(std::cout);
}

/**
 * get a char from stdin
 *
 * @return the readed value
 */
int Shell::getChar() {
    int c;
    bufferLock.lock();

    // If there are chars in the buffer, take these
    if ( buffer && bufferedChars > 0 ) {
        c = buffer[0];
        memcpy(buffer, &buffer[1], (bufferSize - 1) * sizeof(int));
        --bufferedChars;
    }

    else
        c = readUnbuffered();

    bufferLock.unlock();
    return c;
}

/**
 * put the provided char on the stdout
 *
 * @param c:	the character to write
 */
void Shell::putChar(int c) {
    std::cout << (char)SHELLKEY_ESC << "{" << c << "p";
    std::flush(std::cout);
}

/**
 * set the cursor position from provided position Object
 *
 * @param position:		the position to set
 */
void Shell::setCursor(const ShellCursorPosition& position) {
    std::cout << (char)SHELLKEY_ESC << "[" << position.y << ";" << position.x << "f";
    std::flush(std::cout);
}

/**
 * @return the position of the cursor
 */
ShellCursorPosition Shell::getCursor() {
    // read request
    std::cout << (char)SHELLKEY_ESC << "[6n";
    std::flush(std::cout);

    // read response
    readAndBufferUntilESC();

    int ch = readUnbuffered();
    if ( ch == '[' ) {
        int parameters[SHELL_STREAM_CONTROL_MAX_PARAMETERS];
        for ( int i = 0; i < SHELL_STREAM_CONTROL_MAX_PARAMETERS; i++ )
            parameters[i] = 0;

        // read into parameters
        int mode = readEscapedParameters(parameters);
        if ( mode == 'R' )
            return ShellCursorPosition(parameters[1], parameters[0]);
    }

    // return a non valid position
    return ShellCursorPosition(-1, -1);
}

/**
 * move up the cursor of n positions
 *
 * @param n:	the number of positions to move
 */
void Shell::moveCursorUp(int n) {
    std::cout << (char)SHELLKEY_ESC << "[" << n << "A";
    std::flush(std::cout);
}

/**
 * move down the cursor of n positions
 *
 * @param n:	the number of positions to move
 */
void Shell::moveCursorDown(int n) {
    std::cout << (char)SHELLKEY_ESC << "[" << n << "B";
    std::flush(std::cout);
}

/**
 * move forward the cursor of n positions
 *
 * @param n:	the number of positions to move
 */
void Shell::moveCursorForward(int n) {
    std::cout << (char)SHELLKEY_ESC << "[" << n << "C";
    std::flush(std::cout);
}

/**
 * move back the cursor of n positions
 *
 * @param n:	the number of positions to move
 */
void Shell::moveCursorBack(int n) {
    std::cout << (char)SHELLKEY_ESC << "[" << n << "D";
    std::flush(std::cout);
}

/**
 * @return the size of the shell screen
 */
ShellDimension Shell::getSize() {
    // read request
    std::cout << (char)SHELLKEY_ESC << "{i";
    std::flush(std::cout);

    // read response
    readAndBufferUntilESC();

    int ch = readUnbuffered();
    if ( ch == '{' ) {
        int parameters[SHELL_STREAM_CONTROL_MAX_PARAMETERS];
        for ( int i = 0; i < SHELL_STREAM_CONTROL_MAX_PARAMETERS; i++ )
            parameters[i] = 0;

        // read from parameters
        int mode = readEscapedParameters(parameters);
        if ( mode == 'i' )
            return ShellDimension(parameters[0], parameters[1]);
    }

    // return a non valid size
    return ShellDimension(-1, -1);
}

/**
 * set the provided process as the constroller of the shell
 *
 * @param pid:		the pid of the process
 */
void Shell::setControlProcess(Pid pid) {
    std::cout << (char)SHELLKEY_ESC << "{" << pid << "c";
    std::flush(std::cout);
}

/**
 * clear the shell screen
 */
void Shell::clear() {
    std::cout << (char)SHELLKEY_ESC << "[2J";
    std::flush(std::cout);
}

/**
 * set the scroll area to screen
 */
void Shell::setScrollAreaToScreen() {
    std::cout << (char)SHELLKEY_ESC << "[r";
    std::flush(std::cout);
}

/**
 * set the scroll area to screen from parameters
 *
 * @param start:	the start of the scroll area
 * @param end:		the end of the scroll area
 */
void Shell::setScrollArea(int start, int end) {
    std::cout << (char)SHELLKEY_ESC << "[" << start << ";" << end << "r";
    std::flush(std::cout);
}

/**
 * scroll for provided amount lines
 *
 * @param amount:	the number of line to be scrolled
 */
void Shell::scroll(int value) {
    if ( value >= 0 )
        std::cout << (char)SHELLKEY_ESC << "[" << value << "S";
    else
        std::cout << (char)SHELLKEY_ESC << "[" << -value << "T";
    std::flush(std::cout);
}

/**
 * set the visibility of the cursor
 *
 * @param visible:		the flag for visibility
 */
void Shell::setCursorVisible(bool visible) {
    std::cout << (char)SHELLKEY_ESC << "{" << 0 << ";" << (visible ? 1 : 0) << "C";
    std::flush(std::cout);
}
