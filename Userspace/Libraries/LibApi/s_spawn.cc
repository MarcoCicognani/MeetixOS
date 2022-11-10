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

#include "__internal.hh"

#include <Api/User.h>
#include <Api/utils/local.hpp>

SpawnStatus s_spawn(const char* path, const char* args, const char* work_dir, SecurityLevel security_level) {
    return s_spawn_poi(path, args, work_dir, security_level, nullptr, nullptr, nullptr);
}

SpawnStatus
s_spawn_p(const char* path, const char* args, const char* work_dir, SecurityLevel security_level, Pid* out_pid) {
    return s_spawn_poi(path, args, work_dir, security_level, out_pid, nullptr, nullptr);
}

SpawnStatus s_spawn_po(const char*   path,
                       const char*   args,
                       const char*   work_dir,
                       SecurityLevel security_level,
                       Pid*          out_pid,
                       FileHandle    out_stdio[3]) {
    return s_spawn_poi(path, args, work_dir, security_level, out_pid, out_stdio, nullptr);
}

SpawnStatus s_spawn_poi(const char*   path,
                        const char*   args,
                        const char*   work_dir,
                        SecurityLevel security_level,
                        Pid*          out_pid,
                        FileHandle    out_stdio[3],
                        FileHandle    in_stdio[3]) {
    auto spawn_status = SPAWN_STATUS_UNKNOWN;

    /* obtain the Spawner thread id as message endpoint */
    Tid spawner_tid = s_task_get_id(SPAWNER_IDENTIFIER);
    if ( spawner_tid < 0 )
        return spawn_status;

    /* obtain a message transaction to create a close communication */
    auto msg_tx = s_get_message_tx_id();

    /* prepare the buffer for the request */
    auto path_len       = string_len(path) + 1;
    auto args_len       = string_len(args) + 1;
    auto work_dir_len   = string_len(work_dir) + 1;
    auto message_len    = sizeof(SpawnCommandSpawnRequest) + path_len + args_len + work_dir_len;
    auto request_buffer = Local{ new u8[message_len] };

    /* fill the request header */
    auto request_ptr                        = request_buffer();
    auto request_cmd                        = reinterpret_cast<SpawnCommandSpawnRequest*>(request_ptr);
    request_cmd->m_command_header.m_command = SPAWN_COMMAND_SPAWN_REQUEST;
    request_cmd->m_security_level           = security_level;
    request_cmd->m_path_len                 = path_len;
    request_cmd->m_args_len                 = args_len;
    request_cmd->m_workdir_len              = work_dir_len;

    /* fill-out the standard I/O */
    if ( in_stdio ) {
        request_cmd->m_stdin  = in_stdio[0];
        request_cmd->m_stdout = in_stdio[1];
        request_cmd->m_stderr = in_stdio[2];
    } else {
        request_cmd->m_stdin  = FD_NONE;
        request_cmd->m_stdout = FD_NONE;
        request_cmd->m_stderr = FD_NONE;
    }

    /* append to the header the other values */
    request_ptr += sizeof(SpawnCommandSpawnRequest);
    memory_copy(request_ptr, path, path_len);
    request_ptr += path_len;
    memory_copy(request_ptr, args, args_len);
    request_ptr += args_len;
    memory_copy(request_ptr, work_dir, work_dir_len);

    /* send the message request */
    auto send_status = s_send_message_t(spawner_tid, request_buffer(), message_len, msg_tx);
    if ( send_status != MESSAGE_SEND_STATUS_SUCCESSFUL )
        return SPAWN_STATUS_IO_ERROR;

    /* prepare the response buffer */
    auto response_len    = sizeof(MessageHeader) + sizeof(SpawnCommandSpawnResponse);
    auto response_buffer = Local{ new u8[response_len] };

    /* receive now the response from the Spawner */
    auto recv_status = s_receive_message_t(response_buffer(), response_len, msg_tx);
    if ( recv_status != MESSAGE_RECEIVE_STATUS_SUCCESSFUL )
        return SPAWN_STATUS_IO_ERROR;

    /* extract spawner response */
    auto spawner_response = reinterpret_cast<SpawnCommandSpawnResponse*>(MESSAGE_CONTENT(response_buffer()));
    if ( spawner_response->m_spawn_status == SPAWN_STATUS_SUCCESSFUL ) {
        if ( out_pid )
            *out_pid = spawner_response->m_new_process_id;
        if ( out_stdio ) {
            out_stdio[0] = spawner_response->m_stdin_write;
            out_stdio[1] = spawner_response->m_stdout_read;
            out_stdio[2] = spawner_response->m_stderr_read;
        }
    }
    return spawner_response->m_spawn_status;
}
