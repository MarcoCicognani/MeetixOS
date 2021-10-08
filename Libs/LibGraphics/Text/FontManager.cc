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

#include <Graphics/Text/FontManager.hh>
#include <Tasking/LockGuard.hh>
#include <Utils/Utils.hh>

namespace Graphics::Text {

FontManager& FontManager::instance() {
    static FontManager* s_instance = nullptr;
    if ( !s_instance )
        s_instance = new FontManager();
    return *s_instance;
}

bool FontManager::create_font(const std::string& name, u8* source, usize len, Font::Style style) {
    Tasking::LockGuard guard{ m_lock };

    /* check if font already exists */
    if ( m_font_registry.count(name) > 0 ) {
        Utils::log("Tried to create font '" + name + "' that already exists");
        return false;
    }

    /* create the font */
    auto new_font = new Font(name, source, len, style);
    if ( !new_font->is_loaded() )
        delete new_font;

    /* register the font into the library */
    m_font_registry[name] = new_font;
    Utils::log("Created font '" + name + "'");
    return true;
}

Font* FontManager::font_by_name(const std::string& name) {
    Tasking::LockGuard guard{ m_lock };

    auto it = m_font_registry.find(name);
    if ( it != m_font_registry.end() )
        return it->second;
    else
        return nullptr;
}

void FontManager::destroy_font(Font* font) {
    Tasking::LockGuard guard{ m_lock };

    m_font_registry.erase(font->name());
    delete font;
}

FontManager::FontManager() {
    init_freetype();
}

FontManager::~FontManager() {
    fini_freetype();
}

void FontManager::init_freetype() {
    auto ft_status = FT_Init_FreeType(&m_freetype_library);
    if ( ft_status )
        Utils::log("freetype2 failed at FT_Init_FreeType with error code %i", ft_status);
}

void FontManager::fini_freetype() {
    auto ft_status = FT_Done_Library(m_freetype_library);
    if ( ft_status )
        Utils::log("freetype2 failed at FT_Done_Library with error code %i", ft_status);
}

} /* namespace Graphics::Text */