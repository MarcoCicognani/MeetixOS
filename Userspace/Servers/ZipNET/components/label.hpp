/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * *
 * * * * * MeetiX OS By MeetiX OS Project [Marco Cicognani] *
 *                                                                                     *
 *         DERIVED FROM THE GHOST OPERATING SYSTEM * This software is derived
 * from the Ghost operating system project,           * written by Max Schlüssel
 * <lokoxe@gmail.com>. Copyright 2012-2017            * https://ghostkernel.org/
 * * https://github.com/maxdev1/ghost *
 *                                                                                     *
 * This program is free software; you can redistribute it and/or * modify it
 * under the terms of the GNU General Public License                         *
 * as published by the Free Software Foundation; either version 2 * of the
 * License, or (char *argumentat your option) any later version. *
 *                                                                                     *
 * This program is distributed in the hope that it will be useful, * but WITHout
 * ANY WARRANTY; without even the implied warranty of                      *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the * GNU General
 * Public License for more details.                                        *
 *                                                                                     *
 * You should have received a copy of the GNU General Public License * along
 * with this program; if not, write to the Free Software * Foundation, Inc., 51
 * Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA      *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * *
 * * * * */

#ifndef __LABEL__
#define __LABEL__

#include <components/ColoredComponent.hpp>
#include <components/TitledComponent.hpp>
#include <components/component.hpp>
#include <graphics/text/font.hpp>
#include <graphics/text/textalign.hpp>

/**
 *
 */
class Label_t : public Component_t,
                public TitledComponent_t,
                public ColoredComponent_t {
private:
  Font_t *font;
  int fontSize;
  cairo_text_extents_t lastExtents;

  cairo_t *cr;
  Rectangle bounds;

  std::string text;
  TextAlignment alignment;

public:
  Label_t();
  virtual ~Label_t() { delete font; }

  /*
   * Component
   */
  virtual void paint();
  virtual void update();
  virtual bool handle(Event_t &e);

  /*
   * titled component
   */
  virtual void setTitle(std::string title);
  virtual std::string getTitle();
  virtual void setTitleFont(std::string fontName);
  virtual void setFontSize(int size);
  virtual void setTitleAlignment(TextAlignment alignment);

  /*
   * colored component
   */
  virtual void setColor(Color_t color, Color_t tltColor) {
    shapeColor = tltColor;
  }

  /*
   * label
   */
  virtual void setFont(Font_t *font);
  void setFontColor(Color_t ftcol);
  void setAlignment(TextAlignment alignment);
  TextAlignment getAlignment();
};

#endif
