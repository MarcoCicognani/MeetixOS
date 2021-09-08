add_executable(MXLogin MXLogin.cpp)
target_include_directories(MXLogin PRIVATE .)
target_link_libraries(MXLogin GUI Utils)