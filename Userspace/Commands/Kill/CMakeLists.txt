add_executable(Kill Kill.c)
target_link_libraries(Kill LibApi LibC)
install(TARGETS Kill DESTINATION ${RD_IMAGE_INSTALL_BINS})
