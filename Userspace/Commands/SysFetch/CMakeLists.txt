add_executable(SysFetch SysFetch.cc)
target_link_libraries(SysFetch LibApi LibC LibUtils LibIO LibTasking)
install(TARGETS SysFetch DESTINATION ${RD_IMAGE_INSTALL_BINS})
