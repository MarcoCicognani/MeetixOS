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

#ifndef __TEXT_COMPONENT__
#define __TEXT_COMPONENT__

#include <components/component.hpp>
#include <components/text/CaretDirection.hpp>
#include <components/text/move/CaretMoveStrategy.hpp>
#include <LibGraphics/Metrics/Range.hh>
#include <LibGraphics/Text/Font.hh>

/**
 *
 */
class TextComponent_t : public Component_t {
protected:
    CaretMoveStrategy_t* caretMoveStrategy;

public:
    virtual ~TextComponent_t() {}

    /**
     *
     */
    virtual void setCursor(int position) = 0;

    /**
     *
     */
    virtual int getCursor() = 0;

    /**
     *
     */
    virtual void setMarker(int position) = 0;

    /**
     *
     */
    virtual int getMarker() = 0;

    /**
     *
     */
    virtual void setText(std::string text) = 0;

    /**
     *
     */
    virtual void setGhostText(std::string text) = 0;

    /**
     *
     */
    virtual std::string getText() = 0;

    /**
     *
     */
    virtual Graphics::Metrics::Range getSelectedRange() = 0;

    /**
     *
     */
    virtual void setFont(Graphics::Text::Font* font) = 0;
};

#endif
