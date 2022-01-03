add_executable(ListPCI ListPCI.cc List.cc)
target_include_directories(ListPCI PRIVATE .)
target_link_libraries(ListPCI LibApi LibC LibUtils)
install(TARGETS ListPCI DESTINATION Bins)
