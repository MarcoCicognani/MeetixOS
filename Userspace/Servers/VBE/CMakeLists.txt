add_executable(VBE.sv vbe.cpp)
target_include_directories(VBE.sv PRIVATE .)
target_link_libraries(VBE.sv IO Utils Graphics)