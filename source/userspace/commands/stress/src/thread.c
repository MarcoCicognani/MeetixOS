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
#include <pthread.h>

#include <tests.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>

// iterator for threads
uint32_t threadCount = 10;
uint8_t live = true;
uint8_t _verbose = false;

/**
 * routine executed by the MeetiX threads
 */
static void threadRoutine(void *arg)
{
    int count = *((int*)arg);
    Tid threadID = GetTid();
    if (_verbose) println("Hi from the MeetiX thread %i, count start from %i", threadID, count);
    while (live)
    {
        count++;
        if (_verbose) println("Hi from the MeetiX thread %i, count is %i", threadID, count);
        Yield();
    }

    if (_verbose) println("Bye bye from MeetiX thread %i", threadID);
}

/**
 * routine executed by the posix threads
 */
static void *pthreadRoutine(void *arg)
{
    int count = *((int*)arg);
    pthread_t *self = pthread_self();
    if (_verbose) println("Hi from the pthread %i, count start from %i", self->id, count);
    while (live)
    {
        count++;
        if (_verbose) println("Hi from the pthread %i, count is %i", self->id, count);
        sched_yield();
    }

    if (_verbose) println("Bye bye from pthread %i", self->id);

    int *intptr = malloc(sizeof(int));
    if (intptr) *intptr = count;
    return intptr;
}

/**
 * test the native implementation of the threads on the system
 */
int testThread(uint8_t verbose)
{
    Tid tids[threadCount];
    _verbose = verbose;
    for (int i = 0; i < threadCount; i++)
    {
        tids[i] = CreateThreadD((void*)threadRoutine, &i);
        if (_verbose) println("Created thread number %i with tid %i", i, tids[i]);
    }

    println("Created %i threads, waiting for 5 seconds until to kill all", threadCount);
    sleep(5);

    // after this instruction all threads when become on the while conditions breaks
    live = false;

    // wait for last termination
    for (int i = 0; i < threadCount; i++)
        Join(tids[i]);

    println("Bye bye");
}

/**
 * test the implementation of the Posix thread on the system
 */
int testPosixThread(uint8_t verbose)
{
    _verbose = verbose;
    pthread_t pthreads[threadCount];
    for (int i = 0; i < threadCount; i++)
    {
        pthread_create(&pthreads[i], NULL, pthreadRoutine, &i);
        if (_verbose) println("Created pthread number %i with tid %i", i, pthreads[i].id);
    }

    println("Created %i threads, waiting for 5 seconds until to kill all", threadCount);
    sleep(5);

    // after this instruction all threads when become on the while conditions breaks
    live = false;

    // wait for last termination
    for (int i = 0; i < threadCount; i++)
        pthread_join(&pthreads[i], NULL);

    // show returns values
    if (_verbose)
        for (int i = 0; i < threadCount; i++)
            if (pthreads[i].ret_val)
                println("Pthread number %i, with tid %i have the return value %i", i, pthreads[i].id, *((int*)pthreads[i].ret_val));

    println("Bye bye");
}
