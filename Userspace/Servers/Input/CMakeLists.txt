add_executable(Input.sv Input.cpp Ps2IrqTriggered.cpp Ps2Polling.cpp)
target_include_directories(Input.sv PRIVATE .)
target_link_libraries(Input.sv LibApi LibC LibIO LibUtils)
install(TARGETS Input.sv DESTINATION ${RD_IMAGE_INSTALL_SERVERS})
