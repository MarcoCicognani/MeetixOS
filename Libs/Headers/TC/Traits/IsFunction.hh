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

#include <TC/Traits/FalseType.hh>
#include <TC/Traits/TrueType.hh>

namespace TC::Traits {

template<typename>
struct IsFunction : FalseType {
    /* Empty Body */
};

template<typename Ret, typename... Args>
struct IsFunction<Ret(Args...)> : TrueType {
    /* Empty Body */
};

template<typename Ret, typename... Args>
struct IsFunction<Ret(Args..., ...)> : TrueType {
    /* Empty Body */
};

template<typename Ret, typename... Args>
struct IsFunction<Ret(Args...) const> : TrueType {
    /* Empty Body */
};

template<typename Ret, typename... Args>
struct IsFunction<Ret(Args..., ...) const> : TrueType {
    /* Empty Body */
};

template<typename Ret, typename... Args>
struct IsFunction<Ret(Args...) volatile> : TrueType {
    /* Empty Body */
};

template<typename Ret, typename... Args>
struct IsFunction<Ret(Args..., ...) volatile> : TrueType {
    /* Empty Body */
};

template<typename Ret, typename... Args>
struct IsFunction<Ret(Args...) const volatile> : TrueType {
    /* Empty Body */
};

template<typename Ret, typename... Args>
struct IsFunction<Ret(Args..., ...) const volatile> : TrueType {
    /* Empty Body */
};

template<typename Ret, typename... Args>
struct IsFunction<Ret(Args...)&> : TrueType {
    /* Empty Body */
};

template<typename Ret, typename... Args>
struct IsFunction<Ret(Args..., ...)&> : TrueType {
    /* Empty Body */
};

template<typename Ret, typename... Args>
struct IsFunction<Ret(Args...) const&> : TrueType {
    /* Empty Body */
};

template<typename Ret, typename... Args>
struct IsFunction<Ret(Args..., ...) const&> : TrueType {
    /* Empty Body */
};

template<typename Ret, typename... Args>
struct IsFunction<Ret(Args...) volatile&> : TrueType {
    /* Empty Body */
};

template<typename Ret, typename... Args>
struct IsFunction<Ret(Args..., ...) volatile&> : TrueType {
    /* Empty Body */
};

template<typename Ret, typename... Args>
struct IsFunction<Ret(Args...) const volatile&> : TrueType {
    /* Empty Body */
};

template<typename Ret, typename... Args>
struct IsFunction<Ret(Args..., ...) const volatile&> : TrueType {
    /* Empty Body */
};

template<typename Ret, typename... Args>
struct IsFunction<Ret(Args...) &&> : TrueType {
    /* Empty Body */
};

template<typename Ret, typename... Args>
struct IsFunction<Ret(Args..., ...) &&> : TrueType {
    /* Empty Body */
};

template<typename Ret, typename... Args>
struct IsFunction<Ret(Args...) const&&> : TrueType {
    /* Empty Body */
};

template<typename Ret, typename... Args>
struct IsFunction<Ret(Args..., ...) const&&> : TrueType {
    /* Empty Body */
};

template<typename Ret, typename... Args>
struct IsFunction<Ret(Args...) volatile&&> : TrueType {
    /* Empty Body */
};

template<typename Ret, typename... Args>
struct IsFunction<Ret(Args..., ...) volatile&&> : TrueType {
    /* Empty Body */
};

template<typename Ret, typename... Args>
struct IsFunction<Ret(Args...) const volatile&&> : TrueType {
    /* Empty Body */
};

template<typename Ret, typename... Args>
struct IsFunction<Ret(Args..., ...) const volatile&&> : TrueType {
    /* Empty Body */
};

} /* namespace TC::Traits */