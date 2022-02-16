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

#include <LibTC/Trait/Constant.hh>

namespace TC {
namespace Trait {

using TrueType = Constant<bool, true>;

} /* namespace Trait */

using Trait::TrueType;

} /* namespace TC */