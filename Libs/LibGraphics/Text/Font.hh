/**
 * @brief
 * This file is part of the MeetiX Operating System.
 * Copyright (c) 2017-2022, Marco Cicognani (marco.cicognani@meetixos.org)
 *
 * @developers
 * Marco Cicognani (marco.cicognani@meetixos.org)
 *
 * @license
 * GNU General Public License version 3
 */

#pragma once

#include <cairo/cairo-ft.h>
#include <cairo/cairo.h>
#include <LibGraphics/Text/FreeType.hh>
#include <map>
#include <string>

namespace Graphics::Text {

/* ------------------------------------ C++ Type Definitions ------------------------------------ */

/**
 * @brief High-level wrapper for cairo fonts
 */
class Font {
public:
    /**
     * @brief Font styles
     */
    enum class Style : u8 {
        NORMAL,
        ITALIC,
        BOLD
    };

public:
    /**
     * @brief Constructor/Destructor
     */
    Font(const std::string& name, u8* source, usize len, Style font_style);
    ~Font();

    /**
     * @brief Loads a Font from the given stream
     */
    static Font* from_file(std::ifstream& in, const std::string& name);

    /**
     * @brief Returns whether this font is correctly loaded
     */
    [[nodiscard]] bool is_loaded() const { return m_is_loaded; }

    /**
     * @brief Returns the name of this font
     */
    [[nodiscard]] const std::string& name() const { return m_name; }

    /**
     * @brief Returns the cairo_font_face
     */
    [[nodiscard]] cairo_font_face_t* cairo_font_face() const { return m_cairo_font_face; }

private:
    u8*                m_data{ nullptr };
    std::string        m_name{};
    Style              m_font_style{ Style::NORMAL };
    FT_Face            m_freetype_face{};
    cairo_font_face_t* m_cairo_font_face{ nullptr };
    usize              m_active_size{ 0 };
    bool               m_is_loaded{ false };
};

} /* namespace Graphics::Text */
