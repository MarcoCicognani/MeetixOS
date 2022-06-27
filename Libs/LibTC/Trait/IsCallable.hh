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

namespace TC {
namespace Trait {

template<typename>
inline constexpr bool IsCallable = false;

template<typename Ret, typename... Args>
inline constexpr bool IsCallable<Ret(Args...)> = true;

template<typename Ret, typename... Args>
inline constexpr bool IsCallable<Ret(Args..., ...)> = true;

template<typename Ret, typename... Args>
inline constexpr bool IsCallable<Ret(Args...) const> = true;

template<typename Ret, typename... Args>
inline constexpr bool IsCallable<Ret(Args..., ...) const> = true;

template<typename Ret, typename... Args>
inline constexpr bool IsCallable<Ret(Args...) volatile> = true;

template<typename Ret, typename... Args>
inline constexpr bool IsCallable<Ret(Args..., ...) volatile> = true;

template<typename Ret, typename... Args>
inline constexpr bool IsCallable<Ret(Args...) const volatile> = true;

template<typename Ret, typename... Args>
inline constexpr bool IsCallable<Ret(Args..., ...) const volatile> = true;

template<typename Ret, typename... Args>
inline constexpr bool IsCallable<Ret(Args...)&> = true;

template<typename Ret, typename... Args>
inline constexpr bool IsCallable<Ret(Args..., ...)&> = true;

template<typename Ret, typename... Args>
inline constexpr bool IsCallable<Ret(Args...) const&> = true;

template<typename Ret, typename... Args>
inline constexpr bool IsCallable<Ret(Args..., ...) const&> = true;

template<typename Ret, typename... Args>
inline constexpr bool IsCallable<Ret(Args...) volatile&> = true;

template<typename Ret, typename... Args>
inline constexpr bool IsCallable<Ret(Args..., ...) volatile&> = true;

template<typename Ret, typename... Args>
inline constexpr bool IsCallable<Ret(Args...) const volatile&> = true;

template<typename Ret, typename... Args>
inline constexpr bool IsCallable<Ret(Args..., ...) const volatile&> = true;

template<typename Ret, typename... Args>
inline constexpr bool IsCallable<Ret(Args...) &&> = true;

template<typename Ret, typename... Args>
inline constexpr bool IsCallable<Ret(Args..., ...) &&> = true;

template<typename Ret, typename... Args>
inline constexpr bool IsCallable<Ret(Args...) const&&> = true;

template<typename Ret, typename... Args>
inline constexpr bool IsCallable<Ret(Args..., ...) const&&> = true;

template<typename Ret, typename... Args>
inline constexpr bool IsCallable<Ret(Args...) volatile&&> = true;

template<typename Ret, typename... Args>
inline constexpr bool IsCallable<Ret(Args..., ...) volatile&&> = true;

template<typename Ret, typename... Args>
inline constexpr bool IsCallable<Ret(Args...) const volatile&&> = true;

template<typename Ret, typename... Args>
inline constexpr bool IsCallable<Ret(Args..., ...) const volatile&&> = true;

template<typename TReturn, typename... TArgs>
concept Callable = IsCallable<TReturn(TArgs...)>;

} /* namespace Trait */

using Trait::IsCallable;
using Trait::Callable;

} /* namespace TC */