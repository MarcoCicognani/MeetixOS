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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* getenv(const char* key) {
    FILE* env_stream = fopen("/MeetiX/Configs/Env/Global", "r");

    if ( env_stream ) {
        usize env_buf_len = 1024;
        usize env_key_len = strlen(key);
        char* env_buf     = malloc(sizeof(char) * env_buf_len);

        /* read file line by line */
        while ( readline(env_stream, env_buf, env_buf_len) ) {
            char* env_var_separator = strchr(env_buf, '=');

            /* is this the key? */
            if ( env_key_len == (usize)(env_var_separator - env_buf)
                 && !strncmp(key, env_buf, env_key_len) ) {
                fclose(env_stream);

                /* skip the separator */
                return ++env_var_separator;
            }

            memset(env_buf, '\0', env_buf_len);
        }

        /* drop the stream */
        fclose(env_stream);
    }
    return NULL;
}
