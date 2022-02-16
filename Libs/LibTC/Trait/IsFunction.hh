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
inline constexpr bool IsFunction = false;

template<typename Ret, typename... Args>
inline constexpr bool IsFunction<Ret(Args...)> = true;

template<typename Ret, typename... Args>
inline constexpr bool IsFunction<Ret(Args..., ...)> = true;

template<typename Ret, typename... Args>
inline constexpr bool IsFunction<Ret(Args...) const> = true;

template<typename Ret, typename... Args>
inline constexpr bool IsFunction<Ret(Args..., ...) const> = true;

template<typename Ret, typename... Args>
inline constexpr bool IsFunction<Ret(Args...) volatile> = true;

template<typename Ret, typename... Args>
inline constexpr bool IsFunction<Ret(Args..., ...) volatile> = true;

template<typename Ret, typename... Args>
inline constexpr bool IsFunction<Ret(Args...) const volatile> = true;

template<typename Ret, typename... Args>
inline constexpr bool IsFunction<Ret(Args..., ...) const volatile> = true;

template<typename Ret, typename... Args>
inline constexpr bool IsFunction<Ret(Args...)&> = true;

template<typename Ret, typename... Args>
inline constexpr bool IsFunction<Ret(Args..., ...)&> = true;

template<typename Ret, typename... Args>
inline constexpr bool IsFunction<Ret(Args...) const&> = true;

template<typename Ret, typename... Args>
inline constexpr bool IsFunction<Ret(Args..., ...) const&> = true;

template<typename Ret, typename... Args>
inline constexpr bool IsFunction<Ret(Args...) volatile&> = true;

template<typename Ret, typename... Args>
inline constexpr bool IsFunction<Ret(Args..., ...) volatile&> = true;

template<typename Ret, typename... Args>
inline constexpr bool IsFunction<Ret(Args...) const volatile&> = true;

template<typename Ret, typename... Args>
inline constexpr bool IsFunction<Ret(Args..., ...) const volatile&> = true;

template<typename Ret, typename... Args>
inline constexpr bool IsFunction<Ret(Args...) &&> = true;

template<typename Ret, typename... Args>
inline constexpr bool IsFunction<Ret(Args..., ...) &&> = true;

template<typename Ret, typename... Args>
inline constexpr bool IsFunction<Ret(Args...) const&&> = true;

template<typename Ret, typename... Args>
inline constexpr bool IsFunction<Ret(Args..., ...) const&&> = true;

template<typename Ret, typename... Args>
inline constexpr bool IsFunction<Ret(Args...) volatile&&> = true;

template<typename Ret, typename... Args>
inline constexpr bool IsFunction<Ret(Args..., ...) volatile&&> = true;

template<typename Ret, typename... Args>
inline constexpr bool IsFunction<Ret(Args...) const volatile&&> = true;

template<typename Ret, typename... Args>
inline constexpr bool IsFunction<Ret(Args..., ...) const volatile&&> = true;

} /* namespace Trait */

using Trait::IsFunction;

} /* namespace TC */