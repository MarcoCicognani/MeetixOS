add_executable(Exec Exec.cpp)
target_link_libraries(Exec LibApi LibC LibGUI LibGraphics LibTasking LibUtils)
install(TARGETS Exec DESTINATION ${RD_IMAGE_INSTALL_APPS}/Exec/Bin)
