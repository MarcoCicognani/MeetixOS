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

#pragma once

#include <Api/Common.h>
#include <Api/StdInt.h>

__BEGIN_C

/**
 * @brief Address types
 */
typedef usize   Address;
typedef Address PhysAddr;
typedef Address VirtAddr;
typedef usize   FarPtr;

/**
 * @brief Far-pointers management
 */
#define FAR_PTR_SEGMENT(far_ptr) (((FarPtr)(far_ptr)) >> 16)
#define FAR_PTR_OFFSET(far_ptr)  (((FarPtr)(far_ptr)) & 0xFFFF)

/**
 * @brief Far pointer conversion macros
 */
#define SEGMENT_OFFSET_TO_LINEAR(segment, offset)                                                  \
    ((void*)((((segment)&0xFFFF) << 4) + ((offset)&0xFFFF)))
#define FAR_PTR_TO_LINEAR(far_ptr)                                                                 \
    SEGMENT_OFFSET_TO_LINEAR(FAR_PTR_SEGMENT(far_ptr), FAR_PTR_OFFSET(far_ptr))

#define SEGMENT_OFFSET_TO_FAR_PTR(segment, offset)                                                 \
    ((FarPointer)((((segment)&0xFFFF) << 16) | ((offset)&0xFFFF)))
#define LINEAR_TO_FAR_PTR(linear)                                                                  \
    (((linear) > 0x100000) ? 0 : (((((linear) >> 4) & 0xFFFF) << 16) + ((linear)&0xFL)))

__END_C
