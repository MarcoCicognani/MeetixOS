add_executable(GuiNotify GuiNotify.cc)
target_link_libraries(GuiNotify LibApi LibC LibGUI LibGraphics LibTasking LibUtils)
install(TARGETS GuiNotify DESTINATION Bins)
