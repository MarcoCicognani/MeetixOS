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

#include <LibTC/Forward.hh>

#include <LibTC/Assertions.hh>
#include <LibTC/BitCast.hh>
#include <LibTC/Collection/Range.hh>
#include <LibTC/Concept.hh>
#include <LibTC/Cxx.hh>
#include <LibTC/DenyCopy.hh>
#include <LibTC/Math.hh>

namespace TC {
namespace Collection {
namespace Details {

template<typename T>
constexpr bool IsFunctionObject = false;

template<typename TReturn, typename... TArgs>
constexpr bool IsFunctionObject<Function<TReturn(TArgs...)>> = true;

template<typename T, typename TReturn, typename... TArgs>
concept CallableNotFunction = (Callable<T, TReturn, TArgs...> && !IsFunctionObject<T>);

} /* namespace Details */

template<typename TReturn, typename... TArgs>
class Function<TReturn(TArgs...)> {
private:
    TC_DENY_COPY(Function);

public:
    /**
     * @brief Constructors
     */
    template<Details::CallableNotFunction<TReturn, TArgs...> TCallable>
    constexpr explicit(false) Function(TCallable callable) noexcept {
        static_assert(sizeof(TCallable) <= sizeof(m_inline_storage), "Function<> TCallable cannot be stored into inline storage");

        new (m_inline_storage) CallableWrapper<TCallable>{ Cxx::forward<TCallable>(callable) };
    }
    Function(Function<TReturn(TArgs...)>&& rhs) noexcept {
        rhs.storage_as_callable()->move_this_to(m_inline_storage);
    }

    ~Function() = default;

    template<Details::CallableNotFunction<TReturn, TArgs...> TCallable>
    auto operator=(TCallable&& callable) noexcept -> Function<TReturn(TArgs...)>& {
        Function function{ callable };
        swap(function);
        return *this;
    }
    auto operator=(Function<TReturn(TArgs...)>&& rhs) noexcept -> Function<TReturn(TArgs...)>& {
        Function function{ Cxx::move(rhs) };
        swap(function);
        return *this;
    }

    /**
     * @brief Swaps in O(1) the content of this List with another
     */
    auto swap(Function<TReturn(TArgs...)>& rhs) noexcept {
        for ( auto const i : Range{ 0u, C_INLINE_STORAGE_SIZE } )
            Cxx::swap(m_inline_storage[i], rhs.m_inline_storage[i]);
    }

    /**
     * @brief Calls the stored lambda with the given arguments
     * @note Despite the operator is marked const, the lambda can still modify his captures
     */
    [[gnu::always_inline]] auto operator()(TArgs... args) const -> TReturn {
        return storage_as_callable()->invoke(Cxx::forward<TArgs>(args)...);
    }

private:
    static constexpr usize C_INLINE_STORAGE_SIZE = 6 * sizeof(void*);

    class ICallable {
    public:
        virtual ~ICallable() = default;

        virtual auto invoke(TArgs...) -> TReturn = 0;
        virtual auto move_this_to(u8*) -> void   = 0;
    };

    template<typename TCallable>
    class CallableWrapper : public ICallable {
    public:
        explicit constexpr CallableWrapper(TCallable&& callable)
            : m_callable{ Cxx::forward<TCallable>(callable) } {
        }
        ~CallableWrapper() final = default;

        [[gnu::always_inline]] auto invoke(TArgs... args) -> TReturn final {
            return m_callable(Cxx::forward<TArgs>(args)...);
        }

        auto move_this_to(u8* destination) -> void final {
            new (destination) CallableWrapper{ Cxx::forward<TCallable>(m_callable) };
        }

    private:
        TCallable m_callable;
    };

    [[gnu::always_inline]] auto storage_as_callable() const -> ICallable* {
        return bit_cast<ICallable*>(&m_inline_storage);
    }

    alignas(alignof(ICallable)) u8 m_inline_storage[C_INLINE_STORAGE_SIZE]{};
};

} /* namespace Collection */

using Collection::Function;

} /* namespace TC */