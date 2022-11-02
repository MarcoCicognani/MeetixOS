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
#pragma ide diagnostic   ignored "bugprone-reserved-identifier"

#include <LibRT/Runtime.hh>

#include <LibApi/Api.h>

#include <LibTC/Alloc/Box.hh>
#include <LibTC/Alloc/StringBuilder.hh>
#include <LibTC/Alloc/Vector.hh>
#include <LibTC/Core/ErrorOr.hh>
#include <LibTC/Lang/Range.hh>
#include <LibTC/Lang/StringView.hh>
#include <LibTC/Text/Format.hh>

template<usize SIZE>
struct StringBuffer {
    char m_inline_buffer[SIZE];
};

extern "C" void (*__init_array_start[])();
extern "C" void (*__init_array_end[])();
extern "C" void (*__preinit_array_start[])();
extern "C" void (*__preinit_array_end[])();
extern "C" void (*__fini_array_start[])();
extern "C" void (*__fini_array_end[])();

extern "C" {

void _init();
void _fini();

} /* extern "C" */

extern "C++" auto cc_main(Vector<StringView>) -> ErrorOr<void>;

namespace RT {

static Function<void(Error const&)> s_runtime_error_catcher = [](Error const& error) {
    auto string_builder = StringBuilder::construct_empty();
    must$(format(string_builder, "\e[31mRuntime Error\e[0m in {}"sv, error));
    s_log(string_builder.as_string_view().as_cstr());
};

auto set_runtime_error_catcher(void (*error_catcher)(Error const&)) -> void {
    s_runtime_error_catcher = error_catcher;
}

/* TODO Module internal partition */

static auto call_global_constructors() -> void {
    /* call pre-init constructors */
    auto const pre_init_array_len = __preinit_array_end - __preinit_array_start;
    for ( auto const i : Range{ 0, pre_init_array_len } )
        (*__preinit_array_start[i])();

    /* call init section */
    _init();

    /* call init constructors */
    auto const init_array_len = __init_array_end - __init_array_start;
    for ( auto const i : Range{ 0, init_array_len } )
        (*__init_array_start[i])();
}

static auto call_global_destructors() -> void {
    /* call all the destructors */
    auto const fini_array_len = __fini_array_end - __fini_array_start;
    for ( auto const i : Range{ 0, fini_array_len } )
        (*__fini_array_start[i])();

    /* call fini section */
    _fini();
}

static auto split_cli_args() -> ErrorOr<Vector<StringView>> {
    /* allocate a buffer which can contain all the arguments and the executable path */
    auto cli_args_buffer_ptr  = try$(Box<StringBuffer<CLIARGS_BUFFER_LENGTH>>::try_construct_from_emplace()).leak_ptr();
    auto exec_path_buffer_ptr = try$(Box<StringBuffer<PATH_MAX>>::try_construct_from_emplace()).leak_ptr();

    /* obtain raw command line string from the kernel */
    auto const cli_args_len  = s_cli_args_release(cli_args_buffer_ptr->m_inline_buffer);
    auto const cli_args_view = StringView::construct_from_raw_parts(cli_args_buffer_ptr->m_inline_buffer, cli_args_len);

    /* obtain the executable path from the kernel */
    auto const exec_path_len  = s_get_executable_path(exec_path_buffer_ptr->m_inline_buffer);
    auto const exec_path_view = StringView::construct_from_raw_parts(exec_path_buffer_ptr->m_inline_buffer, exec_path_len);

    /* split_view the single string arguments into a vector of views and prepend the executable path */
    auto split_cli_args = try$(cli_args_view.try_split_view(' ', KeepEmpty::No));
    try$(split_cli_args.try_prepend(exec_path_view));

    /* split_view the arguments and remove the prefix and the post fix of each argument */
    for ( auto& cli_arg : split_cli_args ) {
        if ( cli_arg.starts_with('"') || cli_arg.starts_with('\'') )
            cli_arg = cli_arg.sub_string_view(1);
        if ( cli_arg.ends_with('"') || cli_arg.ends_with('\'') )
            cli_arg = cli_arg.sub_string_view(0, cli_arg.len() - 2);
    }

    return split_cli_args;
}

static auto run_main() -> ErrorOr<void> {
    try$(cc_main(try$(split_cli_args())));
    return {};
}

[[noreturn]]
static auto terminate(isize code) -> void {
    call_global_destructors();
    s_exit(code);
}

[[noreturn]]
static auto catch_error_and_terminate(Error const& error) -> void {
    s_runtime_error_catcher(error);
    terminate(error.code());
}

[[noreturn]]
auto run() -> void {
    call_global_constructors();

    auto error_or_void = run_main();
    if ( error_or_void.is_error() ) {
        catch_error_and_terminate(error_or_void.unwrap_error());
    }
    terminate(0);
}

} /* namespace RT */

#pragma clang diagnostic pop
