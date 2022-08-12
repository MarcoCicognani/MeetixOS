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

#pragma once

#include <LibC/getopt.h>
#include <string_view>
#include <vector>

/**
 * @brief getopt_long back implementation
 */
class OptionsParser {
public:
    /**
     * @brief Constructor
     */
    OptionsParser(int                     argc,
                  const char**            argv,
                  const std::string_view& short_options,
                  const option*           long_options,
                  int*                    out_long_option_index = nullptr);

    /**
     * @brief Returns the next parsed option short code
     */
    int next_option();

private:
    bool find_next_option();
    int  handle_long_option();
    int  handle_short_option();
    void shift_argv();

    const option* lookup_long_option(const char* raw_option) const;
    bool          lookup_short_option(char option, int& needs_value) const;

    [[nodiscard]] std::vector<std::string_view> split_short_options(char option) const;

private:
    int              m_argc{ 0 };
    const char**     m_argv{ nullptr };
    std::string_view m_short_options{};
    const option*    m_long_options{ nullptr };
    int*             m_out_long_option_index{ nullptr };
    bool             m_stop_on_first_non_option{ false };
    usize            m_arg_index{ 0 };
    usize            m_consumed_arguments{ 0 };
};
