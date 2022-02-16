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

#define TC_DENY_MOVE(ClassName)                                                                                        \
                                                                                                                       \
public:                                                                                                                \
    ClassName(ClassName&&) = delete;                                                                                   \
    ClassName& operator=(ClassName&&) = delete;
