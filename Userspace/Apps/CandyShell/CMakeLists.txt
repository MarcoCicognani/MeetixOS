add_executable(CandyShell CandyShell.cpp GuiScreen.cpp HeadlessScreen.cpp history.cpp login.cpp)
target_include_directories(CandyShell PRIVATE . ${TOOLCHAIN_INCLUDE}/freetype2)
target_link_libraries(CandyShell LibApi LibC LibGUI LibTasking LibGraphics LibIO LibUtils LibCairo LibFreeType LibPixman LibPNG LibZ)
install(TARGETS CandyShell DESTINATION ${RD_IMAGE_INSTALL_APPS}/CandyShell/Bin)
