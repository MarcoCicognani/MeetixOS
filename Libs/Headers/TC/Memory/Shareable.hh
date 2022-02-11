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

#include <TC/Assertions.hh>
#include <TC/DenyCopy.hh>
#include <TC/DenyMove.hh>
#include <TC/IntTypes.hh>

namespace TC {
namespace Memory::Details {

struct PlainRefCountTrait {
    static usize fetch_add(usize volatile* value, usize addition);
    static usize fetch_sub(usize volatile* value, usize subtraction);
    static usize load(usize volatile* value);
};

class AtomicRefCountTrait {
    static usize fetch_add(usize volatile* value, usize addition);
    static usize fetch_sub(usize volatile* value, usize subtraction);
    static usize load(usize volatile* value);
};

template<typename RefCountTrait>
struct Shareable {
    TC_DENY_COPY(Shareable);
    TC_DENY_MOVE(Shareable);

public:
    /**
     * @brief Constructors
     */
    Shareable() = default;
    virtual ~Shareable();

    /**
     * @brief Reference management
     */
    void add_ref() const;
    bool dec_ref() const;

    /**
     * @brief Getters
     */
    [[nodiscard]] usize ref_count() const;

protected:
    /**
     * @brief Called before delete on no reference left
     */
    virtual void on_no_ref_count();

private:
    usize mutable m_ref_count{ 1 };
};

} /* namespace Memory::Details */

using Shareable       = Memory::Details::Shareable<Memory::Details::PlainRefCountTrait>;
using AtomicShareable = Memory::Details::Shareable<Memory::Details::AtomicRefCountTrait>;

} /* namespace TC */

#include <../LibTC/Memory/Shareable.hhi>