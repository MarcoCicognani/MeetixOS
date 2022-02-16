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

#include <LibTC/Hashing/Integer.hh>
#include <LibTC/Hashing/Pointer.hh>

namespace TC::Hashing {

usize pointer_hash(void const* value) {
    return integer_hash(reinterpret_cast<u64>(value));
}

} /* namespace TC::Hashing */