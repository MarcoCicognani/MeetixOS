add_executable(candyShell CandyShell.cpp GuiScreen.cpp HeadlessScreen.cpp history.cpp login.cpp)
target_include_directories(candyShell PRIVATE . ${TOOLCHAIN_INCLUDE}/freetype2)
target_link_libraries(candyShell LibGUI LibGraphics LibIO LibUtils LibCairo LibFreeType LibPixman LibPNG LibZ)
