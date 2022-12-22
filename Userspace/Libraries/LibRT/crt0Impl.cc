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

#include <LibApi/Api.h>

#include <LibRT/Runtime.hh>

#include <CCLang/Alloc/Box.hh>
#include <CCLang/Alloc/StringBuilder.hh>
#include <CCLang/Alloc/Text/Format.hh>
#include <CCLang/Alloc/Vector.hh>
#include <CCLang/Core/ErrorOr.hh>
#include <CCLang/Lang/Range.hh>
#include <CCLang/Lang/StringView.hh>

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

/**
 * @brief MeetixC++ application entry-point.
 *        Each application which is intended to run under the MeetixOS
 *        environment is expected to define and start from this symbols
 */
extern "C++" auto cc_main(Vector<StringView>) -> ErrorOr<void>;

static Function<void(Error const&)> s_runtime_error_catcher = [](Error const& error) {
    auto string_builder = StringBuilder::empty();
    must$(format(string_builder, "\e[31mRuntime Error\e[0m in {}"sv, error));

    s_log(string_builder.as_string_view().as_cstr());
};

namespace Runtime {

auto set_runtime_error_catcher(void (*error_catcher)(Error const&)) -> void {
    s_runtime_error_catcher = error_catcher;
}

} /* namespace Runtime */

static auto __rt_call_constructors() -> void {
    /* call pre-init constructors */
    auto const pre_init_array_len = usize(__preinit_array_end - __preinit_array_start);
    for ( auto const i : usize::range(0, pre_init_array_len) ) {
        auto const ni = i.unwrap();
        (*__preinit_array_start[ni])();
    }

    /* call init section */
    _init();

    /* call init constructors */
    auto const init_array_len = usize(__init_array_end - __init_array_start);
    for ( auto const i : usize::range(0, init_array_len) ) {
        auto const ni = i.unwrap();
        (*__init_array_start[ni])();
    }
}

static auto __rt_call_destructors() -> void {
    /* call all the destructors */
    auto const fini_array_len = usize(__fini_array_end - __fini_array_start);
    for ( auto const i : usize::range(0, fini_array_len) ) {
        auto const ni = i.unwrap();
        (*__fini_array_start[ni])();
    }

    /* call fini section */
    _fini();
}

static auto __rt_split_cli_args() -> ErrorOr<Vector<StringView>> {
    auto const fill_string_view_with = [](unsigned int (*syscall)(char*), usize max_len) -> ErrorOr<StringView> {
        auto buffer_ptr = try$(Box<char[]>::try_from_len(max_len)).leak_ptr();

        auto const chars_count = syscall(buffer_ptr);
        return StringView::from_raw_parts(buffer_ptr, chars_count);
    };

    auto const exec_path_view = try$(fill_string_view_with(s_get_executable_path, PATH_MAX));
    auto const cli_args_view  = try$(fill_string_view_with(s_cli_args_release, CLIARGS_BUFFER_LENGTH));

    /* split_view the single string arguments into a vector of views and prepend the executable path */
    auto split_cli_args = try$(cli_args_view.try_split_view(' ', StringView::KeepEmpty::No));
    try$(split_cli_args.try_prepend(exec_path_view));

    /* split_view the arguments and remove the prefix and the post fix of each argument */
    for ( auto& cli_arg : split_cli_args ) {
        cli_arg = cli_arg.trim(" \"\'\n\t\v\f\r"sv);
    }
    return split_cli_args;
}

/**
 * @brief Called by <crt0.o> in _start()
 */
[[noreturn]]
auto __rt_run() -> void {
    auto exit_error_code = ErrorCode::None;
    auto run_user_main   = []() -> ErrorOr<void> {
        try$(cc_main(try$(__rt_split_cli_args())));
        return {};
    };

    /* initialize the environment then execute the program */
    __rt_call_constructors();
    auto error_or_void = run_user_main();
    if ( error_or_void.is_error() ) {
        auto const error = error_or_void.unwrap_error();

        /* call the defined runtime error catcher and save the error code */
        s_runtime_error_catcher(error);
        exit_error_code = error.code();
    }
    __rt_call_destructors();

    s_exit((int)exit_error_code);
}

#pragma clang diagnostic pop
