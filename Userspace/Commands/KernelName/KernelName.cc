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

#include <iostream>
#include <unistd.h>
#include <Utils/ArgsParser.hh>
#include <Utils/Environment.hh>

#define V_MAJOR 0
#define V_MINOR 0
#define V_PATCH 1

void do_show_user() {
    std::cout << Utils::Environment::logged_user() << " on " << Utils::Environment::hostname()
              << " v" << Utils::Environment::version()
              << " Copyright (C) 2017-2021, MeetiX OS Project\n";
}

void do_show_loader(const KernelName& kern_name) {
    std::cout << "  Loader " << kern_name.m_loader_name << " v" << kern_name.m_loader_major << '.'
              << kern_name.m_loader_minor << '.' << kern_name.m_loader_patch << '\n';
}

void do_show_kernel(const KernelName& kern_name) {
    std::cout << "  Kernel " << kern_name.m_kernel_name << " v" << kern_name.m_kernel_major << '.'
              << kern_name.m_kernel_minor << '.' << kern_name.m_kernel_sub << '.'
              << kern_name.m_kernel_patch << '\n';
}

void do_show_system_info() {
    SystemInfo sys_info;
    s_system_info(&sys_info);

    std::cout << "Used memory "
              << (sys_info.m_memory_total_amount - sys_info.m_memory_free_amount) / 1024 << "/"
              << sys_info.m_memory_total_amount / 1024 << " MiB\n";
    std::cout << sys_info.m_cpu_count << " available cores, vendor: " << sys_info.m_cpu_vendor
              << '\n';
}

int main(int argc, const char** argv) {
    auto show_all    = true;
    auto show_os     = false;
    auto show_uname  = false;
    auto show_loader = false;
    auto show_kernel = false;
    auto show_info   = false;

    auto args_parser = Utils::ArgsParser{ "Kernel Info Utility", V_MAJOR, V_MINOR, V_PATCH };
    args_parser.add_option(show_all, "Show all the information", "all", 'a');
    args_parser.add_option(show_os, "Shows username, hostname, version and copyright", "os", 'o');
    args_parser.add_option(show_uname, "Shows loader and kernel information", "uname", 'u');
    args_parser.add_option(show_loader, "Shows loader's only information", "loader", 'l');
    args_parser.add_option(show_kernel, "Shows kernel's only information", "kernel", 'k');
    args_parser.add_option(show_info, "Shows system's information", "sysinfo", 's');

    /* parse the arguments */
    args_parser.parse(argc, argv);

    /* obtain kernel information */
    KernelName kern_name;
    s_kernel_name(&kern_name);

    if ( show_os ) {
        do_show_user();
    } else if ( show_uname ) {
        do_show_loader(kern_name);
        do_show_kernel(kern_name);
    } else if ( show_loader ) {
        do_show_loader(kern_name);
    } else if ( show_kernel ) {
        do_show_kernel(kern_name);
    } else if ( show_info ) {
        do_show_system_info();
    } else if ( show_all ) {
        do_show_user();
        do_show_loader(kern_name);
        do_show_kernel(kern_name);
        do_show_system_info();
    }
    return EXIT_SUCCESS;
}
