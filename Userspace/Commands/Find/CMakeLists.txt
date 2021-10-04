add_executable(Find Find.cc)
target_link_libraries(Find LibApi LibC)
install(TARGETS Find DESTINATION ${RD_IMAGE_INSTALL_BINS})
