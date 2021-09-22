add_executable(Stress Stress.c thread.c mem.c FileSystem.c)
target_include_directories(Stress PRIVATE .)
target_link_libraries(Stress LibApi LibC)
install(TARGETS Stress DESTINATION ${RD_IMAGE_INSTALL_BINS})
