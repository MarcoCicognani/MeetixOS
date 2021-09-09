/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * *
 * MeetiX OS By MeetiX OS Project [Marco Cicognani]                                    *
 *                                                                                     *
 * This program is free software; you can redistribute it and/or                       *
 * modify it under the terms of the GNU General Public License                         *
 * as published by the Free Software Foundation; either version 2                      *
 * of the License, or (char *argumentat your option) any later version.                *
 *                                                                                     *
 * This program is distributed in the hope that it will be useful,                     *
 * but WITHout ANY WARRANTY; without even the implied warranty of                      *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                       *
 * GNU General Public License for more details.                                        *
 *                                                                                     *
 * You should have received a copy of the GNU General Public License                   *
 * along with this program; if not, write to the Free Software                         *
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA      *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * */

#include "CpuDetect.h"

int main(int argc, char* argv[]) {
    uint32_t ebx;
    uint32_t unused;

    cpuID(0, unused, ebx, unused, unused);

    switch ( ebx ) {
        case 0x756e6547: /* Intel Magic Code */
            doINTeL(argc, argv);
            break;

        case 0x68747541: /* AMD Magic Code */
            doAMD();
            break;

        default:
            println("Unknown x86 CPU Detected");
            break;
    }

    return 0;
}

/* Intel-specific information */
int doINTeL(int argc, char* argv[]) {
    uint32_t eax;
    uint32_t ebx;
    uint32_t ecx;
    uint32_t edx;
    uint32_t max_eax;
    uint32_t signature;
    uint32_t unused;

    int model;
    int family;
    int type;
    int brand;
    int stepping;
    int reserved;
    int extended_family = -1;

    cpuID(1, eax, ebx, unused, unused);

    model     = (eax >> 4) & 0xf;
    family    = (eax >> 8) & 0xf;
    type      = (eax >> 12) & 0x3;
    brand     = ebx & 0xff;
    stepping  = eax & 0xf;
    reserved  = eax >> 14;
    signature = eax;

    if ( argc > 1 ) {
        if ( !strcmp("--brand", argv[0]) ) {
            cpuID(0x80000000, max_eax, unused, unused, unused);
            /* Quok said: If the max extended eax value is high enough to support the processor
            brand string (values 0x80000002 to 0x80000004), then we'll use that information to
            return the brand information. Otherwise, we'll refer back to the brand tables above for
            backwards compatibility with older processors. According to the Sept. 2006 Intel Arch
            Software Developer's Guide, if extended eax values are supported,
            then all 3 values for the processor brand string are supported, but we'll test just to
            make sure and be safe. */
            if ( max_eax >= 0x80000004 ) {
                if ( max_eax >= 0x80000002 ) {
                    cpuID(0x80000002, eax, ebx, ecx, edx);
                    printregs(eax, ebx, ecx, edx);
                }
                if ( max_eax >= 0x80000003 ) {
                    cpuID(0x80000003, eax, ebx, ecx, edx);
                    printregs(eax, ebx, ecx, edx);
                }
                if ( max_eax >= 0x80000004 ) {
                    cpuID(0x80000004, eax, ebx, ecx, edx);
                    printregs(eax, ebx, ecx, edx);
                }
                println("");
            }

            else if ( brand > 0 ) {
                printf("Brand %d - ", brand);
                if ( brand < 0x18 ) {
                    if ( signature == 0x000006B1 || signature == 0x00000F13 ) {
                        println("%s", otherIntel[brand]);
                    }

                    else {
                        println("%s", intel[brand]);
                    }
                }

                else {
                    println("Reserved");
                }
            }
        }
    }

    else {
        println("Intel Specific Features:");

        printf("Type %d - ", type);
        switch ( type ) {
            case 0:
                println("Original OEM");
                break;

            case 1:
                println("Overdrive");
                break;

            case 2:
                println("Dual-capable");
                break;

            case 3:
                println("Reserved");
                break;
        }

        printf("Family %d - ", family);
        switch ( family ) {
            case 3:
                println("i386");
                break;

            case 4:
                println("i486");
                break;

            case 5:
                println("Pentium");
                break;

            case 6:
                println("Pentium Pro");
                break;

            case 15:
                println("Pentium 4");
        }

        if ( family == 15 ) {
            extended_family = (eax >> 20) & 0xff;
            println("Extended family %d", extended_family);
        }

        printf("Model %d - ", model);
        switch ( family ) {
            case 3:
                break;

            case 4:
                switch ( model ) {
                    case 0:

                    case 1:
                        println("DX");
                        break;

                    case 2:
                        println("SX");
                        break;

                    case 3:
                        println("487/DX2");
                        break;

                    case 4:
                        println("SL");
                        break;

                    case 5:
                        println("SX2");
                        break;

                    case 7:
                        println("Write-back enhanced DX2");
                        break;

                    case 8:
                        println("DX4");
                        break;
                }
                break;

            case 5:
                switch ( model ) {
                    case 1:
                        println("60/66");
                        break;

                    case 2:
                        println("75-200");
                        break;

                    case 3:
                        println("for 486 system");
                        break;

                    case 4:
                        println("MMX");
                        break;
                }
                break;

            case 6:
                switch ( model ) {
                    case 1:
                        println("Pentium Pro");
                        break;

                    case 3:
                        println("Pentium II Model 3");
                        break;

                    case 5:
                        println("Pentium II Model 5/Xeon/Celeron");
                        break;

                    case 6:
                        println("Celeron");
                        break;

                    case 7:
                        println("Pentium III/Pentium III Xeon - external L2 cache");
                        break;

                    case 8:
                        println("Pentium III/Pentium III Xeon - internal L2 cache");
                        break;
                }
                break;

            case 15:
                break;
        }

        cpuID(0x80000000, max_eax, unused, unused, unused);
        /* Quok said: If the max extended eax value is high enough to support the processor brand
        string (values 0x80000002 to 0x80000004), then we'll use that information to return the
        brand information. Otherwise, we'll refer back to the brand tables above for backwards
        compatibility with older processors. According to the Sept. 2006 Intel Arch Software
        Developer's Guide, if extended eax values are supported,
        then all 3 values for the processor brand string are supported, but we'll test just to make
        sure and be safe. */
        if ( max_eax >= 0x80000004 ) {
            printf("Brand: ");
            if ( max_eax >= 0x80000002 ) {
                cpuID(0x80000002, eax, ebx, ecx, edx);
                printregs(eax, ebx, ecx, edx);
            }
            if ( max_eax >= 0x80000003 ) {
                cpuID(0x80000003, eax, ebx, ecx, edx);
                printregs(eax, ebx, ecx, edx);
            }
            if ( max_eax >= 0x80000004 ) {
                cpuID(0x80000004, eax, ebx, ecx, edx);
                printregs(eax, ebx, ecx, edx);
            }
            println("");
        }

        else if ( brand > 0 ) {
            printf("Brand %d - ", brand);
            if ( brand < 0x18 ) {
                if ( signature == 0x000006B1 || signature == 0x00000F13 ) {
                    printf("%s", otherIntel[brand]);
                }

                else {
                    printf("%s", intel[brand]);
                }
            }

            else {
                println("Reserved");
            }
        }
        println("Stepping: %d Reserved: %d", stepping, reserved);
    }
    return 0;
}

/* Print Registers */
void printregs(int eax, int ebx, int ecx, int edx) {
    int  j;
    char string[17];
    string[16] = '\0';
    for ( j = 0; j < 4; j++ ) {
        string[j]      = eax >> (8 * j);
        string[j + 4]  = ebx >> (8 * j);
        string[j + 8]  = ecx >> (8 * j);
        string[j + 12] = edx >> (8 * j);
    }
    printf("%s", string);
}

/* AMD-specific information */
int doAMD(void) {
    println("AMD Specific Features:");
    uint32_t extended;
    uint32_t eax;
    uint32_t ebx;
    uint32_t ecx;
    uint32_t edx;
    uint32_t unused;

    int family;
    int model;
    int stepping;
    int reserved;

    cpuID(1, eax, unused, unused, unused);

    model    = (eax >> 4) & 0xf;
    family   = (eax >> 8) & 0xf;
    stepping = eax & 0xf;
    reserved = eax >> 12;

    printf("Family: %d Model: %d [", family, model);

    switch ( family ) {
        case 4:
            printf("486 Model %d");
            break;

        case 5:
            switch ( model ) {
                case 0:

                case 1:

                case 2:

                case 3:

                case 6:

                case 7:
                    printf("K6 Model %d", model);
                    break;

                case 8:
                    printf("K6-2 Model 8");
                    break;

                case 9:
                    printf("K6-III Model 9");
                    break;

                default:
                    printf("K5/K6 Model %d", model);
                    break;
            }
            break;

        case 6:
            switch ( model ) {
                case 1:

                case 2:

                case 4:
                    printf("Athlon Model %d", model);
                    break;

                case 3:
                    printf("Duron Model 3");
                    break;

                case 6:
                    printf("Athlon MP/Mobile Athlon Model 6");
                    break;

                case 7:
                    printf("Mobile Duron Model 7");
                    break;

                default:
                    printf("Duron/Athlon Model %d", model);
                    break;
            }
            break;
    }
    println("]");

    cpuID(0x80000000, extended, unused, unused, unused);

    if ( extended == 0 ) {
        return 0;
    }

    if ( extended >= 0x80000002 ) {
        size_t j;
        printf("Detected Processor Name: ");

        for ( j = 0x80000002; j <= 0x80000004; j++ ) {
            cpuID(j, eax, ebx, ecx, edx);
            printregs(eax, ebx, ecx, edx);
        }
        println("");
    }

    if ( extended >= 0x80000007 ) {
        cpuID(0x80000007, unused, unused, unused, edx);
        if ( edx & 1 ) {
            println("Temperature Sensing Diode Detected!");
        }
    }
    println("Stepping: %d Reserved: %d", stepping, reserved);

    return 0;
}
