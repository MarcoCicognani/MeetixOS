add_executable(Spawner.sv Elf32Loader.cpp power.cpp spawner.cpp)
target_include_directories(Spawner.sv PRIVATE .)
target_link_libraries(Spawner.sv LibApi LibC LibIO LibUtils LibTasking)
install(TARGETS Spawner.sv DESTINATION MeetiX/Kernel/Servers)
