add_executable(ListThreads ListThreads.cpp)
target_link_libraries(ListThreads LibTasking)
install(TARGETS ListThreads DESTINATION ${RD_IMAGE_INSTALL_BINS})
