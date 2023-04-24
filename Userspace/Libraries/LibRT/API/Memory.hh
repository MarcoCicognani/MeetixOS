/**
 * @brief
 * This file is part of the MeetiX Operating System.
 * Copyright (c) 2017-2023, Marco Cicognani (marco.cicognani@meetixos.org)
 *
 * @developers
 * Marco Cicognani (marco.cicognani@meetixos.org)
 *
 * @license
 * GNU General Public License version 3
 */

#pragma once

#include <CCLang/Core/ErrorOr.hh>
#include <CCLang/Lang/Option.hh>

enum class MmMapProt {
    ReadOnly,
    ReadWrite,
    Exec,
};

enum class MmMapMode {
    Private,
    Shared
};

struct MmMapFile {
    i32 m_file_handle;
    u64 m_offset;
    u64 m_len;
};

ErrorOr<void*> s_mm_map(Option<void*> addr, MmMapProt prot, MmMapMode mode, Option<MmMapFile> file);

ErrorOr<void> s_mm_unmap(void* addr, u64 len);