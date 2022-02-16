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

#include "Screen/Screen.hh"
#include "StreamStatus.hh"

#include <algorithm>
#include <Api.h>
#include <LibIO/Shell.hh>
#include <LibTasking/Lock.hh>
#include <LibTasking/Thread.hh>

class Terminal {
    enum class HeadLessMode {
        Gui,
        Text,
        None
    };

    static HeadLessMode from_string(std::string headless_mode) {
        std::transform(headless_mode.begin(), headless_mode.end(), headless_mode.begin(), ::tolower);

        if ( headless_mode == "gui" )
            return HeadLessMode::Gui;
        else if ( headless_mode == "text" )
            return HeadLessMode::Text;
        else
            return HeadLessMode::None;
    }

    class OutputRoutineThread : public Tasking::Thread {
    public:
        OutputRoutineThread(bool is_error, Terminal* terminal)
            : Tasking::Thread{ is_error ? "ErrOutput" : "StdOutput" }
            , m_is_error{ is_error }
            , m_terminal{ terminal } {}
        ~OutputRoutineThread() override = default;

    protected:
        [[noreturn]] void run() override;

    private:
        bool      m_is_error;
        Terminal* m_terminal;
    };

public:
    /**
     * @brief Constructor
     */
    Terminal(const std::string& headless_mode)
        : m_headless_mode{ (from_string(headless_mode)) } {}
    ~Terminal() {
        delete m_std_out_thread;
        delete m_err_out_thread;
        delete m_screen;
    }

    /**
     * @brief Executes the terminal mainloop
     */
    int run();

private:
    void init_screen();
    void start_shell();

    void write_string_to_shell(const std::string& line) const;
    void write_shellkey_to_shell(int shell_key) const;

    void process_output_character(StreamControlStatus& status, bool is_err_stream, char c);
    void process_vt100_sequence(StreamControlStatus& status);
    void process_term_sequence(StreamControlStatus& status);

    static ScreenColor convert_vt100_to_screen_color(int color);

    [[nodiscard]] bool shell_is_alive() const;
    int                input_routine();

private:
    FileHandle           m_shell_in{ FD_NONE };
    FileHandle           m_shell_out{ FD_NONE };
    FileHandle           m_shell_err{ FD_NONE };
    OutputRoutineThread* m_std_out_thread{ nullptr };
    OutputRoutineThread* m_err_out_thread{ nullptr };
    HeadLessMode         m_headless_mode{ HeadLessMode::None };
    Screen*              m_screen{ nullptr };
    Tasking::Lock        m_screen_lock{};
    bool                 m_do_echo{ true };
    IO::Shell::Mode      m_shell_input_mode{ IO::Shell::MODE_DEFAULT };
    Pid                  m_current_proc_id{ -1 };
    Pid                  m_shell_proc_id{ -1 };
};
