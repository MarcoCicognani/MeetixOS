add_executable(RamdiskWriter RamdiskWriter.cpp)
add_custom_command(
    TARGET RamdiskWriter
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:RamdiskWriter> ${TOOLCHAIN_BIN}/RamdiskWriter
)