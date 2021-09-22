add_executable(ListProcs ListProcs.cpp)
target_link_libraries(ListProcs LibApi LibC LibTasking)
install(TARGETS ListProcs DESTINATION ${RD_IMAGE_INSTALL_BINS})
