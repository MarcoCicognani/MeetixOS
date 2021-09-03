add_executable(Time.sv time.cpp)
target_include_directories(Time.sv PRIVATE .)
target_link_libraries(Time.sv IO Utils)