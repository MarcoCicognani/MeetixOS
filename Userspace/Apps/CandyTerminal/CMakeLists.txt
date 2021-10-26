add_executable(CandyTerminal CandyTerminal.cc GUIScreen.cc HeadlessScreen.cc)
target_include_directories(CandyTerminal PRIVATE . ${TOOLCHAIN_INCLUDE}/freetype2)
target_link_libraries(CandyTerminal LibApi LibC LibGUI LibTasking LibGraphics LibIO LibUtils LibCairo LibFreeType LibPixman LibPNG LibZ)
install(TARGETS CandyTerminal DESTINATION ${RD_IMAGE_INSTALL_APPS}/CandyTerminal/Bin)
