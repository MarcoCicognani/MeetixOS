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

#include <LibLS/Alloc/Vector.hh>
#include <LibLS/Collection/StringView.hh>
#include <LibLS/Core/ErrorOr.hh>

/**
 * @brief Custom MeetixOS entry point for userspace applications
 */
auto entry(Vector<StringView> args) -> ErrorOr<void>;