add_executable(Idle Idle.cc)
target_link_libraries(Idle LibApi LibC)
install(TARGETS Idle DESTINATION ${RD_IMAGE_INSTALL_BINS})
