add_executable(VBE.sv vbe.cpp)
target_include_directories(VBE.sv PRIVATE .)
target_link_libraries(VBE.sv LibIO LibUtils LibGraphics)
install(TARGETS VBE.sv DESTINATION ${RD_IMAGE_INSTALL_SERVERS})
