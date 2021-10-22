add_executable(Echo Echo.cc)
target_link_libraries(Echo LibApi LibC LibUtils)
install(TARGETS Echo DESTINATION ${RD_IMAGE_INSTALL_BINS})
