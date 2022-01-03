add_executable(List List.cc Directory.cc)
target_link_libraries(List LibApi LibC LibUtils)
install(TARGETS List DESTINATION Bins)
