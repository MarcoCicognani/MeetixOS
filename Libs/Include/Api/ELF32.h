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

#pragma once

#include <Api/StdInt.h>

__BEGIN_C

/**
 * @brief ELF Ident magics
 */
#define EI_MAG0    0
#define EI_MAG1    1
#define EI_MAG2    2
#define EI_MAG3    3
#define EI_CLASS   4
#define EI_DATA    5
#define EI_VERSION 6
#define EI_PAD     7
#define EI_NIDENT  16

#define ELFMAG0 0x7F
#define ELFMAG1 'E'
#define ELFMAG2 'L'
#define ELFMAG3 'F'

#define ELFCLASSNONE 0
#define ELFCLASS32   1
#define ELFCLASS64   2

#define ELFDATANONE 0
#define ELFDATA2LSB 1
#define ELFDATA2MSB 2

/**
 * @brief ELF header types
 */
#define ET_NONE   0
#define ET_REL    1
#define ET_EXEC   2
#define ET_DYN    3
#define ET_CORE   4
#define ET_LOPROC 0xFF00
#define ET_HIPROC 0xFFFF

#define EM_NONE  0
#define EM_M32   1
#define EM_SPARC 2
#define EM_386   3
#define EM_68K   4
#define EM_88K   5
#define EM_860   6
#define EM_MIPS  8

#define EV_NONE    0
#define EV_CURRENT 1

/**
 * @brief ELF header
 */
typedef struct {
    u8  e_ident[EI_NIDENT]; /* Ident structure */
    u16 e_type;             /* Type */
    u16 e_machine;          /* Target architecture */
    u32 e_version;          /* EV_CURRENT for standard ELF files */
    u32 e_entry;            /* Entry's virtual address */
    u32 e_phoff;            /* Offset to program header table */
    u32 e_shoff;            /* Offset to section header table */
    u32 e_flags;            /* Architecture-specific flags */
    u16 e_ehsize;           /* Size of ELF header in bytes */
    u16 e_phentsize;        /* Size of one entry in program header table */
    u16 e_phnum;            /* Number of entries in program header table */
    u16 e_shentsize;        /* Size of one entry in section header table */
    u16 e_shnum;            /* Number of entries in section header table */
    u16 e_shstrndx;         /* Section header table index of section name string table */
} A_PACKED Elf32Ehdr;

/**
 * @brief ELF program headers
 */
#define PT_NULL    0
#define PT_LOAD    1
#define PT_DYNAMIC 2
#define PT_INTERP  3
#define PT_NOTE    4
#define PT_SHLIB   5
#define PT_PHDR    6
#define PT_TLS     7
#define PT_LOPROC  0x70000000
#define PT_HIPROC  0x7FFFFFFF

/**
 * @brief ELF Program header protection flags
 */
#define PF_X        1
#define PF_W        2
#define PF_R        4
#define PF_MASKOS   0x0FF00000
#define PF_MASKPROC 0xF0000000

typedef struct {
    u32 p_type;   /* Type of the segment */
    u32 p_offset; /* Offset of the segment in the binary file */
    u32 p_vaddr;  /* Virtual address */
    u32 p_paddr;  /* Not relevant for System V */
    u32 p_filesz; /* Size of the segment in the binary file */
    u32 p_memsz;  /* Size of the segment in memory */
    u32 p_flags;  /* Segment flags */
    u32 p_align;  /* Alignment information */
} A_PACKED Elf32Phdr;

__END_C
