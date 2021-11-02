add_executable(ListThreads ListThreads.cc)
target_link_libraries(ListThreads LibApi LibC LibTasking LibUtils)
install(TARGETS ListThreads DESTINATION ${RD_IMAGE_INSTALL_BINS})
