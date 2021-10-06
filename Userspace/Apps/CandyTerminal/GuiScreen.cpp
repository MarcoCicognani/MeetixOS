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

#include "GuiScreen.hpp"

#include <Api.h>
#include <IO/Keyboard.hh>
#include <list>
#include <string.h>
#include <Tasking/Lock.hh>

GuiScreen* instance;

/**
 *
 */
class CanvasResizeBoundsListener_t : public BoundsListener {
public:
    GuiScreen* screen;

    CanvasResizeBoundsListener_t(GuiScreen* screen) : screen(screen) {
    }

    virtual void handleBoundsChanged(Graphics::Metrics::Rectangle bounds) {
        screen->updateVisibleBufferSize();
    }
};

/**
 *
 */
class InputKeyListener_t : public KeyListener {
private:
    GuiScreen* screen;

public:
    /**
     *
     */
    InputKeyListener_t(GuiScreen* screen) : screen(screen) {
    }

    /**
     *
     */
    virtual void handleKeyEvent(KeyEvent& e) {
        screen->bufferInput(IO::Keyboard::instance().full_key_info(e.info));
    }
};

/**
 *
 */
class CanvasBufferListener_t : public CanvasBufferListener {
private:
    GuiScreen* screen;

public:
    /**
     *
     */
    CanvasBufferListener_t(GuiScreen* screen) : screen(screen) {
    }

    /**
     *
     */
    virtual void handleBufferChanged() {
        screen->updateVisibleBufferSize();
        screen->repaint();
    }
};

/**
 *
 */
class TerminalFocusListener_t : public FocusListener {
private:
    GuiScreen* screen;

public:
    /**
     *
     */
    TerminalFocusListener_t(GuiScreen* screen) : screen(screen) {
    }

    /**
     *
     */
    virtual void handleFocusChanged(bool nowFocused) {
        screen->setFocused(nowFocused);
        screen->repaint();
    }
};

/*
 *
 */
static void exitTerminalEntry() {
    delete instance;
}

/*
 *
 */
GuiScreen::GuiScreen() {
    instance = this;
}

/*
 *
 */
GuiScreen::~GuiScreen() {
    if ( existingContext )
        cairo_destroy(existingContext);
    // delete existingSurfaceBuffer;
    // delete rasterBuffer;
    // delete font;

    UI::close();
}

/**
 *
 */
bool GuiScreen::initialize() {
    // initialize user interface
    auto status = UI::open();
    if ( status != UI_OPEN_STATUS_SUCCESSFUL ) {
        klog("Terminal : Failed to initialize gui with status %i", status);
        return false;
    }

    window = Window::create();
    window->setTitle("CandyTerminal");
    window->setColor(Graphics::Color::as_argb(100, 0, 0, 0), Graphics::Color::as_argb(255, 255, 255, 255));
    window->onClose(exitTerminalEntry);

    canvas = Canvas::create();
    window->setLayout(UI_LAYOUT_MANAGER_GRID);
    window->addChild(canvas);

    auto windowBounds = Graphics::Metrics::Rectangle(200, 200, 600, 480);
    window->setBounds(windowBounds);
    canvas->setBounds(Graphics::Metrics::Rectangle(0, 0, windowBounds.width(), windowBounds.height()));

    canvas->setBufferListener(new CanvasBufferListener_t(this));
    canvas->setBoundsListener(new CanvasResizeBoundsListener_t(this));
    canvas->setListener(UI_COMPONENT_EVENT_TYPE_KEY, new InputKeyListener_t(this));
    window->setListener(UI_COMPONENT_EVENT_TYPE_FOCUS, new TerminalFocusListener_t(this));
    window->setVisible(true);

    font = Graphics::Text::FontLoader::get("consolas");

    s_create_thread_d((void*)paintEntry, this);
    s_create_thread_d((void*)blinkCursorEntry, this);
    return true;
}

/**
 *
 */
void GuiScreen::blinkCursorEntry(GuiScreen* screen) {
    while ( true ) {
        screen->blinkCursor();
        s_sleep(500);
    }
}

/**
 *
 */
void GuiScreen::blinkCursor() {
    cursorBlink = !cursorBlink;
    repaint();
}

/**
 *
 */
void GuiScreen::paintEntry(GuiScreen* screen) {
    screen->paint();
}

/**
 *
 */
CharLayout* GuiScreen::getCharLayout(cairo_scaled_font_t* scaledFace, char c) {
    // Take char from layout cache
    auto entry = charLayoutCache.find(c);
    if ( entry != charLayoutCache.end() ) {
        return (*entry).second;
    }

    // Perform layouting
    char cbuf[2];
    cbuf[0] = c;
    cbuf[1] = 0;

    CharLayout*                layout = new CharLayout();
    cairo_text_cluster_flags_t clusterFlags;
    cairo_status_t             stat = cairo_scaled_font_text_to_glyphs(scaledFace,
                                                                       0,
                                                                       0,
                                                                       cbuf,
                                                                       1,
                                                                       &layout->glyphBuffer,
                                                                       &layout->glyphCount,
                                                                       &layout->clusterBuffer,
                                                                       &layout->clusterCount,
                                                                       &clusterFlags);

    if ( stat == CAIRO_STATUS_SUCCESS ) {
        charLayoutCache[c] = layout;
        return layout;
    }
    return 0;
}

/**
 *
 */
[[noreturn]] void GuiScreen::paint() {
    int padding = 0;
    while ( true ) {
        auto windowBounds = window->getBounds();
        updateVisibleBufferSize();

        cairo_t* cr = getGraphics();
        if ( !cr ) {
            s_sleep(100);
            continue;
        }

        // clear
        cairo_save(cr);
        cairo_set_source_rgba(cr, 0, 0, 0, 0);
        cairo_set_operator(cr, CAIRO_OPERATOR_SOURCE);
        cairo_paint(cr);
        cairo_restore(cr);

        rasterLock.lock();

        // paint cursor
        bool blinkON = false;
        if ( focused ) {
            blinkON = (s_millis() - lastInputTime < 300) || cursorBlink;

            if ( blinkON ) {
                cairo_save(cr);
                cairo_set_source_rgba(cr, 255, 255, 255, 255);
                cairo_rectangle(cr,
                                cursorX * charWidth + padding,
                                cursorY * charHeight + padding + 1,
                                charWidth,
                                charHeight + 1);
                cairo_fill(cr);
                cairo_restore(cr);
            }
        }

        // paint characters
        if ( rasterBuffer ) {
            cairo_set_font_face(cr, font->cairo_font_face());
            cairo_set_font_size(cr, 14);
            auto scaledFace = cairo_get_scaled_font(cr);

            for ( int y = 0; y < rasterSize.height(); y++ ) {
                for ( int x = 0; x < rasterSize.width(); x++ ) {
                    uint8_t c = rasterBuffer[y * rasterSize.width() + x];
                    if ( !c )
                        continue;

                    // Render only the character
                    CharLayout* charLayout = getCharLayout(scaledFace, c);

                    if ( charLayout ) {
                        cairo_save(cr);
                        if ( cursorX == x && cursorY == y && blinkON )
                            cairo_set_source_rgba(cr, 0, 0, 0, 1);
                        else
                            cairo_set_source_rgba(cr, 1, 1, 1, 1);

                        cairo_translate(cr,
                                        x * charWidth + padding,
                                        (y + 1) * charHeight + padding);
                        cairo_glyph_path(cr,
                                         charLayout->glyphBuffer,
                                         charLayout->clusterBuffer[0].num_glyphs);
                        cairo_fill(cr);
                        cairo_restore(cr);
                    }
                }
            }
        }

        rasterLock.unlock();

        canvas->blit(Graphics::Metrics::Rectangle(0, 0, bufferSize.width(), bufferSize.height()));

        paintUpToDate = true;
        s_atomic_block(&paintUpToDate);
    }
}

/**
 *
 */
cairo_t* GuiScreen::getGraphics() {
    // get buffer
    auto bufferInfo = canvas->getBuffer();
    if ( !bufferInfo.buffer )
        return 0;

    bufferSize.set_width  ( bufferInfo.width);
    bufferSize.set_height ( bufferInfo.height);

    // get the surface ready and go:
    if ( !existingSurface || existingSurfaceBuffer != bufferInfo.buffer ) {
        if ( existingContext )
            cairo_destroy(existingContext);

        existingSurface = cairo_image_surface_create_for_data(
            (uint8_t*)bufferInfo.buffer,
            CAIRO_FORMAT_ARGB32,
            bufferInfo.width,
            bufferInfo.height,
            cairo_format_stride_for_width(CAIRO_FORMAT_ARGB32, bufferInfo.width));
        existingSurfaceBuffer = bufferInfo.buffer;
        existingContext       = cairo_create(existingSurface);
    }

    return existingContext;
}

/**
 *
 */
bool charIsUtf8(char c) {
    if ( (c == 0x09 || c == 0x0A || c == 0x0D || (0x20 <= c && c <= 0x7E)) )
        return true;
    return false;
}

/**
 *
 */
void GuiScreen::clean() {
    rasterLock.lock();
    memset(rasterBuffer, 0, rasterSize.height() * rasterSize.width());
    rasterLock.unlock();
    repaint();
}

/**
 *
 */
void GuiScreen::backspace() {
    moveCursor(cursorX - 1, cursorY);
    writeChar(' ');
    moveCursor(cursorX - 1, cursorY);
    repaint();
}

/**
 *
 */
void GuiScreen::writeChar(char c) {
    if ( charIsUtf8(c) ) {
        if ( rasterBuffer ) {
            if ( c == '\n' )
                moveCursor(0, cursorY + 1);
            else {
                rasterLock.lock();
                rasterBuffer[cursorY * rasterSize.width() + cursorX] = c;
                rasterLock.unlock();
                moveCursor(cursorX + 1, cursorY);
            }
        }

        paintUpToDate = false;
    }
}

/**
 *
 */
IO::Keyboard::Info GuiScreen::readInput() {
    if ( !inputBuffer.size() )
        s_atomic_block(&inputBufferEmpty);

    inputBufferLock.lock();

    IO::Keyboard::Info result = inputBuffer.front();
    inputBuffer.pop_front();
    if ( !inputBuffer.size() )
        inputBufferEmpty = true;

    inputBufferLock.unlock();

    return result;
}

/**
 *
 */
void GuiScreen::moveCursor(int x, int y) {
    rasterLock.lock();
    cursorX = x;
    cursorY = y;

    // Break line when cursor is at end of screen
    if ( cursorX >= rasterSize.width() ) {
        cursorX = 0;
        cursorY++;
    }
    if ( cursorX < 0 ) {
        cursorX = rasterSize.width() - 1;
        cursorY--;
    }

    // Scroll screen if required
    if ( cursorY >= rasterSize.height() ) {
        int rasterWidth = rasterSize.width();
        int rasterBytes = rasterWidth * rasterSize.height();

        memcpy(rasterBuffer, &rasterBuffer[rasterWidth], rasterBytes - rasterWidth);

        for ( uint32_t i = 0; i < rasterWidth; i++ )
            rasterBuffer[rasterBytes - rasterWidth + i] = ' ';

        cursorY--;
    }

    rasterLock.unlock();

    repaint();
}

/**
 *
 */
int GuiScreen::getCursorX() {
    return cursorX;
}

/**
 *
 */
int GuiScreen::getCursorY() {
    return cursorY;
}

/**
 *
 */
void GuiScreen::bufferInput(const IO::Keyboard::Info& info) {
    inputBufferLock.lock();
    inputBuffer.push_back(info);
    lastInputTime    = s_millis();
    inputBufferEmpty = false;
    inputBufferLock.unlock();
}

/**
 *
 */
void GuiScreen::repaint() {
    paintUpToDate = false;
}

/**
 *
 */
void GuiScreen::setFocused(bool _focused) {
    lastInputTime = s_millis();
    focused       = _focused;
}

/**
 *
 */
void GuiScreen::updateVisibleBufferSize() {
    auto canvasBounds   = canvas->getBounds();
    int       requiredWidth  = canvasBounds.width() / charWidth;
    int       requiredHeight = canvasBounds.height() / charHeight;

    if ( requiredWidth < charWidth )
        requiredWidth = charWidth;
    if ( requiredHeight < charHeight )
        requiredHeight = charHeight;

    if ( !rasterBuffer
         || (rasterSize.width() < requiredWidth || rasterSize.height() < requiredHeight) ) {
        rasterLock.lock();

        uint8_t*  oldBuffer     = rasterBuffer;
        auto oldBufferSize = rasterSize;

        // Create a new buffer
        rasterBuffer = new uint8_t[requiredWidth * requiredHeight];
        rasterSize   = Graphics::Metrics::Dimension(requiredWidth, requiredHeight);
        memset(rasterBuffer, 0, requiredWidth * requiredHeight);

        // Copy contents of old buffer and delete it
        if ( oldBuffer ) {
            for ( int y = 0; y < oldBufferSize.height(); y++ )
                for ( int x = 0; x < oldBufferSize.width(); x++ )
                    rasterBuffer[y * rasterSize.width() + x] = oldBuffer[y * oldBufferSize.width() + x];

            delete oldBuffer;
        }

        // Ensure cursor is in bounds
        if ( cursorX > rasterSize.width() )
            cursorX = rasterSize.width();
        if ( cursorY > rasterSize.height() )
            cursorY = rasterSize.height();

        rasterLock.unlock();

        repaint();
    }
}

/**
 *
 */
void GuiScreen::setScrollAreaScreen() { /* TODO */
}

/**
 *
 */
void GuiScreen::setScrollArea(int start, int end) { /* TODO */
}

/**
 *
 */
void GuiScreen::scroll(int value) { /* TODO */
}

/**
 *
 */
void GuiScreen::setCursorVisible(bool visible) { /* TODO */
}

/**
 *
 */
int GuiScreen::getWidth() {
    return rasterSize.width();
}

/**
 *
 */
int GuiScreen::getHeight() {
    return rasterSize.height();
}
