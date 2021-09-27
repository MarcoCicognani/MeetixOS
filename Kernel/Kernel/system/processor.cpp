/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * *
 * MeetiX OS By MeetiX OS Project [Marco Cicognani]                                    *
 *                                                                                     *
 *         DERIVED FROM THE GHOST OPERATING SYSTEM                                     *
 *         This software is derived from the Ghost operating system project,           *
 *         written by Max Schlüssel <lokoxe@gmail.com>. Copyright 2012-2017            *
 *         https://ghostkernel.org/                                                    *
 *         https://github.com/maxdev1/ghost                                            *
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

#include <EvangelionNG.hpp>
#include <logger/logger.hpp>
#include <system/processor.hpp>

/**
 * check the cpuid support
 *
 * @return true if the cpu support the cpuid, false otherwise
 */
bool Processor::supportsCpuid() {
    return _checkForCPUID();
}

/**
 * performs the cpuid
 *
 * @param code:		cpuid code
 * @param outA:		pointer to a regex result
 * @param outB:		pointer to b regex result
 * @param outC:		pointer to c regex result
 * @param outD:		pointer to d regex result
 */
void Processor::cpuid(uint32_t  code,
                      uint32_t* outA,
                      uint32_t* outB,
                      uint32_t* outC,
                      uint32_t* outD) {
    asm volatile("cpuid" : "=a"(*outA), "=b"(*outB), "=c"(*outC), "=d"(*outD) : "a"(code));
}

/**
 * check the standart features of the cpu
 *
 * @param feature:		one of the CpuidStandardEdxFeature code
 */
bool Processor::hasFeature(CpuidStandardEdxFeature feature) {
    uint32_t eax;
    uint32_t ebx;
    uint32_t ecx;
    uint32_t edx;
    cpuid(1, &eax, &ebx, &ecx, &edx);
    return (edx & (uint64_t)feature);
}

/**
 * check the extended features of the cpu
 *
 * @param feature.		one of the CpuidExtendedEcxFeature code
 */
bool Processor::hasFeature(CpuidExtendedEcxFeature feature) {
    uint32_t eax;
    uint32_t ebx;
    uint32_t ecx;
    uint32_t edx;
    cpuid(1, &eax, &ebx, &ecx, &edx);
    return (ecx & (uint64_t)feature);
}

/**
 * print cpu informations
 */
void Processor::printInformation() {
    char vendor[13];
    Processor::getVendor(vendor);
    vendor[12] = 0;
    logInfo("%! vendor: '%s'", "cpu", vendor);

    uint32_t eax;
    uint32_t ebx;
    uint32_t ecx;
    uint32_t edx;
    cpuid(1, &eax, &ebx, &ecx, &edx);

    logInfon("%! cpu features:", "cpu");
    if ( edx & (int64_t)CpuidStandardEdxFeature::PAE )
        logInfon(" PAE");
    if ( edx & (int64_t)CpuidStandardEdxFeature::MMX )
        logInfon(" MMX");
    if ( edx & (int64_t)CpuidStandardEdxFeature::SSE )
        logInfon(" SSE");
    if ( edx & (int64_t)CpuidStandardEdxFeature::SSE2 )
        logInfon(" SSE2");
    logInfo("");
}

/**
 * call the assembly implementation to enable the SSe instructions set
 */
void Processor::enableSSE() {
    _enableSSE();
}

/**
 * fill the out buffer with the vendor of the cpu
 *
 * @param out:		pointer to a buffer at least of 12 bytes
 */
void Processor::getVendor(char* out) {
    uint32_t eax;
    uint32_t ebx;
    uint32_t ecx;
    uint32_t edx;
    cpuid(0, &eax, &ebx, &ecx, &edx);

    uint32_t* io = (uint32_t*)out;
    io[0]        = ebx;
    io[1]        = edx;
    io[2]        = ecx;
}

/**
 * read on the cpu model-specific register
 *
 * @param msr:		the msr code to read
 * @param lo:		pointer to lowest value readed
 * @param hi:		pointer to highest value readed
 */
void Processor::readMsr(uint32_t msr, uint32_t* lo, uint32_t* hi) {
    asm volatile("rdmsr" : "=a"(*lo), "=d"(*hi) : "c"(msr));
}

/**
 * write on the cpu model-specific register
 *
 * @param msr:		the msr code to write
 * @param lo:		the lowest value to write
 * @param hi:		the highest value to write
 */
void Processor::writeMsr(uint32_t msr, uint32_t lo, uint32_t hi) {
    asm volatile("wrmsr" : : "a"(lo), "d"(hi), "c"(msr));
}
