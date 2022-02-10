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
    static inline usize fetch_add(usize volatile* value, usize addition) {
        auto old_value = load(value);

        *value = old_value + addition;
        return old_value;
    }

    static inline usize fetch_sub(usize volatile* value, usize subtraction) {
        auto old_value = load(value);

        *value = old_value - subtraction;
        return old_value;
    }

    static inline usize load(usize volatile* value) {
        return *value;
    }
};

class AtomicRefCountTrait {
    static inline usize fetch_add(usize volatile* value, usize addition) {
        return __atomic_fetch_add(value, addition, __ATOMIC_SEQ_CST);
    }

    static inline usize fetch_sub(usize volatile* value, usize subtraction) {
        return __atomic_fetch_sub(value, subtraction, __ATOMIC_SEQ_CST);
    }

    static inline usize load(usize volatile* value) {
        return __atomic_load_n(value, __ATOMIC_SEQ_CST);
    }
};

template<typename RefCountTrait>
struct Shareable {
    TC_DENY_COPY(Shareable);
    TC_DENY_MOVE(Shareable);

public:
    Shareable() = default;
    virtual ~Shareable();

    void add_ref() const;
    bool dec_ref() const;

    [[nodiscard]] usize ref_count() const;

protected:
    virtual void on_no_ref_count();

private:
    usize mutable m_ref_count{ 1 };
};

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

} /* namespace Memory::Details */

using Shareable       = Memory::Details::Shareable<Memory::Details::PlainRefCountTrait>;
using AtomicShareable = Memory::Details::Shareable<Memory::Details::AtomicRefCountTrait>;

} /* namespace TC */