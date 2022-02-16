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

#include "OptionsParser.hh"

#include <cstring>
#include <iostream>
#include <sstream>

/**
 * @brief POSIX says, "When an element of argv[] contains multiple option characters, it is
 * unspecified how getopt() determines which options have already been processed". Well, this is how
 * this implementation do it
 */
static usize s_multi_option_argument_index = 0;

OptionsParser::OptionsParser(int                     argc,
                             const char**            argv,
                             const std::string_view& short_options,
                             const option*           long_options,
                             int*                    out_long_option_index)
    : m_argc{ argc }
    , m_argv{ argv }
    , m_short_options{ short_options }
    , m_long_options{ long_options }
    , m_out_long_option_index{ out_long_option_index } {
    /* GNU extension to disable argv re-ordering */
    m_stop_on_first_non_option = short_options.starts_with('+');

    /* See if we should reset the internal state */
    if ( g_optreset || !g_optind ) {
        g_optreset = 0;
        g_optind   = 1;

        s_multi_option_argument_index = 0;
    }

    /* reset the other states */
    g_optopt = 0;
    g_optarg = nullptr;
}

int OptionsParser::next_option() {
    auto res          = -1;
    auto reorder_argv = !m_stop_on_first_non_option;

    /* find the next option into argv */
    auto opt_found = find_next_option();
    auto arg_view  = std::string_view{ m_argv[m_arg_index] };
    if ( !opt_found ) {
        if ( arg_view == "--" )
            m_consumed_arguments = 1;
        else
            m_consumed_arguments = 0;
    } else {
        /* handle the current option */
        auto is_long_option = arg_view.starts_with("--");
        if ( is_long_option )
            res = handle_long_option();
        else
            res = handle_short_option();

        /* check for errors, return immediately */
        if ( res == '?' )
            return res;
    }

    /* shift arguments if necessary */
    if ( reorder_argv )
        shift_argv();

    g_optind += static_cast<int>(m_consumed_arguments);
    return res;
}

bool OptionsParser::find_next_option() {
    for ( m_arg_index = g_optind; m_arg_index < m_argc && m_argv[m_arg_index]; ++m_arg_index ) {
        auto arg_view = std::string_view{ m_argv[m_arg_index] };

        /* anything that doesn't start with at least '-' is not an option */
        if ( !arg_view.starts_with('-') ) {
            if ( m_stop_on_first_non_option )
                return false;
            else
                continue;
        }

        if ( arg_view == "-" ) {
            /* special case: a single '-' is used by GNU programs to refer to stdin */
            continue;
        } else if ( arg_view == "--" ) {
            /* special case: a "--" is not an option too, and after this the parser stops */
            return false;
        } else
            return true;
    }

    return false;
}

int OptionsParser::handle_long_option() {
    /* reset the optopt to 0 */
    g_optopt = 0;

    /* search for the long option */
    auto* option = lookup_long_option(m_argv[m_arg_index] + 2);
    if ( !option ) {
        std::cerr << "Unrecognized option: " << m_argv[m_arg_index] << '\n';
        return '?';
    }

    /* check for argument expects */
    switch ( option->has_arg ) {
        case no_argument:
            /* check whether the optarg has value */
            if ( g_optarg ) {
                std::cerr << "--" << option->name << ": Option doesn't accept an argument\n";
                return '?';
            }

            /* only this option is valuated */
            m_consumed_arguments = 1;
            break;
        case required_argument:
            if ( g_optarg ) {
                /* because of --option=value is treated as single argument */
                m_consumed_arguments = 1;
            } else if ( m_arg_index + 1 < m_argc ) {
                /* because of --option value is treated as double argument */
                g_optarg             = const_cast<char*>(m_argv[m_arg_index + 1]);
                m_consumed_arguments = 2;
            } else {
                std::cerr << "--" << option->name << ": Missing value for option\n";
                return '?';
            }
            break;
        case optional_argument:
            m_consumed_arguments = 1;
            break;
        default:
            std::cerr << "--" << option->name << ": Option has unrecognized has_arg value\n";
            return '?';
    }

    /* well now report the option to the caller */
    if ( option->flag ) {
        *option->flag = option->val;
        return 0;
    }
    return option->val;
}

int OptionsParser::handle_short_option() {
    auto arg_view = std::string_view{ m_argv[m_arg_index] };

    /* skip '-' character */
    if ( !s_multi_option_argument_index )
        s_multi_option_argument_index = 1;

    /* obtain the next single-char option */
    auto option      = arg_view[s_multi_option_argument_index++];
    auto needs_value = required_argument;

    /* find the short option */
    auto opt_option_found = lookup_short_option(option, needs_value);
    if ( !opt_option_found ) {
        g_optopt = static_cast<int>(option);
        std::cerr << "Unrecognized option: -" << option << '\n';
        return '?';
    }

    /* check for the end of the short arguments list */
    if ( s_multi_option_argument_index < arg_view.length() ) {
        if ( needs_value == no_argument ) {
            g_optarg             = nullptr;
            m_consumed_arguments = 0;
        } else {
            /* treat the rest of the argument as the value, the "-ovalue" syntax */
            g_optarg = const_cast<char*>(m_argv[m_arg_index] + s_multi_option_argument_index);

            /* next time, process the next argument */
            s_multi_option_argument_index = 0;
            m_consumed_arguments          = 1;
        }
    } else {
        s_multi_option_argument_index = 0;
        if ( needs_value != required_argument ) {
            g_optarg             = nullptr;
            m_consumed_arguments = 1;
        } else if ( m_arg_index + 1 < m_argc ) {
            /* treat the next argument as a value, the "-o value" syntax */
            g_optarg             = const_cast<char*>(m_argv[m_arg_index + 1]);
            m_consumed_arguments = 2;
        } else {
            std::cerr << "-" << option << ": Missing value for option\n";
            return '?';
        }
    }

    return option;
}

void OptionsParser::shift_argv() {
    /* check for nothing to do */
    if ( g_optind == static_cast<int>(m_arg_index) || !m_consumed_arguments )
        return;

    char* buffer[m_consumed_arguments];
    auto  new_argv = const_cast<char**>(m_argv);

    std::memcpy(buffer, &new_argv[m_arg_index], sizeof(char*) * m_consumed_arguments);
    std::memmove(&new_argv[optind + m_consumed_arguments], &new_argv[optind], sizeof(char*) * (m_arg_index - optind));
    std::memcpy(&new_argv[optind], buffer, sizeof(char*) * m_consumed_arguments);
}

const option* OptionsParser::lookup_long_option(const char* raw_option) const {
    auto arg_view = std::string_view{ raw_option };

    /* search for the long option */
    for ( auto i = 0; m_long_options[i].name; ++i ) {
        auto& long_option = m_long_options[i];

        /* check for the name. use start_with to handle option=value */
        auto name_view = std::string_view{ long_option.name };
        if ( !arg_view.starts_with(name_view) )
            continue;

        /* write-out the index of the option */
        if ( m_out_long_option_index )
            *m_out_long_option_index = i;

        if ( arg_view.length() == name_view.length() ) {
            /* optarg will be set by handle_long_option() if required argument */
            g_optarg = nullptr;
            return &long_option;
        } else if ( arg_view[name_view.length()] == '=' ) {
            g_optarg = const_cast<char*>(raw_option + name_view.length() + 1);
            return &long_option;
        }
    }
    return nullptr;
}

bool OptionsParser::lookup_short_option(char option, int& needs_value) const {
    auto parts = split_short_options(option);
    if ( parts.empty() )
        return false;

    /* with two colons means optional argument, only one means required */
    if ( parts[1].starts_with("::") )
        needs_value = optional_argument;
    else if ( parts[1].starts_with(':') )
        needs_value = required_argument;
    else
        needs_value = no_argument;

    return true;
}

[[nodiscard]] std::vector<std::string_view> OptionsParser::split_short_options(char option) const {
    if ( m_short_options.empty() )
        return {};

    auto sub_start = 0;
    auto sub_parts = std::vector<std::string_view>{};
    for ( auto i = 0; i < m_short_options.length(); ++i ) {
        auto c = m_short_options[i];
        if ( c == option ) {
            sub_parts.push_back(m_short_options.substr(sub_start, i - sub_start));
            sub_start = i + 1;
        }
    }

    auto sub_tail = m_short_options.substr(sub_start, m_short_options.length() - sub_start);
    sub_parts.push_back(sub_tail);
    return sub_parts;
}
