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

#include "ELFInspector.hh"

#include <Api/ELF32.h>
#include <sstream>

std::string ELFInspector::inspection() {
    Elf32Ehdr elf_hdr{};
    m_stream.seekg(0, std::ifstream::beg);
    m_stream.read(reinterpret_cast<char*>(&elf_hdr), sizeof(Elf32Ehdr));

    std::stringstream ss{};
    ss << "ELF Object\n";
    ss << "\tType: ";
    switch ( elf_hdr.e_type ) {
        case ET_REL:
            ss << "Relocatable\n";
            break;
        case ET_EXEC:
            ss << "Executable\n";
            break;
        case ET_DYN:
            ss << "Shared\n";
            break;
        case ET_CORE:
            ss << "Core\n";
            break;
        case ET_LOPROC:
        case ET_HIPROC:
            ss << "Processor Specific\n";
            break;
        default:
            ss << "Unknown\n";
            break;
    }

    ss << "\tArch: ";
    switch ( elf_hdr.e_machine ) {
        case EM_M32:
            ss << "AT&T WE 32100\n";
            break;
        case EM_SPARC:
            ss << "SPARC\n";
            break;
        case EM_386:
            ss << "Intel 386\n";
            break;
        case EM_68K:
            ss << "Motorola 68000\n";
            break;
        case EM_88K:
            ss << "Motorola 88000\n";
            break;
        case EM_860:
            ss << "Intel 80860\n";
            break;
        case EM_MIPS:
            ss << "MIPS\n";
            break;
        default:
            ss << "Unknown\n";
            break;
    }

    ss << "\tClass: ";
    switch ( elf_hdr.e_ident[EI_CLASS] ) {
        case ELFCLASSNONE:
            ss << "Unknown\n";
            break;
        case ELFCLASS32:
            ss << "32 bits\n";
            break;
        case ELFCLASS64:
            ss << "64 bits\n";
            break;
        default:
            ss << "Unknown\n";
            break;
    }

    ss << "\tEndianness: ";
    switch ( elf_hdr.e_ident[EI_DATA] ) {
        case ELFDATANONE:
            ss << "Unknown\n";
            break;
        case ELFDATA2LSB:
            ss << "Little\n";
            break;
        case ELFDATA2MSB:
            ss << "Big\n";
            break;
        default:
            ss << "Unknown\n";
            break;
    }

    ss << "\tVersion: ";
    switch ( elf_hdr.e_version ) {
        case EV_NONE:
            ss << "Unknown\n";
            break;
        case EV_CURRENT:
            ss << "Current\n";
            break;
        default:
            ss << "Unknown\n";
            break;
    }

    ss << "\tEntry Address: 0x" << std::hex << elf_hdr.e_entry << "\n";
    ss << "\tProgram Headers: " << elf_hdr.e_phnum << "\n";
    return ss.str();
}
