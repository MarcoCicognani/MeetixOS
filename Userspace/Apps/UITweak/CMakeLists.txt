add_executable(UITweak UITweak.cpp)
target_link_libraries(UITweak LibApi LibC LibGUI LibGraphics LibTasking LibUtils)
install(TARGETS UITweak DESTINATION ${RD_IMAGE_INSTALL_APPS}/UITweak/Bin)
