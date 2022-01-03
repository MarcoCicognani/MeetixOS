add_executable(Copy Copy.cc)
target_link_libraries(Copy LibApi LibC LibUtils)
install(TARGETS Copy DESTINATION Bins)
