add_executable(CpuDetect CpuDetect.c)
target_include_directories(CpuDetect PRIVATE .)
install(TARGETS CpuDetect DESTINATION ${RD_IMAGE_INSTALL_BINS})
