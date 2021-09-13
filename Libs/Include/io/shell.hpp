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

#ifndef MEETIX_LIBRARY_IO_SHELL
#define MEETIX_LIBRARY_IO_SHELL

#include <eva.h>

#define SHELL_STREAM_CONTROL_MAX_PARAMETERS 4

/**
 * ASCII codes, used for escaping
 */
#define SHELLKEY_SUB 26
#define SHELLKEY_ESC 27

/**
 * Extended key codes, derived from ncurses
 */
#define SHELLKEY_MIN       0x101 /* minimum extended key value */
#define SHELLKEY_BREAK     0x101 /* break key */
#define SHELLKEY_DOWN      0x102 /* down arrow */
#define SHELLKEY_UP        0x103 /* up arrow */
#define SHELLKEY_LEFT      0x104 /* left arrow */
#define SHELLKEY_RIGHT     0x105 /* right arrow*/
#define SHELLKEY_HOME      0x106 /* home key */
#define SHELLKEY_BACKSPACE 0x107 /* Backspace */

#define SHELLKEY_F0   0x108
#define SHELLKEY_F(n) (SHELLKEY_F0 + (n))

#define SHELLKEY_DL     0x148 /* Delete Line */
#define SHELLKEY_IL     0x149 /* Insert Line*/
#define SHELLKEY_DC     0x14A /* Delete Character */
#define SHELLKEY_IC     0x14B /* Insert Character */
#define SHELLKEY_EIC    0x14C /* Exit Insert Char mode */
#define SHELLKEY_CLEAR  0x14D /* Clear screen */
#define SHELLKEY_EOS    0x14E /* Clear to end of screen */
#define SHELLKEY_EOL    0x14F /* Clear to end of line */
#define SHELLKEY_SF     0x150 /* Scroll one line forward */
#define SHELLKEY_SR     0x151 /* Scroll one line back */
#define SHELLKEY_NPAGE  0x152 /* Next Page */
#define SHELLKEY_PPAGE  0x153 /* Prev Page */
#define SHELLKEY_STAB   0x154 /* Set Tab */
#define SHELLKEY_CTAB   0x155 /* Clear Tab */
#define SHELLKEY_CATAB  0x156 /* Clear All Tabs */
#define SHELLKEY_ENTER  0x157 /* Enter or Send */
#define SHELLKEY_SRESET 0x158 /* Soft Reset */
#define SHELLKEY_RESET  0x159 /* Hard Reset */
#define SHELLKEY_PRINT  0x15A /* Print */
#define SHELLKEY_LL     0x15B /* Home Down */

/*
 * "Keypad" keys arranged like this:
 *
 *  A1   up  A3
 * left  B2 right
 *  C1  down C3
 *
 */
#define SHELLKEY_A1 0x15C /* Keypad upper left */
#define SHELLKEY_A3 0x15D /* Keypad upper right */
#define SHELLKEY_B2 0x15E /* Keypad centre key */
#define SHELLKEY_C1 0x15F /* Keypad lower left */
#define SHELLKEY_C3 0x160 /* Keypad lower right */

#define SHELLKEY_BTAB      0x161 /* Back Tab */
#define SHELLKEY_BEG       0x162 /* Begin key */
#define SHELLKEY_CANCEL    0x163 /* Cancel key */
#define SHELLKEY_CLOSE     0x164 /* Close Key */
#define SHELLKEY_COMMAND   0x165 /* Command Key */
#define SHELLKEY_COPY      0x166 /* Copy key */
#define SHELLKEY_CREATE    0x167 /* Create key */
#define SHELLKEY_END       0x168 /* End key */
#define SHELLKEY_EXIT      0x169 /* Exit key */
#define SHELLKEY_FIND      0x16A /* Find key */
#define SHELLKEY_HELP      0x16B /* Help key */
#define SHELLKEY_MARK      0x16C /* Mark key */
#define SHELLKEY_MESSAGE   0x16D /* Message key */
#define SHELLKEY_MOVE      0x16E /* Move key */
#define SHELLKEY_NEXT      0x16F /* Next Object key */
#define SHELLKEY_OPEN      0x170 /* Open key */
#define SHELLKEY_OPTIONS   0x171 /* Options key */
#define SHELLKEY_PREVIOUS  0x172 /* Previous Object key */
#define SHELLKEY_REDO      0x173 /* Redo key */
#define SHELLKEY_REFERENCE 0x174 /* Ref Key */
#define SHELLKEY_REFRESH   0x175 /* Refresh key */
#define SHELLKEY_REPLACE   0x176 /* Replace key */
#define SHELLKEY_RESTART   0x177 /* Restart key */
#define SHELLKEY_RESUME    0x178 /* Resume key */
#define SHELLKEY_SAVE      0x179 /* Save key */
#define SHELLKEY_SBEG      0x17A /* Shift begin key */
#define SHELLKEY_SCANCEL   0x17B /* Shift Cancel key */
#define SHELLKEY_SCOMMAND  0x17C /* Shift Command key */
#define SHELLKEY_SCOPY     0x17D /* Shift Copy key */
#define SHELLKEY_SCREATE   0x17E /* Shift Create key */
#define SHELLKEY_SDC       0x17F /* Shift Delete Character */
#define SHELLKEY_SDL       0x180 /* Shift Delete Line */
#define SHELLKEY_SELECT    0x181 /* Select key */
#define SHELLKEY_SEND      0x182 /* Send key */
#define SHELLKEY_SEOL      0x183 /* Shift Clear Line key */
#define SHELLKEY_SEXIT     0x184 /* Shift Exit key */
#define SHELLKEY_SFIND     0x185 /* Shift Find key */
#define SHELLKEY_SHELP     0x186 /* Shift Help key */
#define SHELLKEY_SHOME     0x187 /* Shift Home key */
#define SHELLKEY_SIC       0x188 /* Shift Input key */
#define SHELLKEY_SLEFT     0x189 /* Shift Left Arrow key */
#define SHELLKEY_SMESSAGE  0x18A /* Shift Message key */
#define SHELLKEY_SMOVE     0x18B /* Shift Move key */
#define SHELLKEY_SNEXT     0x18C /* Shift Next key */
#define SHELLKEY_SOPTIONS  0x18D /* Shift Options key */
#define SHELLKEY_SPREVIOUS 0x18E /* Shift Previous key */
#define SHELLKEY_SPRINT    0x18F /* Shift Print key */
#define SHELLKEY_SREDO     0x190 /* Shift Redo key */
#define SHELLKEY_SREPLACE  0x191 /* Shift Replace key */
#define SHELLKEY_SRIGHT    0x192 /* Shift Right Arrow key */
#define SHELLKEY_SRSUME    0x193 /* Shift Resume key */
#define SHELLKEY_SSAVE     0x194 /* Shift Save key */
#define SHELLKEY_SSUSPEND  0x195 /* Shift Suspend key */
#define SHELLKEY_SUNDO     0x196 /* Shift Undo key */
#define SHELLKEY_SUSPEND   0x197 /* Suspend key */
#define SHELLKEY_UNDO      0x198 /* Undo key */
#define SHELLKEY_MOUSE     0x199 /* Mouse event has occurred */
#define SHELLKEY_RESIZE    0x200 /* Resize event has occurred */
#define SHELLKEY_MAX       0x240 /* maximum extended key value */

/**
 * Different terminal modes
 */
typedef int ShellMode;
#define SHELL_MODE_DEFAULT ((ShellMode)0)
#define SHELL_MODE_RAW     ((ShellMode)1)
#define SHELL_MODE_CBREAK  ((ShellMode)2)

/**
 * Cursor position descriptor
 */
typedef struct _ShellCursorPosition {
public:
    /**
     * filled constructor
     *
     * @param x:	the x position requested
     * @param y:	the y position requested
     */
    _ShellCursorPosition(int x, int y) : x(x), y(y) {
    }

    /**
     * empty constructor
     */
    _ShellCursorPosition() : x(0), y(0) {
    }

    /**
     * internal data
     */
    int y;
    int x;
} ShellCursorPosition;

/**
 * Shell dimension descriptor
 */
typedef struct _ShellDimension {
    /**
     * filled constructor
     *
     * @param w:	the width size requested
     * @param h:	the height size requested
     */
    _ShellDimension(int w, int h) : w(w), h(h) {
    }

    /**
     * empty constructor
     */
    _ShellDimension() : w(0), h(0) {
    }

    /**
     * internal data
     */
    int w;
    int h;
} ShellDimension;

/**
 * Terminal client access class.
 */
class Shell {
private:
    /**
     * read unbuffered the characters from stdin
     *
     * @return the readed character value
     */
    static int readUnbuffered();

    /**
     * store to buffer the provided character
     *
     * @param c:	the character to store to
     */
    static void bufferChar(int c);

    /**
     * reads each character from stdin until the escaping character
     * and buffer each character to the global buffer
     */
    static void readAndBufferUntilESC();

    /**
     * read the escaped parameter value from the provided array
     *
     * @param parameters:	the provided escaped parameters
     * @return the last escaped parameter value
     */
    static int readEscapedParameters(int* parameters);

public:
    /**
     * set the echo of the Shell
     *
     * @param echo:		the activation flag
     */
    static void setEcho(bool echo);

    /**
     * set the mode of the shell
     *
     * @param mode:		the mode to set
     */
    static void setMode(ShellMode mode);

    /**
     * get a char from stdin
     *
     * @return the readed value
     */
    static int getChar();

    /**
     * put the provided char on the stdout
     *
     * @param c:	the character to write
     */
    static void putChar(int c);

    /**
     * set the cursor position from provided position Object
     *
     * @param position:		the position to set
     */
    static void setCursor(const ShellCursorPosition& position);

    /**
     * @return the position of the cursor
     */
    static ShellCursorPosition getCursor();

    /**
     * move up the cursor of n positions
     *
     * @param n:	the number of positions to move
     */
    static void moveCursorUp(int n);

    /**
     * move down the cursor of n positions
     *
     * @param n:	the number of positions to move
     */
    static void moveCursorDown(int n);

    /**
     * move forward the cursor of n positions
     *
     * @param n:	the number of positions to move
     */
    static void moveCursorForward(int n);

    /**
     * move back the cursor of n positions
     *
     * @param n:	the number of positions to move
     */
    static void moveCursorBack(int n);

    /**
     * @return the size of the shell screen
     */
    static ShellDimension getSize();

    /**
     * set the provided process as the constroller of the shell
     *
     * @param pid:		the pid of the process
     */
    static void setControlProcess(Pid pid);

    /**
     * clear the shell screen
     */
    static void clear();

    /**
     * set the scroll area to screen
     */
    static void setScrollAreaToScreen();

    /**
     * set the scroll area to screen from parameters
     *
     * @param start:	the start of the scroll area
     * @param end:		the end of the scroll area
     */
    static void setScrollArea(int start, int end);

    /**
     * scroll for provided amount lines
     *
     * @param amount:	the number of line to be scrolled
     */
    static void scroll(int amount);

    /**
     * set the visibility of the cursor
     *
     * @param visible:		the flag for visibility
     */
    static void setCursorVisible(bool visible);
};

#endif
