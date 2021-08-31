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

#ifndef __ELF32_LOADER__
#define __ELF32_LOADER__

#include "loader.hpp"
#include <eva/elf32.h>

/**
 *
 */
enum Elf32ValidationStatus
{
	ELF32_VALIDATION_SUCCESSFUL,
	ELF32_VALIDATION_NOT_ELF,
	ELF32_VALIDATION_NOT_EXECUTABLE,
	ELF32_VALIDATION_NOT_I386,
	ELF32_VALIDATION_NOT_32BIT,
	ELF32_VALIDATION_NOT_LITTLE_ENDIAN,
	ELF32_VALIDATION_NOT_STANDARD_ELF
};

/**
 *
 */
class Elf32Loader : public Loader
{
public:
	//
	Elf32Loader(ProcessCreationIdentifier target, File_t file) : Loader(target, file) {}

	//
	virtual ~Elf32Loader() {}

	//
	virtual LoaderStatus load(uintptr_t *outEntryAddr);

	//
	static LoaderStatus readAndValidateElfHeader(File_t file, Elf32Ehdr *hdrBuf);
	static LoaderStatus checkForElfBinaryAndReset(File_t file);

private:
	//
	LoaderStatus loadImage(Elf32Ehdr *hdr);
	LoaderStatus loadTlsSegment(Elf32Phdr *phdr);
	LoaderStatus loadLoadSegment(Elf32Phdr *phdr);

	//
	static Elf32ValidationStatus validate(Elf32Ehdr *header);

};

#endif
