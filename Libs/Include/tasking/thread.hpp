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

#ifndef MEETIX_LIBRARY_TASKING_THREAD
#define MEETIX_LIBRARY_TASKING_THREAD

#include <Api.h>

/**
 * base class incpired by the thread class of java environment
 * permits to create a managed thread starting from a base class
 */
class Thread {
protected:
    Tid id; // the id of the created thread

private:
    /**
     * execute the provided thread instance
     *
     * @param thread:	the thread to be executed
     */
    static void execute(Thread* thread);

public:
    /**
     * virtual destructor
     */
    virtual ~Thread() {
    }

    /**
     * starts the thread defined with the body
     * on the run method and returns the id of the new thread
     *
     * @return the id of new thread
     */
    virtual Tid start();

    /**
     * interface method that must be redefined on the resultant class
     * this method must contains the body of the thread to be executed
     */
    virtual void run() = 0;
};

#endif
