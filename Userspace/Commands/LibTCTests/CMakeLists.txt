add_executable(LibTCTests LibTCTests.cc)
target_link_libraries(LibTCTests LibApi LibC LibTC)
install(TARGETS LibTCTests DESTINATION Bins)
