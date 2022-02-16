/**
 * @brief
 * This file is part of the MeetiX Operating System.
 * Copyright (c) 2017-2021, Marco Cicognani (marco.cicognani@meetixos.org)
 *
 * @developers
 * Marco Cicognani (marco.cicognani@meetixos.org)
 *
 * @license
 * GNU General Public License version 3
 */

#include <cstring>
#include <fstream>
#include <LibGraphics/Text/Font.hh>
#include <LibGraphics/Text/FontManager.hh>
#include <sstream>
#include <LibUtils/File.hh>
#include <LibUtils/Utils.hh>

namespace Graphics::Text {

Font::Font(const std::string& name, u8* source, usize len, Style font_style)
    : m_data{ source }
    , m_name{ name }
    , m_font_style{ font_style } {
    /* load the freetype2 font-face */
    FT_Error ft_status;
    ft_status = FT_New_Memory_Face(FontManager::instance().freetype_library(),
                                   m_data,
                                   static_cast<FT_Long>(len),
                                   0,
                                   &m_freetype_face);
    if ( ft_status ) {
        Utils::log("freetype2 failed at FT_New_Memory_Face with error code %i", ft_status);
        delete m_data;
        return;
    }

    /* create the cairo-font-face */
    m_cairo_font_face = cairo_ft_font_face_create_for_ft_face(m_freetype_face, 0);
    m_is_loaded       = true;
}

Font::~Font() {
    if ( m_is_loaded ) {
        /* cleanup cairo/freetype data */
        cairo_font_face_destroy(m_cairo_font_face);
        FT_Done_Face(m_freetype_face);

        delete m_data;
    }
}

Font* Font::from_file(std::ifstream& in, const std::string& name) {
    /* check for existing font */
    auto font = FontManager::instance().font_by_name(name);
    if ( font )
        return font;

    /* read all the file-content and store it in ss */
    std::stringstream ss;
    ss << in.rdbuf();

    /* copy to the raw-content buffer */
    auto str_content = ss.str();
    auto raw_content = new u8[str_content.length()];
    std::memcpy(raw_content, str_content.c_str(), str_content.length());

    /* create the new font-now */
    if ( !FontManager::instance().create_font(name, raw_content, str_content.length()) ) {
        delete[] raw_content;
        return nullptr;
    }

    /* return the new font instance */
    return FontManager::instance().font_by_name(name);
}

} /* namespace Graphics::Text */
