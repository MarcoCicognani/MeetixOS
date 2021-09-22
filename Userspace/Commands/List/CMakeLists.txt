add_executable(List List.c)
target_link_libraries(List LibApi LibC)
install(TARGETS List DESTINATION ${RD_IMAGE_INSTALL_BINS})
