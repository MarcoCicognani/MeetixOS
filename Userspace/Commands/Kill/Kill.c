/*********************************************************************************
 * MeetiX OS By MeetiX OS Project [Marco Cicognani]                               *
 * 																			     *
 * This program is free software; you can redistribute it and/or                  *
 * modify it under the terms of the GNU General Public License                    *
 * as published by the Free Software Foundation; either version 2				 *
 * of the License, or (char *argumentat your option) any later version.			 *
 *																				 *
 * This program is distributed in the hope that it will be useful,				 *
 * but WITHout ANY WARRANTY; without even the implied warranty of                 *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 				 *
 * GNU General Public License for more details.
 **
 *																				 *
 * You should have received a copy of the GNU General Public License				 *
 * along with this program; if not, write to the Free Software                    *
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA *
 **********************************************************************************/

#include <eva.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/*
 * show the help
 */
void usage(const char* cmdname) {
    println("");
    println("Kill command utility");
    println("");
    println("usage: %s [filter] <task-id>");
    println("avaible filters");
    println("\t-r    [restart the killed task, only if it is a process]");
    println("\t-h/-? [show this help]");
    println("");
}

/**
 * kill command utility
 */
int main(int argc, const char* argv[]) {
    // create flag for help
    bool showHelp = false;
    bool restart  = false;

    // check for args
    char opt;
    while ( (opt = getopt(argc, argv, "rh?")) != EOF ) {
        switch ( opt ) {
            case 'r':
                restart = true;
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
        // create target pid
        Pid target;

        // check the tid to kill
        for ( int arg = 0; arg < argc; ++arg )
            if ( argv[arg][0] != '-' )
                target = atoi(argv[arg]);

        // only if conversion is good
        if ( target ) {
            // create a string for output
            char tasktype[8];
            char procname[512];
            procname[0] = '\0';

            // check if the target is a process or a Thread
            if ( GetPidForTid(target) == target ) {
                strcpy(tasktype, "Process");

                // only if we have a process we can restart it
                if ( restart )
                    TaskGetIdentifier(target, procname);
            } else
                strcpy(tasktype, "Thread");

            // call kernel to kill the target
            switch ( Kill(target) ) {
                case KILL_STATUS_SUCCESSFUL:
                    {
                        println("%s %d successfully killed", tasktype, target);
                        if ( restart && procname[0] ) {
                            Pid pid;
                            SpawnP(procname, "", "/", SECURITY_LEVEL_APPLICATION, &pid);
                            println("restarted process %s with Pid %d", procname, pid);
                        } else if ( restart && !procname[0] )
                            fprintf(stderr, "unable to restart a Thread\n");
                    }
                    break;

                case KILL_STATUS_NOT_FOUND:
                    fprintf(stderr, "%s %d does not exist\n", tasktype, target);
                    break;
                default:
                    fprintf(stderr, "Please supply a valid task id.\n");
                    break;
            }
        }
    }

    else
        usage(argv[0]);

    return 0;
}
