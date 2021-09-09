add_executable(Calculator Calculator.cpp)
target_include_directories(Calculator PRIVATE .)
target_link_libraries(Calculator LibGUI LibUtils)
install(TARGETS Calculator DESTINATION ${RD_IMAGE_INSTALL_APPS}/Calculator/Bin)
