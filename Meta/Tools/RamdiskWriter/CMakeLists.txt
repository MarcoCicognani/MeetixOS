cmake_minimum_required(VERSION 3.18)
project(RamdiskWriterTool)
set(CMAKE_CXX_STANDARD 20)

set(TOOLCHAIN_BIN ${CMAKE_SOURCE_DIR}/../../../Toolchain/Local/bin)

add_executable(RamdiskWriter RamdiskWriter.cpp)
add_custom_command(
    TARGET RamdiskWriter
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:RamdiskWriter> ${TOOLCHAIN_BIN}/RamdiskWriter
)
