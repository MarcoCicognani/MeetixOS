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

#ifndef __MEETIX_LIBC_SIGNAL__
#define __MEETIX_LIBC_SIGNAL__

#include <eva/common.h>
#include <eva/signal.h>

__BEGIN_C

// (N1548-7.14-2)
typedef __SIG_ATOMIC_TYPE__ sig_atomic_t;

// type of signal handler functions
typedef void (*sig_handler_t)(int);

// standard signal handlers
void sig_handler_SIG_IGN(int);

// (N1548-7.14-3)
#define SIG_IGN sig_handler_SIG_IGN // ignore handler
#define SIG_DFL 0                   // default action
#define SIG_ERR -1                  // error return value

/**
 * Defines a given signal handler for the signal <sig>. (N1548-7.14.1.1)
 *
 * @param sig:          signal number
 * @param handler:      either <SIG_DFL>, <SIG_IGN> or a custom signal handler
 * @return value of <handler> if successful, otherwise <SIG_ERR>
 */
sig_handler_t signal(int sig, sig_handler_t handler);

/**
 * Raises a signal. (N1548-7.14.2.1)
 *
 * @param sig:      signal number
 * @return zero if successful, otherwise non-zero
 */
int raise(int sig);

__END_C

#endif
