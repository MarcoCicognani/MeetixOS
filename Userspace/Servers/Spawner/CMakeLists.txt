add_executable(Spawner.sv
               Elf32Loader.cpp
               power.cpp
               spawner.cpp)
target_include_directories(Spawner.sv PRIVATE .)
target_link_libraries(Spawner.sv IO Utils Tasking)