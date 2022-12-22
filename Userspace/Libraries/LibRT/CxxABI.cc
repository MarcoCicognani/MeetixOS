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

#pragma clang diagnostic push
#pragma ide diagnostic   ignored "modernize-use-trailing-return-type"
#pragma ide diagnostic   ignored "bugprone-reserved-identifier"

#include <CCLang/Alloc/Vector.hh>
#include <CCLang/Core/RAIIInhert.hh>
#include <CCLang/Lang/Panic.hh>

using AtExitParam    = void*;
using AtExitFunction = void (*)(AtExitParam);

struct AtExitEntry {
    AtExitFunction m_function   = nullptr;
    AtExitParam    m_parameter  = nullptr;
    AtExitParam    m_dso_handle = nullptr;
    bool           m_executed   = false;
};

/* to exclude the vector from being active part of the atexit entries */
static RAIIInert<Vector<AtExitEntry>> s_inert_atexit_entries = Vector<AtExitEntry>::empty();

extern "C" {

int __cxa_atexit(AtExitFunction function, AtExitParam parameter, AtExitParam dso_handle) {
    /* TODO thread locking */
    auto error_or_void = s_inert_atexit_entries.as_ref().try_emplace_last(function, parameter, dso_handle);
    if ( error_or_void.is_error() ) {
        /* TODO logging */
        return -1;
    } else {
        return 0;
    }
}

void __cxa_finalize(void* dso_handle) {
    /* TODO thread locking */
    for ( auto& at_exit_entry : s_inert_atexit_entries.as_ref().reverse_iter() ) {
        auto const needs_calling = !at_exit_entry.m_executed && (dso_handle == nullptr || dso_handle == at_exit_entry.m_dso_handle);
        if ( needs_calling ) {
            at_exit_entry.m_executed = true;
            at_exit_entry.m_function(at_exit_entry.m_parameter);
        }
    }
}

[[noreturn]]
void __cxa_pure_virtual() {
    panic("__cxa_pure_virtual() called..."sv);
}

[[noreturn]]
void __stack_chk_fail() {
    panic("__stack_chk_fail() called..."sv);
}

} /* extern "C" */

#pragma clang diagnostic pop
