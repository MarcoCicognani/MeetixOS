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

#ifndef EVA_SYSTEM_INTERRUPTS_APIC
#define EVA_SYSTEM_INTERRUPTS_APIC

#include "Api/StdInt.h"
#include "Api/Types.h"

#define EXPECTED_APIC_PHYSICAL_ADDRESS 0xFEE00000

/**
 * APIC register offsets (to APICs base address)
 */
#define APIC_REGISTER_ID                 0x20
#define APIC_REGISTER_VERSION            0x30
#define APIC_REGISTER_TASK_PRIO          0x80
#define APIC_REGISTER_ARBITRATION_PRIO   0x90
#define APIC_REGISTER_EOI                0xB0
#define APIC_REGISTER_LOGICAL_DEST       0xD0
#define APIC_REGISTER_DEST_FORMAT        0xE0
#define APIC_REGISTER_SPURIOUS_IVT       0xF0
#define APIC_REGISTER_ERROR_STATUS       0x280
#define APIC_REGISTER_LVT_CMCI           0x2F0
#define APIC_REGISTER_INT_COMMAND_LOW    0x300
#define APIC_REGISTER_INT_COMMAND_HIGH   0x310
#define APIC_REGISTER_LVT_TIMER          0x320
#define APIC_REGISTER_LVT_THERMAL_SENSOR 0x320
#define APIC_REGISTER_LVT_PERFMON        0x340
#define APIC_REGISTER_LVT_LINT0          0x350
#define APIC_REGISTER_LVT_LINT1          0x360
#define APIC_REGISTER_LVT_ERROR          0x370
#define APIC_REGISTER_TIMER_INITCNT      0x380
#define APIC_REGISTER_TIMER_CURRCNT      0x390
#define APIC_REGISTER_TIMER_DIV          0x3E0

/**
 * Spurious interrupt entry flags
 */
#define APIC_SPURIOUS_IVT_SOFTWARE_ENABLE 0x100

/**
 * LVT flags
 */
#define APIC_LVT_DELIVERY_MODE_FIXED  (0 << 8) // 000........
#define APIC_LVT_DELIVERY_MODE_SMI    (2 << 8) // 010........
#define APIC_LVT_DELIVERY_MODE_NMI    (4 << 8) // 100........
#define APIC_LVT_DELIVERY_MODE_INIT   (5 << 8) // 101........
#define APIC_LVT_DELIVERY_MODE_EXTINT (7 << 8) // 111........

#define APIC_LVT_GET_DELIVERY_STATUS(i) ((i >> 12) & 1)

#define APIC_LVT_INT_POLARITY_HIGH (1 << 13)
#define APIC_LVT_INT_POLARITY_LOW  (0 << 13)

#define APIC_LVT_GET_REMOTE_IRR(i) ((i << 14) & 1)

#define APIC_LVT_TRIGGER_MODE_EDGE  (0 << 15)
#define APIC_LVT_TRIGGER_MODE_LEVEL (1 << 15)

#define APIC_LVT_INT_UNMASKED (0 << 16)
#define APIC_LVT_INT_MASKED   (1 << 16)

#define APIC_LVT_TIMER_MODE_ONESHOT      (0 << 17) // 00
#define APIC_LVT_TIMER_MODE_PERIODIC     (1 << 17) // 01
#define APIC_LVT_TIMER_MODE_TSC_DEADLINE (2 << 17) // 10

/**
 * Interrupt command register (ICR) parts
 */
#define APIC_ICR_INTVEC_MAKE (0xFF)

#define APIC_ICR_DELMOD_FIXED  (0 << 8) // 000........
#define APIC_ICR_DELMOD_LOWEST (1 << 8) // 001........
#define APIC_ICR_DELMOD_SMI    (2 << 8) // 010........
#define APIC_ICR_DELMOD_NMI    (4 << 8) // 100........
#define APIC_ICR_DELMOD_INIT   (5 << 8) // 101........
#define APIC_ICR_DELMOD_SIPI   (6 << 8) // 110........

#define APIC_ICR_DESTMOD_PHYSICAL (0 << 11)
#define APIC_ICR_DESTMOD_LOGICAL  (1 << 11)

#define APIC_ICR_DELIVS_IDLE         (0 << 12)
#define APIC_ICR_DELIVS_SEND_PENDING (1 << 12)

#define APIC_ICR_LEVEL_DEASSERT (0 << 14)
#define APIC_ICR_LEVEL_ASSERT   (1 << 14)

#define APIC_ICR_TRIGGERMOD_EDGE  (0 << 15)
#define APIC_ICR_TRIGGERMOD_LEVEL (1 << 15)

#define APIC_ICR_DEST_SHORTHAND_NONE          (0 << 18)
#define APIC_ICR_DEST_SHORTHAND_SELF          (1 << 18)
#define APIC_ICR_DEST_SHORTHAND_ALL_WITH_SELF (2 << 18)
#define APIC_ICR_DEST_SHORTHAND_ALL_NOT_SELF  (3 << 18)

#define APIC_ICR_DESTINATION_MAKE(i) (((uint64_t)i & 0xFF) << 56)

// the APIC timer is configured to tick every 1 milliseconds.
#define APIC_MILLISECONDS_PER_TICK 1

/**
 * Advanced programmable interrupt controller driver
 */
class Lapic {
private:
    /**
     * map virtual and physical memory for the APIC
     */
    static void createMapping();

public:
    /**
     * initialize the apic driver
     */
    static void initialize();

    /**
     * prepare the APIC driver from the lapic physical address
     *
     * @param lapicAddress:		the physical address of the lapic
     */
    static void prepare(PhysAddr lapicAddress);

    /**
     * start the interrupt timer
     */
    static void startTimer();

    /**
     * read the regex provided from virtual base address
     *
     * @param reg:		regex to read
     * @return	the readed byte value
     */
    static uint32_t read(uint32_t reg);

    /**
     * write on the regex provided the provided value
     *
     * @param reg:			regex to write
     * @param value:		value to write
     */
    static void write(uint32_t reg, uint32_t value);

    /**
     * read from APIC_REGISTER_INT_COMMAND_HIGH regex to wait delivery status
     */
    static void waitForIcrSend();

    /**
     * write on the register the End Of Interrupts
     */
    static void sendEoi();

    /**
     * get from regex the id
     *
     * @return the id
     */
    static uint32_t readId();

    /**
     * check driver working state
     *
     * @return whether the driver is prepared
     */
    static bool isPrepared();
};

#endif
