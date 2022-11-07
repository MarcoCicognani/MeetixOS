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

#ifndef EVA_FILESYSTEM_TRANSACTION_HANDLER_DISCOVERY_SET_CWD
#define EVA_FILESYSTEM_TRANSACTION_HANDLER_DISCOVERY_SET_CWD

#include "Api/utils/local.hpp"
#include "filesystem/FsDescriptors.hpp"
#include "filesystem/FsNode.hpp"
#include "filesystem/FsTransactionHandlerDiscovery.hpp"
#include "logger/logger.hpp"
#include "memory/contextual.hpp"

/**
 *
 */
class FsTransactionHandlerDiscoverySetCwd : public FsTransactionHandlerDiscovery {
public:
    Contextual<SyscallFsSetWorkingDirectory*> data;
    Thread*                                   unspawnedTarget;

    /**
     * @param unspawned_target
     * 		when an unspawned target is supplied, the working directory is set to
     * 		the unspawned target process instead of the waiting process.
     */
    FsTransactionHandlerDiscoverySetCwd(char*                                     absolutePathIn,
                                        Contextual<SyscallFsSetWorkingDirectory*> data,
                                        Thread*                                   unspawnedTarget)
        : FsTransactionHandlerDiscovery(absolutePathIn), data(data),
          unspawnedTarget(unspawnedTarget) {
    }

    /**
     *
     */
    virtual FsTransactionHandlerFinishStatus afterFinishTransaction(Thread* task) {
        if ( status == FS_DISCOVERY_SUCCESSFUL ) {
            if ( !(node->type == FS_NODE_TYPE_PIPE || node->type == FS_NODE_TYPE_FILE) ) {
                Local<char> nodeRealpath(new char[PATH_MAX]);
                FileSystem::getRealPathToNode(node, nodeRealpath());

                Thread* target = (unspawnedTarget != nullptr ? unspawnedTarget : task);
                StringUtils::copy(target->process->workingDirectory, nodeRealpath());
                data()->m_working_directory_status = SET_WORKING_DIRECTORY_SUCCESSFUL;
                logDebug("%! cwd of process %i is now '%s'",
                         "filesystem",
                         target->process->main->id,
                         target->process->workingDirectory);
            }

            else {
                data()->m_working_directory_status = SET_WORKING_DIRECTORY_NOT_A_FOLDER;
                logInfo("%! could not set current working directory to '%s', not a folder",
                        "filesystem",
                        data()->m_path);
            }

        }

        else if ( status == FS_DISCOVERY_NOT_FOUND ) {
            data()->m_working_directory_status = SET_WORKING_DIRECTORY_NOT_FOUND;
            logInfo("%! could not set current working directory to '%s', node not found",
                    "filesystem",
                    data()->m_path);
        }

        else if ( status == FS_DISCOVERY_ERROR ) {
            data()->m_working_directory_status = SET_WORKING_DIRECTORY_ERROR;
            logInfo("%! could not set current working directory to '%s', missing delegate on way",
                    "filesystem",
                    data()->m_path);
        }

        else if ( status == FS_DISCOVERY_BUSY ) {
            data()->m_working_directory_status = SET_WORKING_DIRECTORY_ERROR;
            logInfo("%! could not set current working directory to '%s', delegate busy",
                    "filesystem",
                    data()->m_path);
        }

        return FS_TRANSACTION_HANDLING_DONE;
    }
};

#endif
