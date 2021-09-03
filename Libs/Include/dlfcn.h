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

#ifndef __MEETIX_LIBC_DLFCN__
#define __MEETIX_LIBC_DLFCN__

/**
 * flags definitions
 */
#define RTLD_LAZY 0     // Perform lazy binding. Only resolve symbols as the code that references them is executed
#define RTLD_NOW  1     // all undefined symbols in the library are resolved before dlopen() returns

/**
 * load a dynamic linked library
 *
 * @param path:     the library path
 * @param flag:     one of the RTLD value
 * @return an opaque "handle" for the dynamic library
 */
extern void *dlopen(const char *path, int flag);

/**
 * unload a dynamic linked library previously loaded by a dlopen function
 *
 * @param handler:      the handler of the library that the function have to close
 * @return 0 on success, non zero code otherwise
 */
extern int dlclose(void *handler);

/**
 * get the address of the provided symbol into the library handler
 *
 * @param handler:      the library handler returned by dlopen
 * @param sysmbol:      the symbol that we want from the linked library
 * @return the address of the symbol if exists, NULL otherwise
 */
extern void *dlsym(void *handler, const char *symbol);

/**
 * @return the last linker error
 */
extern char *dlerror();

#endif
