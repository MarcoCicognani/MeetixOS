add_library(LibGraphics
        STATIC
        Vbe.cc
        Text/Layouter.cc
        Text/FontManager.cc
        Text/Font.cc
        Text/FontLoader.cc
        Context.cc
        Cairo.cc Color.cc)

target_include_directories(LibGraphics PRIVATE ${TOOLCHAIN_INCLUDE}/freetype2)
