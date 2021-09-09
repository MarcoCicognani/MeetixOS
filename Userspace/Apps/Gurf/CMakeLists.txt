add_executable(Gurf Gurf.cpp)
target_include_directories(Gurf PRIVATE .)
target_link_libraries(Gurf LibGUI LibUtils LibTasking)
install(TARGETS Gurf DESTINATION ${RD_IMAGE_INSTALL_APPS}/Gurf/Bin)
