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
#include "screen.hpp"

#include <Api/utils/llist.hpp>
#include <cairo/cairo.h>
#include <graphics/text/font.hpp>
#include <graphics/text/fontldr.hpp>
#include <graphics/text/textlyot.hpp>
#include <gui/actionlistener.hpp>
#include <gui/button.hpp>
#include <gui/canvas.hpp>
#include <gui/focuslistener.hpp>
#include <gui/keylistener.hpp>
#include <gui/label.hpp>
#include <gui/textfield.hpp>
#include <gui/ui.hpp>
#include <gui/window.hpp>
#include <tasking/lock.hpp>
#include <utils/utils.hpp>

using namespace std;

/**
 *
 */
class GuiScreen : public Screen {
private:
    Window* window;
    Canvas* canvas;

    cairo_surface_t* existingSurface       = 0;
    Color_t*         existingSurfaceBuffer = 0;
    Dimension        bufferSize;
    cairo_t*         existingContext;

    LayoutedText* viewModel;
    Font_t*       font;

    Color_t backgroundColor = ARGB(0, 0, 0, 0);
    Color_t fontColor       = RGB(255, 255, 255);

    void     initialize();
    cairo_t* getGraphics();

    static void blinkCursorThread();

public:
    GuiScreen();
    ~GuiScreen();

    static void paintEntry();
    void        paint();

    void clean();
    void deactivate();
    void activate();

    void close();

    bool setColor(string color);

    void backspace();
    void cleanLine(int lineLenght);
    void write(string message, Color_t color = RGB(255, 255, 255), bool visible = true);
    void writeChar(char c, Color_t color = RGB(255, 255, 255));
    void updateCursor();

    Keyboard::Info readInput(bool* cancelCondition);

    void workingDirectoryChanged(string str);
};
