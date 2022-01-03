add_executable(GZip GZip.cc)
target_link_libraries(GZip LibApi LibC LibUtils LibZ)
install(TARGETS GZip DESTINATION Bins)
