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

namespace TC::Traits {

template<typename T>
struct RemoveVolatile {
    using Type = T;
};

template<typename T>
struct RemoveVolatile<T volatile> {
    using Type = T;
};

} /* namespace TC::Traits */