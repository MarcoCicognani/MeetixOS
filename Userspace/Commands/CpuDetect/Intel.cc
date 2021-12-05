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

#include "Intel.hh"

#include "CpuDetect.hh"

#include <iostream>
#include <string_view>

static const char* g_intel_brand_list[] = { "Brand ID Not Supported.",
                                            "Intel(R) Celeron(R) processor",
                                            "Intel(R) Pentium(R) III processor",
                                            "Intel(R) Pentium(R) III Xeon(R) processor",
                                            "Intel(R) Pentium(R) III processor",
                                            "Reserved",
                                            "Mobile Intel(R) Pentium(R) III processor-M",
                                            "Mobile Intel(R) Celeron(R) processor",
                                            "Intel(R) Pentium(R) 4 processor",
                                            "Intel(R) Pentium(R) 4 processor",
                                            "Intel(R) Celeron(R) processor",
                                            "Intel(R) Xeon(R) Processor",
                                            "Intel(R) Xeon(R) processor MP",
                                            "Reserved",
                                            "Mobile Intel(R) Pentium(R) 4 processor-M",
                                            "Mobile Intel(R) Pentium(R) Celeron(R) processor",
                                            "Reserved",
                                            "Mobile Genuine Intel(R) processor",
                                            "Intel(R) Celeron(R) M processor",
                                            "Mobile Intel(R) Celeron(R) processor",
                                            "Intel(R) Celeron(R) processor",
                                            "Mobile Genuine Intel(R) processor",
                                            "Intel(R) Pentium(R) M processor",
                                            "Mobile Intel(R) Celeron(R) processor" };

static const char* g_intel_extended_list[] = { "Reserved",
                                               "Reserved",
                                               "Reserved",
                                               "Intel(R) Celeron(R) processor",
                                               "Reserved",
                                               "Reserved",
                                               "Reserved",
                                               "Reserved",
                                               "Reserved",
                                               "Reserved",
                                               "Reserved",
                                               "Intel(R) Xeon(R) processor MP",
                                               "Reserved",
                                               "Reserved",
                                               "Intel(R) Xeon(R) processor",
                                               "Reserved",
                                               "Reserved",
                                               "Reserved",
                                               "Reserved",
                                               "Reserved",
                                               "Reserved",
                                               "Reserved",
                                               "Reserved",
                                               "Reserved" };

int inspect_intel(int argc, const char** argv) {
    usize eax, ebx, ecx, edx, max_eax, unused, extended_family;
    CPU_ID(1, eax, ebx, unused, unused);

    usize model     = (eax >> 4) & 0xf;
    usize family    = (eax >> 8) & 0xf;
    usize type      = (eax >> 12) & 0x3;
    usize brand     = ebx & 0xff;
    usize stepping  = eax & 0xf;
    usize reserved  = eax >> 14;
    usize signature = eax;

    if ( argc > 1 ) {
        if ( std::string_view{ argv[0] } == std::string_view{ "--brand" } ) {
            CPU_ID(0x80000000, max_eax, unused, unused, unused);

            /* Quok said: If the max extended eax value is high enough to support the processor
             * brand string (values 0x80000002 to 0x80000004), then we'll use that information to
             * return the brand information. Otherwise, we'll refer back to the brand tables above
             * for backwards compatibility with older processors. According to the Sept. 2006 Intel
             * Arch Software Developer's Guide, if extended eax values are supported, then all 3
             * values for the processor brand string are supported, but we'll test just to make sure
             * and be safe.
             */
            if ( max_eax >= 0x80000004 ) {
                if ( max_eax >= 0x80000002 ) {
                    CPU_ID(0x80000002, eax, ebx, ecx, edx);
                    print_regs(eax, ebx, ecx, edx);
                }
                if ( max_eax >= 0x80000003 ) {
                    CPU_ID(0x80000003, eax, ebx, ecx, edx);
                    print_regs(eax, ebx, ecx, edx);
                }
                if ( max_eax >= 0x80000004 ) {
                    CPU_ID(0x80000004, eax, ebx, ecx, edx);
                    print_regs(eax, ebx, ecx, edx);
                }
                std::cout << '\n';
            } else if ( brand > 0 ) {
                std::cout << "Brand " << brand << " - ";
                if ( brand < 0x18 ) {
                    if ( signature == 0x000006B1 || signature == 0x00000F13 )
                        std::cout << g_intel_extended_list[brand] << '\n';
                    else
                        std::cout << g_intel_brand_list[brand] << '\n';
                } else
                    std::cout << "Reserved\n";
            }
        }
    } else {
        std::cout << "Intel Specific Features:\n";

        std::cout << "Type " << type << " - ";
        switch ( type ) {
            case 0:
                std::cout << "Original OEM\n";
                break;
            case 1:
                std::cout << "Overdrive\n";
                break;
            case 2:
                std::cout << "Dual-capable\n";
                break;
            case 3:
                std::cout << "Reserved\n";
                break;
            default:
                std::cout << "Unknown\n";
                break;
        }

        std::cout << "Family " << family << " - ";
        switch ( family ) {
            case 3:
                std::cout << "i386\n";
                break;
            case 4:
                std::cout << "i486\n";
                break;
            case 5:
                std::cout << "Pentium\n";
                break;
            case 6:
                std::cout << "Pentium Pro\n";
                break;
            case 15:
                std::cout << "Pentium 4\n";
                break;
            default:
                break;
        }

        /* check for extended family */
        if ( family == 15 ) {
            extended_family = (eax >> 20) & 0xff;
            std::cout << "Extended family " << extended_family << '\n';
        }

        std::cout << "Model " << model << " - ";
        switch ( family ) {
            case 4:
                switch ( model ) {
                    case 0:
                    case 1:
                        std::cout << "DX\n";
                        break;
                    case 2:
                        std::cout << "SX\n";
                        break;
                    case 3:
                        std::cout << "487/DX2\n";
                        break;
                    case 4:
                        std::cout << "SL\n";
                        break;
                    case 5:
                        std::cout << "SX2\n";
                        break;
                    case 7:
                        std::cout << "write-back enhanced DX2\n";
                        break;
                    case 8:
                        std::cout << "DX4\n";
                        break;
                    default:
                        std::cout << "Unknown\n";
                        break;
                }
                break;
            case 5:
                switch ( model ) {
                    case 1:
                        std::cout << "60/66\n";
                        break;
                    case 2:
                        std::cout << "75-200\n";
                        break;
                    case 3:
                        std::cout << "for 486 system\n";
                        break;
                    case 4:
                        std::cout << "MMX\n";
                        break;
                    default:
                        std::cout << "Unknown\n";
                        break;
                }
                break;
            case 6:
                switch ( model ) {
                    case 1:
                        std::cout << "Pentium Pro\n";
                        break;
                    case 3:
                        std::cout << "Pentium II Model 3\n";
                        break;
                    case 5:
                        std::cout << "Pentium II Model 5/Xeon/Celeron\n";
                        break;
                    case 6:
                        std::cout << "Celeron\n";
                        break;
                    case 7:
                        std::cout << "Pentium III/Pentium III Xeon - external L2 cache\n";
                        break;
                    case 8:
                        std::cout << "Pentium III/Pentium III Xeon - internal L2 cache\n";
                        break;
                    default:
                        std::cout << "Unknown\n";
                        break;
                }
                break;
            case 3:
            case 15:
            default:
                std::cout << "Unknown\n";
                break;
        }

        CPU_ID(0x80000000, max_eax, unused, unused, unused);

        /* Quok said: If the max extended eax value is high enough to support the processor
         * brand string (values 0x80000002 to 0x80000004), then we'll use that information to
         * return the brand information. Otherwise, we'll refer back to the brand tables above
         * for backwards compatibility with older processors. According to the Sept. 2006 Intel
         * Arch Software Developer's Guide, if extended eax values are supported, then all 3
         * values for the processor brand string are supported, but we'll test just to make sure
         * and be safe.
         */
        if ( max_eax >= 0x80000004 ) {
            std::cout << "Brand: ";
            if ( max_eax >= 0x80000002 ) {
                CPU_ID(0x80000002, eax, ebx, ecx, edx);
                print_regs(eax, ebx, ecx, edx);
            }
            if ( max_eax >= 0x80000003 ) {
                CPU_ID(0x80000003, eax, ebx, ecx, edx);
                print_regs(eax, ebx, ecx, edx);
            }
            if ( max_eax >= 0x80000004 ) {
                CPU_ID(0x80000004, eax, ebx, ecx, edx);
                print_regs(eax, ebx, ecx, edx);
            }
            std::cout << '\n';
        } else if ( brand > 0 ) {
            std::cout << "Brand " << brand << " - ";
            if ( brand < 0x18 ) {
                if ( signature == 0x000006B1 || signature == 0x00000F13 )
                    std::cout << g_intel_extended_list[brand] << '\n';
                else
                    std::cout << g_intel_brand_list[brand] << '\n';
            } else
                std::cout << "Reserved\n";
        }

        std::cout << "Stepping: " << stepping << " Reserved: " << reserved << '\n';
    }
    return EXIT_SUCCESS;
}