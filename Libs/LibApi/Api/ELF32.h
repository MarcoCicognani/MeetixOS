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

#pragma once

#include <Api/StdInt.h>

#ifdef __cplusplus
extern "C" {
#endif

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
    u16 e_shstrndx;         /* Section header string table index */
} A_PACKED Elf32Ehdr;

/**
 * @brief ELF program headers
 */
#define PT_NULL    0 /* Unused header */
#define PT_LOAD    1 /* Loadable segment */
#define PT_DYNAMIC 2 /* Linking information  */
#define PT_INTERP  3 /* Path to the interpreter */
#define PT_NOTE    4 /* Auxiliary information */
#define PT_SHLIB   5 /* Reserved */
#define PT_PHDR    6 /* Program header table */
#define PT_TLS     7 /* Thread local storage */
#define PT_LOPROC  0x70000000
#define PT_HIPROC  0x7FFFFFFF

/**
 * @brief ELF program header protection flags
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
    u32 p_paddr;  /* Physical address */
    u32 p_filesz; /* Size of the segment in the binary file */
    u32 p_memsz;  /* Size of the segment in memory */
    u32 p_flags;  /* Segment flags */
    u32 p_align;  /* Alignment information */
} A_PACKED Elf32Phdr;

/**
 * @brief ELF section header
 */
#define SHT_NULL     0  /* Inactive header */
#define SHT_PROGBITS 1  /* Information defined by the program */
#define SHT_SYMTAB   2  /* Symbol table */
#define SHT_STRTAB   3  /* String table */
#define SHT_RELA     4  /* Relocation entries with explicit addend. Not in x86 ABI */
#define SHT_HASH     5  /* Symbol hash table */
#define SHT_DYNAMIC  6  /* Information for dynamic linking */
#define SHT_NOTE     7  /* Notes */
#define SHT_NOBITS   8  /* Program defined with empty space */
#define SHT_REL      9  /* Relocation entries with implicit addend */
#define SHT_SHLIB    10 /* Reserved */
#define SHT_DYNSYM   11 /* Dynamic symbols */

typedef struct {
    u32 sh_name;      /* Section name */
    u32 sh_type;      /* Section type */
    u32 sh_flags;     /* Section flags */
    u32 sh_addr;      /* Section virtual address at execution */
    u32 sh_offset;    /* Section file offset */
    u32 sh_size;      /* Section size in bytes */
    u32 sh_link;      /* Link to another section */
    u32 sh_info;      /* Additional section information */
    u32 sh_addralign; /* Section alignment */
    u32 sh_entsize;   /* Entry size if section holds table */
} A_PACKED Elf32Shdr;

#ifdef __cplusplus
}
#endif
