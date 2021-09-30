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

#include <Api.h>
#include <Api/ELF32.h>
#include <ctype.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/**
 * show help
 */
void usage(const char* cmdname) {
    println("");
    println("File Specifier utility");
    println("usage: %s <file>\tfiles you want specification", cmdname);
    println("");
}

/*
 * read elf header
 */
bool readElfHeader(FileHandle file, uint8_t* buffer) {
    // read the elf header
    FsReadStatus stat;
    size_t       read = s_read_s(file, buffer, sizeof(Elf32Ehdr), &stat);

    // ckeck for status
    if ( stat != FS_READ_SUCCESSFUL || read <= 0 )
        return false;

    return true;
}

/*
 * return a printable string of elf type
 */
const char* getStrType(int32_t code) {
    switch ( code ) {
        case ET_NONE:
            return "unknow";
        case ET_REL:
            return "relocatable";
        case ET_EXEC:
            return "executable";
        case ET_DYN:
            return "shared object";
        case ET_CORE:
            return "core";
        case ET_LOPROC:
            return "processor specific";
        case ET_HIPROC:
            return "processor specific";
    }
}

/*
 * return a printable string of elf compile architecture
 */
const char* getStrArch(int32_t code) {
    switch ( code ) {
        case EM_NONE:
            return "unknow";
        case EM_M32:
            return "AT&T WE 32100";
        case EM_SPARC:
            return "SPARC";
        case EM_386:
            return "Intel 386";
        case EM_68K:
            return "Motorola 68000";
        case EM_88K:
            return "Motorola 88000";
        case EM_860:
            return "Intel 80860";
        case EM_MIPS:
            return "MIPS";
    }
}

/*
 * return a printable string of elf ident
 */
const char* getStrIdent(int32_t code) {
    switch ( code ) {
        case ELFCLASSNONE:
            return "unknow";
        case ELFCLASS32:
            return "32";
        case ELFCLASS64:
            return "64";
    }
}

/*
 * return a printable string of elf format
 */
const char* getStrFormat(int32_t code) {
    switch ( code ) {
        case ELFDATANONE:
            return "unknow";
        case ELFDATA2LSB:
            return "little endian";
        case ELFDATA2MSB:
            return "big endian";
    }
}

/*
 * return a printable string of elf version
 */
const char* getStrVersion(int32_t code) {
    switch ( code ) {
        case EV_NONE:
            return "unknow";
        case EV_CURRENT:
            return "current";
    }
}

/*
 * try to read the file as elf
 */
bool tryElf(FileHandle  file,
            const char* elftype,
            const char* arch,
            const char* ident,
            const char* format,
            const char* version) {
    // create buffer for elf
    Elf32Ehdr elfhead;

    // try read elf header
    if ( readElfHeader(file, (uint8_t*)&elfhead) ) {
        // Valid ELF header
        if ( (elfhead.e_ident[EI_MAG0] == ELFMAG0) || (elfhead.e_ident[EI_MAG1] == ELFMAG1)
             || (elfhead.e_ident[EI_MAG2] == ELFMAG2) || (elfhead.e_ident[EI_MAG3] == ELFMAG3) ) {
            // fill buffers
            elftype = getStrType(elfhead.e_type);
            arch    = getStrArch(elfhead.e_machine);
            ident   = getStrIdent(elfhead.e_ident[EI_CLASS]);
            format  = getStrFormat(elfhead.e_ident[EI_DATA]);
            version = getStrVersion(elfhead.e_version);
        }

        // file is not elf
        else
            return false;
    }

    // unable to read the elf
    else
        return false;
}

/*
 * print the file with specification
 */
void scanFile(FileHandle file) {
    // try to read as elf file
    {
        char* elftype = NULL;
        char* arch    = NULL;
        char* ident   = NULL;
        char* format  = NULL;
        char* version = NULL;

        // try read
        if ( tryElf(file, elftype, arch, ident, format, version) )
            println("Elf file: type %s, compiled for %s %s bit, with format %s, version %s",
                    elftype,
                    arch,
                    ident,
                    format,
                    version);

        return;
    }

    // try read as script file
    {
// each script must begin with this string + the name of the interpreter
#define SCRIPT_BEGIN "//>"

        // check for javascript
        { // get javascript interpreter dir
          const char* jsDir = getenv("JS");

    // create a compare buffer with the content that we should find at the begin of the file
    char jsBegin[strlen(SCRIPT_BEGIN) + strlen(jsDir) + 1];
    strcpy(jsBegin, SCRIPT_BEGIN);
    strcat(jsBegin, jsDir);

    // create a buffer to read the first line of file
    uint8_t buffer[strlen(SCRIPT_BEGIN) + strlen(jsDir)];

    // read from file
    s_read(file, buffer, strlen(buffer));

    // print result
    if ( !strcmp(buffer, jsBegin) )
        println("Script file: applicative script, interpreter %s", jsDir);

    return;
}

// check for shell script
{
    // get shell interpreter dir
    const char* mxDir = getenv("SHELL");

    // create a compare buffer with the content that we should find at the begin of the file
    char mxBegin[strlen(SCRIPT_BEGIN) + strlen(mxDir) + 1];
    strcpy(mxBegin, SCRIPT_BEGIN);
    strcat(mxBegin, mxDir);

    // create a buffer to read the first line of file
    uint8_t buffer[strlen(SCRIPT_BEGIN) + strlen(mxDir)];

    // read from file
    s_read(file, buffer, strlen(buffer));

    // print result
    if ( !strcmp(buffer, mxBegin) )
        println("Script file: shell script, interpreter %s", mxDir);
}
}

// try read as ACII text file
{
    // boolean flags
    bool asciitext = true;
    bool utf8text  = true;

    // get first the file length
    uint64_t flength = s_length(file);

    // create buffer
    uint8_t buffer[flength / 2];

    // we read only the half of file, is enougth to undestand if is a text file
    int32_t read = s_read(file, buffer, flength / 2);

    // read file
    for ( int index = 0; index < flength; ++index ) {
        // if it isn't ascii try utf8
        if ( !isascii(buffer[index]) ) {
            asciitext = false;

            // if it isn't neither an utf8 text the file is an unknow binary type
            if ( !isutf8(buffer[index]) ) {
                utf8text = false;
                break;
            }
        }
    }

    // show as ascii or utf8
    if ( asciitext )
        println("ASCII text file, size: %d", flength);
    if ( utf8text )
        println("Utf-8 text file, size: %d", flength);

    return;
}

// unknow file type
println("Unknow binary file");
}

/**
 * file specificator utility
 */
int main(int argc, char* argv[]) {
    // flag for help option
    bool showHelp = false;

    // path to file
    char* path = NULL;

    // search for help
    if ( getopt_is_help(argc, argv) )
        showHelp = true;

    // if we haven't to show the help
    if ( !showHelp ) {
        // search for file
        for ( int arg = 0; arg < argc; ++arg )
            if ( argv[arg][0] != '-' )
                path = argv[arg];

        // check for path
        if ( path ) {
            // open the file
            FileHandle file = FD_NONE;
            if ( (file = s_open_f(path, O_RDONLY)) != FD_NONE ) {
                // parse the file and print on screen the properties
                scanFile(file);

                // always close the file after usage
                s_close(file);
            } else
                fprintf(stderr, "unable to open the file %s", path);
        }

        else
            fprintf(stderr, "no path or filename provided");
    }

    // show the usage for the command
    else
        usage(argv[0]);

    return 0;
}
