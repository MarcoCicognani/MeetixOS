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

#include <fstream>
#include <iostream>
#include <string>
#include <Utils/ArgsParser.hh>
#include <vector>

#define V_MAJOR 0
#define V_MINOR 0
#define V_PATCH 1

/*
 * show the help
 */
void show_usages(const char* cmdname) {
    println("");
    println("%s command utility", cmdname);
    println("usage: %s [opt] <filename> <buffer to write>", cmdname);
    println("avible options:");
    println("\t-a    [append to provided filename, file must exist]");
    println("\t-h/-? [show this help]");
    println("");
}

/**
 * write command utility
 */
int main(int argc, const char* argv[]) {
    // create flags
    bool showHelp = false;
    bool append   = false;

    // get options
    char opt;
    while ( (opt = getopt(argc, argv, "ah?")) != EOF ) {
        switch ( opt ) {
            case 'a':
                append = true;
                break;
            case 'h':
                showHelp = true;
                break;
            case '?':
                showHelp = true;
                break;
        }
    }

    // if we haven't to show the help
    if ( !showHelp ) {
        // get filepath where write
        char* path = NULL;
        for ( int arg = 0; arg < argc; ++arg )
            if ( argv[arg][0] != '-' ) {
                path = (char*)argv[arg];
                break;
            }

        // check argument
        if ( path ) {
            // open the file to write
            FILE* file;
            if ( (file = fopen(path, append ? "a" : "w")) ) {
                // create a buffer
                uint32_t bufsize = 4096;
                uint8_t  buffer[bufsize];

                // write from file getting from stdinput
                ssize_t len;
                while ( (len = fread(buffer, 1, bufsize, stdin)) > 0 )
                    for ( ssize_t pos = 0; pos < len; pos++ )
                        putc(buffer[pos], file);

                fclose(file);
            }

            else {
                fprintf(stderr, "unable to open %s", path);
                return 1;
            }
        }

        else {
            fprintf(stderr, "no filename or path provided\n");
            return 1;
        }
    }

    else
        show_usages(argv[0]);

    return 0;
}
