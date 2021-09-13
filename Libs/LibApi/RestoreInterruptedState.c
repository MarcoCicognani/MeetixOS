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
 * The address of this function is inserted as the return address for signal & irq handlers.
 * It does nothing but calling the <RestoreInterruptedState> function.
 */
void __RestoreInterruptedStateCallback() {
    RestoreInterruptedState();
    __builtin_unreachable();
}

/**
 * Restores the interruption state (for example after signal/irq handling) of the current thread.
 */
void RestoreInterruptedState() {
    syscall(SYSCALL_RESTORE_INTERRUPTED_STATE, 0);
}
