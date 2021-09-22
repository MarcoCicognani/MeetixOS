add_executable(Ps2.sv Ps2.cpp Ps2IrqTriggered.cpp Ps2Polling.cpp)
target_include_directories(Ps2.sv PRIVATE .)
target_link_libraries(Ps2.sv LibApi LibC LibIO LibUtils)
install(TARGETS Ps2.sv DESTINATION ${RD_IMAGE_INSTALL_SERVERS})
