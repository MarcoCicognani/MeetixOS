add_executable(MXLogin MXLogin.cpp)
target_include_directories(MXLogin PRIVATE .)
target_link_libraries(MXLogin LibApi LibC LibGUI LibUtils)
install(TARGETS MXLogin DESTINATION ${RD_IMAGE_INSTALL_APPS}/MXLogin/Bin)
