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

#include "signal_internal.hh"

#include <Api/User.h>
#include <signal.h>

extern "C" sig_handler_t signal(int sig, sig_handler_t sig_handler) {
    if ( sig_handler == SIG_DFL ) {
        if ( sig == SIGINT ) {
            sig_handler = sig_handler_SIG_INT;
        }
    } else if ( sig_handler == SIG_IGN ) {
        sig_handler = sig_handler_SIG_IGN;
    }

    return reinterpret_cast<sig_handler_t>(
        s_register_signal_handler(sig, reinterpret_cast<void*>(sig_handler)));
}
