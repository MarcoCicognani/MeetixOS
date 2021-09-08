add_executable(Gurf Gurf.cpp)
target_include_directories(Gurf PRIVATE .)
target_link_libraries(Gurf GUI Utils Tasking)