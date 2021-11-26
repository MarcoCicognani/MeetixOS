cmake_minimum_required(VERSION 3.18.4)
project(MeetixOS C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

# Triplet and installation directory for toolchains
set(TOOLCHAIN_TRIPLET i686-pc-meetix)
set(TOOLCHAIN_ROOT ${CMAKE_SOURCE_DIR}/Toolchain/Local)
set(TOOLCHAIN_BIN ${TOOLCHAIN_ROOT}/bin)
set(TOOLCHAIN_INCLUDE ${TOOLCHAIN_ROOT}/include)
set(TOOLCHAIN_LIB ${TOOLCHAIN_ROOT}/lib)

# Define compilers
set(CMAKE_C_COMPILER ${TOOLCHAIN_BIN}/${TOOLCHAIN_TRIPLET}-gcc)
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_BIN}/${TOOLCHAIN_TRIPLET}-g++)
set(CMAKE_ASM_COMPILER ${TOOLCHAIN_BIN}/${TOOLCHAIN_TRIPLET}-gcc)
set(CMAKE_LINKER ${TOOLCHAIN_BIN}/${TOOLCHAIN_TRIPLET}-ld)
set(CMAKE_RANLIB ${TOOLCHAIN_BIN}/${TOOLCHAIN_TRIPLET}-ranlib)
set(CMAKE_STRIP ${TOOLCHAIN_BIN}/${TOOLCHAIN_TRIPLET}-strip)
set(CMAKE_AR ${TOOLCHAIN_BIN}/${TOOLCHAIN_TRIPLET}-ar)
set(CMAKE_OBJCOPY ${TOOLCHAIN_BIN}/${TOOLCHAIN_TRIPLET}-objcopy)
set(CMAKE_CXXFILT ${TOOLCHAIN_BIN}/${TOOLCHAIN_TRIPLET}-c++filt)
set(CMAKE_ASM_NASM_COMPILER nasm)
set(CMAKE_ASM_NASM_COMPILE_OBJECT "${CMAKE_ASM_NASM_COMPILER} -f elf -o <OBJECT> -s <SOURCE>")

set(IMAGE_INSTALL_ROOT ${CMAKE_BINARY_DIR}/Image)
set(ISO_IMAGE_INSTALL_ROOT ${IMAGE_INSTALL_ROOT}/ISO)
set(RD_IMAGE_INSTALL_ROOT ${IMAGE_INSTALL_ROOT}/Root)
set(RD_IMAGE_INSTALL_APPS ${RD_IMAGE_INSTALL_ROOT}/Apps)
set(RD_IMAGE_INSTALL_BINS ${RD_IMAGE_INSTALL_ROOT}/Bins)
set(RD_IMAGE_INSTALL_SERVERS ${RD_IMAGE_INSTALL_ROOT}/MeetiX/Kernel/Servers)
set(RD_IMAGE_INSTALL_MODULES ${RD_IMAGE_INSTALL_ROOT}/MeetiX/Kernel/Modules)

set(CMAKE_CXX_FLAGS "-fdiagnostics-color=always")
set(CMAKE_C_FLAGS "-fdiagnostics-color=always")

set(CMAKE_CXX_FLAGS_RELEASE "-g0")
set(CMAKE_C_FLAGS_RELEASE "-g0")

# Project sub-directories
add_subdirectory(Kernel)
add_subdirectory(Libs)
add_subdirectory(Userspace)

# Custom target for build ISO
add_custom_target(repack
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Meta/Boot ${ISO_IMAGE_INSTALL_ROOT}/boot/grub
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Root ${RD_IMAGE_INSTALL_ROOT}
        COMMAND ${CMAKE_COMMAND} -E env TOOLCHAIN_BIN=${TOOLCHAIN_BIN}
        ${CMAKE_SOURCE_DIR}/Meta/Scripts/Repack.sh --out-dir=${CMAKE_BINARY_DIR}
        --iso-root=${ISO_IMAGE_INSTALL_ROOT}
        --ramdisk-root=${RD_IMAGE_INSTALL_ROOT}
        DEPENDS install
        USES_TERMINAL)

# Run and run-debug with build
add_custom_target(run
        COMMAND ${CMAKE_SOURCE_DIR}/Meta/Scripts/Run.sh --iso-dir=${CMAKE_BINARY_DIR}
        DEPENDS repack
        USES_TERMINAL)
add_custom_target(run_debug
        COMMAND ${CMAKE_SOURCE_DIR}/Meta/Scripts/Run.sh --iso-dir=${CMAKE_BINARY_DIR} --debug
        DEPENDS repack
        USES_TERMINAL)

# Run and run-debug without build
add_custom_target(just_run
        COMMAND ${CMAKE_SOURCE_DIR}/Meta/Scripts/Run.sh --iso-dir=${CMAKE_BINARY_DIR}
        USES_TERMINAL)
add_custom_target(just_run_debug
        COMMAND ${CMAKE_SOURCE_DIR}/Meta/Scripts/Run.sh --iso-dir=${CMAKE_BINARY_DIR} --debug
        USES_TERMINAL)
