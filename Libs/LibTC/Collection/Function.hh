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

#include "Range.hh"
#include <LibTC/Assertions.hh>
#include <LibTC/BitCast.hh>
#include <LibTC/Cxx.hh>
#include <LibTC/DenyCopy.hh>
#include <LibTC/Math.hh>
#include <LibTC/Trait/IsCallable.hh>

namespace TC {
namespace Collection {

template<typename>
class Function;

template<typename TReturn, typename... TArgs>
class Function<TReturn(TArgs...)> {
    TC_DENY_COPY(Function);

public:
    /**
     * @brief Constructors
     */
    Function() = delete;
    template<Callable TCallable>
    Function(TCallable&& callable) {
        VERIFY_GREATER_EQUAL(sizeof(TCallable), sizeof(m_inline_storage));

        new (m_inline_storage) CallableWrapper<TCallable>{ forward<TCallable>(callable) };
    }
    Function(Function&& rhs) noexcept {
        rhs.callable_wrapper()->move_to_other(m_inline_storage);
    }

    template<Callable TCallable>
    Function& operator=(TCallable&& callable) {
        Function func{ callable };
        swap(func);
        return *this;
    }
    Function& operator=(Function&& function) noexcept {
        Function func{ move(function) };
        swap(func);
        return *this;
    }

    /**
     * @brief Swaps in O(1) the content of this List with another
     */
    void swap(Function& rhs) noexcept {
        for ( usize i : Range{ 0u, C_INLINE_STORAGE_SIZE } )
            Cxx::swap(m_inline_storage[i], rhs.m_inline_storage[i]);
    }

    TReturn operator()(TArgs&&... args) {
        callable_wrapper()->operator()(forward<TArgs>(args)...);
    }

private:
    static constexpr usize C_INLINE_STORAGE_SIZE = 16 * sizeof(void*);

    class ICallable {
    public:
        virtual ~ICallable() = default;

        virtual void move_to_other(u8*) = 0;

        virtual TReturn operator()(TArgs&&...) = 0;
    };

    template<typename TCallable>
    class CallableWrapper : public ICallable {
    public:
        CallableWrapper(TCallable&& callable)
            : m_callable{ move(callable) } {
        }
        ~CallableWrapper() final = default;

        void move_to_other(u8* destination) final {
            new (destination) CallableWrapper<TCallable>{ move(m_callable) };
        }

        TReturn operator()(TArgs&&... args) final {
            return m_callable(forward<TArgs>(args)...);
        }

    private:
        TCallable m_callable;
    };

    ICallable* callable_wrapper() const {
        return bit_cast<ICallable*>(&m_inline_storage);
    }

    alignas(alignof(ICallable)) u8 m_inline_storage[C_INLINE_STORAGE_SIZE];
};

} /* namespace Collection */

using Collection::Function;

} /* namespace TC */