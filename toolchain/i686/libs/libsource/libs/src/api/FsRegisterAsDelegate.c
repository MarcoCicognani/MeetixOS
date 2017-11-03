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
* GNU General Public License for more details.									 *
*																				 *
* You should have received a copy of the GNU General Public License				 *
* along with this program; if not, write to the Free Software                    *
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA *
**********************************************************************************/

#include "eva/user.h"

/**
 * Creates a mountpoint and registers the current thread as its file system delegate.
 *
 * @param name:                     the wanted name
 * @param physMountpointID:         the physical id to set for the mountpoint
 * @param outMountpointID:          is filled with the node id of the mountpoint on success
 * @param outTransactionStorage:    is filled with the address of the transaction storage
 * @return one of the {FsRegisterAsDelegateStatus} codes
 *
 * @security-level DRIVER
 */
FsRegisterAsDelegateStatus FsRegisterAsDelegate(const char *name, FsPhysID physMountpointID, FsVirtID *outMountpointID, Address *outTransactionStorage)
{
	// prepare data
	SyscallFsRegisterAsDelegate data;
	data.name = (char*) name;
	data.physMountpointID = physMountpointID;

	// performs the call
	syscall(SYSCALL_FS_REGISTER_AS_DELEGATE, (uint32_t) &data);

	// fills returns
	*outMountpointID = data.mountpointID;
	*outTransactionStorage = data.transactionStorage;
	return data.result;
}
