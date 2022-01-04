add_executable(Write Write.cc)
target_link_libraries(Write LibApi LibC LibUtils)
install(TARGETS Write DESTINATION Bins)
