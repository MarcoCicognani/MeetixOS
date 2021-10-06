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

#ifndef MEETIX_LIBRARY_UI_CANVAS
#define MEETIX_LIBRARY_UI_CANVAS

#include <cstdint>
#include <Graphics/Context.hh>
#include <gui/canvasbufferlistener.hpp>
#include <gui/component.hpp>

/**
 *
 */
struct CanvasBufferInfo {
    Graphics::Color::ArgbGradient* buffer;
    uint16_t width;
    uint16_t height;
};

/**
 *
 */
class Canvas : public Component {
protected:
    Graphics::Context* graphics;
    Address            currentBuffer;
    Address            nextBuffer;

    /**
     * Listener only for user purpose, so a client gets an event once the
     * buffer was changed.
     */
    CanvasBufferListener* userListener;

    /*
     *
     */
    Canvas(uint32_t id)
        : graphics(0), Component(id, UI_COMPONENT_TYPE_CANVAS), currentBuffer(0), nextBuffer(0),
          userListener(0) {
    }

public:
    virtual ~Canvas();

    /*
     *
     */
    static Canvas* create();

    /*
     *
     */
    void acknowledgeNewBuffer(Address address);

    /*
     *
     */
    void blit(Graphics::Metrics::Rectangle rect);

    /*
     *
     */
    CanvasBufferInfo getBuffer();

    /*
     *
     */
    void setBufferListener(CanvasBufferListener* l) {
        userListener = l;
    }
};

#endif
