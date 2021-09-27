/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * *
 * MeetiX OS By MeetiX OS Project [Marco Cicognani]                                    *
 *                                                                                     *
 *         DERIVED FROM THE GHOST OPERATING SYSTEM                                     *
 *         This software is derived from the Ghost operating system project,           *
 *         written by Max Schlüssel <lokoxe@gmail.com>. Copyright 2012-2017            *
 *         https://ghostkernel.org/                                                    *
 *         https://github.com/maxdev1/ghost                                            *
 *                                                                                     *
 * This program is free software; you can redistribute it and/or                       *
 * modify it under the terms of the GNU General Public License                         *
 * as published by the Free Software Foundation; either version 2                      *
 * of the License, or (char *argumentat your option) any later version.                *
 *                                                                                     *
 * This program is distributed in the hope that it will be useful,                     *
 * but WITHout ANY WARRANTY; without even the implied warranty of                      *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                       *
 * GNU General Public License for more details.                                        *
 *                                                                                     *
 * You should have received a copy of the GNU General Public License                   *
 * along with this program; if not, write to the Free Software                         *
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA      *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * */

#ifndef EVA_KERNEL
#define EVA_KERNEL

#include "memory/collections/AddressRangePool.hpp"
#include "ramdisk/ramdisk.hpp"
#include "tasking/thread.hpp"

#include <BuildConfig.hpp>
#include <kernelloader/SetupInformation.hpp>

/**
 * main kernel class
 */
class EvaKernel {
private:
    /**
     * Performs the very first kernel setup, copying all necessary information that
     * is provided by the loader.
     *
     * @param info:		the info provided by the loader
     */
    static void preSetup(SetupInformation* info);

    /**
     * print a human-readable header of the kernel
     *
     * @param info:		the info provided by the loader
     */
    static void printHeader(SetupInformation* info);

    /**
     * load the ramdisk from the multiboot module.
     * performs a copy on the kernel heap of the module, instantiate the ramdisk object
     * and delegate the interpretation to the object
     *
     * @param ramdiskModule:	the multiboot module that be used as ramdisk
     * @return the size of the ramdisk in KB
     */
    static uint32_t loadRamdisk(MultibootModule* ramdiskModule);

public:
    /**
     * Pool of virtual ranges used in the kernel to map memory.
     */
    static AddressRangePool* evaKernelRangePool;

    /**
     * Pointer to the global ramdisk instance.
     */
    static Ramdisk* ramdisk;

    /**
     * Does the initial setup of the kernel. The setup information struct is provided
     * from the bootloader and contains important information like the position of the
     * kernel image, or the memory bitmap.
     *
     * @param info the setup information provided by the kernel loader
     */
    static void run(SetupInformation* info);

    /**
     * BSP setup routine
     *
     * @param initialPdPhysical:	the physical page where initialize
     */
    static void runBasicSystemPackage(PhysAddr initialPdPhysical);

    /**
     * AP setup routine
     */
    static void runAdvancedSystemPackage();

    /**
     * Loads a system process binary as a process with the specified priority.
     *
     * @param path:			the ramdisk path to the binary
     * @param priority:		the thread priority to assign
     */
    static void loadSystemProcess(const char* path, ThreadPriority priority);

    /**
     * Triggers a kernel panic, means halting the entire system and displaying the given
     * message/diagnostic information.
     *
     * @param message:		the panic message, written in a format that the g_logger class
     * understands
     * @param ...:			variable arguments for the message
     */
    static void panic(const char* message, ...);
};

#endif
