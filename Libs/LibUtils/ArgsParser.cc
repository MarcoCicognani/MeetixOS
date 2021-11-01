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

#include <cstdlib>
#include <getopt.h>
#include <iomanip>
#include <iostream>
#include <map>
#include <Utils/ArgsParser.hh>

namespace Utils {

ArgsParser::ArgsParser(const std::string_view& description,
                       u16                     version_major,
                       u16                     version_minor,
                       u16                     version_patch)
    : m_program_description{ description } {
    add_option(m_show_help, "Shows this help", "help", '?');
    add_option(m_show_version, "Shows program version", "version", 'v');

    std::stringstream ss;
    ss << version_major << '.' << version_minor << '.' << version_patch;
    ss >> m_program_version;
}

bool ArgsParser::parse(int argc, const char** argv, FailureBehavior failure_behavior) {
    auto fail = [this, argv, failure_behavior]() {
        if ( failure_behavior == FailureBehavior::PrintUsageAndExit
             || failure_behavior == FailureBehavior::PrintUsage )
            print_usage(std::cerr, argv[0]);
        if ( failure_behavior == FailureBehavior::Exit
             || failure_behavior == FailureBehavior::PrintUsageAndExit )
            exit(EXIT_FAILURE);
    };

    /* tell to getopt to reset his internal state */
    optind   = 0;
    optreset = 1;

    /* construct getopt options parameters */
    auto short_options = std::string{};
    auto long_options  = std::vector<option>{};
    construct_getopt_options(short_options, long_options);

    /* parse the options */
    if ( !parse_options(argc, argv, short_options, long_options) ) {
        fail();
        return false;
    }

    /* print version or help if are requested */
    if ( m_show_version ) {
        print_version(std::cout);
        if ( failure_behavior == FailureBehavior::Exit
             || failure_behavior == FailureBehavior::PrintUsageAndExit )
            exit(EXIT_SUCCESS);
        return false;
    }
    if ( m_show_help ) {
        print_usage(std::cout, argv[0]);
        if ( failure_behavior == FailureBehavior::Exit
             || failure_behavior == FailureBehavior::PrintUsageAndExit )
            exit(EXIT_SUCCESS);
        return false;
    }

    /* count how many positional arguments are present and which the program could accept them */
    u32 values_count_for_arg[m_positional_args.size()];
    if ( !count_cli_positional_arguments(argc, values_count_for_arg) ) {
        fail();
        return false;
    }

    /* accept the positional arguments */
    for ( auto i = 0; i < m_positional_args.size(); ++i ) {
        auto& positional_arg = m_positional_args[i];
        for ( auto c = 0; c < values_count_for_arg[i]; ++c ) {
            auto str_value = argv[optind++];
            if ( !positional_arg.m_accept_value(str_value) ) {
                std::cerr << "Invalid value for argument: " << positional_arg.m_name;
                fail();
                return false;
            }
        }
    }
    return true;
}

void ArgsParser::print_usage(std::ostream& stream, const char* argv0) {
    if ( !m_program_description.empty() )
        stream << m_program_description << " v" << m_program_version << '\n';

    /* compose the usage syntax of the program */
    stream << "Usage:\n\t" << argv0 << ' ';

    /* first the options */
    for ( auto& option : m_options ) {
        if ( option.m_long_name == "help" )
            continue;
        if ( option.m_requires_argument )
            stream << '[' << option.name() << ' ' << option.m_value_name << ']';
        else
            stream << '[' << option.name() << ']';

        stream << ' ';
    }

    /* then the positional arguments */
    for ( auto& positional_arg : m_positional_args ) {
        auto is_required = positional_arg.m_min_values > 0;
        auto is_repeated = positional_arg.m_max_values > 1;

        if ( is_required && is_repeated )
            stream << '<' << positional_arg.m_name << "...>";
        else if ( is_required )
            stream << '<' << positional_arg.m_name << '>';
        else if ( is_repeated )
            stream << '[' << positional_arg.m_name << "...]";
        else
            stream << '[' << positional_arg.m_name << ']';

        stream << ' ';
    }
    stream << std::endl;

    /* construct the list of the options */
    if ( !m_options.empty() )
        stream << "\nOptions:\n";
    for ( auto& option : m_options ) {
        std::stringstream ss;

        auto print_argument = [&]() {
            if ( !option.m_value_name.empty() ) {
                if ( option.m_requires_argument )
                    ss << ' ' << option.m_value_name;
                else
                    ss << " [" << option.m_value_name << ']';
            }
        };

        /* buffer the option modes inside a stringstream to use alignment */
        ss << '\t';
        if ( option.m_short_name ) {
            ss << '-' << option.m_short_name;
            print_argument();
        }
        if ( option.m_short_name && !option.m_long_name.empty() )
            ss << ", ";
        if ( !option.m_long_name.empty() ) {
            ss << "--" << option.m_long_name;
            print_argument();
        }

        /* write the buffered content out to the stream */
        stream << std::setw(35) << std::setfill(' ') << std::left << ss.str();
        if ( !option.m_help_message.empty() )
            stream << std::right << option.m_help_message << std::endl;
    }

    /* as last construct the list of the arguments */
    if ( !m_positional_args.empty() )
        stream << "\nArguments:\n";
    for ( auto& positional_arg : m_positional_args ) {
        stream << '\t' << std::setw(35) << std::setfill(' ') << std::left << positional_arg.m_name;
        if ( !positional_arg.m_help_message.empty() )
            stream << std::right << positional_arg.m_help_message;
        stream << '\n';
    }

    stream << "Compiled with g++ v" << __VERSION__ << " (" << __TIMESTAMP__ << ')' << std::endl;
}

void ArgsParser::print_version(std::ostream& stream) {
    stream << m_program_version << std::endl;
}

void ArgsParser::add_option(ArgsParser::Option&& option) {
    m_options.push_back(std::move(option));
}

void ArgsParser::add_option(bool&       value,
                            const char* help_message,
                            const char* long_name,
                            char        short_name) {
    auto option = Option{ false,
                          std::string_view{ help_message },
                          std::string_view{ long_name },
                          short_name,
                          std::string_view{},
                          [&value](const char* raw_str) {
                              value= !raw_str;
                              return value;
                          } };
    add_option(std::move(option));
}

void ArgsParser::add_option(std::string& value,
                            const char*  help_message,
                            const char*  long_name,
                            char         short_name,
                            const char*  value_name) {
    auto option = Option{ true,
                          std::string_view{ help_message },
                          std::string_view{ long_name },
                          short_name,
                          std::string_view{ value_name },
                          [&value](const char* raw_str) {
                              value= raw_str;
                              return true;
                          } };
    add_option(std::move(option));
}

void ArgsParser::add_option(u32&        value,
                            const char* help_message,
                            const char* long_name,
                            char        short_name,
                            const char* value_name) {
    auto option = Option{ true,
                          std::string_view{ help_message },
                          std::string_view{ long_name },
                          short_name,
                          std::string_view{ value_name },
                          [&value](const char* raw_str) {
                              value= static_cast<u32>(std::strtol(raw_str, nullptr, 10));
                              return true;
                          } };
    add_option(std::move(option));
}

void ArgsParser::add_option(i32&        value,
                            const char* help_message,
                            const char* long_name,
                            char        short_name,
                            const char* value_name) {
    auto option = Option{ true,
                          std::string_view{ help_message },
                          std::string_view{ long_name },
                          short_name,
                          std::string_view{ value_name },
                          [&value](const char* raw_str) {
                              value= static_cast<i32>(std::strtol(raw_str, nullptr, 10));
                              return true;
                          } };
    add_option(std::move(option));
}

void ArgsParser::add_positional_argument(PositionalArgument&& argument) {
    m_positional_args.push_back(std::move(argument));
}

void ArgsParser::add_positional_argument(std::string& value,
                                         const char*  help_message,
                                         const char*  name,
                                         bool         is_required) {
    auto positional_arg = PositionalArgument{ std::string_view{ help_message },
                                              std::string_view{ name },
                                              static_cast<u32>(is_required ? 1 : 0),
                                              1,
                                              [&value](const char* raw_str) {
                                                  value= raw_str;
                                                  return true;
                                              } };
    add_positional_argument(std::move(positional_arg));
}

void ArgsParser::add_positional_argument(u32&        value,
                                         const char* help_message,
                                         const char* name,
                                         bool        is_required) {
    auto positional_arg
        = PositionalArgument{ std::string_view{ help_message },
                              std::string_view{ name },
                              static_cast<u32>(is_required ? 1 : 0),
                              1,
                              [&value](const char* raw_str) {
                                  value = static_cast<u32>(std::strtol(raw_str, nullptr, 10));
                                  return true;
                              } };
    add_positional_argument(std::move(positional_arg));
}

void ArgsParser::add_positional_argument(i32&        value,
                                         const char* help_message,
                                         const char* name,
                                         bool        is_required) {
    auto positional_arg
        = PositionalArgument{ std::string_view{ help_message },
                              std::string_view{ name },
                              static_cast<u32>(is_required ? 1 : 0),
                              1,
                              [&value](const char* raw_str) {
                                  value = static_cast<i32>(std::strtol(raw_str, nullptr, 10));
                                  return true;
                              } };
    add_positional_argument(std::move(positional_arg));
}

void ArgsParser::add_positional_argument(std::vector<std::string>& value,
                                         const char*               help_message,
                                         const char*               name,
                                         bool                      is_required) {
    auto positional_arg = PositionalArgument{ std::string_view{ help_message },
                                              std::string_view{ name },
                                              static_cast<u32>(is_required ? 1 : 0),
                                              INT32_MAX,
                                              [&value](const char* raw_str) {
                                                  value.emplace_back(raw_str);
                                                  return true;
                                              } };
    add_positional_argument(std::move(positional_arg));
}

void ArgsParser::construct_getopt_options(std::string&         short_options,
                                          std::vector<option>& long_options) {
    if ( m_stop_on_first_non_option )
        short_options += '+';

    /* construct the short and long options for the getopt */
    auto index = 0;
    for ( auto& opt : m_options ) {
        /* construct the long option */
        if ( !opt.m_long_name.empty() ) {
            long_options.push_back({ opt.m_long_name.data(),
                                     opt.m_requires_argument ? required_argument : no_argument,
                                     &m_long_option_index,
                                     index++ });
        }

        /* construct the short option */
        if ( opt.m_short_name ) {
            short_options += opt.m_short_name;
            if ( opt.m_requires_argument )
                short_options += ':';
        }
    }
    long_options.push_back({ nullptr, 0, nullptr, 0 });
}

bool ArgsParser::parse_options(int                  argc,
                               const char**         argv,
                               const std::string&   short_options,
                               std::vector<option>& long_options) {
    /* parse the options */
    while ( true ) {
        /* obtain the next option */
        auto option_id
            = getopt_long(argc, argv, short_options.c_str(), long_options.data(), nullptr);
        if ( option_id == EOF )
            break;
        else if ( option_id == '?' )
            return false;

        /* extract the option */
        Option* parser_option;
        if ( !option_id ) {
            /* obtain the reference to the option and reset the index */
            parser_option       = &m_options[m_long_option_index];
            m_long_option_index = -1;
        } else {
            /* find via short name */
            auto it = std::find_if(m_options.begin(), m_options.end(), [option_id](Option& value) {
                return option_id == value.m_short_name;
            });

            parser_option = &*it;
        }

        /* give the argument to the value acceptor */
        auto option_arg = parser_option->m_requires_argument ? optarg : nullptr;
        if ( !parser_option->m_accept_value(option_arg) ) {
            std::cerr << "Invalid value for option " << parser_option->name() << '\n';
            return false;
        }
    }
    return true;
}

bool ArgsParser::count_cli_positional_arguments(int argc, u32 values_count_for_arg[]) {
    /* parse positional arguments */
    auto cli_args_left         = argc - optind;
    auto total_values_required = 0U;
    for ( auto i = 0; i < m_positional_args.size(); ++i ) {
        auto& positional_arg = m_positional_args[i];

        values_count_for_arg[i] = positional_arg.m_min_values;
        total_values_required += positional_arg.m_min_values;
    }

    /* less value that the expected */
    if ( total_values_required > cli_args_left )
        return false;

    /* distribute remaining values */
    auto extra_values_to_distribute = cli_args_left - total_values_required;
    for ( auto i = 0; i < m_positional_args.size(); ++i ) {
        auto& positional_arg = m_positional_args[i];

        /* calculate how many arguments this argument could accept */
        auto extra_values_for_arg
            = std::min(positional_arg.m_max_values - positional_arg.m_min_values,
                       extra_values_to_distribute);

        /* update counters */
        values_count_for_arg[i] += extra_values_for_arg;
        extra_values_to_distribute -= extra_values_for_arg;
        if ( !extra_values_to_distribute )
            break;
    }

    /* too many arguments provided to the program */
    if ( extra_values_to_distribute > 0 )
        return false;
    else
        return true;
}

} /* namespace Utils */