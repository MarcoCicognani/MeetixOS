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

bool s_get_pci_device(usize position, PCIDeviceHeader* header) {
    if ( header ) {
        header->m_list_pos = position;
        do_syscall(SYSCALL_GET_PCI_DEVICE, (usize)header);

        return header->m_is_valid;
    }
    return false;
}