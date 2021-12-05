/**
 * @brief
 * This file is part of the MeetiX Operating System.
 * Copyright (c) 2017-2021, Marco Cicognani (marco.cicognani@meetixos.org)
 *
 * @developers
 * Marco Cicognani (marco.cicognani@meetixos.org)
 *
 * @license
 * GNU General Public License version 3
 */

#include "AMD.hh"

#include "CpuDetect.hh"

#include <iostream>
#include <string_view>

int inspect_amd(int argc, const char** argv) {
    std::cout << "AMD Specific Features:";

    usize extended, eax, ebx, ecx, edx, unused;
    CPU_ID(1, eax, unused, unused, unused);

    usize model    = (eax >> 4) & 0xf;
    usize family   = (eax >> 8) & 0xf;
    usize stepping = eax & 0xf;
    usize reserved = eax >> 12;

    std::cout << "Family " << family << " Model: " << model << " [";
    switch ( family ) {
        case 4:
            std::cout << "486 Model" << model;
            break;
        case 5:
            switch ( model ) {
                case 0:
                case 1:
                case 2:
                case 3:
                case 6:
                case 7:
                    std::cout << "K6 Model " << model;
                    break;
                case 8:
                    std::cout << "K6-2 Model 8";
                    break;
                case 9:
                    std::cout << "K6-III Model 9";
                    break;
                default:
                    std::cout << "K5/K6 Model " << model;
                    break;
            }
            break;
        case 6:
            switch ( model ) {
                case 1:
                case 2:
                case 4:
                    std::cout << "Athlon Model " << model;
                    break;
                case 3:
                    std::cout << "Duron Model 3";
                    break;
                case 6:
                    std::cout << "Athlon MP/Mobile Athlon Model 6";
                    break;
                case 7:
                    std::cout << "Mobile Duron Model 7";
                    break;
                default:
                    std::cout << "Duron/Athlon Model " << model;
                    break;
            }
            break;
        default:
            std::cout << "Unknown";
            break;
    }
    std::cout << "]\n";

    /* check for extended features */
    CPU_ID(0x80000000, extended, unused, unused, unused);
    if ( !extended )
        return EXIT_SUCCESS;

    if ( extended >= 0x80000002 ) {
        std::cout << "Detected Processor Name: ";
        for ( auto i = 0x80000002; i <= 0x80000004; i++ ) {
            CPU_ID(i, eax, ebx, ecx, edx);
            print_regs(eax, ebx, ecx, edx);
        }
        std::cout << '\n';
    }

    if ( extended >= 0x80000007 ) {
        CPU_ID(0x80000007, unused, unused, unused, edx);
        if ( edx & 1 )
            std::cout << "Temperature Sensing Diode Detected!\n";
    }

    std::cout << "Stepping: " << stepping << " Reserved: " << reserved << std::endl;
    return EXIT_SUCCESS;
}
