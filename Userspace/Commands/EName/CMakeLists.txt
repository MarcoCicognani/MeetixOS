add_executable(EName EName.c)
target_link_libraries(EName LibApi LibC)
install(TARGETS EName DESTINATION ${RD_IMAGE_INSTALL_BINS})
