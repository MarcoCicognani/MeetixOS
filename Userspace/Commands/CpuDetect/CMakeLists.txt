add_executable(CpuDetect CpuDetect.cc AMD.cc Intel.cc)
target_include_directories(CpuDetect PRIVATE .)
target_link_libraries(CpuDetect LibApi LibC)
install(TARGETS CpuDetect DESTINATION ${RD_IMAGE_INSTALL_BINS})
