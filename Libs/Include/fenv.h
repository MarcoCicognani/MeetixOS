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

#ifndef __MEETIX_LIBC_FENV__
#define __MEETIX_LIBC_FENV__

// This header originates from the musl C library http://www.musl-libc.org/

#include <Api/Common.h>

__BEGIN_C

// Definitions for x86_64 platform
#if __x86_64__
#    error "not implemented for architecuter x86_64"

// Definitions for x86 platform
#elif __i386__

/**
 * floating-point exceptions
 */
#    define FE_INVALID   1  // Invalid argument exception
#    define __FE_DENORM  2  // Inexact result exception
#    define FE_DIVBYZERO 4  // Pole error exception
#    define FE_OVERFLOW  8  // Overflow range error exception
#    define FE_UNDERFLOW 16 // Underflow range error exception
#    define FE_INEXACT   32 // Inexact result exception

#    define FE_ALL_EXCEPT 63    // All exception

/**
 * floating-point rounding direction
 */
#    define FE_TONEAREST  0     // To-nearest rounding direction mode
#    define FE_DOWNWARD   0x400 // Downward rounding direction mode
#    define FE_UPWARD     0x800 // Upward rounding direction mode
#    define FE_TOWARDZERO 0xc00 // Toward-zero rounding direction mode

// creating floating env exception type
typedef unsigned short fexcept_t;

/**
 * define the floating environment type
 */
typedef struct {
    unsigned short __control_word;
    unsigned short __unused1;
    unsigned short __status_word;
    unsigned short __unused2;
    unsigned short __tags;
    unsigned short __unused3;
    unsigned int   __eip;
    unsigned short __cs_selector;
    unsigned int   __opcode  : 11;
    unsigned int   __unused4 : 5;
    unsigned int   __data_offset;
    unsigned short __data_selector;
    unsigned short __unused5;
} fenv_t;

/**
 * default floating-point environment
 */
#    define FE_DFL_ENV    ((const fenv_t*)-1)

// Other architectures are not supported
#else
#    error "current architecture not supported"

#endif

/**
 * Attempts to clear the floating-point exceptions specified by excepts
 *
 * @param excepts:	the exception to be cleared
 * @return 0 if excepts value is cleared, non-zero otherwise
 */
int feclearexcept(int excepts);

/**
 *  Attempts to store a representation of the floating-point exceptions specified by excepts into
 * the fexcept_t object pointed by flagp.
 *
 * @param flagp: 	Pointer to a fexcept_t object where the representation is stored
 * @param excepts:	bitmask value
 * @return 0 if representation is successful stored, non-zero value otherwise
 */
int fegetexceptflag(fexcept_t* flagp, int excepts);

/**
 * Attempts to raise the floating-point exceptions specified by excepts
 *
 * @param excepts:	the exception to be raised
 * @return 0 if excepts value is raised, non-zero otherwise
 */
int feraiseexcept(int);

/**
 * Attempts to set the exceptions indicated by excepts with the states stored in the object pointed
 * by flagp
 *
 * @param flagp: 	pointer to a fexcept_t object with a representation of floating-point
 * exceptions
 * @param excepts:	bitmask value
 * @return 0 if flag id successful set, non-zero value otherwise
 */
int fesetexceptflag(const fexcept_t* flagp, int excepts);

/**
 * Returns the exceptions currently set, among those specified by excepts.
 * The value returned is the bitwise OR representation of the subset of excepts that are currently
 * set in the floating point environment. Or zero, if none of the exceptions in excepts are
 * currently set. Programs calling this function shall ensure that pragma FENV_ACCESS is enabled for
 * the call.
 *
 * @param excepts:	the exception to test
 * @return 0 if none of the exceptions in excepts are set, non-zero otherwise
 */
int fetestexcept(int excepts);

/**
 * Returns a value that indicates the rounding direction mode, as specified in the current fenv.
 *
 * @return one of the floating-point rounding direction codes
 */
int fegetround(void);

/**
 * Sets rdir as the current rounding direction mode for the floating point environment.
 *
 * @param rdir:		one of the rounding direction codes
 * @return 0 if the requested rounding direction was successfully set, non-zero otherwise
 */
int fesetround(int rdir);

/**
 * Attempts to store the current state of the floating-point environment in the object pointed by
 * envp
 *
 * @param envp:		    Pointer to a fenv_t object where the state of the floating-point
 * environment is stored.
 * @return 0 if state was successfully stored, non-zero otherwise
 */
int fegetenv(fenv_t* envp);

/**
 * Saves the current state of the floating-point environment in the object pointed by envp.
 * It then resets the current state and -if supported- puts the environment in non-stop mode.
 * The non-stop mode prevents floating-point exceptions from stopping the normal flow of the program
 * when raised (with traps or abortions).
 *
 * @param envp:    Pointer to a fenv_t object where the state of the floating-point environment is
 * stored.
 * @return 0 if function completed successfully, non-zero otherwise
 */
int feholdexcept(fenv_t* envp);

/**
 * Attempts to establish the state of the floating-point environment as represented by the object
 * pointed by envp. The floating point environment is a set of status flags and control modes
 * affecting floating-point calculations (including both floating-point exceptions and the rounding
 * direction mode). If successful, the function changes the current state of the floating-point
 * environment without actually raising the exceptions specified in such state.
 *
 * @param envp:		Either a pointer to a fenv_t value
 * @return 0 if the state was successfully established, non-zero otherwise
 */
int fesetenv(const fenv_t* envp);

/**
 * Attempts to establish the state of the floating-point environment as represented by the object
 * pointed by envp. It then attempts to raise the exceptions that were set in the floating-point
 * environment before the call.
 *
 * @param envp:		Either a pointer to a fenv_t value
 * @return 0 if successful, non-zero otherwise
 */
int feupdateenv(const fenv_t* envp);

__END_C

#endif
