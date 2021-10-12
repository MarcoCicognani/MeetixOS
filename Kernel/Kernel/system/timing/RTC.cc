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

#include <system/IOPorts.hpp>
#include <system/timing/RTC.hh>

namespace RTC {

enum
{
    PORT_IDX  = 0x70,
    PORT_DATA = 0x71
};

enum
{
    REG_SEC      = 0x0, /* 00-59 */
    REG_MIN      = 0x2, /* 00-59 */
    REG_HOUR     = 0x4, /* 00-23 */
    REG_WEEKDAY  = 0x6, /* 01-07; Sunday=1 */
    REG_MONTHDAY = 0x7, /* 01-31 */
    REG_MONTH    = 0x8, /* 01-12 */
    REG_YEAR     = 0x9, /* 00-99 */
    REG_STATUS_B = 0xB,
};

static bool g_is_initialized = false;

static u8 read_register(u8 reg) {
    IOPorts::writeByte(PORT_IDX, reg);
    asm volatile("nop");
    asm volatile("nop");
    asm volatile("nop");
    return IOPorts::readByte(PORT_DATA);
}

static u32 decode_bcd(u8 value) {
    return (value >> 4) * 10 + (value & 0xF);
}

void init() {
    /* ensure that RTC interrupts are disabled */
    IOPorts::writeByte(PORT_IDX, REG_STATUS_B);  /* select status register */
    auto prev = IOPorts::readByte(PORT_DATA);    /* read the current value of register B */
    IOPorts::writeByte(PORT_IDX, REG_STATUS_B);  /* set the index again */
    IOPorts::writeByte(PORT_DATA, prev & ~0x40); /* disable interrupts */

    g_is_initialized = true;
}

void read(DateTime* date_time) {
    if ( !g_is_initialized )
        init();

    date_time->m_month_day = decode_bcd(read_register(REG_MONTHDAY)) - 1;
    date_time->m_month     = decode_bcd(read_register(REG_MONTH)) - 1;
    date_time->m_year      = decode_bcd(read_register(REG_YEAR));
    if ( date_time->m_year < 70 )
        date_time->m_year += 100;
    date_time->m_hours    = decode_bcd(read_register(REG_HOUR));
    date_time->m_minutes  = decode_bcd(read_register(REG_MIN));
    date_time->m_seconds  = decode_bcd(read_register(REG_SEC));
    date_time->m_week_day = decode_bcd(read_register(REG_WEEKDAY)) - 1;
}

} /* namespace RTC */