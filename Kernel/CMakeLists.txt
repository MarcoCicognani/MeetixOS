#
# @brief
# This file is part of the MeetiX Operating System.
# Copyright (c) 2017-2021, Marco Cicognani (marco.cicognani@meetixos.org)
#
# @developers
# Marco Cicognani (marco.cicognani@meetixos.org)
#
# @license
# GNU General Public License version 3
#

add_compile_definitions(_ARCH_X86_=1)
add_compile_definitions(IN_KERNEL=1)
add_compile_options(-Wall)
add_compile_options(-Wno-address-of-packed-member)
add_compile_options(-Wunused-variable)
add_compile_options(-Wno-unused-but-set-variable)
add_compile_options(-ffreestanding)

add_subdirectory(SMPStart)
add_subdirectory(Shared)
add_subdirectory(Loader)
add_subdirectory(Kernel)
