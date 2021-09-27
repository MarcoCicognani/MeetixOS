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

#include <Api/User.h>

RegisterAsServerStatus s_register_as_server(const char*   server_identifier,
                                            SecurityLevel security_level) {
    SyscallRegisterAsServer data{ server_identifier, security_level };
    do_syscall(SYSCALL_REGISTER_AS_SERVER, (usize)&data);
    return data.status;
}