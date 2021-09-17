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

#include <EvangelionNG.hpp>
#include <logger/logger.hpp>
#include <memory/AddressSpace.hpp>
#include <memory/constants.hpp>
#include <memory/memory.hpp>
#include <memory/physical/PPallocator.hpp>
#include <system/interrupts/lapic.hpp>
#include <system/smp/smp.hpp>
#include <system/system.hpp>
#include <system/timing/pit.hpp>

/**
 * initialize the provided cpu
 *
 * @param cpu:		the cpu to be initialized
 */
void Smp::initializeCore(Processor* cpu) {
    // Calculate the vector value for the code start
    uint32_t vectorValue = (CONST_SMP_STARTUP_AREA_CODE_START >> 12) & 0xFF;

    // Send INIT
    Lapic::write(APIC_REGISTER_INT_COMMAND_HIGH, cpu->apic << 24);
    Lapic::write(APIC_REGISTER_INT_COMMAND_LOW, APIC_ICR_DELMOD_INIT | APIC_ICR_LEVEL_ASSERT);
    Lapic::waitForIcrSend();

    // Sleep 10 milliseconds
    Pit::prepareSleep(10000);
    Pit::performSleep();

    // Send SIPI
    Lapic::write(APIC_REGISTER_INT_COMMAND_HIGH, cpu->apic << 24);
    Lapic::write(APIC_REGISTER_INT_COMMAND_LOW,
                 vectorValue | APIC_ICR_DELMOD_SIPI | APIC_ICR_LEVEL_ASSERT);

    // Sleep 200µs
    Pit::prepareSleep(200);
    Pit::performSleep();

    // Send SIPI
    Lapic::write(APIC_REGISTER_INT_COMMAND_HIGH, cpu->apic << 24);
    Lapic::write(APIC_REGISTER_INT_COMMAND_LOW,
                 vectorValue | APIC_ICR_DELMOD_SIPI | APIC_ICR_LEVEL_ASSERT);

    // Sleep 200µs
    Pit::prepareSleep(200);
    Pit::performSleep();
}

/**
 * initialize the smp from the first physical address
 *
 * @param initialPageDirectoryPhysical
 *		the first physical address where the initialization begin
 */
void Smp::initialize(PhysicalAddress initialPageDirectoryPhysical) {
    // Write values to lower memory for use within startup code
    *((uint32_t*)CONST_SMP_STARTUP_AREA_PAGE_DIRECTORY) = initialPageDirectoryPhysical;
    *((uint32_t*)CONST_SMP_STARTUP_AREA_AP_START_ADDRESS)
        = (VirtualAddress)EvaKernel::runAdvancedSystemPackage;
    *((uint32_t*)CONST_SMP_STARTUP_AREA_AP_COUNTER) = 0;

    logDebug("%! initial page directory for APs: %h",
             "smp",
             *((uint32_t*)CONST_SMP_STARTUP_AREA_PAGE_DIRECTORY));
    logDebug("%! kernel entry point for APs: %h",
             "smp",
             *((uint32_t*)CONST_SMP_STARTUP_AREA_AP_START_ADDRESS));
    logDebug("%! initial AP counter value: %i",
             "smp",
             *((uint32_t*)CONST_SMP_STARTUP_AREA_AP_COUNTER));

    // Create enough stacks for all APs
    PhysicalAddress* stackArray = (PhysicalAddress*)CONST_SMP_STARTUP_AREA_AP_STACK_ARRAY;
    for ( uint32_t i = 0; i < System::getNumberOfProcessors(); i++ ) {
        PhysicalAddress stackPhysical = PPallocator::allocate();
        if ( !stackPhysical ) {
            logInfo("%*%! could not allocate physical page for AP stack", 0x0C, "smp");
            return;
        }
        VirtualAddress stackVirtual = EvaKernel::evaKernelRangePool->allocate(1);
        if ( !stackPhysical ) {
            logInfo("%*%! could not allocate virtual range for AP stack", 0x0C, "smp");
            return;
        }

        AddressSpace::map(stackVirtual,
                          stackPhysical,
                          DEFAULT_KERNEL_TABLE_FLAGS,
                          DEFAULT_KERNEL_PAGE_FLAGS);

        VirtualAddress stackTop = (stackVirtual + PAGE_SIZE);
        stackArray[i]           = stackTop;

        logDebug("%! created AdvancedPackage stack (%h) placed at %h",
                 "smp",
                 stackArray[i],
                 &stackArray[i]);
    }

    // Copy start object from ramdisk to lower memory
    const char*   smpstartPath  = "/MeetiX/Kernel/Modules/SMPStart.ko";
    RamdiskEntry* startupObject = EvaKernel::ramdisk->findAbsolute(smpstartPath);
    if ( !startupObject ) {
        logInfo("%*%! could not initialize due to missing smpstart object at '%s'",
                0x0C,
                "smp",
                smpstartPath);
        return;
    }
    Memory::copy((uint8_t*)CONST_SMP_STARTUP_AREA_CODE_START,
                 (uint8_t*)startupObject->data,
                 startupObject->datalength);

    // Start APs
    Processor* current = System::getProcessorList();
    while ( current ) {
        if ( !current->bsp )
            initializeCore(current);
        current = current->next;
    }
}
