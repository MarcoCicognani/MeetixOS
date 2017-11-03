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
* GNU General Public License for more details.									 *
*																				 *
* You should have received a copy of the GNU General Public License				 *
* along with this program; if not, write to the Free Software                    *
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA *
**********************************************************************************/

#ifndef MEETIX_LIBRARY_GRAPHICS_TEXT_TEXTLAYOUTER
#define MEETIX_LIBRARY_GRAPHICS_TEXT_TEXTLAYOUTER

#include <graphics/metrics/point.hpp>
#include <graphics/metrics/dimension.hpp>
#include <graphics/metrics/rectangle.hpp>
#include <graphics/text/font.hpp>
#include <graphics/text/textalign.hpp>
#include <vector>

/**
 * descriptor for cairo rendered glyphs
 */
struct PositionedGlyph
{
public:
	/**
	 * empty constructor
	 */
	PositionedGlyph() : line(-1), glyph(nullptr), glyphCount(0) {}

	/**
	 * filled constructor
	 *
	 * @param line:		line value to assign
	 * @param pos:		position to assign
	 * @param size:		size to assign
	 * @param adv:		advanced point to assign
	 * @param glyph:	glyph buffer to assign
	 * @param count:	glyph count
	 */
	PositionedGlyph(int32_t line, const Point &pos, const Dimension &size, const Point &adv, cairo_glyph_t *glyph, uint32_t count) :
		line(line), position(pos), size(size), advance(adv), glyph(glyph), glyphCount(count) {}

	/**
	 * position infos
	 */
	int32_t line;  	 // current line
	Point position;	 // position on the screen

	/**
	 * size infos
	 */
	Dimension size;  // size of the glyph
	Point advance;   // adavanced position

	/**
	 * cairo infos
	 */
	cairo_glyph_t *glyph;  // cairo glyph object collection
	uint32_t glyphCount;   // object count
};

/**
 * descriptor for layouted cairo text
 */
struct LayoutedText
{
public:
	/**
	 * empty constructor
	 */
	LayoutedText() : glyphBuffer(nullptr), glyphCount(0), clusterBuffer(nullptr), clusterCount(0) {}

	/**
	 * filled constructor
	 *
	 * @param bounds:		the text bounds
	 * @param glbuf:		the glyphs buffer pointer
	 * @param glcount:		the count of instance into glbuf
	 * @param clbuf:		buffer cluster pointer
	 * @param clcount:		the cluster count
	 */
	LayoutedText(const Rectangle &bounds, cairo_glyph_t *glbuf, uint32_t glcount, cairo_text_cluster_t *clbuf, uint32_t clcount) :
		textBounds(bounds), glyphBuffer(glbuf), glyphCount(glcount), clusterBuffer(clbuf), clusterCount(clcount) {}

	/**
	 * glyphs descriptor
	 */
	std::vector<PositionedGlyph> positions; // List of glyphs with their positions

	/**
	 * position and size infos
	 */
	Rectangle textBounds;	// Bounds of the entire layouted text

	/**
	 * cairo buffers
	 */
	cairo_glyph_t *glyphBuffer;				// buffer of glyphs
	cairo_text_cluster_t *clusterBuffer;    // cluster of text
	int32_t glyphCount;						// glyphs count
	int32_t clusterCount;					// cluster count
};

/**
 * class to manage cairo text layout
 */
class TextLayouter
{
private:
	/**
	 * constructor have to be private, this class is a singletone
	 */
	TextLayouter() {}

	/**
	 * align the text to the right
	 *
	 * @param text:			the layouted text to align
	 * @param line:			the line of the alignment
	 * @param lineWidth:	the line width
	 * @param bounds:		bounds of text
	 */
	void rightAlign(LayoutedText *text, int32_t line, int32_t lineWidth, Rectangle &bounds);

	/**
	 * align the text to the center
	 *
	 * @param text:			the layouted text to align
	 * @param line:			the line of the alignment
	 * @param lineWidth:	the line width
	 * @param bounds:		bounds of text
	 */
	void centerAlign(LayoutedText *text, int32_t line, int32_t lineWidth, Rectangle &bounds);

public:
	/**
	 * @return the instance of the font manager singleton
	 */
	static TextLayouter *getInstance();

	/**
	 * @return a new layouted text instance
	 */
	LayoutedText *initializeBuffer();

	/**
	 * layout a text with provided parameters
	 *
	 * @param cr:					the cairo instance
	 * @param text:					the text to layout
	 * @param font:					the font to use to layout to
	 * @param size:					the font size size of each char to layout
	 * @param bounds:				the bounds of layouting
	 * @param alignment:			the text alignment on cairo's buffer
	 * @param layout:				the returned layouted text instance
	 * @param-opt breakOnOverflow:	break the layouting if size overflow the provided bounds
	 */
	void layout(cairo_t *cr, const char *text, Font_t *font, int32_t size, Rectangle bounds, const TextAlignment &alignment, LayoutedText *layout, bool breakOnOverflow = true);

	/**
	 * free the memory of a layouted text
	 *
	 * @param layout:	the layout to destroy
	 */
	void destroyLayout(LayoutedText *layout);

};

#endif
