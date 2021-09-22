add_executable(Echo Echo.c)
target_link_libraries(Echo LibApi LibC)
install(TARGETS Echo DESTINATION ${RD_IMAGE_INSTALL_BINS})
