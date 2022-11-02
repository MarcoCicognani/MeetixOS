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

#define TCDenyCopy$(ClassName)                             \
public:                                                    \
    ClassName(ClassName const&)                  = delete; \
    auto operator=(ClassName const&)->ClassName& = delete;
