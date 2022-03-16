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

#include "Elf32Loader.hpp"

#include <cstdio>
#include <cstring>
#include <LibUtils/File.hh>

/**
 *
 */
LoaderStatus Elf32Loader::load(uintptr_t* outEntryAddr) {
    // check header
    Elf32Ehdr    hdr;
    LoaderStatus headerStat = readAndValidateElfHeader(file, &hdr);
    if ( headerStat != LS_SUCCESSFUL )
        return headerStat;

    // perform image loading
    LoaderStatus status = loadImage(&hdr);
    *outEntryAddr       = hdr.e_entry;
    return status;
}

/**
 *
 */
LoaderStatus Elf32Loader::readAndValidateElfHeader(FileHandle file, Elf32Ehdr* hdrBuf) {
    // read header
    if ( !Utils::File::read_bytes(file, 0, (uint8_t*)hdrBuf, sizeof(Elf32Ehdr)) ) {
        klog("unable to read ELF header from file %i", file);
        return LS_IO_ERROR;
    }

    // validate header
    Elf32ValidationStatus validity = validate(hdrBuf);

    if ( validity != ELF32_VALIDATION_SUCCESSFUL ) {
        klog("ELF validation of file %i, failed with code %i", file, validity);
        return LS_FORMAT_ERROR;
    }

    return LS_SUCCESSFUL;
}

/**
 *
 */
LoaderStatus Elf32Loader::checkForElfBinaryAndReset(FileHandle file) {
    // read and validate ELF header
    Elf32Ehdr    hdrBuf;
    LoaderStatus stat = readAndValidateElfHeader(file, &hdrBuf);

    // reset file
    s_seek(file, 0, FS_SEEK_SET);

    return stat;
}

/**
 *
 */
LoaderStatus Elf32Loader::loadImage(Elf32Ehdr* hdr) {
    // Load segments
    for ( uint32_t i = 0; i < hdr->e_phnum; i++ ) {
        uint32_t phdrOffset = hdr->e_phoff + hdr->e_phentsize * i;
        uint32_t phdrLength = sizeof(Elf32Phdr);
        uint8_t  phdrBuffer[phdrLength];

        if ( !Utils::File::read_bytes(file, phdrOffset, phdrBuffer, phdrLength) ) {
            klog("unable to read segment header from file");
            return LS_IO_ERROR;
        }

        Elf32Phdr* phdr = (Elf32Phdr*)phdrBuffer;

        if ( phdr->p_type == PT_LOAD ) {
            LoaderStatus segStat = loadLoadSegment(phdr);
            if ( segStat != LS_SUCCESSFUL ) {
                klog("unable to load PT_LOAD segment from file");
                return segStat;
            }

        }

        else if ( phdr->p_type == PT_TLS ) {
            LoaderStatus segStat = loadTlsSegment(phdr);
            if ( segStat != LS_SUCCESSFUL ) {
                klog("unable to load PT_TLS segment from file");
                return segStat;
            }
        }
    }

    return LS_SUCCESSFUL;
}

/**
 *
 */
LoaderStatus Elf32Loader::loadTlsSegment(Elf32Phdr* phdr) {
    // take values
    uint32_t numBytesCopy = phdr->p_filesz;
    uint32_t numBytesZero = phdr->p_memsz;
    uint32_t alignment    = phdr->p_align;

    LoaderStatus result = LS_UNKNOWN;

    // read contents
    uint8_t* tlsContent = new uint8_t[numBytesCopy];

    if ( !Utils::File::read_bytes(file, phdr->p_offset, (uint8_t*)tlsContent, numBytesCopy) ) {
        result = LS_IO_ERROR;
        klog("unable to read TLS segment");

    }

    else {
        // call kernel to write TLS
        if ( s_write_tls_master_for_process(procIdent, tlsContent, numBytesCopy, numBytesZero, alignment) )
            result = LS_SUCCESSFUL;

        else {
            klog("unable to allocate TLS storage for spawning process");
            result = LS_MEMORY_ERROR;
        }
    }

    delete[] tlsContent;

    return result;
}

/**
 *
 */
LoaderStatus Elf32Loader::loadLoadSegment(Elf32Phdr* phdr) {
    uint32_t memStart = phdr->p_vaddr & ~0xFFF;
    uint32_t memEnd   = ((phdr->p_vaddr + phdr->p_memsz) + 0x1000) & ~0xFFF;

    uint32_t totalPages  = (memEnd - memStart) / 0x1000;
    uint32_t loadedPages = 0;

    uint32_t offsetInFile = 0;

    while ( loadedPages < totalPages ) {
        uint32_t step = (totalPages - loadedPages);
        if ( step > MAXIMUM_LOAD_PAGES_AT_ONCE )
            step = MAXIMUM_LOAD_PAGES_AT_ONCE;

        uint32_t startVirt = memStart + loadedPages * 0x1000;
        uint8_t* area      = (uint8_t*)s_create_pages_in_spaces(procIdent, startVirt, step);
        if ( area == 0 ) {
            klog("unable to allocate necessary pages for spawning process");
            return LS_MEMORY_ERROR;
        }

        // Copy content
        uint32_t copyOffsetInArea = (startVirt < phdr->p_vaddr) ? (phdr->p_vaddr - startVirt) : 0;
        uint32_t virtualFileEnd   = phdr->p_vaddr + phdr->p_filesz;

        // Is there anything left to copy?
        uint32_t copyAmount = 0;
        if ( startVirt < virtualFileEnd ) {
            // Check if file ends in this area
            if ( (virtualFileEnd >= startVirt) && (virtualFileEnd < (startVirt + step * 0x1000)) )
                copyAmount = virtualFileEnd - startVirt - copyOffsetInArea;

            else
                copyAmount = step * 0x1000 - copyOffsetInArea;
        }

        // Read file to memory
        if ( !Utils::File::read_bytes(file, phdr->p_offset + offsetInFile, &area[copyOffsetInArea], copyAmount) ) {
            klog("unable to read LOAD segment");
            return LS_IO_ERROR;
        }

        // Zero area before and after file
        if ( copyOffsetInArea > 0 )
            memset(area, 0, copyOffsetInArea);

        if ( (copyOffsetInArea + copyAmount) < step * 0x1000 ) {
            uint32_t endZeroAreaStart  = ((uint32_t)area) + copyOffsetInArea + copyAmount;
            uint32_t endZeroAreaLength = (step * 0x1000) - (copyOffsetInArea + copyAmount);
            memset((void*)endZeroAreaStart, 0, endZeroAreaLength);
        }

        // Unmap area
        s_unmap_mem(area);
        loadedPages += step;
        offsetInFile += copyAmount;
    }

    return LS_SUCCESSFUL;
}

/**
 * Validates the ELF header
 */
Elf32ValidationStatus Elf32Loader::validate(Elf32Ehdr* header) {
    // Valid ELF header
    if ( /**/ (header->e_ident[EI_MAG0] != ELFMAG0) || // 0x7F
         (header->e_ident[EI_MAG1] != ELFMAG1) ||      // E
         (header->e_ident[EI_MAG2] != ELFMAG2) ||      // L
         (header->e_ident[EI_MAG3] != ELFMAG3) )       // F
    {
        return ELF32_VALIDATION_NOT_ELF;
    }

    // Must be executable
    if ( header->e_type != ET_EXEC )
        return ELF32_VALIDATION_NOT_EXECUTABLE;

    // Must be i386 architecture compatible
    if ( header->e_machine != EM_386 )
        return ELF32_VALIDATION_NOT_I386;

    // Must be 32 bit
    if ( header->e_ident[EI_CLASS] != ELFCLASS32 )
        return ELF32_VALIDATION_NOT_32BIT;

    // Must be little endian
    if ( header->e_ident[EI_DATA] != ELFDATA2LSB )
        return ELF32_VALIDATION_NOT_LITTLE_ENDIAN;

    // Must comply to current ELF standard
    if ( header->e_version != EV_CURRENT )
        return ELF32_VALIDATION_NOT_STANDARD_ELF;

    // All fine
    return ELF32_VALIDATION_SUCCESSFUL;
}
