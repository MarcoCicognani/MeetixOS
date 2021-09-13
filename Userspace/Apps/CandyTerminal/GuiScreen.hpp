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

#include "screen.hpp"

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
#include <list>
#include <map>
#include <tasking/lock.hpp>
#include <utils/utils.hpp>

/**
 * Used to cache information about the layout of a specific character.
 */
struct CharLayout {
    cairo_glyph_t*        glyphBuffer = nullptr;
    int                   glyphCount;
    cairo_text_cluster_t* clusterBuffer = nullptr;
    int                   clusterCount;
};

/**
 *
 */
class GuiScreen : public Screen {
private:
    /**
     * UI related properties
     */
    Window* window;
    Canvas* canvas;

    Font_t* font;

    cairo_surface_t* existingSurface       = 0;
    Color_t*         existingSurfaceBuffer = 0;
    Dimension        bufferSize;
    cairo_t*         existingContext = 0;

    uint8_t paintUpToDate = false;
    bool    cursorBlink   = false;

    std::list<Keyboard::Info> inputBuffer;
    uint8_t                   inputBufferEmpty = true;
    Lock                      inputBufferLock;

    bool     focused       = false;
    uint64_t lastInputTime = 0;

    /**
     * Screen buffer
     */
    uint8_t*  rasterBuffer = 0;
    Dimension rasterSize;
    Lock      rasterLock;
    int       cursorX = 0;
    int       cursorY = 0;

    int charWidth  = 8;
    int charHeight = 12;

    std::map<char, CharLayout*> charLayoutCache;

    /**
     * Prepares the canvas buffer for painting.
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
    static void blinkCursorEntry(GuiScreen* screen);
    void        blinkCursor();

    /**
     * paint routine
     */
    static void paintEntry(GuiScreen* screen);
    void        paint();

    /**
     *
     */
    CharLayout* getCharLayout(cairo_scaled_font_t* scaledFace, char c);

public:
    GuiScreen();
    ~GuiScreen();

    /**
     * Initializes the UI components for the screen.
     *
     * @return whether initialization was successful
     */
    bool initialize();

    Keyboard::Info readInput();
    void           clean();
    void           writeChar(char c);
    void           moveCursor(int x, int y);
    int            getCursorX();
    int            getCursorY();
    void           backspace();

    void bufferInput(const Keyboard::Info& info);
    void repaint();
    void setFocused(bool focused);

    void updateVisibleBufferSize();

    virtual int getWidth();
    virtual int getHeight();

    void setScrollAreaScreen();
    void setScrollArea(int start, int end);
    void scroll(int value);

    void setCursorVisible(bool visible);
};
