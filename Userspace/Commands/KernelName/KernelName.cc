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
#include <Utils/Environment.hh>

#define VERSION "0.0.1"

enum class Mode : u8
{
    SHOW_ALL,
    SHOW_OS,
    SHOW_UNAME,
    SHOW_LOADER,
    SHOW_KERNEL,
    SHOW_INFO,
    SHOW_HELP
};

void show_usages(int, const char** argv) {
    std::cout << "Kernel Info Utility v" << VERSION << '\n';
    std::cout << "Usage:\n";
    std::cout << "\t" << argv[0] << " [options...]\n";
    std::cout << "\t -a | --all:       Shows all the kernel information\n";
    std::cout << "\t -o | --os:        Shows username, hostname, version and copyright\n";
    std::cout << "\t -u | --uname:     Shows loader and kernel information\n";
    std::cout << "\t -l | --loader:    Shows loader's only information\n";
    std::cout << "\t -k | --kernel:    Shows kernel's only information\n";
    std::cout << "\t -s | --sys-info:  Shows system's information\n";
    std::cout << "\t -h | --help | -?: Shows this help\n";
    std::cout << '\n';
    std::cout << "Compiled with g++ v" << __VERSION__ << " (" << __TIMESTAMP__ << ")" << std::endl;
}

Mode parse_args(int argc, const char** argv) {
    if ( getopt_is_help(argc, argv) )
        return Mode::SHOW_HELP;

    struct option long_opts[] = {
        { "all", no_argument, 0, 'a' },
        { "os", no_argument, 0, 'o' },
        { "uname", no_argument, 0, 'u' },
        { "loader", no_argument, 0, 'l' },
        { "kernel", no_argument, 0, 'k' },
        { "sys-info", no_argument, 0, 's' },
        { 0, 0, 0, 0 },
    };

    /* parse cmdline arguments */
    int opt;
    while ( (opt = getopt_long(argc, argv, "h?", long_opts, nullptr)) != EOF ) {
        switch ( opt ) {
            case 'a':
                return Mode::SHOW_ALL;
            case 'o':
                return Mode::SHOW_OS;
            case 'u':
                return Mode::SHOW_UNAME;
            case 'l':
                return Mode::SHOW_LOADER;
            case 'k':
                return Mode::SHOW_KERNEL;
            case 's':
                return Mode::SHOW_INFO;
            case 'h':
            default:
                return Mode::SHOW_HELP;
        }
    }

    return Mode::SHOW_ALL;
}

void show_user() {
    std::cout << Utils::Environment::logged_user() << " on " << Utils::Environment::hostname()
              << " v" << Utils::Environment::version()
              << " Copyright (C) 2017-2021, MeetiX OS Project\n";
}

void show_loader(const KernelName& kern_name) {
    std::cout << "  Loader " << kern_name.m_loader_name << " v" << kern_name.m_loader_major << '.'
              << kern_name.m_loader_minor << '.' << kern_name.m_loader_patch << '\n';
}

void show_kernel(const KernelName& kern_name) {
    std::cout << "  Kernel " << kern_name.m_kernel_name << " v" << kern_name.m_kernel_major << '.'
              << kern_name.m_kernel_minor << '.' << kern_name.m_kernel_sub << '.'
              << kern_name.m_kernel_patch << '\n';
}

void show_system_info() {
    SystemInfo sys_info;
    s_system_info(&sys_info);

    std::cout << "Used memory "
              << (sys_info.m_memory_total_amount - sys_info.m_memory_free_amount) / 1024 << "/"
              << sys_info.m_memory_total_amount / 1024 << " MiB\n";
    std::cout << sys_info.m_cpu_count << " available cores, vendor: " << sys_info.m_cpu_vendor
              << '\n';
}

int main(int argc, const char* argv[]) {
    auto mode = parse_args(argc, argv);

    KernelName kern_name;
    s_kernel_name(&kern_name);

    if ( mode == Mode::SHOW_ALL ) {
        show_user();
        show_loader(kern_name);
        show_kernel(kern_name);
        show_system_info();
    } else if ( mode == Mode::SHOW_OS )
        show_user();
    else if ( mode == Mode::SHOW_UNAME ) {
        show_loader(kern_name);
        show_kernel(kern_name);
    } else if ( mode == Mode::SHOW_LOADER )
        show_loader(kern_name);
    else if ( mode == Mode::SHOW_KERNEL )
        show_kernel(kern_name);
    else if ( mode == Mode::SHOW_INFO )
        show_system_info();
    else
        show_usages(argc, argv);

    return EXIT_SUCCESS;
}
