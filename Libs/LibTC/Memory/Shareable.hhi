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

namespace TC::Memory::Details {

template<typename RefCountTrait>
Shareable<RefCountTrait>::~Shareable() {
    VERIFY_EQUAL(RefCountTrait::load(&m_ref_count), 0);
}

template<typename RefCountTrait>
void Shareable<RefCountTrait>::add_ref() const {
    auto old_ref_count = RefCountTrait::fetch_add(&m_ref_count, 1);
    VERIFY_GREATER(old_ref_count, 0);
}

template<typename RefCountTrait>
bool Shareable<RefCountTrait>::dec_ref() const {
    auto old_ref_count = RefCountTrait::fetch_sub(&m_ref_count, 1);
    VERIFY_GREATER_EQUAL(old_ref_count, 1);

    if ( old_ref_count - 1 == 0 ) {
        auto* self = const_cast<Shareable*>(this);
        self->on_no_ref_count();
        delete this;
        return true;
    }
    return false;
}

template<typename RefCountTrait>
usize Shareable<RefCountTrait>::ref_count() const {
    return RefCountTrait::load(&m_ref_count);
}

template<typename RefCountTrait>
void Shareable<RefCountTrait>::on_no_ref_count() {
}

} /* namespace TC::Memory::Details */