add_executable(Calculator Calculator.cpp)
target_include_directories(Calculator PRIVATE .)
target_link_libraries(Calculator LibApi LibC LibGUI LibGraphics LibTasking LibUtils)
install(TARGETS Calculator DESTINATION Apps/Calculator/Bin)
