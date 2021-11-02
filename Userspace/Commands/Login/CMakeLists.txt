add_executable(Login Login.cc)
target_link_libraries(Login LibApi LibC LibUtils LibIO LibTasking)
install(TARGETS Login DESTINATION ${RD_IMAGE_INSTALL_BINS})
