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

#ifndef POSIX_THREAD_H
#define POSIX_THREAD_H

#include "stdint.h"

__BEGIN_C

/**
* Definition of the posix thread attribute
*/
typedef uint32_t pthread_attr_t;
#define PTHREAD_CREATE_DETACHED 0
#define PTHREAD_CREATE_JOINABLE 1

/**
 * Definition of the posix thread type
 */
typedef struct
{
    Tid id;                // id of the thread
    void *ret_val;         // return value of the thread
    pthread_attr_t attr;   // the setupped attribute for the thread creation
} pthread_t;


/**
 * Initializes a posix thread attribute provided by pointer
 *
 * @param attr:     the posix thread attribute which be initialized
 * @return 0 on success, non zero value on error
 */
int pthread_attr_init(pthread_attr_t *attr);

/**
 * Set the detach state of a posix thread attribute previously initialized
 *
 * @param attr:             the attribute of the thread to be setted
 * @param detachstate:      the state to set
 * @return 0 on success, non zero value on error
 */
int pthread_set_detachstate(pthread_attr_t *attr, int detachstate);

/**
 * Destroy a posix attribute instance provided by pointer
 *
 * @param attr;     the posix thread attribute which be destroyed
 * @return 0 on success, non zero value on error
 */
int pthread_attr_destroy(pthread_attr_t *attr);

/**
 * Creates a new posix thread from the provided parameters and execute the <start_routine> argument
 *
 * @param thread:           a valid posix thread descriptor buffer
 * @param attr:             pointer to a valid posix thread attribute for thread creation
 * @param start_routine:    the pointer to the function which the thread have to execute
 * @param arg:              the argument for the start_routine
 * @return 0 on success, non zero value on error
 */
int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine)(void*), void *arg);

/**
 * This function waits for the thread specified by <thread>
 * to terminate. The thread specified by thread must be joinable.
 *
 * @param thread:       the thread which be joined
 * @param retval:       the return value of the thread
 * @return 0 on success, non zero value on error
 */
int pthread_join(pthread_t *thread, void **retval);

/**
 * Returns the descriptor of the current thread
 */
pthread_t *pthread_self(void);

/**
 * Kills the thread identified by the descriptor <thread> with the signal <sig>
 *
 * @param thread:       the descriptor of the thread to kill
 * @param sig:          the signal which send to the thread
 * @return 0 on success, non zero value on error
 */
int pthread_kill(pthread_t *thread, int sig);

/**
 * This function is callable only into a thread context,
 * performs a nice cleanup of the thread and terminates
 * the thread which have called the function
 *
 * @param value:    the return value of the thread
 */
void pthread_exit(void *value);

__END_C

#endif
