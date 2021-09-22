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

void s_halt_machine(int mode) {
    Tid spawnerTid = s_task_get_id(SPAWNER_IDENTIFIER);
    if ( spawnerTid == -1 )
        return;

    SpawnerRequestHaltMachine request;
    request.m_command_header.m_command = mode;
    s_send_message_t(spawnerTid,
                     &request,
                     sizeof(SpawnerRequestHaltMachine),
                     s_get_message_tx_id());
}

void s_reboot_system() {
    s_halt_machine(SPAWN_COMMAND_REBOOT_MACHINE);
}

void s_shutdown_system() {
    s_halt_machine(SPAWN_COMMAND_SHUTDOWN_MACHINE);
}
