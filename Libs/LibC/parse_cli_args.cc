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

#include <Api.h>
#include <LibTC/Collection/StringView.hh>
#include <LibTC/Collection/Vector.hh>
#include <LibTC/Functional/Try.hh>
#include <LibTC/Memory/Box.hh>

static auto s_cli_arguments_vector = Vector<char const*>::construct_empty();

template<usize SIZE>
struct StringBuffer {
    char m_inline_buffer[SIZE];
};

auto parse_cli_args(i32& out_argc, char**& out_args) -> ErrorOr<void> {
    /* allocate a buffer which can contain all the arguments and the executable path */
    auto cli_args_buffer_ptr  = TRY(Box<StringBuffer<CLIARGS_BUFFER_LENGTH>>::try_construct_from_args()).leak_ptr();
    auto exec_path_buffer_ptr = TRY(Box<StringBuffer<PATH_MAX>>::try_construct_from_args()).leak_ptr();

    /* obtain raw command line string from the kernel */
    auto const cli_args_len  = s_cli_args_release(cli_args_buffer_ptr->m_inline_buffer);
    auto const cli_args_view = StringView{ cli_args_buffer_ptr->m_inline_buffer, cli_args_len };

    /* obtain the executable path from the kernel */
    auto const exec_path_len  = s_get_executable_path(exec_path_buffer_ptr->m_inline_buffer);
    auto const exec_path_view = StringView{ exec_path_buffer_ptr->m_inline_buffer, exec_path_len };

    /* split_view the single string arguments into a vector of views and prepend the executable path */
    auto split_args = TRY(cli_args_view.try_split_view(' ', KeepEmpty::No));
    TRY(split_args.try_prepend(exec_path_view));

    /* split_view the arguments and remove the prefix and the post fix of each argument */
    auto split_args_pointers = TRY(Vector<char const*>::try_construct_with_capacity(split_args.count()));
    for ( auto const& arg : split_args ) {
        auto fixed_arg = arg;
        if ( fixed_arg.starts_with('"') || fixed_arg.starts_with('\'') )
            fixed_arg = fixed_arg.sub_string_view(1);
        if ( fixed_arg.ends_with('"') || fixed_arg.ends_with('\'') )
            fixed_arg = fixed_arg.sub_string_view(0, fixed_arg.len() - 2);

        *const_cast<char*>(fixed_arg.as_cstr() + fixed_arg.len()) = '\0'; /* TODO Find a better way to do this */

        /* put into the arguments vector of pointers */
        TRY(split_args_pointers.try_append(fixed_arg.as_cstr()));
    }

    /* ensure the allocated memory of the arguments remains alive */
    s_cli_arguments_vector = Cxx::move(split_args_pointers);

    out_argc = static_cast<i32>(s_cli_arguments_vector.count());
    out_args = const_cast<char**>(s_cli_arguments_vector.raw_data());
    return {};
}
