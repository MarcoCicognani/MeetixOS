add_executable(CpuDetect CpuDetect.c)
target_include_directories(CpuDetect PRIVATE .)
target_link_libraries(CpuDetect LibApi LibC)
install(TARGETS CpuDetect DESTINATION ${RD_IMAGE_INSTALL_BINS})
