add_executable(Echo Echo.cc)
target_link_libraries(Echo LibApi LibC LibUtils)
install(TARGETS Echo DESTINATION Bins)
