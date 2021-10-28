add_executable(Terminal Terminal.cc GUIScreen.cc HeadlessScreen.cc HeadlessGuiScreen.cc)
target_include_directories(Terminal PRIVATE . ${TOOLCHAIN_INCLUDE}/freetype2)
target_link_libraries(Terminal LibApi LibC LibGUI LibTasking LibGraphics LibIO LibUtils LibCairo LibFreeType LibPixman LibPNG LibZ)
install(TARGETS Terminal DESTINATION ${RD_IMAGE_INSTALL_APPS}/Terminal/Bin)
