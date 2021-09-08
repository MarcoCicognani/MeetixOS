add_library(LibZ STATIC IMPORTED)
set_target_properties(LibZ PROPERTIES IMPORTED_LOCATION ${TOOLCHAIN_LIB}/libz.a)

add_library(LibPNG STATIC IMPORTED)
set_target_properties(LibPNG PROPERTIES IMPORTED_LOCATION ${TOOLCHAIN_LIB}/libpng.a)

add_library(LibPixman STATIC IMPORTED)
set_target_properties(LibPixman PROPERTIES IMPORTED_LOCATION ${TOOLCHAIN_LIB}/libpixman-1.a)

add_library(LibFreeType STATIC IMPORTED)
set_target_properties(LibFreeType PROPERTIES IMPORTED_LOCATION ${TOOLCHAIN_LIB}/libfreetype.a)

add_library(LibCairo STATIC IMPORTED)
set_target_properties(LibCairo PROPERTIES IMPORTED_LOCATION ${TOOLCHAIN_LIB}/libcairo.a)

add_subdirectory(Apps)
#add_subdirectory(Commands)
add_subdirectory(Servers)
