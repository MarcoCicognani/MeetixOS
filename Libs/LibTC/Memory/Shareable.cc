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

#include <LibTC/Assertions.hh>
#include <LibTC/Memory/Shareable.hh>

namespace TC::Memory {

Shareable::~Shareable() {
    VERIFY_EQUAL(ref_atomic_load(), 0);
}

void Shareable::add_ref() const {
    auto old_ref_count = ref_atomic_add();
    VERIFY_GREATER(old_ref_count, 0);
}

bool Shareable::dec_ref() const {
    auto old_ref_count = ref_atomic_sub();
    VERIFY_GREATER_EQUAL(old_ref_count, 1);

    if ( old_ref_count - 1 == 0 ) {
        auto* self = const_cast<Shareable*>(this);
        self->on_no_ref_count();
        delete this;
        return true;
    }
    return false;
}

usize Shareable::ref_count() const {
    return ref_atomic_load();
}

void Shareable::on_no_ref_count() {
    /* default implementation */
}

usize Shareable::ref_atomic_load() const {
    return __atomic_load_n(&m_ref_count, __ATOMIC_SEQ_CST);
}

usize Shareable::ref_atomic_add() const {
    return __atomic_fetch_add(&m_ref_count, 1, __ATOMIC_SEQ_CST);
}

usize Shareable::ref_atomic_sub() const {
    return __atomic_fetch_sub(&m_ref_count, 1, __ATOMIC_SEQ_CST);
}

} /* namespace TC::Memory */