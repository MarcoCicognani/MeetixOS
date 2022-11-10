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

#include <CCLang/Alloc/Vector.hh>
#include <CCLang/Collection/StringView.hh>
#include <CCLang/Core/ErrorOr.hh>

/**
 * @brief Custom MeetixOS entry point for userspace applications
 */
auto entry(Vector<StringView> args) -> ErrorOr<void>;