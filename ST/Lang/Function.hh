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

#include <ST/Forward.hh>

#include <ST/Core/Assertions.hh>
#include <ST/Core/Concept.hh>
#include <ST/Core/Math.hh>
#include <ST/Lang/Cxx.hh>
#include <ST/Lang/DenyCopy.hh>
#include <ST/Lang/Range.hh>

namespace Details {

template<typename T>
constexpr bool IsFunctionObject = false;

template<typename TReturn, typename... TArgs>
constexpr bool IsFunctionObject<Function<TReturn(TArgs...)>> = true;

template<typename T, typename TReturn, typename... TArgs>
concept CallableNotFunctor = Callable<T, TReturn, TArgs...> && !IsFunctionObject<T>;

} /* namespace Details */

template<typename TReturn, typename... TArgs>
class Function<TReturn(TArgs...)> final {
    TCDenyCopy$(Function);

public:
    /**
     * @brief Constructors
     */
    template<Details::CallableNotFunctor<TReturn, TArgs...> TCallable>
    constexpr explicit(false) Function(TCallable callable) {
        static_assert(sizeof(TCallable) <= sizeof(m_inline_storage), "Function<TCallable> Cannot be stored into inline storage");

        new (m_inline_storage) CallableWrapper<TCallable>{ Cxx::move(callable) };
    }
    Function(Function<TReturn(TArgs...)>&& rhs) {
        rhs.storage_as_callable()->move_this_to(m_inline_storage);
    }

    ~Function() = default;

    template<Details::CallableNotFunctor<TReturn, TArgs...> TCallable>
    auto operator=(TCallable&& callable) -> Function<TReturn(TArgs...)>& {
        Function function{ callable };
        swap(function);
        return *this;
    }
    auto operator=(Function<TReturn(TArgs...)>&& rhs) -> Function<TReturn(TArgs...)>& {
        Function function{ Cxx::move(rhs) };
        swap(function);
        return *this;
    }

    /**
     * @brief Swaps in O(1) the content of this List with another
     */
    auto swap(Function<TReturn(TArgs...)>& rhs) {
        for ( auto const i : Range{ 0u, INLINE_STORAGE_SIZE } )
            Cxx::swap(m_inline_storage[i], rhs.m_inline_storage[i]);
    }

    /**
     * @brief Calls the stored lambda with the given arguments
     * @note Despite the operator is marked const, the lambda can still modify his captures
     */
    [[gnu::always_inline]]
    auto operator()(TArgs... args) const -> TReturn {
        return storage_as_callable()->invoke(Cxx::forward<TArgs>(args)...);
    }

private:
    static constexpr usize INLINE_STORAGE_SIZE = sizeof(void*) * 8;

    class ICallable {
    public:
        virtual ~ICallable() = default;

        virtual auto invoke(TArgs&&...) -> TReturn = 0;
        virtual auto move_this_to(u8*) -> void     = 0;
    };

    template<typename TCallable>
    class CallableWrapper final : public ICallable {
    public:
        explicit constexpr CallableWrapper(TCallable callable)
            : m_callable{ Cxx::move(callable) } {
        }
        ~CallableWrapper() override = default;

        [[gnu::always_inline]]
        auto invoke(TArgs&&... args) -> TReturn override {
            return m_callable(Cxx::forward<TArgs>(args)...);
        }

        auto move_this_to(u8* destination) -> void override {
            new (destination) CallableWrapper{ Cxx::move(m_callable) };
        }

    private:
        TCallable m_callable;
    };

    [[nodiscard]]
    constexpr auto storage_as_callable() const -> ICallable* {
        return Cxx::bit_cast<ICallable*>(&m_inline_storage);
    }

    alignas(alignof(ICallable)) u8 m_inline_storage[INLINE_STORAGE_SIZE]{};
};
