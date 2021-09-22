add_executable(Read Read.c)
target_link_libraries(Read LibApi LibC)
install(TARGETS Read DESTINATION ${RD_IMAGE_INSTALL_BINS})
