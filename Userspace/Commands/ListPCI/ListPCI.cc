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

#include "List.hh"

#include <Utils/ArgsParser.hh>

#define V_MAJOR 0
#define V_MINOR 0
#define V_PATCH 1

int main(int argc, const char** argv) {
    Utils::ArgsParser args_parser{ "List PCI Devices", V_MAJOR, V_MINOR, V_PATCH };

    /* parse the arguments */
    args_parser.parse(argc, argv);

    /* list PCI devices */
    return list_pci_devices();
}
