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
    auto const old_strong_count = m_strong_ref_count.atomic_fetch_add(1, MemOrder::Relaxed);
    verify_greater_equal$(old_strong_count, 1);
}
auto RefCounted::remove_strong_ref() const -> void {
    auto const old_strong_count = m_strong_ref_count.atomic_fetch_sub(1, MemOrder::Total);
    verify_greater_equal$(old_strong_count, 1);

    /* time to free the referenced object */
    if ( old_strong_count - 1 == 0 )
        delete this;
}

auto RefCounted::strong_ref_count() const -> usize {
    return m_strong_ref_count.atomic_load(MemOrder::Total);
}

RefCounted::~RefCounted() {
    verify_equal$(strong_ref_count(), 0);
}
