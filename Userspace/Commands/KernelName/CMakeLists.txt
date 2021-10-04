add_executable(KernelName KernelName.cc)
target_link_libraries(KernelName LibApi LibC LibUtils)
install(TARGETS KernelName DESTINATION ${RD_IMAGE_INSTALL_BINS})
