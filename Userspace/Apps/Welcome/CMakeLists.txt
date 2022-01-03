add_executable(Welcome Welcome.cpp)
target_link_libraries(Welcome LibApi LibC LibGUI LibGraphics LibTasking LibUtils)
install(TARGETS Welcome DESTINATION Apps/Welcome/Bin)
