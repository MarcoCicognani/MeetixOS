add_executable(Gurf Gurf.cpp)
target_include_directories(Gurf PRIVATE .)
target_link_libraries(Gurf LibApi LibC LibGUI LibGraphics LibTasking LibUtils)
install(TARGETS Gurf DESTINATION Apps/Gurf/Bin)
