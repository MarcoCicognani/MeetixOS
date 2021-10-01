add_executable(CandyNote CandyNote.cpp GFrontEnd.cpp TFrontEnd.cpp)
target_include_directories(CandyNote PRIVATE . ${TOOLCHAIN_INCLUDE}/freetype2)
target_link_libraries(CandyNote LibApi LibC LibGUI LibTasking LibGraphics LibIO LibUtils LibCairo LibFreeType LibPixman LibPNG LibZ)
install(TARGETS CandyNote DESTINATION ${RD_IMAGE_INSTALL_APPS}/CandyNote/Bin)
