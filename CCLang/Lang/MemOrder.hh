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

enum class MemOrder : int {
    Relaxed        = __ATOMIC_RELAXED,
    Consume        = __ATOMIC_CONSUME,
    Acquire        = __ATOMIC_ACQUIRE,
    Release        = __ATOMIC_RELEASE,
    AcquireRelease = __ATOMIC_ACQ_REL,
    Total          = __ATOMIC_SEQ_CST
};