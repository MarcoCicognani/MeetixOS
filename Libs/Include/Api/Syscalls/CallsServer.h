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

#pragma once

#include <Api/Kernel.h>
#include <Api/System.h>

__BEGIN_C

/**
 * parameter struct used by {s_register_as_server} Syscall
 *
 * @field status:			filled with the return code of the call
 * @field identifier:		the identifier of the server to register
 * @field securityLevel:	the new security level to apply for the server
 */
typedef struct {
    const char*            identifier;
    SecurityLevel          securityLevel;
    RegisterAsServerStatus status;
} A_PACKED SyscallRegisterAsServer;

/**
 * parameter struct used by {s_server_manage} do_syscall
 *
 * @field identifier:   the identifier of the server
 * @field buffer:       the buffer for the command
 * @field status:       the resultant state of the command sending
 */
typedef struct {
    const char*        identifier;
    ServerManageBuffer buffer;
    ServerManageStatus status;
} A_PACKED SyscallServerManage;

__END_C
