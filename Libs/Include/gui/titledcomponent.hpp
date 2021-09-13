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

#ifndef MEETIX_LIBRARY_UI_TITLED_COMPONENT
#define MEETIX_LIBRARY_UI_TITLED_COMPONENT

#include <cstdint>
#include <eva.h>
#include <gui/uispech.hpp>

/**
 *
 */
class TitledComponent {
private:
    uint32_t id;

protected:
    TitledComponent(uint32_t id) : id(id) {
    }

public:
    /**
     *
     */
    virtual ~TitledComponent() {
    }

    /**
     *
     */
    virtual bool setTitle(std::string title);

    /**
     *
     */
    virtual bool setGhostTitle(std::string text);

    /**
     *
     */
    virtual bool setFont(std::string fontName);

    /**
     *
     */
    virtual bool setFontSize(int size);

    /**
     *
     */
    virtual bool setTitleAlignment(TextAlignment alignment);

    /**
     *
     */
    virtual std::string getTitle();

    /**
     *
     */
    virtual UiComponentID getId();
};

#endif
