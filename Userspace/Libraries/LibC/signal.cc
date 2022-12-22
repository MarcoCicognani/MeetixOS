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

#pragma clang diagnostic push
#pragma ide diagnostic   ignored "modernize-use-trailing-return-type"

#include <LibApi/Api/User.h>
#include <LibC/errno.h>
#include <LibC/signal.h>
#include <LibC/stdlib.h>
#include <CCLang/Lang/Cxx.hh>

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

    auto const prev_sig_handler = s_register_signal_handler(sig, Cxx::bit_cast<void*>(&sig_handler));
    return Cxx::bit_cast<sig_handler_t>(prev_sig_handler);
}

int raise(int signal) {
    auto const raise_status = s_raise_signal(s_get_tid(), signal);
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

} /* extern "C" */

#pragma clang diagnostic pop
