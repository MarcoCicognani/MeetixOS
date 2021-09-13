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

#include "__internal.h"
#include "eva.h"
#include "eva/bytewise.h"

#include <eva/utils/local.hpp>
#include <malloc.h>

// redirect
SpawnStatus
Spawn(const char* path, const char* args, const char* workdir, SecurityLevel securityLevel) {
    return SpawnPOI(path, args, workdir, securityLevel, NULL, NULL, NULL);
}

// redirect
SpawnStatus SpawnP(const char*   path,
                   const char*   args,
                   const char*   workdir,
                   SecurityLevel securityLevel,
                   Pid*          pid) {
    return SpawnPOI(path, args, workdir, securityLevel, pid, NULL, NULL);
}

// redirect
SpawnStatus SpawnPO(const char*   path,
                    const char*   args,
                    const char*   workdir,
                    SecurityLevel securityLevel,
                    Pid*          pid,
                    File_t        outStdio[3]) {
    return SpawnPOI(path, args, workdir, securityLevel, pid, outStdio, NULL);
}

/**
 * Spawns a program binary.
 *
 * @param path:             absolute path of the executable
 * @param args:             unparsed arguments
 * @param workdir:          working directory for the execution
 * @param securityLevel:    security level to spawn the process to
 * @param pid:          	is filled with the process id
 * @param outStdio:     	is filled with stdio file descriptors, 0 is write end of stdin, 1 is
 * read end of stdout, 2 is read end of stderr
 * @param inStdio:      	if supplied, the given descriptors which are valid for the executing
 * process are used as the stdin/out/err for the spawned process; an entry might be -1 to be ignored
 * and default behaviour being applied
 * @return one of the {SpawnStatus} codes
 *
 * @security-level APPLICATION
 */
SpawnStatus SpawnPOI(const char*   path,
                     const char*   args,
                     const char*   workdir,
                     SecurityLevel securityLevel,
                     Pid*          pid,
                     File_t        outStdio[3],
                     File_t        inStdio[3]) {
    SpawnStatus res = SPAWN_STATUS_UNKNOWN;

    // get spawner task identifier
    Tid spawnerTid = TaskGetID(SPAWNER_IDENTIFIER);
    if ( spawnerTid == -1 )
        return res;

    // create transaction
    MessageTransaction tx = GetMessageTxId();

    // create request
    size_t   pathBytes    = strlength(path) + 1;
    size_t   argsBytes    = strlength(args) + 1;
    size_t   workdirBytes = strlength(workdir) + 1;
    size_t   requestlen   = sizeof(SpawnCommandSpawnRequest) + pathBytes + argsBytes + workdirBytes;
    uint8_t* request      = malloc(sizeof(uint8_t) * requestlen);

    // copy request contents
    SpawnCommandSpawnRequest* req = (SpawnCommandSpawnRequest*)request;
    req->header.command           = SPAWN_COMMAND_SPAWN_REQUEST;
    req->securityLevel            = securityLevel;
    req->pathBytes                = pathBytes;
    req->argsBytes                = argsBytes;
    req->workdirBytes             = workdirBytes;

    if ( inStdio != NULL ) {
        req->stdin  = inStdio[0];
        req->stdout = inStdio[1];
        req->stderr = inStdio[2];
    }

    else {
        req->stdin  = FD_NONE;
        req->stdout = FD_NONE;
        req->stderr = FD_NONE;
    }

    uint8_t* insert = request;
    insert          = insert + sizeof(SpawnCommandSpawnRequest);
    memcopy(insert, path, pathBytes);
    insert = insert + pathBytes;
    memcopy(insert, args, argsBytes);
    insert = insert + argsBytes;
    memcopy(insert, workdir, workdirBytes);

    // send request to spawner
    SendMessageT(spawnerTid, request, requestlen, tx);

    // receive response
    size_t   respLen = sizeof(MessageHeader) + sizeof(SpawnCommandSpawnResponse);
    uint8_t* respBuf = malloc(sizeof(uint8_t) * respLen);
    ReceiveMessageT(respBuf, respLen, tx);

    SpawnCommandSpawnResponse* response = (SpawnCommandSpawnResponse*)MESSAGE_CONTENT(respBuf);

    // if successful, take response parameters
    if ( response->status == SPAWN_STATUS_SUCCESSFUL ) {
        if ( pid != NULL )
            *pid = response->spawnedProcessID;
        if ( outStdio != NULL ) {
            outStdio[0] = response->stdinWrite;
            outStdio[1] = response->stdoutRead;
            outStdio[2] = response->stderrRead;
        }
    }

    // free the buffer
    free(request);
    return response->status;
}
