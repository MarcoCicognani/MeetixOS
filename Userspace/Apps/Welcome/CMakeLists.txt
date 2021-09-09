add_executable(Welcome Welcome.cpp)
target_link_libraries(Welcome LibGUI LibUtils)
install(TARGETS Welcome DESTINATION ${RD_IMAGE_INSTALL_APPS}/Welcome/Bin)
