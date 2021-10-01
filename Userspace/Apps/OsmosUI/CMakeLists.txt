add_executable(OsmosUI data.cpp hourManager.cpp memoryManager.cpp dock.cpp notifier.cpp OsmosUI.cpp)
target_include_directories(OsmosUI PRIVATE .)
target_link_libraries(OsmosUI LibApi LibC LibGUI LibTasking LibUtils LibIO)
install(TARGETS OsmosUI DESTINATION ${RD_IMAGE_INSTALL_APPS}/OsmosUI/Bin)
