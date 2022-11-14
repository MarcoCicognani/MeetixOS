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

#include <CCLang/Alloc/RefCounted.hh>
#include <CCLang/Core/Assertions.hh>

auto RefCounted::add_strong_ref() const -> void {
    auto const old_strong_count = __atomic_fetch_add(&m_strong_ref_count, 1, __ATOMIC_RELAXED);
    verify_greater_equal$(old_strong_count, 1);
}
auto RefCounted::remove_strong_ref() const -> void {
    auto const old_strong_count = __atomic_fetch_sub(&m_strong_ref_count, 1, __ATOMIC_SEQ_CST);
    verify_greater_equal$(old_strong_count, 1);

    /* time to free the referenced object */
    if ( old_strong_count - 1 == 0 )
        delete this;
}

auto RefCounted::strong_ref_count() const -> usize {
    return __atomic_load_n(&m_strong_ref_count, __ATOMIC_SEQ_CST);
}

RefCounted::~RefCounted() {
    verify_equal$(strong_ref_count(), 0);
}
