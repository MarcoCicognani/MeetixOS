/**
 * @brief
 * This file is part of the MeetiX Operating System.
 * Copyright (c) 2017-2022, Marco Cicognani (marco.cicognani@meetixos.org)
 *
 * @developers
 * Marco Cicognani (marco.cicognani@meetixos.org)
 *
 * @license
 * GNU General Public License version 3
 */

#include "CpuDetect.hh"

#include "AMD.hh"
#include "Intel.hh"

#include <iostream>

int main(int argc, const char** argv) {
    usize ebx, dummy;
    CPU_ID(0, dummy, ebx, dummy, dummy);

    switch ( ebx ) {
        case INTEL_MAGIC_CODE:
            return inspect_intel(argc, argv);
        case AMD_MAGIC_CODE:
            return inspect_amd(argc, argv);
        default:
            std::cerr << "Unknown x86 CPU detected" << std::endl;
            return EXIT_FAILURE;
    }
}

void print_regs(usize eax, usize ebx, usize ecx, usize edx) {
    char content_buf[17]{ '\0' };
    for ( auto i = 0; i < 4; ++i ) {
        content_buf[i]      = static_cast<char>(eax >> (8 * i));
        content_buf[i + 4]  = static_cast<char>(ebx >> (8 * i));
        content_buf[i + 8]  = static_cast<char>(ecx >> (8 * i));
        content_buf[i + 12] = static_cast<char>(edx >> (8 * i));
    }

    std::cout << content_buf;
}
