/**
 * @brief
 * This file is part of the MeetiX Operating System.
 * Copyright (c) 2017-2021, Marco Cicognani (marco.cicognani@meetixos.org)
 *
 * @developers
 * Marco Cicognani (marco.cicognani@meetixos.org)
 *
 * @license
 * GNU General Public License version 3
 */

#include <Api.h>
#include <stdio.h>
#include <stdlib.h>

/* TODO find a better way to do this */

int setenv(const char* key, const char* val, A_UNUSED int overwrite) {
    char shell_cmd[CLIARGS_BUFFER_LENGTH];
    snprintf(shell_cmd, CLIARGS_BUFFER_LENGTH, "/Bins/MxSh --setenv %s=%s", key, val);

    /* execute the shell to set the environment */
    return system(shell_cmd);
}
