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

#include <Api.h>
#include <sstream>
#include <tasking/timer.hpp>
#include <utility>

/**
 * constructor
 *
 * @param tm:			timing of the timer
 * @param function:		the body of the timed thread
 * @param-opt begin:	start execution at instantiation
 */
Timer::Timer(const TimeSpan& tm, Functor_t func, bool begin)
    : timedThreadID(-1), timing(tm.getTotTime()), function(std::move(func)), lock(false) {
    if ( begin )
        start();
}

/**
 * destructor
 */
Timer::~Timer() {
    lock = false;
    s_kill(timedThreadID);
}

/**
 * start the timed thread
 * to do only one time because this function
 * creates a new thread each time is called
 *
 * @return whether the operation success, return false if
 * 		   thread is already created or if there was a problem
 */
bool Timer::start() {
    // only if timer isn't already create
    if ( timedThreadID == -1 ) {
        // create the name for the timer
        std::stringstream name;
        name << s_get_tid() << "timer";

        // create the function body
        Functor_t body = [this] {
            while ( true ) {
                function();            // execute the function provided
                s_sleep(timing);       // sleep for the timing provided
                s_atomic_block(&lock); // lock if necessary
            }
        };

        // create the thread now
        timedThreadID = s_create_thread_n((void*)body.target<void*>(), name.str().c_str());

        // check success
        if ( timedThreadID != -1 )
            return true;
    }

    return false;
}

/**
 * stop the timed thread
 */
void Timer::stop() {
    lock = true;
}

/**
 * restart a previous stopped timer
 */
void Timer::restart() {
    lock = false;
}
