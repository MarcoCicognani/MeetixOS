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

#include <fstream>
#include <LibGraphics/Text/FontLoader.hh>

namespace Graphics::Text::FontLoader {

static Font* font_at_path(const std::string& path, const std::string& name) {
    auto stream = std::ifstream{ path };
    if ( stream.is_open() )
        return Font::from_file(stream, name);
    else
        return nullptr;
}

static Font* system_font(const std::string& name) {
    return font_at_path("/MeetiX/Configs/WM/Fonts/" + name + ".ttf", name);
}

Font* get(const std::string& name) {
    auto font = system_font(name);
    if ( !font )
        font = get_default();
    return font;
}

Font* get_default() {
    return system_font("default");
}

} /* namespace Graphics::Text::FontLoader */