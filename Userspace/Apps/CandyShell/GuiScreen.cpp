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

#include "CandyShell.hpp"

#include <eva.h>
#include <io/keyboard.hpp>
#include <list>
#include <string.h>
#include <tasking/lock.hpp>

uint8_t  paintIsFresh = false;
bool     cursorBlink  = false;
bool     focus        = false;
uint64_t lastInput    = 0;

list<Keyboard::Info> waitingInput;
uint8_t              noInputAvailable = true;
Lock                 waitingInputLock;

GuiScreen* instance;

vector<Color_t> colors;
string          output;

/*
 *
 */
class InputKeyListener : public KeyListener {
    virtual void handleKeyEvent(KeyEvent& e) {
        waitingInputLock.lock();
        waitingInput.push_back(Keyboard::instance().fullKeyInfo(e.info));
        lastInput        = Millis();
        noInputAvailable = false;
        waitingInputLock.unlock();
    }
};

/*
 *
 */
class CanvasResizeBoundsListener : public BoundsListener {
public:
    Canvas* localCanvas;

    CanvasResizeBoundsListener(Canvas* canvas) : localCanvas(canvas) {
    }

    virtual void handleBoundsChanged(Rectangle bounds) {
        paintIsFresh = false;
    }
};

/*
 *
 */
class CanvasBufferListener_t : public CanvasBufferListener {
public:
    virtual void handleBufferChanged() {
        paintIsFresh = false;
    }
};

/*
 *
 */
class ShellFocusListener : public FocusListener {
public:
    virtual void handleFocusChanged(bool nowFocused) {
        focus        = nowFocused;
        paintIsFresh = false;
        lastInput    = Millis();
    }
};

/**
 *
 */
GuiScreen::GuiScreen() {
    instance = this;
    initialize();
}

GuiScreen::~GuiScreen() {
    UI::close();
}

/*
 *
 */
void exitEntry() {
    CandyShell::exit(instance);
}

/**
 *
 */
void GuiScreen::initialize() {
    window = Window::create();
    window->setTitle("CandyShell");
    window->onClose([] { UI::close(); });

    canvas = Canvas::create();
    window->addChild(canvas);

    window->setBoundsListener(new CanvasResizeBoundsListener(canvas));

    Dimension resolution = UI::getResolution();
    Rectangle windowBounds
        = Rectangle(resolution.width / 2 - 275, resolution.height / 2 - 175, 550, 350);
    window->setBounds(windowBounds);
    window->setColor(ARGB(100, 0, 0, 0), ARGB(255, 255, 255, 255));
    canvas->setBounds(Rectangle(0, 0, windowBounds.width, windowBounds.height));

    canvas->setListener(UI_COMPONENT_EVENT_TYPE_KEY, new InputKeyListener());
    canvas->setBufferListener(new CanvasBufferListener_t());
    window->setListener(UI_COMPONENT_EVENT_TYPE_FOCUS, new ShellFocusListener());
    window->setVisible(true);

    font      = FontLoader::get("consolas");
    viewModel = TextLayouter::getInstance()->initializeBuffer();

    CreateThreadN((void*)&paintEntry, "canvas");
    CreateThreadN((void*)&blinkCursorThread, "blinker");
}

/**
 *
 */
void GuiScreen::paintEntry() {
    instance->paint();
}

void GuiScreen::blinkCursorThread() {
    while ( true ) {
        cursorBlink  = !cursorBlink;
        paintIsFresh = false;
        Sleep(300);
    }
}

/**
 *
 */
void GuiScreen::paint() {
    int       padding = 5;
    Rectangle windowBounds;
    cairo_t*  cr;

    while ( true ) {
        windowBounds = window->getBounds();
        canvas->setBounds(Rectangle(0, 0, windowBounds.width, windowBounds.height));

        cr = getGraphics();
        if ( !cr ) {
            Sleep(100);
            continue;
        }

        // clear
        cairo_save(cr);
        // background
        cairo_set_source_rgba(cr, ARGB_TO_CAIRO_PARAMS(backgroundColor));
        cairo_set_operator(cr, CAIRO_OPERATOR_SOURCE);
        cairo_paint(cr);
        cairo_restore(cr);

        // relayout text
        TextLayouter::getInstance()->layout(cr,
                                            output.c_str(),
                                            font,
                                            14,
                                            Rectangle(padding,
                                                      padding,
                                                      windowBounds.width - 2 * padding - 20,
                                                      windowBounds.height - 2 * padding),
                                            TextAlignment::LEFT,
                                            viewModel,
                                            true);

        // check which is the lowest-down-the-screen character
        int highesty = 0;
        for ( PositionedGlyph& g : viewModel->positions ) {
            int ypos = g.position.y - g.glyph->y;
            if ( ypos > highesty )
                highesty = ypos;
        }

        // calculate limit
        int yLimit  = windowBounds.height - 60;
        int yOffset = 0;
        if ( highesty > yLimit )
            yOffset = yLimit - highesty;

        // paint characters
        Point last;
        int   i = 0;

        for ( PositionedGlyph& g : viewModel->positions ) {
            last = g.position;

            cairo_save(cr);
            cairo_set_source_rgba(cr, ARGB_TO_CAIRO_PARAMS(colors[i]));
            cairo_translate(cr, g.position.x - g.glyph->x, yOffset + g.position.y - g.glyph->y);
            cairo_glyph_path(cr, g.glyph, g.glyphCount);
            cairo_fill(cr);
            cairo_restore(cr);

            i++;
        }

        // paint cursor
        if ( (Millis() - lastInput < 300) || cursorBlink ) {
            cairo_save(cr);
            // cursor
            cairo_set_source_rgba(cr, ARGB_TO_CAIRO_PARAMS(fontColor));
            cairo_rectangle(cr, last.x + 10, yOffset + last.y - 12, 3, 14);

            focus ? cairo_fill(cr) : cairo_stroke(cr);
            cairo_restore(cr);
        }

        canvas->blit(Rectangle(0, 0, bufferSize.width, bufferSize.height));

        paintIsFresh = true;
        AtomicBlock(&paintIsFresh);
    }
}

/**
 *
 */
cairo_t* GuiScreen::getGraphics() {
    // get buffer
    CanvasBufferInfo bufferInfo = canvas->getBuffer();
    if ( bufferInfo.buffer == 0 )
        return 0;

    bufferSize.width  = bufferInfo.width;
    bufferSize.height = bufferInfo.height;

    // get the surface ready and go:
    if ( existingSurface == 0 || existingSurfaceBuffer != bufferInfo.buffer ) {
        if ( existingContext != 0 )
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
    return (c == 0x09 || c == 0x0A || c == 0x0D || (0x20 <= c && c <= 0x7E));
}

/**
 *
 */
void GuiScreen::clean() {
    output = "";
    colors.clear();
    paintIsFresh = false;
}

/**
 *
 */
void GuiScreen::activate() {
}

/*
 *
 */
void GuiScreen::close() {
    delete instance;
}

/**
 *
 */
bool GuiScreen::setColor(string color) {
    if ( color == "red" ) {
        backgroundColor = RGB(255, 0, 0);
        fontColor       = RGB(0, 0, 0);

        return true;
    }

    else if ( color == "green" ) {
        backgroundColor = RGB(0, 255, 0);
        fontColor       = RGB(0, 0, 0);

        return true;
    }

    else if ( color == "black" ) {
        backgroundColor = RGB(0, 0, 0);
        fontColor       = RGB(255, 255, 255);

        return true;
    }

    else if ( color == "white" ) {
        backgroundColor = RGB(255, 255, 255);
        fontColor       = RGB(0, 0, 0);

        return true;
    }

    else
        return false;
}

/**
 *
 */
void GuiScreen::deactivate() {
}

/**
 *
 */
void GuiScreen::updateCursor() {
}

/**
 *
 */
void GuiScreen::writeChar(char c, Color_t color) {
    if ( charIsUtf8(c) ) {
        output = output + c;
        if ( c != '\n' )
            colors.push_back(color);

        paintIsFresh = false;
    }
}

/**
 *
 */
void GuiScreen::backspace() {
    output = output.substr(0, output.length() - 1);
    colors.pop_back();
    paintIsFresh = false;
}

/**
 *
 */
void GuiScreen::cleanLine(int lineLength) {
    output = output.substr(0, output.length() - lineLength);
    colors.clear();
    paintIsFresh = false;
}

/**
 *
 */
void GuiScreen::write(string message, Color_t color, bool visible) {
    for ( char c : message )
        writeChar(c, color);
}

/**
 *
 */
Keyboard::Info GuiScreen::readInput(bool* cancelCondition) {
    if ( waitingInput.size() == 0 ) {
        if ( cancelCondition )
            AtomicBlockDual(&noInputAvailable, (uint8_t*)&cancelCondition);
        else
            AtomicBlock(&noInputAvailable);
    }

    waitingInputLock.lock();

    Keyboard::Info result = waitingInput.front();
    waitingInput.pop_front();

    if ( waitingInput.size() == 0 )
        noInputAvailable = true;

    waitingInputLock.unlock();

    return result;
}

/**
 *
 */
void GuiScreen::workingDirectoryChanged(string str) {
    window->setTitle("CandyShell - " + str);
}
