add_executable(Power Power.c)
target_link_libraries(Power LibApi LibC)
install(TARGETS Power DESTINATION ${RD_IMAGE_INSTALL_BINS})
