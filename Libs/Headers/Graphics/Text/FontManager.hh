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

#pragma once

#include <Graphics/Text/Font.hh>
#include <Graphics/Text/FreeType.hh>
#include <LibTasking/Lock.hh>
#include <map>
#include <string>

namespace Graphics::Text {

/* ------------------------------------ C++ Type Definitions ------------------------------------ */

/**
 * @brief Singleton manager which holds all the registered fonts
 */
class FontManager {
public:
    /**
     * @brief Returns the instance of the singleton
     */
    static FontManager& instance();

    /**
     * @brief Creates and registers a new font with the given parameters
     */
    bool create_font(const std::string& name, u8* source, usize len, Font::Style style = Font::Style::NORMAL);

    /**
     * @brief Returns the registered font which corresponds to the given name
     */
    Font* font_by_name(const std::string& name);

    /**
     * @brief Deletes the given font and removes it from the registry
     */
    void destroy_font(Font* font);

    /**
     * @brief Returns the freetype2 library handle
     */
    [[nodiscard]] FT_Library freetype_library() const { return m_freetype_library; }

private:
    FontManager();
    ~FontManager();

    void init_freetype();
    void fini_freetype();

private:
    Tasking::Lock                m_lock{};
    FT_Library                   m_freetype_library{};
    std::map<std::string, Font*> m_font_registry{};
};

} /* namespace Graphics::Text */
