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

#include <BuildConfig.hpp>
#include <calls/SyscallHandler.hpp>
#include <Api/Info.h>
#include <memory/physical/PPallocator.hpp>
#include <system/pci/pci.hpp>
#include <system/system.hpp>
#include <tasking/tasking.hpp>
#include <tasking/ThreadManager.hpp>
#include <utils/string.hpp>

/**
 * Returns kernel version
 */
SYSCALL_HANDLER(ename) {
    KernelName* data = (KernelName*)SYSCALL_DATA(currentThread->cpuState);

    // copy loader name & version
    String::copy(data->m_loader_name, LOADER_NAME);
    data->m_loader_major = L_VERSION_MAJOR;
    data->m_loader_minor = L_VERSION_MINOR;
    data->m_loader_patch = L_VERSION_PATCH;

    // copy kernel name & version
    String::copy(data->m_kernel_name, KERNEL_NAME);
    data->m_kernel_major = VERSION_MAJOR;
    data->m_kernel_minor = VERSION_MINOR;
    data->m_kernel_sub   = VERSION_SUB;
    data->m_kernel_patch = VERSION_PATCH;

    return currentThread;
}

/**
 * Returns system specifications
 */
SYSCALL_HANDLER(sysinfo) {
    SystemInfo* data = (SystemInfo*)SYSCALL_DATA(currentThread->cpuState);

    // get ram size in KB
    data->m_memory_total_amount = (PPallocator::getInitialAmount() * PAGE_SIZE / 1024);
    data->m_memory_free_amount  = (PPallocator::getFreePageCount() * PAGE_SIZE / 1024);

    // get number of cpu cores
    data->m_cpu_count = System::getNumberOfProcessors();

    // get the cpu vendor name
    char vendor[13];
    Processor::getVendor(vendor);
    vendor[12] = '\0';
    String::copy(data->m_cpu_vendor, vendor);

    return currentThread;
}

/**
 * Returns the pci devide count
 */
SYSCALL_HANDLER(getPciDeviceCount) {
    SyscallGetPciDeviceCount* data
        = (SyscallGetPciDeviceCount*)SYSCALL_DATA(currentThread->cpuState);
    data->m_count = Pci::getDeviceCount();
    return currentThread;
}

/**
 * Returns the pci device descriptor by the id provided
 */
SYSCALL_HANDLER(getPciDevice) {
    PciDeviceHeader* data      = (PciDeviceHeader*)SYSCALL_DATA(currentThread->cpuState);
    PciHeader*       pciHeader = Pci::getDeviceAt(data->m_list_pos);
    data->m_is_valid           = false;

    // only if header is avaible
    if ( pciHeader ) {
        data->m_is_valid = true;

        data->m_dev_bus      = pciHeader->bus;
        data->m_dev_slot     = pciHeader->slot;
        data->m_dev_function = pciHeader->function;

        data->m_vendor_id = pciHeader->vendorId;
        data->m_device_id = pciHeader->deviceId;

        data->m_class_code    = pciHeader->classCode;
        data->m_subclass_code = pciHeader->subclassCode;
        data->m_program       = pciHeader->progIf;
    }

    return currentThread;
}

/**
 * Returns the Thread count
 */
SYSCALL_HANDLER(getThreadCount) {
    SyscallGetThreadCount* data = (SyscallGetThreadCount*)SYSCALL_DATA(currentThread->cpuState);
    data->m_count               = Tasking::count(data->m_thread_type);
    return currentThread;
}

/**
 * Gets the Thread pid list
 */
SYSCALL_HANDLER(getThreadIDs) {
    SyscallGetThreadIDs* data = (SyscallGetThreadIDs*)SYSCALL_DATA(currentThread->cpuState);
    data->m_stored_ids_count
        = Tasking::getTaskIDs(data->m_id_buffer, data->m_id_buffer_len, data->m_thread_type);
    return currentThread;
}

/**
 * Gets the Thread descriptor by it's tid
 */
SYSCALL_HANDLER(getThreadDescriptor) {
    ThreadDescriptor* data   = (ThreadDescriptor*)SYSCALL_DATA(currentThread->cpuState);
    Thread*           thread = Tasking::getTaskById(data->m_tid);

    // check validity
    if ( thread ) {
        // set the parent process
        data->found = true;
        if ( thread->process->main )
            data->m_parent = thread->process->main->id;
        else
            data->m_parent = -1;

        // set the type
        data->m_thread_type = thread->type;

        // provide the stack size
        data->m_thread_user_stack_size = ThreadManager::getThreadStackSize(thread);

        // copy identifier
        const char* threadName = thread->getIdentifier();
        if ( threadName )
            String::copy(data->m_identifier, threadName);
        else
            data->m_identifier[0] = '\0';

        // provide infos about rounds wait and scheduled times
        data->m_wait_count      = thread->waitCount;
        data->m_execution_time  = thread->rounds * APIC_MILLISECONDS_PER_TICK;
        data->m_scheduled_times = thread->rounds;
    }

    else
        data->found = false;

    return currentThread;
}

/**
 * Returns the process count
 */
SYSCALL_HANDLER(getProcessCount) {
    SyscallGetProcessCount* data = (SyscallGetProcessCount*)SYSCALL_DATA(currentThread->cpuState);
    data->m_count                = Tasking::count(THREAD_TYPE_MAIN | THREAD_TYPE_VM86);
    return currentThread;
}

/**
 * Gets the process pid list
 */
SYSCALL_HANDLER(getProcessIDs) {
    SyscallGetProcessIDs* data = (SyscallGetProcessIDs*)SYSCALL_DATA(currentThread->cpuState);
    data->m_stored_ids_count   = Tasking::getTaskIDs(data->m_id_buffer,
                                                     data->m_id_buffer_len,
                                                     THREAD_TYPE_MAIN | THREAD_TYPE_VM86);
    return currentThread;
}

/**
 * Gets the process descriptor by it's pid
 */
SYSCALL_HANDLER(getProcessDescriptor) {
    ProcessDescriptor* data = (ProcessDescriptor*)SYSCALL_DATA(currentThread->cpuState);
    Thread*            main = Tasking::getTaskById(data->m_main_thread.m_tid);

    // check validity
    if ( main ) {
        // set the parent process
        data->m_main_thread.found = true;
        if ( main->process->parent )
            data->m_main_thread.m_parent = main->process->parent->main->id;
        else
            data->m_main_thread.m_parent = -1;

        // set the type
        data->m_main_thread.m_thread_type = main->type;

        // only for main or vm86 provide process size informations
        data->m_heap_size  = ThreadManager::getProcessHeapSize(main);
        data->m_image_size = ThreadManager::getProcessImageSize(main);

        // copy identifier
        const char* name = main->getIdentifier();
        if ( name )
            String::copy(data->m_main_thread.m_identifier, name);
        else
            data->m_main_thread.m_identifier[0] = '\0';

        // copy process source
        char* sourcePath = main->process->sourcePath;
        if ( sourcePath )
            String::copy(data->m_source_path, sourcePath);
        else
            data->m_source_path[0] = '\0';

        // provide infos about rounds wait and scheduled times
        data->m_main_thread.m_wait_count      = main->waitCount;
        data->m_main_thread.m_execution_time  = main->rounds * APIC_MILLISECONDS_PER_TICK;
        data->m_main_thread.m_scheduled_times = main->rounds;
    }

    else
        data->m_main_thread.found = false;

    return currentThread;
}
