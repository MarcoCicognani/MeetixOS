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


#ifndef EVA_SHARED_ELF_ELF32
#define EVA_SHARED_ELF_ELF32

#include "eva/stdint.h"

__BEGIN_C

/**
 * ELF32 types
 */
typedef uint32_t Elf32Addr;
typedef uint16_t Elf32Half;
typedef uint32_t Elf32Off;
typedef int32_t Elf32Sword;
typedef uint32_t Elf32Word;

/**
 * Ident header
 */
#define EI_MAG0			0
#define EI_MAG1			1
#define EI_MAG2			2
#define EI_MAG3			3
#define EI_CLASS		4
#define EI_DATA			5
#define EI_VERSION		6
#define EI_PAD			7
#define EI_NIDENT		16

#define ELFMAG0			0x7F
#define ELFMAG1			'E'
#define ELFMAG2			'L'
#define ELFMAG3			'F'

#define ELFCLASSNONE	0
#define ELFCLASS32		1
#define ELFCLASS64		2

#define ELFDATANONE		0
#define ELFDATA2LSB		1
#define ELFDATA2MSB		2

/**
 * ELF header
 */
#define ET_NONE			0
#define ET_REL			1
#define ET_EXEC			2
#define ET_DYN			3
#define ET_CORE			4
#define ET_LOPROC		0xFF00
#define ET_HIPROC		0xFFFF

#define EM_NONE			0
#define EM_M32			1
#define EM_SPARC		2
#define EM_386			3
#define EM_68K			4
#define EM_88K			5
#define EM_860			6
#define EM_MIPS			8

#define EV_NONE			0
#define EV_CURRENT		1

typedef struct 
{
	uint8_t e_ident[EI_NIDENT]; 	// Ident structure
	Elf32Half e_type; 				// Type
	Elf32Half e_machine; 			// Target architecture
	Elf32Word e_version; 			// EV_CURRENT for standard ELF files
	Elf32Addr e_entry; 				// Entry's virtual address
	Elf32Off e_phoff;  				// Offset to program header table
	Elf32Off e_shoff;  				// Offset to section header table
	Elf32Word e_flags;  			// Architecture-specific flags
	Elf32Half e_ehsize; 			// Size of ELF header in bytes
	Elf32Half e_phentsize; 			// Size of one entry in program header table
	Elf32Half e_phnum; 	   			// Number of entries in program header table
	Elf32Half e_shentsize; 			// Size of one entry in section header table
	Elf32Half e_shnum; 				// Number of entries in section header table
	Elf32Half e_shstrndx; 			// Section header table index of section name string table
} __attribute__((packed)) Elf32Ehdr;

/**
 * Program header
 */
#define PT_NULL			0
#define PT_LOAD			1
#define PT_DYNAMIC		2
#define PT_INTERP		3
#define PT_NOTE			4
#define PT_SHLIB		5
#define PT_PHDR			6
#define PT_TLS			7
#define PT_LOPROC		0x70000000
#define PT_HIPROC		0x7FFFFFFF

#define PF_X			1
#define PF_W			2
#define PF_R			4
#define PF_MASKOS		0x0FF00000
#define PF_MASKPROC		0xF0000000

typedef struct 
{
	Elf32Word p_type; 		// Type of the segment
	Elf32Off p_offset;		// Offset of the segment in the binary file
	Elf32Addr p_vaddr;		// Virtual address
	Elf32Addr p_paddr;		// Not relevant for System V
	Elf32Word p_filesz; 	// Size of the segment in the binary file
	Elf32Word p_memsz;		// Size of the segment in memory
	Elf32Word p_flags;		// Segment flags
	Elf32Word p_align;		// Alignment information
} __attribute__((packed)) Elf32Phdr;

__END_C

#endif
