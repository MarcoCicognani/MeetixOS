add_executable(Terminal Terminal.cc Screen/GUIScreen.cc Screen/HeadlessScreen.cc Screen/HeadlessGUIScreen.cc)
target_include_directories(Terminal PRIVATE . ${TOOLCHAIN_INCLUDE}/freetype2)
target_link_libraries(Terminal LibApi LibC LibGUI LibTasking LibGraphics LibIO LibUtils LibCairo LibFreeType LibPixman LibPNG LibZ)
install(TARGETS Terminal DESTINATION Apps/Terminal/Bin)
