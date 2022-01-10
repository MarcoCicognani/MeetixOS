add_executable(LibTCTests LibTCTests.cc)
target_link_libraries(LibTCTests LibApi LibC)
install(TARGETS LibTCTests DESTINATION Bins)
