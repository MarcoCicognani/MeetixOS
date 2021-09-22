add_executable(GZip GZip.c)
target_link_libraries(GZip LibApi LibC LibZ)
install(TARGETS GZip DESTINATION ${RD_IMAGE_INSTALL_BINS})
