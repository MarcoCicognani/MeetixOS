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

namespace TC {
namespace Trait {

template<typename T>
concept Tryable = requires(T t) {
                      { !t };
                      { t.unwrap() };
                      { t.backward() };
                  };

} /* namespace Trait */

using Trait::Tryable;

} /* namespace TC */