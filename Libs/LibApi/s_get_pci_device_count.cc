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

usize s_get_pci_device_count() {
    SyscallGetPciDeviceCount data;
    do_syscall(SYSCALL_GET_PCI_COUNT, (usize)&data);
    return data.m_count;
}