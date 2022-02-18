cmake_minimum_required(VERSION 3.16.3)
project(RamdiskWriterTool)
set(CMAKE_CXX_STANDARD 20)

set(TOOLCHAIN_BIN ${CMAKE_SOURCE_DIR}/../../../Toolchain/Local/bin)

add_executable(RamdiskWriter RamdiskWriter.cc)
install(TARGETS RamdiskWriter DESTINATION ${TOOLCHAIN_BIN})
