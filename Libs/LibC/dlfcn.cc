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

#include <dlfcn.h>
#include <LibTC/Collection/StringView.hh>

static auto const g_dl_error_string_view = "dlfcn not available"sv;

extern "C" {

A_WEAK void* dlopen(const char*, int) {
    return nullptr;
}

A_WEAK void* dlsym(void*, const char*) {
    return nullptr;
}

A_WEAK char* dlerror(void) {
    return const_cast<char*>(g_dl_error_string_view.as_cstr());
}

A_WEAK int dlclose(void*) {
    return -1;
}
}