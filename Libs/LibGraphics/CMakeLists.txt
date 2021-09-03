add_library(Graphics
            STATIC
            vbe.cpp
            text/textlyot.cpp
            text/fontmgr.cpp
            text/font.cpp
            text/fontldr.cpp
            graphics.cpp
            cairoutils.cpp)

target_include_directories(Graphics PRIVATE ${TOOLCHAIN_INCLUDE}/freetype2)
