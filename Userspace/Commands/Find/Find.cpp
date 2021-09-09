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

#include <eva.h>
#include <eva/utils/llist.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/**
 * show the help on screen
 */
void usage(const char* cmdname) {
    println("");
    println("Find command utility");
    println("usage: %s [filter] <filename>", cmdname);
    println("The following filters are available:");
    println("\t-r    [find recursive from current directory]");
    println("\t-h/-? [show this help]");
    println("");
}

/**
 * return node type as string
 */
const char* getStrNodeType(FsNodeType type) {
    switch ( type ) {
        case FS_NODE_TYPE_NONE:
            return "none";
        case FS_NODE_TYPE_ROOT:
            return "root";
        case FS_NODE_TYPE_MOUNTPOINT:
            return "mountpoint";
        case FS_NODE_TYPE_FOLDER:
            return "folder";
        case FS_NODE_TYPE_FILE:
            return "file";
        case FS_NODE_TYPE_PIPE:
            return "pipe";
    }
    return "none";
}

/**
 * manage the add to dir stack
 */
void addToStack(llist<char*>* dirstack, char* newentry) {
    // get the last path
    char* lastdir = dirstack->back();

    // create new buffer
    uint32_t newPathLength = strlen(lastdir) + strlen(newentry) + 1;
    char*    newPath       = new char[newPathLength];

    // concat last with newpath
    strcat(newPath, lastdir);
    strcat(newPath, newentry);

    // add to list
    dirstack->add(newPath);
}

/**
 * check with the provided iterator the filename, and return type of file
 */
bool findOn(char*         directory,
            char*         filename,
            bool          recursive,
            FsNodeType*   type,
            llist<char*>* dirstack) {
    // flag for return
    bool finded = false;

    // open the directory
    FsOpenDirectoryStatus stat;
    FsDirectoryIterator*  iterator = OpenDirectoryS(directory, &stat);
    if ( stat != FS_OPEN_DIRECTORY_SUCCESSFUL ) {
        fprintf(stderr, "failed to open the '%s' directory\n", directory);
        return finded;
    }

    // read all nodes
    while ( true ) {
        // read each node
        FsReadDirectoryStatus stat;
        FsDirectoryEntry*     node = ReadDirectoryS(iterator, &stat);

        // node readed succesful
        if ( stat == FS_READ_DIRECTORY_SUCCESSFUL ) {
            // always check for allocated node
            if ( node ) {
                // first check if the filename and the node name match
                if ( !strcmp(node->name, filename) ) {
                    *type  = node->type;
                    finded = true;
                    break;
                }

                // if we are in recursive mode, the filename and the node name not match
                // and the node is a directory add to directory stack
                if ( recursive && node->type == FS_NODE_TYPE_FOLDER ) {
                    addToStack(dirstack, node->name);
                    break;
                }
            } else
                break;
        }

        // reached the end of directory
        else if ( stat == FS_READ_DIRECTORY_EOD )
            break;
    }

    // always close directory
    CloseDirectory(iterator);

    return finded;
}

/**
 * find recursive
 */
bool findRecursive(char* firstdir, char* filename) {
    // create the dirstack
    llist<char*> dirstack;

    // add the first
    dirstack.add(firstdir);
    println("firstdir: '%s'", dirstack.back());

    // create flag for return
    bool finded = false;

    // find recursive
    FsNodeType type;
    char*      current = nullptr;
    while ( !(finded = findOn((current = dirstack.back()), filename, true, &type, &dirstack))
            && dirstack.count() > 1 ) {
        println("current: %s", current);
        dirstack.popBack();
        delete current;
    }

    // print if finded
    if ( finded )
        println("\tfound %s on %s [%s]", filename, current, getStrNodeType(type));
    else
        println("\t'%s' not found", filename);

    // return the result flag
    return finded;
}

/**
 * do the find starting from first directory provided with provided mode
 */
bool doFind(char* firstdir, char* filename, bool recursive) {
    if ( recursive )
        return findRecursive(firstdir, filename);
    else {
        FsNodeType type;
        if ( findOn(firstdir, filename, recursive, &type, nullptr) ) {
            println("\tfound '%s' on '%s' [%s]", filename, firstdir, getStrNodeType(type));
            return true;
        }

        return false;
    }
}

/**
 * simple find utility
 */
int main(int argc, const char* argv[]) {
    // create mode flags
    bool recursive = false;
    bool help      = false;

    // parse filter arguments
    char opt;
    while ( (opt = getopt(argc, argv, "rh?")) != EOF ) {
        switch ( opt ) {
            case 'r':
                recursive = true;
                break;
            case 'h':
                help = true;
                break;
            case '?':
                help = true;
                break;
        }
    }

    // if is requested the help we didn't find anything
    if ( !help ) {
        // poiter to path
        char* filename = nullptr;

        // get filename or path to search
        for ( int arg = 1; arg < argc; ++arg ) {
            // if the argument have no filter char assuming is the filename
            if ( argv[arg][0] != '-' ) {
                filename = (char*)argv[arg];
                break;
            }
        }

        // check for filename
        if ( filename ) {
            // get first the working directory
            char* curdir = new char[PATH_MAX];
            GetWorkingDirectory(curdir);

            // check if syscall successful
            if ( curdir ) {
                if ( doFind(curdir, filename, recursive) )
                    return 0;
                else {
                    fprintf(stderr, "unable to find '%s'\n", filename);
                    return 1;
                }
            }

            else {
                fprintf(stderr, "unable to get the working directory\n");
                return 1;
            }
        }

        else {
            fprintf(stderr, "no filename provided\n");
            return 1;
        }
    }

    else
        usage(argv[0]);

    return 0;
}
