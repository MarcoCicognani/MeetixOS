add_executable(CandyNote CandyNote.cpp GFrontEnd.cpp TFrontEnd.cpp)
target_include_directories(CandyNote PRIVATE . ${TOOLCHAIN_INCLUDE}/freetype2)
target_link_libraries(CandyNote GUI Graphics IO Utils LibCairo LibFreeType LibPixman LibPNG LibZ)