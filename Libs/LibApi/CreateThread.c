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
#include "eva/user.h"

/**
 * Thread setup routine, used by the thread creation call. Assumes that the created thread
 * has a valid "userEntry" - otherwise exits with error code -1.
 */
void ThreadSetupRoutine() {
    // perform the call to get the thread entry
    SyscallGetThreadEntry data;
    syscall(SYSCALL_GET_THREAD_ENTRY, (uint32_t)&data);

    // copy to local functor
    void (*userEntry)(void*) = (void(*)(void*))(data.userEntry);

    // functor valid? execute thread body
    if ( userEntry )
        userEntry(data.userData);

    // exits
    return __ExitThread();
}

/**
 * Creates an asynchronous thread added to the current process.
 *
 * @param function:         the thread function
 * @return the new thread's tid, -1 on error
 *
 * @security-level APPLICATION
 */
Tid CreateThread(void* function) {
    return CreateThreadD(function, NULL);
}

/**
 * Creates an asynchronous thread added to the current process.
 *
 * @param function:         the thread function
 * @param data: 	        the parameters for the thread function
 * @return the new thread's tid, -1 on error
 *
 * @security-level APPLICATION
 */
Tid CreateThreadD(void* function, void* data) {
    return CreateThreadDNS(function, data, NULL, NULL);
}

/**
 * Creates an asynchronous thread added to the current process.
 *
 * @param function:         the thread function
 * @param name:		        the name for the thread, the name is appended to the default name
 * created by the kernel
 * @return the new thread's tid, -1 on error
 *
 * @security-level APPLICATION
 */
Tid CreateThreadN(void* function, const char* name) {
    return CreateThreadDNS(function, NULL, name, NULL);
}

/**
 * Creates an asynchronous thread added to the current process.
 *
 * @param function:         the thread function
 * @param data:         	the parameters for the thread function
 * @param name:         	the name for the thread, the name is appended to the default name
 * created by the kernel
 * @return the new thread's tid, -1 on error
 *
 * @security-level APPLICATION
 */
Tid CreateThreadDN(void* function, void* data, const char* name) {
    return CreateThreadDNS(function, data, name, NULL);
}

/**
 * Creates an asynchronous thread added to the current process.
 *
 * @param function:         the thread function
 * @param data:         	the parameters for the thread function
 * @param outStatus:    	the filled out status
 * @return the new thread's tid, -1 on error
 *
 * @security-level APPLICATION
 */
Tid CreateThreadDS(void* function, void* data, CreateThreadStatus* outStatus) {
    return CreateThreadDNS(function, data, NULL, outStatus);
}

/**
 * Creates an asynchronous thread added to the current process.
 *
 * @param function:         the thread function
 * @param data:         	the parameters for the thread function
 * @param name:         	the name for the thread, the name is appended to the default name
 * created by the kernel
 * @param outStatus:    	the filled out status
 * @return the new thread's tid, -1 on error
 *
 * @security-level APPLICATION
 */
Tid CreateThreadDNS(void* function, void* data, const char* name, CreateThreadStatus* outStatus) {
    // prepare data
    SyscallCreateThread callData;
    callData.initialEntry = (void*)ThreadSetupRoutine;
    callData.userEntry    = function;
    callData.userData     = data;
    callData.threadName   = (char*)name;

    // performs call
    syscall(SYSCALL_CREATE_THREAD, (uint32_t)&callData);

    // fill and return status
    if ( outStatus )
        *outStatus = callData.status;
    return callData.threadID;
}
