add_executable(ZipNET.sv
        interface/ApplicationExitCleanupThread.cpp
        interface/CommandMessageReceiverThread.cpp
        interface/TaskManagerThread.cpp
        interface/RegistrationThread.cpp
        interface/ComponentRegistry.cpp
        interface/CommandMessageResponderThread.cpp
        components/panel.cpp
        components/label.cpp
        components/button.cpp
        components/scrollbar.cpp
        components/scrollpane.cpp
        components/background.cpp
        components/cursor.cpp
        components/component.cpp
        components/ActionComponent.cpp
        components/geoshape.cpp
        components/window.cpp
        components/Screen.cpp
        components/checkbox.cpp
        components/BoundsEventComponent.cpp
        components/canvas.cpp
        components/text/TextField.cpp
        components/text/move/DefaultCaretMoveStrategy.cpp
        input/InputReceiver.cpp
        zipNET.cpp
        layout/FlowLayoutManager.cpp
        layout/GridLayoutManager.cpp
        output/ConfigurationBasedVideoOutput.cpp
        output/VbeVideoOutput.cpp
        events/EventProcessor.cpp)
target_include_directories(ZipNET.sv PRIVATE . ${TOOLCHAIN_INCLUDE}/freetype2)
target_link_libraries(ZipNET.sv LibApi LibC LibGraphics LibGUI LibIO LibUtils LibTasking LibCairo LibFreeType LibPixman LibPNG LibZ)
install(TARGETS ZipNET.sv DESTINATION ${RD_IMAGE_INSTALL_SERVERS})