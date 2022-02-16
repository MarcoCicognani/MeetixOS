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

#pragma once

#include <Api/StdInt.h>
#include <Api/Time.h>

namespace RTC {

/**
 * @brief Reads the real-time clock value and fills the given date-time
 */
void read(DateTime* date_time);

} /* namespace RTC */
