/**
 * @brief
 * This file is part of the MeetiX Operating System.
 * Copyright (c) 2017-2022, Marco Cicognani (marco.cicognani@meetixos.org)
 *
 * @developers
 * Marco Cicognani (marco.cicognani@meetixos.org)
 *
 * @license
 * GNU General Public License version 3
 */

#include <Api/User.h>
#include <errno.h>
#include <signal.h>
#include <stdlib.h>

extern "C" {

void sig_handler_SIG_INT(int) {
    exit(0);
}

void sig_handler_SIG_IGN(int) {
}

sig_handler_t signal(int sig, sig_handler_t sig_handler) {
    if ( sig_handler == SIG_DFL ) {
        if ( sig == SIGINT )
            sig_handler = sig_handler_SIG_INT;
    } else if ( sig_handler == SIG_IGN )
        sig_handler = sig_handler_SIG_IGN;

    return reinterpret_cast<sig_handler_t>(s_register_signal_handler(sig, reinterpret_cast<void*>(sig_handler)));
}

int raise(int sig) {
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
}