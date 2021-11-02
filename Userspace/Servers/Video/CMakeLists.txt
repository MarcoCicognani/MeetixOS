add_executable(Video.sv Video.cpp)
target_include_directories(Video.sv PRIVATE .)
target_link_libraries(Video.sv LibApi LibC LibIO LibUtils LibGraphics)
install(TARGETS Video.sv DESTINATION ${RD_IMAGE_INSTALL_SERVERS})
