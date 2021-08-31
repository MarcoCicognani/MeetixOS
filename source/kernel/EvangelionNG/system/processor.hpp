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

#ifndef EVA_SHARED_SYSTEM_CPU
#define EVA_SHARED_SYSTEM_CPU

#include "eva/stdint.h"
#include <system/interrupts/lapic.hpp>

/**
 * CPUID.1 feature flags
 */
enum class CpuidStandardEdxFeature
{
	FPU = 1 << 0, // Onboard x87 FPU
	VME = 1 << 1, // Virtual 8086 supported
	DE = 1 << 2, // Debugging extensions
	PSE = 1 << 3, // Page size extension
	TSC = 1 << 4, // Time stamp counter
	MSR = 1 << 5, // Model specific registers
	PAE = 1 << 6, // Physical address extension
	MCE = 1 << 7, // Machine check exception
	CX8 = 1 << 8, // CMPXCHG8 instruction
	APIC = 1 << 9, // APIC available
	SEP = 1 << 11, // SYSENTER / SYSEXIT
	MTRR = 1 << 12, // Memory type range registers
	PGE = 1 << 13, // Page global enable bit in CR4
	MCA = 1 << 14, // Machine check architecture
	CMOV = 1 << 15, // Cond. move / FCMOV instructions
	PAT = 1 << 16, // Page attribute table
	PSE36 = 1 << 17, // 36 bit page size extension
	PSN = 1 << 18, // Processor serial number
	CLF = 1 << 19, // CLFLUSH instruction
	DTES = 1 << 21, // Debug store available
	ACPI_THERMAL = 1 << 22, // on-board thermal control MSRs
	MMX = 1 << 23, // MMX instructions
	FXSR = 1 << 24, // FXSAVE, FXRESTOR instructions
	SSE = 1 << 25, // Streaming SIMD Extensions
	SSE2 = 1 << 26, // Streaming SIMD Extensions 2
	SS = 1 << 27, // CPU cache supports self-snoop
	HTT = 1 << 28, // Hyperthreading
	TM1 = 1 << 29, // Thermal monitor auto-limits temperature
	IA64 = 1 << 30, // Processor is IA64 that emulates x86
	PBE = 1 << 31, // Pending break enable wakeup support
};

/**
 * CPUID.1 feature flags
 */
enum class CpuidExtendedEcxFeature
{
	SSE3 = 1 << 0,
	PCLMUL = 1 << 1,
	DTES64 = 1 << 2,
	MONITOR = 1 << 3,
	DS_CPL = 1 << 4,
	VMX = 1 << 5,
	SMX = 1 << 6,
	EST = 1 << 7,
	TM2 = 1 << 8,
	SSSE3 = 1 << 9,
	CID = 1 << 10,
	FMA = 1 << 12,
	CX16 = 1 << 13,
	ETPRD = 1 << 14,
	PDCM = 1 << 15,
	DCA = 1 << 18,
	SSE4_1 = 1 << 19,
	SSE4_2 = 1 << 20,
	x2APIC = 1 << 21,
	MOVBE = 1 << 22,
	POPCNT = 1 << 23,
	AES = 1 << 25,
	XSAVE = 1 << 26,
	OSXSAVE = 1 << 27,
	AVX = 1 << 28
};

/**
 * Model specific registers
 */
#define IA32_APIC_BASE_MSR			0x1B
#define IA32_APIC_BASE_MSR_BSP		0x100
#define IA32_APIC_BASE_MSR_ENABLE	0x800

/**
 * Implementation in the assembler file
 */
extern "C" bool _checkForCPUID();
extern "C" void _enableSSE();

/**
 * processor descriptor
 */
class Processor
{
public:
	/**
	 * internal infos
	 */
	bool bsp;                 // single core system flag
	uint32_t apic;            // cpu apic

	/**
	 * linkage info
	 */
	Processor *next;  // next cpu

	/**
	 * empty constructor
	 */
	Processor() : bsp(false), apic(-1), next(0) {}

	/**
	 * get the cpu apic id
	 *
	 * @return the apic code
	 */
	uint32_t getId() const { return apic; }

	/**
	 * check the cpuid support
	 *
	 * @return true if the cpu support the cpuid, false otherwise
	 */
	static bool supportsCpuid();

	/**
	 * performs the cpuid
	 *
	 * @param code:		cpuid code
	 * @param outA:		pointer to a regex result
	 * @param outB:		pointer to b regex result
	 * @param outC:		pointer to c regex result
	 * @param outD:		pointer to d regex result
	 */
	static void cpuid(uint32_t code, uint32_t *outA, uint32_t *outB, uint32_t *outC, uint32_t *outD);

	/**
	 * check the standart features of the cpu
	 *
	 * @param feature:		one of the CpuidStandardEdxFeature code
	 */
	static bool hasFeature(CpuidStandardEdxFeature feature);

	/**
	 * check the extended features of the cpu
	 *
	 * @param feature.		one of the CpuidExtendedEcxFeature code
	 */
	static bool hasFeature(CpuidExtendedEcxFeature feature);

	/**
	 * print cpu informations
	 */
	static void printInformation();

	/**
	 * call the assembly implementation to enable the SSe instructions set
	 */
	static void enableSSE();

	/**
	 * fill the out buffer with the vendor of the cpu
	 *
	 * @param out:		pointer to a buffer at least of 12 bytes
	 */
	static void getVendor(char *out);

	/**
	 * read on the cpu model-specific register
	 *
	 * @param msr:		the msr code to read
	 * @param lo:		pointer to lowest value readed
	 * @param hi:		pointer to highest value readed
	 */
	static void readMsr(uint32_t msr, uint32_t *lo, uint32_t *hi);

	/**
	 * write on the cpu model-specific register
	 *
	 * @param msr:		the msr code to write
	 * @param lo:		the lowest value to write
	 * @param hi:		the highest value to write
	 */
	static void writeMsr(uint32_t msr, uint32_t lo, uint32_t hi);
};

#endif
