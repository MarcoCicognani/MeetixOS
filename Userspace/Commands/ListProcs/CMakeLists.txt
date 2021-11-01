add_executable(ListProcs ListProcs.cc MemoryUsage.cc)
target_link_libraries(ListProcs LibApi LibC LibIO LibTasking LibUtils)
install(TARGETS ListProcs DESTINATION ${RD_IMAGE_INSTALL_BINS})
