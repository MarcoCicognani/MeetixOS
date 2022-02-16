/**
 * @brief
 * This file is part of the MeetiX Operating System.
 * Copyright (c) 2017-2022, Marco Cicognani (marco.cicognani@meetixos.org)
 *
 * @developers
 * Marco Cicognani (marco.cicognani@meetixos.org)
 *
 * @license
 * GNU General Public License version 3
 */

#pragma once

#include <Api.h>
#include <LibTasking/Lock.hh>

#define SHELL_STREAM_CONTROL_MAX_PARAMETERS 4

/**
 * ASCII codes, used for escaping
 */
#define SHELLKEY_SUB 26
#define SHELLKEY_ESC static_cast<char>(27)

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

namespace IO {

/* ------------------------------------------ C++ types ----------------------------------------- */

/**
 * @brief Shell singleton interface
 */
class Shell {
public:
    /**
     * @brief Terminal modes
     */
    enum Mode {
        MODE_DEFAULT,
        MODE_RAW,
        MODE_CBREAK
    };

    /**
     * @brief Cursor position
     */
    struct CursorPosition {
    public:
        int m_y{ 0 };
        int m_x{ 0 };

        CursorPosition() = default;
        CursorPosition(int x, int y)
            : m_x{ x }
            , m_y{ y } {}
    };

    /**
     * @brief Shell dimension
     */
    struct Dimension {
    public:
        int m_width{ 0 };
        int m_height{ 0 };

        Dimension() = default;
        Dimension(int width, int height)
            : m_width{ width }
            , m_height{ height } {}
    };

public:
    /**
     * @brief Returns the global singleton instance
     */
    static Shell& instance();

    /**
     * @brief Reads a character from the <stdin>
     */
    int read_char();

    /**
     * @brief Writes the given character into <stdout>
     */
    void write_char(int c);

    /**
     * @brief Moves up the cursor of <n> positions
     */
    void move_cursor_up(int n);

    /**
     * @brief Moves down the cursor of <n> positions
     */
    void move_cursor_down(int n);

    /**
     * @brief Moves forward the cursor of <n> positions
     */
    void move_cursor_forward(int n);

    /**
     * @brief Moves back the cursor of <n> positions
     */
    void move_cursor_back(int n);

    /**
     * @brief Clears the shell screen
     */
    void clear();

    /**
     * @brief Scrolls for the given <amount> of lines
     */
    void scroll(int amount);

    /**
     * @brief Toggles the shell echo of the input
     */
    void set_echo(bool do_echo);

    /**
     * @brief Sets the shell mode
     */
    void set_mode(Mode mode);

    /**
     * @brief Sets the cursor position on the shell buffer
     */
    void set_cursor(const CursorPosition& position);

    /**
     * @brief Sets the control process of the shell
     */
    void set_control_process(Pid pid);

    /**
     * @brief Sets the scroll area to the screen
     */
    void set_scroll_area_to_screen();

    /**
     * @brief Sets the scroll area to the given range
     */
    void set_scroll_area(int start, int end);

    /**
     * @brief Sets the cursor visibility
     */
    void set_cursor_visible(bool visible);

    /**
     * @brief Returns the current CursorPosition
     */
    CursorPosition cursor();

    /**
     * @brief Returns the shell screen dimension
     */
    Dimension dimension();

private:
    int  read_unbuffered();
    void buffer_char(int c);
    void read_buffered_until_esc();
    int  read_escaped_parameters(int* parameters);

private:
    int*          m_buffer{ nullptr };
    int           m_buffer_len{ 64 };
    int           m_buffered_chars{ 0 };
    Tasking::Lock m_buffer_lock{};
};

} /* namespace IO */