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

#pragma once

#include <functional>
#include <getopt.h>
#include <ostream>
#include <sstream>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace Utils {

/**
 * @brief Command-line argument parser
 */
class ArgsParser {
public:
    /**
     * @brief Describes how the parser must act when encounters an error
     */
    enum class FailureBehavior
    {
        PrintUsageAndExit,
        PrintUsage,
        Exit,
        Ignore
    };

    /**
     * @brief Optional argument descriptor. It cannot have an argument
     */
    struct Option {
    public:
        bool                             m_requires_argument{ true };
        std::string_view                 m_help_message{};
        std::string_view                 m_long_name{};
        char                             m_short_name{};
        std::string_view                 m_value_name{};
        std::function<bool(const char*)> m_accept_value;

        /**
         * @brief Returns the option name to display
         */
        [[nodiscard]] std::string name() const {
            std::stringstream ss;
            if ( !m_long_name.empty() ) {
                ss << "--" << m_long_name;
            } else {
                ss << '-' << m_short_name;
            }
            return ss.str();
        }
    };

    /**
     * @brief Optional argument descriptor. It may have an argument
     */
    struct PositionalArgument {
    public:
        std::string_view                 m_help_message{};
        std::string_view                 m_name{};
        u32                              m_min_values{ 0 };
        u32                              m_max_values{ 1 };
        std::function<bool(const char*)> m_accept_value;
    };

public:
    /**
     * @brief Constructors
     */
    ArgsParser(const std::string_view& description,
               u16                     version_major,
               u16                     version_minor,
               u16                     version_patch);

    /**
     * @brief Parses the command line given
     */
    bool parse(int             argc,
               const char**    argv,
               FailureBehavior failure_behavior = FailureBehavior::PrintUsageAndExit);

    /**
     * @brief Prints the program usage on the given stream
     */
    void print_usage(std::ostream& stream, const char* argv0);

    /**
     * @brief Prints the program version on the given stream
     */
    void print_version(std::ostream& stream);

    /**
     * @brief Adds an option to the parser
     */
    void add_option(Option&& option);
    void add_option(bool& value, const char* help_message, const char* long_name, char short_name);
    void add_option(std::string& value,
                    const char*  help_message,
                    const char*  long_name,
                    char         short_name,
                    const char*  value_name);
    void add_option(u32&        value,
                    const char* help_message,
                    const char* long_name,
                    char        short_name,
                    const char* value_name);
    void add_option(i32&        value,
                    const char* help_message,
                    const char* long_name,
                    char        short_name,
                    const char* value_name);

    /**
     * @brief Adds a positional argument to the parser
     */
    void add_positional_argument(PositionalArgument&& argument);
    void add_positional_argument(std::string& value,
                                 const char*  help_message,
                                 const char*  name,
                                 bool         is_required = true);
    void add_positional_argument(u32&        value,
                                 const char* help_message,
                                 const char* name,
                                 bool        is_required = true);
    void add_positional_argument(i32&        value,
                                 const char* help_message,
                                 const char* name,
                                 bool        is_required = true);
    void add_positional_argument(std::vector<std::string>& value,
                                 const char*               help_message,
                                 const char*               name,
                                 bool                      is_required = true);

    /**
     * @brief Sets whether the parser must stop on error
     */
    void set_stop_on_first_non_option(bool stop_on_first_non_option) {
        m_stop_on_first_non_option = stop_on_first_non_option;
    }

private:
    void construct_getopt_options(std::string& short_options, std::vector<option>& long_options);
    bool parse_options(int                  argc,
                       const char**         argv,
                       const std::string&   short_options,
                       std::vector<option>& long_options);
    bool count_cli_positional_arguments(int argc, u32 values_count_for_arg[]);

private:
    std::vector<Option>             m_options{};
    std::vector<PositionalArgument> m_positional_args{};
    std::string                     m_program_description{};
    std::string                     m_program_version{};
    i32                             m_long_option_index{ -1 };
    bool                            m_show_help{ false };
    bool                            m_show_version{ false };
    bool                            m_stop_on_first_non_option{ false };
};

} /* namespace Utils */