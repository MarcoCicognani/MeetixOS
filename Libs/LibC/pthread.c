/*********************************************************************************
 * MeetiX OS By MeetiX OS Project [Marco Cicognani] *
 * 																			     *
 * This program is free software; you can redistribute it and/or * modify it
 *under the terms of the GNU General Public License                    * as
 *published by the Free Software Foundation; either version 2
 ** of the License, or (char *argumentat your option) any later version.
 **
 *																				 *
 * This program is distributed in the hope that it will be useful,
 ** but WITHout ANY WARRANTY; without even the implied warranty of *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU General Public License for more details.
 **
 *																				 *
 * You should have received a copy of the GNU General Public License
 ** along with this program; if not, write to the Free Software * Foundation,
 *Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA *
 **********************************************************************************/

#include <errno.h>
#include <eva.h>
#include <pthread.h>
#include <stdlib.h>

/**
 * struct container for thread_routine params
 */
typedef struct {
    void*      arg;
    pthread_t* thread;
    void* (*function_routine)(void*);
} pthread_routine_args_t;

/**
 * thread routine to manage a posix thread
 * compatible routine with return value
 */
static void thread_routine(pthread_routine_args_t* args) {
    args->thread->ret_val = args->function_routine(args->arg);
    free(args);
}

/**
 * Initializes a posix thread attribute provided by pointer
 *
 * @param attr:     the posix thread attribute which be initialized
 * @return 0 on success, non zero value on error
 */
int pthread_attr_init(pthread_attr_t* attr) {
    // if not exists create a new one
    if ( !attr )
        attr = malloc(sizeof(pthread_attr_t));
    if ( !attr )
        return ENOMEM;

    // set a value only if the pointer is valid
    *attr = PTHREAD_CREATE_JOINABLE;
    return 0;
}

/**
 * Set the detach state of a posix thread attribute previously initialized
 *
 * @param attr:             the attribute of the thread to be setted
 * @param detachstate:      the state to set
 * @return 0 on success, non zero value on error
 */
int pthread_set_detachstate(pthread_attr_t* attr, int detachstate) {
    if ( !attr || (detachstate >= 0 && detachstate <= 1) )
        return EINVAL;

    *attr = detachstate;
    return 0;
}

/**
 * Destroy a posix attribute instance provided by pointer
 *
 * @param attr;     the posix thread attribute which be destroyed
 * @return 0 on success, non zero value on error
 */
int pthread_attr_destroy(pthread_attr_t* attr) {
    if ( !attr )
        return EINVAL;

    free(attr);
    return 0;
}

/**
 * Creates a new posix thread from the provided parameters and execute the
 * <start_routine> argument
 *
 * @param thread:           a valid posix thread descriptor buffer
 * @param attr:             pointer to a valid posix thread attribute for thread
 * creation
 * @param start_routine:    the pointer to the function which the thread have to
 * execute
 * @param arg:              the argument for the start_routine
 * @return 0 on success, non zero value on error
 */
int pthread_create(pthread_t*            thread,
                   const pthread_attr_t* attr,
                   void* (*start_routine)(void*),
                   void* arg) {
    if ( !thread )
        return EINVAL;

    // create the thread arguments and check malloc validity
    pthread_routine_args_t* threadarguments = malloc(sizeof(pthread_routine_args_t));
    if ( threadarguments ) {
        threadarguments->function_routine = start_routine;
        threadarguments->thread           = thread;
        threadarguments->arg              = arg;

        CreateThreadStatus outStatus;
        thread->id = CreateThreadDS((void*)thread_routine, threadarguments, &outStatus);

        // copy the attribute value
        if ( attr )
            thread->attr = *attr;
        else
            thread->attr = PTHREAD_CREATE_JOINABLE;

        // check return status
        if ( outStatus != CREATE_THREAD_STATUS_SUCCESSFUL )
            return -1;
        return 0;
    }

    return ENOMEM;
}

/**
 * This function waits for the thread specified by <thread>
 * to terminate. The thread specified by thread must be joinable.
 *
 * @param thread:       the thread which be joined
 * @param retval:       the return value of the thread
 */
int pthread_join(pthread_t* thread, void** retval) {
    if ( !thread )
        return EINVAL;
    if ( thread->attr != PTHREAD_CREATE_JOINABLE )
        return -1;

    Join(thread->id);
    if ( *retval )
        *retval = thread->ret_val;

    return 0;
}

/**
 * Returns the descriptor of the current thread
 */
pthread_t* pthread_self(void) {
    pthread_t* descriptor = malloc(sizeof(pthread_t));
    if ( descriptor ) {
        descriptor->id = GetTid();
        return descriptor;
    }

    return NULL;
}

/**
 * Kills the thread identified by the descriptor <thread> with the signal <sig>
 *
 * @param thread:       the descriptor of the thread to kill
 * @param sig:          the signal which send to the thread
 * @return 0 on success, non zero value on error
 */
int pthread_kill(pthread_t* thread, int sig) {
    if ( !thread )
        return EINVAL;
    if ( Kill(thread->id) != KILL_STATUS_SUCCESSFUL )
        return -1;
    return 0;
}

/**
 * This function is callable only into a thread context,
 * performs a nice cleanup of the thread and terminates
 * the thread which have called the function
 *
 * @param value:    the return value of the thread
 */
void pthread_exit(void* value) {
    return;
}
