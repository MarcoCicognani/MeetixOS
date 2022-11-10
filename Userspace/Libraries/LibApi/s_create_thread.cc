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

/**
 * @brief Library entry-point for user sub-threads
 */
static A_NOINLINE void thread_entry_point() {
    SyscallGetThreadEntry data;
    do_syscall(SYSCALL_THREAD_GET_ENTRY, (usize)&data);

    /* execute the thread body */
    auto user_entry = reinterpret_cast<void (*)(void*)>(data.m_user_entry);
    if ( user_entry )
        user_entry(data.m_user_data);

    /* self termination after user entry return */
    return __exit_thread();
}

Tid s_create_thread(void* func_ptr) {
    return s_create_thread_d(func_ptr, nullptr);
}

Tid s_create_thread_d(void* func_ptr, void* data) {
    return s_create_thread_dns(func_ptr, data, nullptr, nullptr);
}

Tid s_create_thread_n(void* func_ptr, const char* name) {
    return s_create_thread_dns(func_ptr, nullptr, name, nullptr);
}

Tid s_create_thread_dn(void* func_ptr, void* data, const char* name) {
    return s_create_thread_dns(func_ptr, data, name, nullptr);
}

Tid s_create_thread_ds(void* func_ptr, void* data, CreateThreadStatus* out_status) {
    return s_create_thread_dns(func_ptr, data, nullptr, out_status);
}

Tid s_create_thread_dns(void* func_ptr, void* data, const char* name, CreateThreadStatus* out_status) {
    SyscallCreateThread call_data = { reinterpret_cast<void*>(thread_entry_point), func_ptr, data, name };
    do_syscall(SYSCALL_THREAD_CREATE, (usize)&call_data);

    if ( out_status )
        *out_status = call_data.m_thread_status;
    return call_data.m_thread_id;
}
