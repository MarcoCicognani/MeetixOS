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

#include <LibIO/Shell.hh>

enum class TerminalStreamStatus {
    Text,
    LastWasEsc,
    WithinVT100,
    WithinTerm,
};

/**
 * Used to remember the status of a stream, for example whether a control
 * sequence is currently being sent.
 */
struct StreamControlStatus {
public:
    TerminalStreamStatus m_stream_status{ TerminalStreamStatus::Text };
    int                  m_parameter_count{ 0 };
    int                  m_parameters[SHELL_STREAM_CONTROL_MAX_PARAMETERS]{ 0 };
    char                 m_control_character{ '\0' };
};
