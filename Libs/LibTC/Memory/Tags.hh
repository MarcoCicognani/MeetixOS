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
namespace Memory {

enum AdoptTag {
    Adopt
};

enum FromArgsTag {
    FromArgs
};

} /* namespace Memory */

using Memory::AdoptTag;
using Memory::FromArgsTag;
using Memory::AdoptTag::Adopt;
using Memory::FromArgsTag::FromArgs;

} /* namespace TC */