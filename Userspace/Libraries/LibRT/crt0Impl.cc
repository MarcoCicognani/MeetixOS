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

#include <LibApi/Api.h> /* TODO Remove */

#include <LibRT/RT.hh>

#include <LibLS/Alloc/Box.hh>
#include <LibLS/Alloc/StringBuilder.hh>
#include <LibLS/Alloc/Text/Format.hh>
#include <LibLS/Alloc/Vector.hh>
#include <LibLS/Core/ErrorOr.hh>
#include <LibLS/Lang/Range.hh>
#include <LibLS/Lang/StringView.hh>

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

static Function<void(Error const&)> s_runtime_error_catcher = [](Error const& error) {
    auto string_builder = StringBuilder::new_empty();
    must$(format(string_builder, "\e[31mRuntime Error\e[0m in {}"sv, error));
    s_log(string_builder.as_string_view().as_cstr());
};

auto rt_set_runtime_error_catcher(void (*error_catcher)(Error const&)) -> void {
    s_runtime_error_catcher = error_catcher;
}

static auto __rt_call_constructors() -> void {
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

static auto __rt_call_destructors() -> void {
    /* call all the destructors */
    auto const fini_array_len = __fini_array_end - __fini_array_start;
    for ( auto const i : Range{ 0, fini_array_len } )
        (*__fini_array_start[i])();

    /* call fini section */
    _fini();
}

static auto __rt_split_cli_args() -> ErrorOr<Vector<StringView>> {
    auto const fill_string_view_with = [](usize (*syscall)(char*), usize max_len) -> ErrorOr<StringView> {
        auto buffer_ptr = try$(Box<char[]>::try_new_from_len(max_len)).leak_ptr();

        auto const chars_count = syscall(buffer_ptr);
        return StringView::new_from_raw_parts(buffer_ptr, chars_count);
    };

    auto const exec_path_view = try$(fill_string_view_with(s_get_executable_path, PATH_MAX));
    auto const cli_args_view  = try$(fill_string_view_with(s_cli_args_release, CLIARGS_BUFFER_LENGTH));

    /* split_view the single string arguments into a vector of views and prepend the executable path */
    auto split_cli_args = try$(cli_args_view.try_split_view(' ', StringView::KeepEmpty::No));
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

/**
 * @brief Called by <Start.cc> which becomes <crt0.o>
 */
[[noreturn]]
auto __rt_run() -> void {
    auto run_main = []() -> ErrorOr<void> {
        try$(cc_main(try$(__rt_split_cli_args())));
        return {};
    };

    /* initialize the environment then execute the program */
    __rt_call_constructors();
    auto error_or_void = run_main();
    __rt_call_destructors();

    ErrorCode exit_code;
    if ( error_or_void.is_error() ) {
        auto const error = error_or_void.unwrap_error();

        s_runtime_error_catcher(error);
        exit_code = error.code();
    }

    s_exit((i32)exit_code);
}

#pragma clang diagnostic pop
