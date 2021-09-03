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

#ifndef __MEETIX_LIBC_UNISTD__
#define __MEETIX_LIBC_UNISTD__

#include <eva/common.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>

__BEGIN_C

extern char** environ;
extern char*  optarg;

// constants for use with <access>
#define F_OK 0 // file existence
#define X_OK 1 // execute or search permission
#define W_OK 2 // write permission
#define R_OK 3 // read permission

// constants for getopt function family
#define no_argument       0
#define required_argument 1
#define optional_argument 2 /* unsupported */

/**
 * Describes a long option
 */
struct option {
    const char* name;
    int         has_arg;
    int*        flag; /* unsupported */
    int         val;
};

/**
 * Points to the argument of an option or is NULL if there is none
 */
extern char* optarg;
/**
 * The next index in argv that is considered
 */
extern int optind;
/**
 * Whether an error message should be printed to stderr
 */
extern int opterr;
/**
 * The character that was considered in case '?' is returned
 */
extern int optopt;

int chdir(const char* path);

/**
 * POSIX wrapper for <Read>
 */
ssize_t read(int fd, void* buf, size_t count);

/**
 * POSIX wrapper for <Write>
 */
ssize_t write(int fd, const void* buf, size_t count);

/**
 * POSIX wrapper for <Seek>
 */
off_t lseek(int fd, off_t offset, int whence);

/**
 * POSIX wrapper for <Tell>
 */
long int tell(int filedes);

/**
 * POSIX wrapper for <Close>
 */
int close(int filedes);

/**
 * POSIX wrapper for <Sbrk>
 */
void* sbrk(intptr_t increment);

/**
 * POSIX wrapper for <GetTid>
 */
pid_t getpid();

/**
 * POSIX wrapper for <GetPid>
 */
pid_t getppid();

/**
 * POSIX wrapper for <Sleep>, but with seconds instead of milliseconds
 */
unsigned sleep(unsigned seconds);

/**
 * POSIX wrapper for <GetWorkingDirectory>
 */
char* getcwd(char* buf, size_t size);

/**
 * Parses the given arguments for given options.
 *
 * <optstring> is used for short options ("-o") and consists of the option characters. A ':' after
 * an option character specifies that the option has an argument.
 * If an option is found, the character is returned. If it has an argument, <optarg> points to the
 * argument (either the text after the option character or the text of the next item in argv).
 *
 * <longopts> can optionally be specified for long options ("--option"). The last array element
 * needs to consist of zeros. <name> specifies the option name ("option"), <has_arg> specifies
 * whether it has an argument (*_argument) and <val> the value to return if that option is found.
 *
 * @param argc:			the number of args, as received in main
 * @param argv:			the arguments, as received in main
 * @param optstring:	the short options specification
 * @param longopts:		the long options specification
 * @param longindex:	if non-NULL, optind will be set to *longindex
 * @return the option or -1 if no option has been found
 */
int getoptlong(int            argc,
               const char*    argv[],
               const char*    optstring,
               struct option* longopts,
               int*           longindex);
int getopt(int argc, const char* argv[], const char* optstring);

/**
 * Converts the given string to a number, supporting the suffixes K,M, and G (lower and upper case)
 *
 * @param str:		the string
 * @return the number
 */
size_t getopttosize(const char* str);

/**
 * Checks whether the given arguments may be a kind of help-request. That means one of:
 * <prog> --help
 * <prog> -h
 * <prog> -?
 *
 * @param argc the number of arguments
 * @param argv the arguments
 * @return true if it is a help-request
 */
int getoptishelp(int argc, char* argv[]);

/**
 * TODO
 */
int isatty(int fd);
int access(const char* pathname, int mode);
int fcntl(int fildes, int cmd, ...);
int rmdir(const char* path);
int symlink(const char* path1, const char* path2);

__END_C

#endif
