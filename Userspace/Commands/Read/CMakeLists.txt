add_executable(Read Read.cc)
target_link_libraries(Read LibApi LibC LibUtils)
install(TARGETS Read DESTINATION ${RD_IMAGE_INSTALL_BINS})
