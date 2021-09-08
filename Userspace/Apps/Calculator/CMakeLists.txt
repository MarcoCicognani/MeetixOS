add_executable(Calculator Calculator.cpp)
target_include_directories(Calculator PRIVATE .)
target_link_libraries(Calculator GUI Utils)