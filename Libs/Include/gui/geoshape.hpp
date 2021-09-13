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

#ifndef MEETIX_LIBRARY_UI_GEOSHAPE
#define MEETIX_LIBRARY_UI_GEOSHAPE

#include <cstdint>
#include <eva.h>
#include <gui/actioncomponent.hpp>
#include <gui/coloredcomponent.hpp>
#include <gui/component.hpp>
#include <gui/pngcomponent.hpp>
#include <gui/properties.hpp>
#include <gui/titledcomponent.hpp>
#include <gui/ui.hpp>
#include <gui/uispech.hpp>

class Geoshape : public Component,
                 public TitledComponent,
                 public PngComponent,
                 public ColoredComponent {
protected:
    /*
     *
     */
    Geoshape(uint32_t id)
        : Component(id, UI_COMPONENT_TYPE_GEOSHAPE), TitledComponent(id), PngComponent(id),
          ColoredComponent(id) {
    }

public:
    virtual ~Geoshape() {
    }

    /**
     *
     */
    static Geoshape* create();

    /**
     *
     */
    void setResizable(bool resizable);
};

#endif
