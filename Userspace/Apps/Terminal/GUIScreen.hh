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

#include "Screen.hh"

#include <cairo/cairo.h>
#include <Graphics/Text/Font.hh>
#include <Graphics/Text/FontLoader.hh>
#include <Graphics/Text/Layouter.hh>
#include <GUI/Application.hh>
#include <GUI/Component/Button.hh>
#include <GUI/Component/Canvas.hh>
#include <GUI/Component/Label.hh>
#include <GUI/Component/TextField.hh>
#include <GUI/Component/Window.hh>
#include <GUI/Listener/ActionListener.hh>
#include <GUI/Listener/FocusListener.hh>
#include <GUI/Listener/KeyListener.hpp>
#include <list>
#include <map>
#include <Tasking/Lock.hh>
#include <Utils/Utils.hh>

/**
 * Used to cache information about the layout of a specific character.
 */
struct CharLayout {
    cairo_glyph_t*        glyphBuffer{ nullptr };
    int                   glyphCount{};
    cairo_text_cluster_t* clusterBuffer{ nullptr };
    int                   clusterCount{};
};

/**
 *
 */
class GUIScreen : public Screen {
private:
    /**
     * UI related properties
     */
    Window* window;
    Canvas* canvas;

    Graphics::Text::Font* font;

    cairo_surface_t*               existingSurface       = 0;
    Graphics::Color::ArgbGradient* existingSurfaceBuffer = 0;
    Graphics::Metrics::Dimension   bufferSize;
    cairo_t*                       existingContext = 0;

    bool paintUpToDate = false;
    bool cursorBlink   = false;

    std::list<IO::Keyboard::Info> inputBuffer;
    bool                          inputBufferEmpty = true;
    Tasking::Lock                 inputBufferLock;

    bool     focused       = false;
    uint64_t lastInputTime = 0;

    /**
     * Screen buffer
     */
    uint8_t*                     rasterBuffer = 0;
    Graphics::Metrics::Dimension rasterSize;
    Tasking::Lock                rasterLock;
    int                          cursorX = 0;
    int                          cursorY = 0;

    int charWidth  = 8;
    int charHeight = 12;

    std::map<char, CharLayout*> charLayoutCache;

    /**
     * Prepares the m_canvas buffer for painting.
     *
     * @return the cairo instance to paint with
     */
    cairo_t* getGraphics();

    /**
     * Thread that is responsible for making the cursor
     * blink in specific intervals.
     *
     * @param screen
     * 		instance of the screen
     */
    static void blinkCursorEntry(GUIScreen* screen);
    void        blinkCursor();

    /**
     * paint routine
     */
    static void       paintEntry(GUIScreen* screen);
    [[noreturn]] [[noreturn]] void paint();

    /**
     *
     */
    CharLayout* getCharLayout(cairo_scaled_font_t* scaledFace, char c);

public:
    GUIScreen();
    ~GUIScreen();

    /**
     * Initializes the UI components for the screen.
     *
     * @return whether initialization was successful
     */
    bool init();

    IO::Keyboard::Info read_input();
    void               clean();
    void               write_char(char c);
    void               move_cursor(int x, int y);
    int                cursor_x();
    int                cursor_y();
    void               backspace();

    void bufferInput(const IO::Keyboard::Info& info);
    void repaint();
    void setFocused(bool focused);

    void updateVisibleBufferSize();

    virtual int width();
    virtual int height();

    void set_scroll_area_screen();
    void set_scroll_area(int start, int end);
    void scroll(int value);

    void set_cursor_visible(bool visible);
};
