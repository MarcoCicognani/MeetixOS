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
#include <LibGraphics/Text/Font.hh>
#include <LibGraphics/Text/FontLoader.hh>
#include <LibGraphics/Text/Layouter.hh>
#include <GUI/Application.hh>
#include <GUI/Component/Button.hh>
#include <GUI/Component/Canvas.hh>
#include <GUI/Component/Label.hh>
#include <GUI/Component/TextField.hh>
#include <GUI/Component/Window.hh>
#include <GUI/Listener/ActionListener.hh>
#include <GUI/Listener/FocusListener.hh>
#include <GUI/Listener/KeyListener.hpp>
#include <LibTasking/Lock.hh>
#include <LibUtils/Utils.hh>

using namespace std;

/**
 *
 */
class GUIScreen : public Screen {
private:
    Window* window;
    Canvas* canvas;

    cairo_surface_t*               existingSurface       = 0;
    Graphics::Color::ArgbGradient* existingSurfaceBuffer = 0;
    Graphics::Metrics::Dimension   bufferSize;
    cairo_t*                       existingContext;

    Graphics::Text::Layouted* viewModel;
    Graphics::Text::Font*     font;

    Graphics::Color::ArgbGradient backgroundColor = Graphics::Color::as_argb(0, 0, 0, 0);
    Graphics::Color::ArgbGradient fontColor       = Graphics::Color::as_rgb(255, 255, 255);

    void     initialize();
    cairo_t* getGraphics();

    static void blinkCursorThread();

public:
    GUIScreen();
    ~GUIScreen();

    static void       paintEntry();
    [[noreturn]] void paint();

    void clean();
    void deactivate();
    void activate();

    void close();

    bool setColor(string color);

    void backspace();
    void cleanLine(int lineLenght);
    void write(string                        message,
               Graphics::Color::ArgbGradient color   = Graphics::Color::as_rgb(255, 255, 255),
               bool                          visible = true);
    void writeChar(char c, Graphics::Color::ArgbGradient color = Graphics::Color::as_rgb(255, 255, 255));
    void updateCursor();

    IO::Keyboard::Info readInput(bool* cancelCondition);

    void workingDirectoryChanged(string str);
};
