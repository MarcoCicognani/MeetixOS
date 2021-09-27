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

#ifndef _CPUDET_h_
#define _CPUDET_h_

#include <stdio.h>
#include <string.h>

/* Required Declarations */
int  doINTeL(int argc, char* argv[]);
int  doAMD(void);
void printregs(int eax, int ebx, int ecx, int edx);

#define cpuID(in, a, b, c, d) __asm__("cpuid" : "=a"(a), "=b"(b), "=c"(c), "=d"(d) : "a"(in));

/* Intel Specific brand list */
const char* intel[] = { "Brand ID Not Supported.",
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

/* This table is for those brand strings that have two values depending on the processor signature.
 * It should have the same number of entries as the above table. */
const char* otherIntel[] = { "Reserved",
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

#endif
