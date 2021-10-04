add_executable(Copy Copy.cc)
target_link_libraries(Copy LibApi LibC)
install(TARGETS Copy DESTINATION ${RD_IMAGE_INSTALL_BINS})
