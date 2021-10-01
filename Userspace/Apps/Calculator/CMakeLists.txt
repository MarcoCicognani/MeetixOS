add_executable(Calculator Calculator.cpp)
target_include_directories(Calculator PRIVATE .)
target_link_libraries(Calculator LibApi LibC LibGUI LibTasking LibUtils)
install(TARGETS Calculator DESTINATION ${RD_IMAGE_INSTALL_APPS}/Calculator/Bin)
