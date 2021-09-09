add_executable(Stress Stress.c thread.c mem.c fs.c)
target_include_directories(Stress PRIVATE .)
install(TARGETS Stress DESTINATION ${RD_IMAGE_INSTALL_BINS})
