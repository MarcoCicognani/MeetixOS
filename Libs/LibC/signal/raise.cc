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
#include <errno.h>
#include <signal.h>

extern "C" int raise(int sig) {
    auto raise_status = s_raise_signal(s_get_tid(), sig);
    if ( raise_status == RAISE_SIGNAL_STATUS_SUCCESSFUL ) {
        return 0;
    } else if ( raise_status == RAISE_SIGNAL_STATUS_INVALID_SIGNAL ) {
        errno = EINVAL;
        return -1;
    } else {
        errno = ENOENT;
        return -1;
    }
}
