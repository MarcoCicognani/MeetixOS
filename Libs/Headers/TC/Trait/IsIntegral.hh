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

#include <TC/Trait/MakeUnsigned.hh>
#include <TC/Trait/RemoveConstVolatile.hh>

namespace TC::Trait {
namespace Details {

template<typename T>
inline constexpr bool IsIntegral = false;

template<>
inline constexpr bool IsIntegral<bool> = true;

template<>
inline constexpr bool IsIntegral<unsigned char> = true;

template<>
inline constexpr bool IsIntegral<char8_t> = true;

template<>
inline constexpr bool IsIntegral<char16_t> = true;

template<>
inline constexpr bool IsIntegral<char32_t> = true;

template<>
inline constexpr bool IsIntegral<unsigned short> = true;

template<>
inline constexpr bool IsIntegral<unsigned int> = true;

template<>
inline constexpr bool IsIntegral<unsigned long> = true;

template<>
inline constexpr bool IsIntegral<unsigned long long> = true;

} /* namespace Details */

template<typename T>
inline constexpr bool IsIntegral = Details::IsIntegral<MakeUnsigned<RemoveConstVolatile<T>>>;

} /* namespace TC::Trait */