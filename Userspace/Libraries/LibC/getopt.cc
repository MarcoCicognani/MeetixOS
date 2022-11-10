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

#include <LibC/getopt.h>
#include <LibC/stdio.h>
#include <LibC/string.h>
#include <CCLang/Lang/StringView.hh>

/**
 * @brief POSIX says, "When an element of argv[] contains multiple option characters, it is
 * unspecified how getopt() determines which options have already been processed". Well, this is how
 * this implementation do it
 */
static size_t s_multi_option_argument_index = 0;

class OptionsParser {
public:
    /* clang-format off */
    [[nodiscard]]
    static auto construct_from_args(int                  argc,
                                    char const**         argv,
                                    char const*          short_options,
                                    struct option const* long_options,
                                    int*                 out_long_option_index = nullptr) -> OptionsParser {
        return OptionsParser{ argc, argv, StringView::construct_from_cstr(short_options), long_options, out_long_option_index };
    }
    /* clang-format on */

    auto next_option() -> int {
        auto res          = -1;
        auto reorder_argv = !m_stop_on_first_non_option;

        /* find the next option into argv */
        auto opt_found = find_next_option();
        auto arg_view  = StringView::construct_from_cstr(m_argv[m_arg_index]);
        if ( !opt_found ) {
            if ( arg_view == "--"sv )
                m_consumed_arguments = 1;
            else
                m_consumed_arguments = 0;
        } else {
            /* handle the current option */
            if ( arg_view.starts_with("--"sv) )
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

private:
    explicit constexpr OptionsParser(int                  argc,
                                     char const**         argv,
                                     StringView           short_options,
                                     struct option const* long_options,
                                     int*                 out_long_option_index = nullptr)
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

    auto find_next_option() -> bool {
        for ( m_arg_index = g_optind; m_arg_index < m_argc && m_argv[m_arg_index]; ++m_arg_index ) {
            auto arg_view = StringView::construct_from_cstr(m_argv[m_arg_index]);

            /* anything that doesn't start with at least '-' is not an option */
            if ( !arg_view.starts_with('-') ) {
                if ( m_stop_on_first_non_option )
                    return false;
                else
                    continue;
            }

            if ( arg_view == "-"sv ) {
                /* special case: a single '-' is used by GNU programs to refer to stdin */
                continue;
            } else if ( arg_view == "--"sv ) {
                /* special case: a "--" is not an option too, and after this the parser stops */
                return false;
            } else
                return true;
        }

        return false;
    }

    auto handle_long_option() -> int {
        /* reset the optopt to 0 */
        g_optopt = 0;

        /* search for the long option */
        auto* option = lookup_long_option(m_argv[m_arg_index] + 2);
        if ( !option ) {
            fprintf(stderr, "Unrecognized option: %s\n", m_argv[m_arg_index]);
            return '?';
        }

        /* check for argument expects */
        switch ( option->has_arg ) {
            case no_argument:
                /* check whether the optarg has value */
                if ( g_optarg ) {
                    fprintf(stderr, "--%s: Option doesn't accept an argument\n", option->name);
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
                    g_optarg = const_cast<char*>(m_argv[m_arg_index + 1]);

                    m_consumed_arguments = 2;
                } else {
                    fprintf(stderr, "--%s: Missing value for option\n", option->name);
                    return '?';
                }
                break;
            case optional_argument:
                m_consumed_arguments = 1;
                break;
            default:
                fprintf(stderr, "--%s: Option has unrecognized has_arg value\n", option->name);
                return '?';
        }

        /* well now report the option to the caller */
        if ( option->flag ) {
            *option->flag = option->val;
            return 0;
        }
        return option->val;
    }

    auto handle_short_option() -> int {
        auto arg_view = StringView::construct_from_cstr(m_argv[m_arg_index]);

        /* skip '-' character */
        if ( !s_multi_option_argument_index )
            s_multi_option_argument_index = 1;

        /* obtain the next single-char option */
        auto option      = arg_view[s_multi_option_argument_index++];
        auto needs_value = required_argument;

        /* find the short option */
        if ( !lookup_short_option(option, needs_value) ) {
            g_optopt = static_cast<int>(option);
            fprintf(stderr, "Unrecognized option: -%c\n", option);
            return '?';
        }

        /* check for the end of the short arguments list */
        if ( s_multi_option_argument_index < arg_view.len() ) {
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
                fprintf(stderr, "-%c: Missing value for option\n", option);
                return '?';
            }
        }

        return option;
    }

    auto shift_argv() const -> void {
        /* check for nothing to do */
        if ( g_optind == static_cast<int>(m_arg_index) || !m_consumed_arguments )
            return;

        char* buffer[m_consumed_arguments];
        auto  new_argv = const_cast<char**>(m_argv);

        memcpy(buffer, &new_argv[m_arg_index], sizeof(char*) * m_consumed_arguments);
        memmove(&new_argv[optind + m_consumed_arguments], &new_argv[optind], sizeof(char*) * (m_arg_index - optind));
        memcpy(&new_argv[optind], buffer, sizeof(char*) * m_consumed_arguments);
    }

    auto lookup_long_option(char const* raw_option) const -> struct option const* {
        auto arg_view = StringView::construct_from_cstr(raw_option);

        /* search for the long option */
        for ( size_t i = 0; m_long_options[i].name; ++i ) {
            auto& long_option = m_long_options[i];

            /* check for the name. use start_with to handle option=value */
            auto name_view = StringView::construct_from_cstr(long_option.name);
            if ( !arg_view.starts_with(name_view) )
                continue;

            /* write-out the index of the option */
            if ( m_out_long_option_index )
                *m_out_long_option_index = i;

            if ( arg_view.len() == name_view.len() ) {
                /* optarg will be set by handle_long_option() if required argument */
                g_optarg = nullptr;
                return &long_option;
            } else if ( arg_view[name_view.len()] == '=' ) {
                g_optarg = const_cast<char*>(raw_option + name_view.len() + 1);
                return &long_option;
            }
        }
        return nullptr;
    }

    auto lookup_short_option(char option, int& needs_value) const -> bool {
        auto const parts = m_short_options.split_view(option, KeepEmpty::Yes);
        if ( parts.is_empty() || parts.count() < 2 )
            return false;

        /* with two colons means optional argument, only one means required */
        if ( parts[1].starts_with("::"sv) )
            needs_value = optional_argument;
        else if ( parts[1].starts_with(':') )
            needs_value = required_argument;
        else
            needs_value = no_argument;

        return true;
    }

private:
    int                  m_argc;
    char const**         m_argv;
    StringView           m_short_options;
    struct option const* m_long_options;
    int*                 m_out_long_option_index;
    bool                 m_stop_on_first_non_option{ false };
    usize                m_arg_index{ 0 };
    usize                m_consumed_arguments{ 0 };
};

#pragma clang diagnostic push
#pragma ide diagnostic   ignored "modernize-use-trailing-return-type"

extern "C" {

char* g_optarg   = nullptr;
int   g_optind   = 1;
int   g_opterr   = 1;
int   g_optopt   = 0;
int   g_optreset = 0;

int getopt(int argc, char const** argv, char const* short_options) {
    auto null_long_option = option{ nullptr, 0, nullptr, 0 };

    auto options_parser = OptionsParser::construct_from_args(argc, argv, short_options, &null_long_option);
    return options_parser.next_option();
}

int getopt_long(int argc, char const** argv, char const* short_options, struct option* long_options, int* out_long_option_index) {
    auto options_parser = OptionsParser::construct_from_args(argc, argv, short_options, long_options, out_long_option_index);
    return options_parser.next_option();
}

} /* extern "C" */

#pragma clang diagnostic pop
