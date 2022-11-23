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

#pragma once

#include <CCLang/Forward.hh>

#include <CCLang/Core/Assertions.hh>
#include <CCLang/Core/Concept.hh>
#include <CCLang/Core/Math.hh>
#include <CCLang/Lang/Array.hh>
#include <CCLang/Lang/Cxx.hh>
#include <CCLang/Lang/DenyCopy.hh>
#include <CCLang/Lang/IntTypes.hh>

namespace Details {

template<typename T>
constexpr bool IsFunctionObject = false;

template<typename TReturn, typename... TArgs>
constexpr bool IsFunctionObject<Function<TReturn(TArgs...)>> = true;

template<typename T, typename TReturn, typename... TArgs>
concept CallableNotFunctor = Callable<T, TReturn, TArgs...> && !IsFunctionObject<T>;

} /* namespace Details */

template<typename TReturn, typename... TArgs>
class Function<TReturn(TArgs...)> final : public DenyCopy {
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
    constexpr auto swap(Function<TReturn(TArgs...)>& rhs) {
        m_inline_storage.swap(rhs);
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
    static constexpr unsigned int INLINE_STORAGE_SIZE = sizeof(void*) * 8;

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
        return Cxx::bit_cast<ICallable*>(m_inline_storage.unwrap());
    }

    alignas(alignof(ICallable)) UnsafeInlineArray<u8, INLINE_STORAGE_SIZE> m_inline_storage;
};

namespace Cxx {

template<typename TReturn, typename... TArgs>
constexpr auto swap(Function<TReturn(TArgs...)>& lhs, Function<TReturn(TArgs...)>& rhs) -> void {
    lhs.swap(rhs);
}

} /* namespace Cxx */
