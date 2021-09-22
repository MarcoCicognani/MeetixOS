add_executable(FileId FileId.c)
target_link_libraries(FileId LibApi LibC)
install(TARGETS FileId DESTINATION ${RD_IMAGE_INSTALL_BINS})
