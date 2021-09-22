add_executable(GuiNotify GuiNotify.cpp)
target_link_libraries(GuiNotify LibApi LibC LibGUI LibUtils)
install(TARGETS GuiNotify DESTINATION ${RD_IMAGE_INSTALL_BINS})
