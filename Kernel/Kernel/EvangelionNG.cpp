/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * *
 * * * * * MeetiX OS By MeetiX OS Project [Marco Cicognani] *
 *                                                                                     *
 *         DERIVED FROM THE GHOST OPERATING SYSTEM * This software is derived
 * from the Ghost operating system project,           * written by Max Schlüssel
 * <lokoxe@gmail.com>. Copyright 2012-2017            * https://ghostkernel.org/
 * * https://github.com/maxdev1/ghost *
 *                                                                                     *
 * This program is free software; you can redistribute it and/or * modify it
 * under the terms of the GNU General Public License                         *
 * as published by the Free Software Foundation; either version 2 * of the
 * License, or (char *argumentat your option) any later version. *
 *                                                                                     *
 * This program is distributed in the hope that it will be useful, * but WITHout
 * ANY WARRANTY; without even the implied warranty of                      *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the * GNU General
 * Public License for more details.                                        *
 *                                                                                     *
 * You should have received a copy of the GNU General Public License * along
 * with this program; if not, write to the Free Software * Foundation, Inc., 51
 * Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA      *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * *
 * * * * */

#include "EvangelionNG.hpp"

#include "Api/StdInt.h"
#include "executable/Elf32Loader.hpp"
#include "filesystem/filesystem.hpp"
#include "kernelloader/SetupInformation.hpp"
#include "logger/logger.hpp"
#include "memory/AddressSpace.hpp"
#include "memory/collections/AddressRangePool.hpp"
#include "memory/collections/AddressStack.hpp"
#include "memory/constants.hpp"
#include "memory/gdt/GdtManager.hpp"
#include "memory/KernelHeap.hpp"
#include "memory/LowerHeap.hpp"
#include "memory/paging.hpp"
#include "memory/physical/PPallocator.hpp"
#include "memory/TemporaryPagingUtil.hpp"
#include "multiboot/MultibootUtil.hpp"
#include "system/BiosDataArea.hpp"
#include "system/pci/pci.hpp"
#include "system/serial/SerialPort.hpp"
#include "system/smp/GlobalLock.hpp"
#include "system/system.hpp"
#include "tasking/tasking.hpp"
#include "video/ConsoleVideo.hpp"
#include "video/PrettyBoot.hpp"

#include <CCLang/Prelude.hh>

// scoping of static internal kernel class properties
AddressRangePool* EvaKernel::evaKernelRangePool;
Ramdisk*          EvaKernel::ramdisk;

// locker for initialization routines
static GlobalLock BspSetupLock;
static GlobalLock ApSetupLock;

// number of core counter
static int coreNumber;

// spawn locker
static GlobalLock systemProcessSpawnLock;

/**
 * Performs the very first kernel setup, copying all necessary information that
 * is provided by the loader.
 *
 * @param info:		the info provided by the loader
 */
void EvaKernel::preSetup(SetupInformation* info) {
    // initialize the serial com port for kernel logging
    ComPortInformation comPortInfo = biosDataArea->comPortInfo;
    if ( comPortInfo.com1 > 0 ) {
        SerialPort::initializePort(comPortInfo.com1,
                                   false); // Initialize in poll mode
        Logger::enableSerialPortLogging();
        DebugInterface::initialize(comPortInfo.com1);
    }

    else
        Logger::println("%! COM1 port not available for serial debug output", "logger");

    // print the kernel header information
    printHeader(info);

    // initialize the physical allocator from start bitmap of the loader
    PRETTY_BOOT_STATUS("Checking available memory", 20, GREEN);
    PPallocator::initializeFromBitmap(info->bitmapStart, info->bitmapEnd);

    // initialize the heap with provided info of the loader
    KernelHeap::initialize(info->heapStart, info->heapEnd);

    // search from loaded modules the ramdisk file
    PRETTY_BOOT_STATUS("Searching ramdisk module", 30, GREEN);
    MultibootModule* ramDiskModule = MultibootUtils::findModule(info->multibootInformation, "/boot/Ramdisk.img");
    if ( !ramDiskModule )
        panic("%! ramdisk MXfs.img does not exist", "Eva Kernel");

    // create the memory address ranges
    PRETTY_BOOT_STATUS("Initializing kernel range pool", 35, GREEN);
    EvaKernel::evaKernelRangePool = new AddressRangePool();
    EvaKernel::evaKernelRangePool->initialize(CONST_KERNEL_VIRTUAL_RANGES_START, CONST_KERNEL_VIRTUAL_RANGES_END);

    // load the ramdisk and get it's size
    PRETTY_BOOT_STATUS("Loading ramdisk", 40, GREEN);
    uint32_t ramdisk_size = loadRamdisk(ramDiskModule);

    // get the available physical memory in kb
    uint32_t avail_mem = (PPallocator::getInitialAmount() * PAGE_SIZE / 1024);

    // print a log with total physical memory avaible
    logInfo("%! total physical memory: %iKB", "Eva Kernel", avail_mem + ramdisk_size);
    logInfo("%! %iMB available for the system, %iMB used by ramdisk",
            "Eva Kernel",
            avail_mem / 1024,
            ramdisk_size / 1024);
}

/**
 * print a human-readable header of the kernel
 *
 * @param info:		the info provided by the loader
 */
void EvaKernel::printHeader(SetupInformation* info) {
    // Print header
    ConsoleVideo::clear();
    ConsoleVideo::setColor(32);
    logInfon("EvangelionNG Kernel");
    ConsoleVideo::setColor(0x0F);
    logInfo(" Version %i.%i.%i.%c", VERSION_MAJOR, VERSION_MINOR, VERSION_SUB, VERSION_PATCH);
    logInfo("");
    logInfo("  Copyright (C) 2017, MeetiX OS Project");
    logInfo("");
    logInfo("%! loading", "Pre Setup");

    // Print setup information
    logDebug("%! setup information:", "Pre Setup");
    logDebug("%#   reserved: %h - %h", info->kernelImageStart, info->kernelImageEnd);
    logDebug("%#   stack:    %h - %h", info->stackStart, info->stackEnd);
    logDebug("%#   bitmap:   %h - %h", info->bitmapStart, info->bitmapEnd);
    logDebug("%#   heap:     %h - %h", info->heapStart, info->heapEnd);
    logDebug("%#   mbstruct: %h", info->multibootInformation);
    logDebug("%! started", "Eva Kernel");
    logDebug("%! got setup information at %h", "Eva Kernel", info);
}

/**
 * load the ramdisk from the multiboot module.
 * performs a copy on the kernel heap of the module, instantiate the ramdisk
 * object and delegate the interpretation to the object
 *
 * @param ramdiskModule:	the multiboot module that be used as ramdisk
 * @return the size of the ramdisk in KB
 */
uint32_t EvaKernel::loadRamdisk(MultibootModule* ramdiskModule) {
    // align the pages to get module size
    int ramdiskPages = PAGE_ALIGN_UP(ramdiskModule->moduleEnd - ramdiskModule->moduleStart) / PAGE_SIZE;

    // allocate memory to a new virtual address
    VirtAddr ramdiskNewLocation = EvaKernel::evaKernelRangePool->allocate(ramdiskPages);

    // not enough memory, Panic
    if ( !ramdiskNewLocation )
        panic("%! not enough virtual space for ramdisk remapping", "Eva Kernel");

    // copy the module on kernel physical memory
    for ( int i = 0; i < ramdiskPages; i++ ) {
        VirtAddr virt = ramdiskNewLocation + i * PAGE_SIZE;
        PhysAddr phys = AddressSpace::virtualToPhysical(ramdiskModule->moduleStart + i * PAGE_SIZE);
        AddressSpace::map(virt, phys, DEFAULT_KERNEL_TABLE_FLAGS, DEFAULT_KERNEL_PAGE_FLAGS);
    }

    // adjust module range
    ramdiskModule->moduleEnd   = ramdiskNewLocation + (ramdiskModule->moduleEnd - ramdiskModule->moduleStart);
    ramdiskModule->moduleStart = ramdiskNewLocation;

    // create the ramdisk and load the module
    EvaKernel::ramdisk = new Ramdisk();
    EvaKernel::ramdisk->load(ramdiskModule);
    logInfo("%! ramdisk mounted", "Eva Kernel");

    // return the ramdisk size as kilobytes
    return ramdiskPages * PAGE_SIZE / 1024;
}

/**
 * Does the initial setup of the kernel. The setup information struct is
 * provided from the bootloader and contains important information like the
 * position of the kernel image, or the memory bitmap.
 *
 * @param info the setup information provided by the kernel loader
 */
void EvaKernel::run(SetupInformation* info) {
    PRETTY_BOOT_STATUS("Running pre setup", 15, GREEN);
    preSetup(info);

    logDebug("%! unmapping old address space area", "Eva Kernel");
    PRETTY_BOOT_STATUS("Unmapping old address space area", 41, GREEN);

    // unmap info and loader
    PhysAddr initialPdPhysical = info->initialPageDirectoryPhysical;
    for ( VirtAddr i = CONST_LOWER_MEMORY_END; i < CONST_KERNEL_AREA_START; i += PAGE_SIZE )
        AddressSpace::unmap(i);

    // begin basic system initialization
    runBasicSystemPackage(initialPdPhysical);
}

/**
 * BSP setup routine
 *
 * @param initialPdPhysical:	the physical page where initialize
 */
void EvaKernel::runBasicSystemPackage(PhysAddr initialPdPhysical) {
    // initialize the paging
    PRETTY_BOOT_STATUS("Initializing Paging", 42, GREEN);
    TemporaryPagingUtil::initialize();

    // initialize lower heap
    PRETTY_BOOT_STATUS("Initializing Lower Heap", 44, GREEN);
    LowerHeap::addArea(CONST_LOWER_HEAP_MEMORY_START, CONST_LOWER_HEAP_MEMORY_END);

    // lock the core
    BspSetupLock.lock();
    {
        // initialize system backend
        PRETTY_BOOT_STATUS("Initializing Basic System package", 45, GREEN);
        System::initializeBasicSystemPackage(initialPdPhysical);

        // initialize the Global Descriptor Table
        // (AFTER the system, so BSP's id is available)
        PRETTY_BOOT_STATUS("Initializing Global Descriptor Table", 50, GREEN);
        GdtManager::prepare();
        GdtManager::initialize();

        // initialize multitasking interface
        PRETTY_BOOT_STATUS("Initializing multitasking", 70, GREEN);
        Tasking::initialize();
        Tasking::enableForThisCore();

        // initialize the FileSystem
        PRETTY_BOOT_STATUS("Initializing FileSystem", 80, GREEN);
        FileSystem::initialize();

        // scan the pci devices
        PRETTY_BOOT_STATUS("Scanning pci devices", 85, GREEN);
        Pci::initialize();

        // spawn initial processes
        PRETTY_BOOT_STATUS("Loading basic binaries", 95, GREEN);
        {
            PRETTY_BOOT_STATUS("Load Idle process", 97, GREEN);
            loadSystemProcess("/Bins/Idle", THREAD_PRIORITY_IDLE, SECURITY_LEVEL_KERNEL);

            PRETTY_BOOT_STATUS("Load Init process", 99, GREEN);
            loadSystemProcess("/MeetiX/Kernel/Servers/Spawner.sv", THREAD_PRIORITY_NORMAL, SECURITY_LEVEL_KERNEL);
        }
        PRETTY_BOOT_STATUS("Starting Userspace", 100, GREEN);
    }
    BspSetupLock.unlock();
    // unlock locker

    // wait for other core initialization
    coreNumber = System::getNumberOfProcessors() - 1;
    logInfo("%! waiting for %i application processors", "Eva Kernel", coreNumber);
    while ( coreNumber > 0 )
        asm("pause");

    // initialization ended, leave, wait for firt scheduler call
    logInfo("%! leaving initialization", "Eva Kernel");
    asm("sti");
    while ( true )
        asm("hlt");
}

/**
 * AP setup routine
 */
void EvaKernel::runAdvancedSystemPackage() {
    // lock core
    ApSetupLock.lock();
    {
        PRETTY_BOOT_STATUS("Initializing Advanced System package", 100, RED);

        // wait bsp setup end
        logInfo("%! waiting for bsp to finish setup", "Eva AP");
        BspSetupLock.lock();
        BspSetupLock.unlock();

        // initialize the Global Descriptor Table for this core
        GdtManager::initialize();

        // initialize system backend
        System::initializeAdvancedPackage();

        // create the scheduler for this core
        Tasking::enableForThisCore();

        // spawn the idle process
        loadSystemProcess("/Bins/Idle", THREAD_PRIORITY_IDLE, SECURITY_LEVEL_KERNEL);

        // decrease the core count to initialize
        --coreNumber;
    }
    ApSetupLock.unlock();
    // unlock core

    // wait other initialization
    logInfo("%! waiting for %i application processors", "Eva AP", coreNumber);
    while ( coreNumber > 0 )
        asm("pause");

    // initialization ended, leave, wait for firt scheduler call
    logInfo("%! leaving initialization", "Eva AP");
    asm("sti");
    while ( true )
        asm("hlt");
}

/**
 * Loads a system process binary as a process with the specified priority.
 *
 * @param path:			the ramdisk path to the binary
 * @param priority:		the thread priority to assign
 */
void EvaKernel::loadSystemProcess(const char* binaryPath, ThreadPriority priority, SecurityLevel security_level) {
    // lock, only once can spawn processes
    systemProcessSpawnLock.lock();

    // spawn the new process
    Thread*          systemProcess;
    Elf32SpawnStatus status = Elf32Loader::spawnFromRamdisk(binaryPath, security_level, &systemProcess, true, priority);
    switch ( status ) {
        case Elf32SpawnStatus::SUCCESSFUL:
            logInfo("%! successful spawned \"%s\"", "sysproc", binaryPath);
            break;
        case Elf32SpawnStatus::FILE_NOT_FOUND:
            panic("%! \"%s\" not found", "Eva Kernel", binaryPath);
            break;
        case Elf32SpawnStatus::VALIDATION_ERROR:
            panic("%! \"%s\" is not a valid elf32 binary", "Eva Kernel", binaryPath);
            break;
        case Elf32SpawnStatus::PROCESS_CREATION_FAILED:
            panic("%! \"%s\" could not be loaded, error creating process", "Eva Kernel", binaryPath);
            break;
        default:
            panic("%! \"%s\" could not be loaded", "Eva Kernel", binaryPath);
            break;
    }

    // unlock resource
    systemProcessSpawnLock.unlock();
}

/**
 * Triggers a kernel panic, means halting the entire system and displaying the
 * given message/diagnostic information.
 *
 * @param message:		the panic message, written in a format that the
 * g_logger class understands
 * @param ...:			variable arguments for the message
 */
void EvaKernel::panic(const char* msg, ...) {
    // other logs are unpermited
    Logger::manualLock();

    // write the panic header
    logInfo("%*%! an unrecoverable error has occured. reason:", 0x0C, "Eva Error");

    // print the provided arguments
    va_list valist;
    va_start(valist, msg);
    Logger::printFormatted(msg, valist);
    va_end(valist);
    Logger::printCharacter('\n');

    // unlock the Logger
    Logger::manualUnlock();

    // halt the system
    asm("cli");
    while ( true )
        asm("hlt");
}
