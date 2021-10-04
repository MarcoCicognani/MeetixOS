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
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/*
 * return the string type of node
 */
const char* getStrNodeType(FsNodeType type) {
    if ( type == FS_NODE_TYPE_NONE )
        return "none";
    if ( type == FS_NODE_TYPE_ROOT )
        return "root";
    if ( type == FS_NODE_TYPE_MOUNTPOINT )
        return "mountpoint";
    if ( type == FS_NODE_TYPE_FOLDER )
        return "folder";
    if ( type == FS_NODE_TYPE_FILE )
        return "file";
    if ( type == FS_NODE_TYPE_PIPE )
        return "pipe";
}

/*
 * return the compact symbol of node
 */
const char* getNodeTypeSymbol(FsNodeType type) {
    if ( type == FS_NODE_TYPE_NONE )
        return " ";
    if ( type == FS_NODE_TYPE_ROOT )
        return "rt";
    if ( type == FS_NODE_TYPE_MOUNTPOINT )
        return "mnt";
    if ( type == FS_NODE_TYPE_FOLDER )
        return ">";
    if ( type == FS_NODE_TYPE_FILE )
        return " ";
    if ( type == FS_NODE_TYPE_PIPE )
        return " ";
}

/*
 * print the provided node on screen with provided mode
 */
void printNode(FsDirectoryEntry* node, bool all, bool list) {
    // if we haven't permission
    if ( node->m_name[0] == '.' && !all )
        return;

    // print on screen
    if ( list ) {
        // check if node is a file
        if ( node->m_node_type == FS_NODE_TYPE_FILE ) {
            // open the file
            FileHandle file = s_open_f(node->m_name, O_RDONLY);
            uint64_t   flen = s_length(file);
            println("%-5d %3d %7s %s",
                    node->m_node_id,
                    flen,
                    getStrNodeType(node->m_node_type),
                    node->m_name);
        } else
            println("%-5d %7s %s",
                    node->m_node_id,
                    getStrNodeType(node->m_node_type),
                    node->m_name);
    }

    else {
        // check if node is a file
        if ( node->m_node_type == FS_NODE_TYPE_FILE ) {
            // open the file
            FileHandle file = s_open_f(node->m_name, O_RDONLY);
            uint64_t   flen = s_length(file);
            println("%3s %10s %d", getNodeTypeSymbol(node->m_node_type), node->m_name, flen);
        }

        else
            println("%3s %10s", getNodeTypeSymbol(node->m_node_type), node->m_name);
    }
}

/*
 * print the help
 */
void show_usages(const char* cmdname) {
    println("");
    println("Filesystem explorer command utility");
    println("usage: %s [filter] [path]", cmdname);
    println("available filters:");
    println("\t-l                [show an ordered list of entryes on provided path]", cmdname);
    println("\t-a                [show the entryes and hidden nodes]", cmdname);
    println("\t-h                [show this help]", cmdname);
    println("");
}

/*
 * ls utils command
 */
int main(int argc, const char* argv[]) {
    // create mode flags
    bool showHelp = false;
    bool showAll  = false;
    bool showList = false;
    bool showBase = true;

    // flag for scandir
    bool current = true;
    int  index;

    // parse filter arguments
    char opt;
    while ( (opt = getopt(argc, argv, "alh")) != EOF ) {
        switch ( opt ) {
            case 'a':
                showAll  = true;
                showBase = false;
                break;
            case 'l':
                showList = true;
                break;
            case 'h':
                showHelp = true;
                break;
        }
    }

    // if we haven't to show the help
    if ( !showHelp ) {
        // create buffer
        char scandir[PATH_MAX];

        // find path if exist
        for ( int arg = 1; arg < argc; ++arg ) {
            // if current arg have no filter assiming is a path
            if ( argv[arg][0] != '-' ) {
                current = false;
                index   = arg;
                break;
            }
        }

        // from parser copy on buffer the directory to scan
        if ( current )
            s_get_working_directory(scandir);
        else
            strcpy(scandir, argv[index]);

        // open the directory
        FsOpenDirectoryStatus stat;
        FsDirectoryIterator*  it = s_open_directory_s(scandir, &stat);

        // check for errors
        if ( stat != FS_OPEN_DIRECTORY_SUCCESSFUL ) {
            fprintf(stderr, "failed to open directory %s\n", scandir);
            return 1;
        }

        // read all nodes
        while ( true ) {
            // read each node
            FsReadDirectoryStatus stat;
            FsDirectoryEntry*     node = s_read_directory_s(it, &stat);

            // node readed succesful
            if ( stat == FS_READ_DIRECTORY_SUCCESSFUL ) {
                if ( node )
                    printNode(node, showAll, showList);
                else
                    break;
            }

            // reached the end of directory
            else if ( stat == FS_READ_DIRECTORY_EOD )
                break;
        }

        // free memory
        s_close_directory(it);
    }

    else
        show_usages(argv[0]);

    return 0;
}
