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

#include <TC/Memory/Shareable.hh>

namespace TC::Memory::Details {

usize PlainRefCountTrait::fetch_add(usize volatile* value, usize addition) {
    usize old_value = load(value);

    *value = old_value + addition;
    return old_value;
}

usize PlainRefCountTrait::fetch_sub(usize volatile* value, usize subtraction) {
    usize old_value = load(value);

    *value = old_value - subtraction;
    return old_value;
}

usize PlainRefCountTrait::load(usize volatile* value) {
    return *value;
}

usize AtomicRefCountTrait::fetch_add(usize volatile* value, usize addition) {
    return __atomic_fetch_add(value, addition, __ATOMIC_SEQ_CST);
}

usize AtomicRefCountTrait::fetch_sub(usize volatile* value, usize subtraction) {
    return __atomic_fetch_sub(value, subtraction, __ATOMIC_SEQ_CST);
}

usize AtomicRefCountTrait::load(usize volatile* value) {
    return __atomic_load_n(value, __ATOMIC_SEQ_CST);
}

} /* namespace TC::Memory::Details */
