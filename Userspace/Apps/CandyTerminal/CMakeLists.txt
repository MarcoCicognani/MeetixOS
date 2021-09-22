add_executable(CandyTerminal CandyTerminal.cpp GuiScreen.cpp HeadlessScreen.cpp)
target_include_directories(CandyTerminal PRIVATE . ${TOOLCHAIN_INCLUDE}/freetype2)
target_link_libraries(CandyTerminal LibApi LibC LibGUI LibGraphics LibIO LibUtils LibCairo LibFreeType LibPixman LibPNG LibZ)
install(TARGETS CandyTerminal DESTINATION ${RD_IMAGE_INSTALL_APPS}/CandyTerminal/Bin)
