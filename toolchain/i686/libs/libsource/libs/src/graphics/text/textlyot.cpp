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

#include <graphics/text/textlyot.hpp>
#include <utils/utils.hpp>

/**
 * global static instance of the singletone object
 */
static TextLayouter *instance = nullptr;

/**
 * @return the instance of the font manager singleton
 */
TextLayouter *TextLayouter::getInstance()
{
	if (!instance) instance = new TextLayouter();
	return instance;
}

/**
 * align the text to the right
 *
 * @param text:			the layouted text to align
 * @param line:			the line of the alignment
 * @param lineWidth:	the line width
 * @param bounds:		bounds of text
 */
void TextLayouter::rightAlign(LayoutedText *text, int line, int lineWidth, Rectangle &bounds)
{
	int difference = bounds.width - bounds.x - lineWidth;
	for (PositionedGlyph &glyph : text->positions)
		if (glyph.line == line) glyph.position.x += difference;
}

/**
 * align the text to the center
 *
 * @param text:			the layouted text to align
 * @param line:			the line of the alignment
 * @param lineWidth:	the line width
 * @param bounds:		bounds of text
 */
void TextLayouter::centerAlign(LayoutedText *text, int line, int lineWidth, Rectangle &bounds)
{
	int difference = (bounds.width - bounds.x) / 2 - lineWidth / 2;
	for (PositionedGlyph &glyph : text->positions)
		if (glyph.line == line) glyph.position.x += difference;
}

/**
 * @return a new layouted text instance
 */
LayoutedText *TextLayouter::initializeBuffer()
{
	return new LayoutedText();
}

/**
 * layout a text with provided parameters
 *
 * @param cr:			the cairo instance
 * @param text:			the text to layout
 * @param font:			the font to use to layout to
 * @param size:			the font size size of each char to layout
 * @param bounds:		the bounds of layouting
 * @param alignment:	the text alignment on cairo's buffer
 * @param layout:		the returned layouted text instance
 * @param-opt breakOnOverflow:	break the layouting if size overflow the provided bounds
 */
void TextLayouter::layout(cairo_t *cr, const char *text, Font_t *font, int32_t size, Rectangle bounds, const TextAlignment &alignment, LayoutedText *layout, bool breakOnOverflow)
{
	// check font validity
	if (!font) return;

	// get text length
	size_t textLen = strlen(text);

	// starting coordinates
	int x = bounds.x;
	int y = bounds.y;
	int lineStartX = x;

	// created the scaled font face
	cairo_set_font_face(cr, font->getFace());
	cairo_set_font_size(cr, size);
	cairo_scaled_font_t *scaledFace = cairo_get_scaled_font(cr);

	int line = 0;
	int lineHeight = size;

	// create glyphs for the text
	cairo_glyph_t *previousGlyphBuffer = layout->glyphBuffer;
	cairo_text_cluster_t *previousClusterBuffer = layout->clusterBuffer;

	cairo_text_cluster_flags_t clusterFlags;
	cairo_status_t stat = cairo_scaled_font_text_to_glyphs(scaledFace, 0, 0, text, textLen, &layout->glyphBuffer, &layout->glyphCount, &layout->clusterBuffer, &layout->clusterCount, &clusterFlags);

	// free old buffer
	if (previousGlyphBuffer != nullptr && layout->glyphBuffer != previousGlyphBuffer) free(previousGlyphBuffer);
	if (previousClusterBuffer != nullptr && layout->clusterBuffer != previousClusterBuffer) free(previousClusterBuffer);

	// clear layout entries
	layout->positions.clear();

	// perform layouting
	if (stat == CAIRO_STATUS_SUCCESS)
	{
		// positions in bytes and glyphs
		size_t bytePos = 0;
		size_t glyphPos = 0;

		// text extents
		cairo_text_extents_t extents;
		for (int i = 0; i < layout->clusterCount; i++)
		{
			cairo_text_cluster_t *cluster = &layout->clusterBuffer[i];
			cairo_glyph_t *glyphs = &layout->glyphBuffer[glyphPos];

			// create new position
			PositionedGlyph positioned;
			positioned.glyph = glyphs;
			positioned.glyphCount = cluster->num_glyphs;
			cairo_scaled_font_glyph_extents(scaledFace, positioned.glyph, positioned.glyphCount, &extents);

			positioned.advance.x = extents.x_advance;
			positioned.advance.y = extents.y_advance;
			positioned.size.width = extents.width;
			positioned.size.height = extents.height;

			// check if newline
			bool isNewline = false;
			if (cluster->num_bytes == 1 && text[bytePos] == '\n') isNewline = true;
			bool invisible = false;

			// Wouldn't match in line or is break character? Start next line
			if (isNewline || (breakOnOverflow && (x + positioned.size.width > bounds.width)))
			{
				if (isNewline) invisible = true;
				if (alignment == TextAlignment::RIGHT) rightAlign(layout, line, x - lineStartX, bounds);
				else if (alignment == TextAlignment::CENTER) centerAlign(layout, line, x - lineStartX, bounds);

				++line;
				x = bounds.x;
				lineStartX = x;
				y += lineHeight;
			}

			if (!invisible)
			{
				// Position
				positioned.line = line;
				positioned.position.x = x;
				positioned.position.y = y + lineHeight;

				// Add position
				layout->positions.push_back(positioned);

				// Jump to next
				x += positioned.advance.x;
			}

			// increase positions
			glyphPos += cluster->num_glyphs;
			bytePos += cluster->num_bytes;
		}
	}

	if (alignment == TextAlignment::RIGHT) rightAlign(layout, line, x - lineStartX, bounds);
	else if (alignment == TextAlignment::CENTER) centerAlign(layout, line, x - lineStartX, bounds);

	// Set text bounds
	#define BOUNDS_EMPTY 0xFFFFFF
	int tbTop = BOUNDS_EMPTY;
	int tbLeft = BOUNDS_EMPTY;
	int tbRight = 0;
	int tbBottom = 0;

	for (PositionedGlyph &p : layout->positions)
	{
		if (p.position.x < tbLeft) tbLeft = p.position.x;
		if (p.position.y < tbTop) tbTop = p.position.y;

		// get extents again
		int r = p.position.x + p.size.width;
		if (r > tbRight) tbRight = r;

		int b = p.position.y + p.size.height;
		if (b > tbBottom) tbBottom = b;
	}

	if (tbTop != BOUNDS_EMPTY && tbLeft != BOUNDS_EMPTY)
	{
		layout->textBounds.x = tbLeft;
		layout->textBounds.y = tbTop;
		layout->textBounds.width = tbRight - tbLeft;
		layout->textBounds.height = tbBottom - tbTop;
	}
}

/**
 * free the memory of a layouted text
 *
 * @param layout:	the layout to destroy
 */
void TextLayouter::destroyLayout(LayoutedText *layout)
{
	if (layout->glyphBuffer) free(layout->glyphBuffer);
	if (layout->clusterBuffer) free(layout->clusterBuffer);
	delete layout;
}
