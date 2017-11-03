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

#include <components/button.hpp>
#include <components/window.hpp>

#include <events/FocusEvent.hpp>
#include <events/MouseEvent.hpp>
#include <graphics/text/textalign.hpp>
#include <gui/properties.hpp>
#include <math.h>

/**
 *
 */
Button_t::Button_t() : insets(Insets(0, 0, 0, 0)), ActionComponent_t(this)
{
	enabled = true;
	pathToLoad = false;

	shapeColor = ARGB(255, 230, 230, 230);
	border = RGB(160, 160, 160);

	addChild(&label);
	label.setAlignment(TextAlignment::CENTER);
}

/**
 *
 */
void Button_t::layout()
{
	Dimension labelPreferred = label.getPreferredSize();

	labelPreferred.width = labelPreferred.width + insets.left + insets.right;
	labelPreferred.height = labelPreferred.height + insets.bottom;
	setPreferredSize(labelPreferred);
}

/**
 *
 */
void Button_t::paint()
{
	cr = graphics.getContext();
	clearSurface();
	bounds = getBounds();

	if (enabled)
	{
		if (shapeColor == ARGB(120, 211, 211, 211)) shapeColor = previous;

		if (state.focused) border = RGB(0, 200, 0);
		else border = RGB(180, 180, 180);
	}
	else
	{
		previous = shapeColor;
		shapeColor = ARGB(120, 211, 211, 211);
	}

	// prepare
	double x = 0.5;
	double y = 0.5;
	double width = bounds.width - 1;
	double height = bounds.height - 1;
	double radius = 2.5;
	double degrees = M_PI / 180.0;

	cairo_new_sub_path(cr);
	cairo_arc(cr, x + width - radius, y + radius, radius, -90 * degrees, 0 * degrees);
	cairo_arc(cr, x + width - radius, y + height - radius, radius, 0 * degrees, 90 * degrees);
	cairo_arc(cr, x + radius, y + height - radius, radius, 90 * degrees, 180 * degrees);
	cairo_arc(cr, x + radius, y + radius, radius, 180 * degrees, 270 * degrees);
	cairo_close_path(cr);

	cairo_set_source_rgba(cr, ARGB_TO_CAIRO_PARAMS(shapeColor));
	cairo_fill_preserve(cr);
	cairo_set_source_rgba(cr, ARGB_TO_CAIRO_PARAMS(border));
	cairo_set_line_width(cr, 1);
	cairo_stroke(cr);

	if (pathToLoad)
	{
		pngSurface = graphics.getContext();

		cairo_set_source_surface(pngSurface, png, pngPosition.x, pngPosition.y);
		cairo_paint(pngSurface);
	}
}

/**
 *
 */
void Button_t::setPNG(std::string path, Point position)
{
	pathToLoad = true;
	pngPosition = position;

	png = cairo_image_surface_create_from_png(path.c_str());

	markFor(COMPONENT_REQUIREMENT_PAINT);
}

/**
 *
 */
void Button_t::PngAnimation(std::string path, Point PNGstartAnimation, Point PNGendAnimation, size_t sleep)
{
}

/**
 *
 */
void Button_t::setColor(Color_t color, Color_t tltColor)
{
	shapeColor = color;

	label.setFontColor(tltColor);
	markFor(COMPONENT_REQUIREMENT_PAINT);
}

/**
 *
 */
bool Button_t::handle(Event_t &event)
{
	MouseEvent_t *mouseEvent = dynamic_cast<MouseEvent_t*>(&event);
	if (mouseEvent)
	{
		if (enabled)
		{
			Rectangle bounds = getBounds();

			if (mouseEvent->type == MOUSE_EVENT_ENTER)
			{
				state.hovered = true;
				markFor(COMPONENT_REQUIREMENT_PAINT);
			}

			else if (mouseEvent->type == MOUSE_EVENT_LEAVE)
			{
				state.hovered = false;
				markFor(COMPONENT_REQUIREMENT_PAINT);
			}

			else if (mouseEvent->type == MOUSE_EVENT_PRESS)
			{
				state.pressed = true;
				markFor(COMPONENT_REQUIREMENT_PAINT);
			}

			else if (mouseEvent->type == MOUSE_EVENT_RELEASE || mouseEvent->type == MOUSE_EVENT_DRAG_RELEASE)
			{
				state.pressed = false;
				markFor(COMPONENT_REQUIREMENT_PAINT);

				if (mouseEvent->type == MOUSE_EVENT_RELEASE)
				{
					if (mouseEvent->position.x >= 0 && mouseEvent->position.y >= 0 && mouseEvent->position.x < getBounds().width && mouseEvent->position.y < getBounds().height) fireAction();
				}
			}
		}
		return true;
	}

	FocusEvent_t *focusEvent = dynamic_cast<FocusEvent_t*>(&event);
	if (focusEvent)
	{
		if (enabled)
		{
			if (focusEvent->type == FOCUS_EVENT_GAINED)
			{
				state.focused = true;
				markFor(COMPONENT_REQUIREMENT_PAINT);
				return true;
			}

			else if (focusEvent->type == FOCUS_EVENT_LOST)
			{
				state.focused = false;
				markFor(COMPONENT_REQUIREMENT_PAINT);
				return true;
			}
		}
	}

	return false;
}

/**
 *
 */
void Button_t::handleBoundChange(Rectangle oldBounds)
{
	Rectangle labelBounds = getBounds();
	labelBounds.x = insets.left;
	labelBounds.y = insets.right;

	label.setBounds(labelBounds);
}

/**
 *
 */
void Button_t::setTitle(std::string title)
{
	label.setTitle(title);
}

/**
 *
 */
std::string Button_t::getTitle()
{
	return label.getTitle();
}

/*
 *
 */
void Button_t::setTitleFont(std::string fontName)
{
	label.setTitleFont(fontName);
}

/*
 *
 */
void Button_t::setTitleAlignment(TextAlignment alignment)
{
	label.setTitleAlignment(alignment);
}

/*
 *
 */
void Button_t::setFontSize(int size)
{
	label.setFontSize(size);
}

/*
 *
 */
void Button_t::setFocus(bool focus)
{
	state.focused = focus;
	markFor(COMPONENT_REQUIREMENT_PAINT);
}

/**
 *
 */
bool Button_t::getNumericProperty(int property, uint32_t *out)
{
	if (property == UI_PROPERTY_ENABLED)
	{
		*out = enabled;
		return true;
	}

	return false;
}

/**
 *
 */
bool Button_t::setNumericProperty(int property, uint32_t value)
{
	if (property == UI_PROPERTY_ENABLED)
	{
		enabled = value;
		state.focused = false;

		markFor(COMPONENT_REQUIREMENT_ALL);

		return true;
	}

	return false;
}
