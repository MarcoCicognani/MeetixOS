add_library(LibGraphics
        STATIC
        vbe.cpp
        text/textlyot.cpp
        text/fontmgr.cpp
        text/font.cpp
        text/fontldr.cpp
        graphics.cpp
        cairoutils.cpp)

target_include_directories(LibGraphics PRIVATE ${TOOLCHAIN_INCLUDE}/freetype2)
