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

#include "Terminal.hh"

#include "Screen/GUIScreen.hh"
#include "Screen/HeadlessGUIScreen.hh"
#include "Screen/HeadlessScreen.hh"

#include <csignal>
#include <IO/Shell.hh>
#include <Tasking/LockGuard.hh>
#include <unistd.h>
#include <Utils/ArgsParser.hh>
#include <Utils/Utils.hh>

#define V_MAJOR 0
#define V_MINOR 0
#define V_PATCH 1

int main(int argc, const char** argv) {
    std::string headless_mode{};

    Utils::ArgsParser args_parser{ "Terminal", V_MAJOR, V_MINOR, V_PATCH };
    args_parser.add_option(headless_mode, "Headless mode: GUI/Text", "headless", 'l', "Mode");

    /* parse the arguments */
    args_parser.parse(argc, argv);

    /* execute the terminal */
    Terminal terminal{ headless_mode };
    return terminal.run();
}

void Terminal::OutputRoutineThread::run() {
    constexpr usize BUFFER_LEN = 1024;

    StreamControlStatus status;

    FsReadStatus read_status;
    Local        buffer{ new char[BUFFER_LEN] };
    auto         output_pipe = m_is_error ? m_terminal->m_shell_err : m_terminal->m_shell_out;
    while ( true ) {
        /* read from the  */
        auto read_bytes = s_read_s(output_pipe, buffer(), BUFFER_LEN, &read_status);
        if ( read_status == FS_READ_SUCCESSFUL ) {
            Tasking::LockGuard lock_guard{ m_terminal->m_screen_lock };

            /* write the output */
            for ( auto i = 0; i < read_bytes; i++ )
                m_terminal->process_output_character(status, m_is_error, buffer()[i]);
        } else
            break;
    }
    __builtin_unreachable();
}

int Terminal::run() {
    /* initialize the screen */
    init_screen();
    if ( !m_screen ) {
        Utils::log("Terminal: Failed to init screen");
        return EXIT_FAILURE;
    }
    m_screen->clean();

    /* load keyboard layout */
    std::string kb_layout{ "en-US" };
    if ( !IO::Keyboard::instance().load_layout(kb_layout) ) {
        Utils::log("Terminal: Failed to load keyboard layout: '%s'", kb_layout.c_str());
        return EXIT_FAILURE;
    }

    /* tell the kernel to stop write logs on video-memory */
    s_set_video_log(false);

    /* start the shell */
    start_shell();
    if ( m_shell_in == FD_NONE || m_shell_out == FD_NONE || m_shell_err == FD_NONE )
        return EXIT_FAILURE;

    /* start output routines */
    m_std_out_thread = new OutputRoutineThread{ false, this };
    m_std_out_thread->start();

    m_err_out_thread = new OutputRoutineThread{ true, this };
    m_err_out_thread->start();

    /* use the main thread for the input routine */
    return input_routine();
}

void Terminal::init_screen() {
    if ( m_headless_mode == HeadLessMode::None ) {
        auto gui_screen = new GUIScreen{};
        if ( gui_screen->init() )
            m_screen = gui_screen;
        else
            Utils::log("Terminal: Failed to init the graphical screen");
    } else {
        /* check for other executing instances */
        auto this_proc_id     = s_get_pid();
        auto terminal_proc_id = s_task_get_id("Terminal");
        if ( this_proc_id != terminal_proc_id ) {
            Utils::log("Terminal: Can only be executed once when in headless mode");
            return;
        }

        /* initialize the screen */
        if ( m_headless_mode == HeadLessMode::Gui ) {
            auto gui_screen = new HeadlessGUIScreen{};
            if ( gui_screen->init() )
                m_screen = gui_screen;
            else {
                delete gui_screen;
                m_screen = new HeadlessScreen{};
            }
        } else {
            m_screen = new HeadlessScreen{};
        }
    }
}

void Terminal::start_shell() {
    /* create the input pipe for the shell */
    FsPipeStatus shell_in_status{};
    FileHandle   shell_in_write{}, shell_in_read{};
    s_pipe_s(&shell_in_write, &shell_in_read, &shell_in_status);
    if ( shell_in_status != FS_PIPE_SUCCESSFUL ) {
        Utils::log("Terminal: Failed to setup stdin pipe for shell");
        return;
    }

    /* create the output pipe for the shell */
    FsPipeStatus shell_out_status{};
    FileHandle   shell_out_write{}, shell_out_read{};
    s_pipe_s(&shell_out_write, &shell_out_read, &shell_out_status);
    if ( shell_out_status != FS_PIPE_SUCCESSFUL ) {
        Utils::log("Terminal: Failed to setup stdout pipe for shell");
        return;
    }

    /* create the error output for the shell */
    FsPipeStatus shell_err_status{};
    FileHandle   shell_err_write{}, shell_err_read{};
    s_pipe_s(&shell_err_write, &shell_err_read, &shell_err_status);
    if ( shell_err_status != FS_PIPE_SUCCESSFUL ) {
        Utils::log("CandyTerminal: Failed to setup stderr pipe for shell");
        return;
    }

    /* span the shell binary */
    FileHandle stdio_target[3]{};
    FileHandle stdio_in[3]{ shell_in_read, shell_out_write, shell_err_write };
    auto       spawn_status = s_spawn_poi("/Bins/MxSh",
                                          "-sh",
                                          "/",
                                          SECURITY_LEVEL_APPLICATION,
                                          &m_shell_proc_id,
                                          stdio_target,
                                          stdio_in);
    if ( spawn_status != SPAWN_STATUS_SUCCESSFUL ) {
        Utils::log("Terminal: Failed to spawn MeetiX shell process");
        return;
    }

    /* copy the pipes */
    m_shell_in  = shell_in_write;
    m_shell_out = shell_out_read;
    m_shell_err = shell_err_read;
}

void Terminal::write_string_to_shell(const std::string& line) const {
    auto line_content = line.c_str();
    auto line_len     = line.length();

    auto written = 0;
    while ( written < line_len ) {
        auto len = write(m_shell_in, &line_content[written], line_len - written);
        if ( len <= 0 )
            break;

        written += len;
    }
}

void Terminal::write_shellkey_to_shell(int shell_key) const {
    char buf[3]{ SHELLKEY_SUB,
                 static_cast<char>(shell_key & 0xFF),
                 static_cast<char>((shell_key >> 8) & 0xFF) };
    write(m_shell_in, &buf, 3);
}

void Terminal::process_output_character(StreamControlStatus& status, bool is_err_stream, char c) {
    if ( status.m_stream_status == TerminalStreamStatus::Text ) {
        /* simple textual output */
        if ( c == '\r' )
            return;
        else if ( c == '\t' ) {
            m_screen->write_char(' ');
            m_screen->write_char(' ');
            m_screen->write_char(' ');
            m_screen->write_char(' ');
        } else if ( c == SHELLKEY_ESC )
            status.m_stream_status = TerminalStreamStatus::LastWasEsc;
        else {
            /* save previous foreground for error */
            auto last_foreground = m_screen->color_foreground();
            if ( is_err_stream )
                m_screen->set_color_foreground(SC_RED);

            m_screen->write_char(c);

            if ( is_err_stream )
                m_screen->set_color_foreground(last_foreground);
        }
    } else if ( status.m_stream_status == TerminalStreamStatus::LastWasEsc ) {
        /* must be followed by [ for VT100 sequence or a terminal sequence */
        if ( c == '[' )
            status.m_stream_status = TerminalStreamStatus::WithinVT100;
        else if ( c == '{' )
            status.m_stream_status = TerminalStreamStatus::WithinTerm;
        else
            status.m_stream_status = TerminalStreamStatus::Text;
    } else if ( status.m_stream_status == TerminalStreamStatus::WithinTerm
                || status.m_stream_status == TerminalStreamStatus::WithinVT100 ) {
        // Parameter value
        if ( c >= '0' && c <= '9' ) {
            if ( status.m_parameter_count == 0 )
                status.m_parameter_count = 1;

            if ( status.m_parameter_count <= SHELL_STREAM_CONTROL_MAX_PARAMETERS ) {
                status.m_parameters[status.m_parameter_count - 1]
                    = status.m_parameters[status.m_parameter_count - 1] * 10;
                status.m_parameters[status.m_parameter_count - 1] += c - '0';

                /* illegal number of parameters is skipped */
            }
        } else if ( c == ';' )
            status.m_parameter_count++;
        else {
            status.m_control_character = c;

            /* process sequences */
            if ( status.m_stream_status == TerminalStreamStatus::WithinVT100 )
                process_vt100_sequence(status);
            else if ( status.m_stream_status == TerminalStreamStatus::WithinTerm )
                process_term_sequence(status);

            /* reset status */
            status.m_parameter_count = 0;
            for ( auto& parameter : status.m_parameters )
                parameter = 0;
            status.m_stream_status = TerminalStreamStatus::Text;
        }
    }
}

void Terminal::process_vt100_sequence(StreamControlStatus& status) {
    switch ( status.m_control_character ) {
        case 'A':
            /* cursor up */
            m_screen->move_cursor(m_screen->cursor_x(),
                                  m_screen->cursor_y() - status.m_parameters[0]);
            break;
        case 'B':
            /* cursor down */
            m_screen->move_cursor(m_screen->cursor_x(),
                                  m_screen->cursor_y() + status.m_parameters[0]);
            break;
        case 'C':
            /* cursor forward */
            m_screen->move_cursor(m_screen->cursor_x() + status.m_parameters[0],
                                  m_screen->cursor_y());
            break;
        case 'D':
            /* cursor back */
            m_screen->move_cursor(m_screen->cursor_x() - status.m_parameters[0],
                                  m_screen->cursor_y());
            break;
        case 'm':
            /* mode setting */
            for ( auto i = 0; i < status.m_parameter_count; ++i ) {
                auto param = status.m_parameters[i];

                if ( !param ) {
                    m_screen->set_color_background(SC_BLACK);
                    m_screen->set_color_foreground(SC_WHITE);
                } else if ( param >= 30 && param < 40 )
                    m_screen->set_color_foreground(convert_vt100_to_screen_color(param - 30));
                else if ( param >= 40 && param < 50 )
                    m_screen->set_color_background(convert_vt100_to_screen_color(param - 40));
            }
            break;
        case 'f':
            /* reposition cursor */
            m_screen->move_cursor(status.m_parameters[1], status.m_parameters[0]);
            break;
        case 'n':
            /* Cursor queries */
            if ( status.m_parameters[0] == 6 ) {
                std::stringstream ss;
                ss << SHELLKEY_ESC << "[" << m_screen->cursor_y() << ";" << m_screen->cursor_x()
                   << "R";

                /* write out the response */
                write_string_to_shell(ss.str());
            }
            break;
        case 'r':
            /* set scroll area */
            if ( !status.m_parameter_count )
                m_screen->set_scroll_area_screen();
            else
                m_screen->set_scroll_area(status.m_parameters[0], status.m_parameters[1]);
            break;
        case 'S':
            /* scroll region up */
            m_screen->scroll(status.m_parameters[0]);
            break;
        case 'T':
            /* scroll region down */
            m_screen->scroll(-status.m_parameters[0]);
            break;
        case 'H':
            /* clear the entire screen */
            m_screen->clean();
            break;
    }
}

void Terminal::process_term_sequence(StreamControlStatus& status) {
    switch ( status.m_control_character ) {
        case 'm':
            /* change mode */
            m_shell_input_mode = static_cast<IO::Shell::Mode>(status.m_parameters[0]);
            break;
        case 'e':
            /* change echo */
            m_do_echo = status.m_parameters[0] == 1;
            break;
        case 'i': {
            /* screen info */
            std::stringstream ss;
            ss << SHELLKEY_ESC << "{" << m_screen->width() << ";" << m_screen->height() << "i";
            write_string_to_shell(ss.str());
            break;
        }
        case 'p':
            /* put char */
            m_screen->write_char(static_cast<char>(status.m_parameters[0]));
            break;
        case 'c':
            /* process control */
            m_current_proc_id = status.m_parameters[0];
            break;
    }
}

ScreenColor Terminal::convert_vt100_to_screen_color(int color) {
    switch ( color ) {
        case VT100_COLOR_BLACK:
            return SC_BLACK;
        case VT100_COLOR_BLUE:
            return SC_BLUE;
        case VT100_COLOR_CYAN:
            return SC_CYAN;
        case VT100_COLOR_GREEN:
            return SC_GREEN;
        case VT100_COLOR_MAGENTA:
            return SC_MAGENTA;
        case VT100_COLOR_RED:
            return SC_RED;
        case VT100_COLOR_WHITE:
            return SC_WHITE;
        case VT100_COLOR_YELLOW:
            return SC_YELLOW;
        case VT100_COLOR_LGREEN:
            return SC_LGREEN;
        default:
            return SC_WHITE;
    }
}

bool Terminal::shell_is_alive() const {
    return s_get_pid_for_tid(m_shell_proc_id) == m_shell_proc_id;
}

int Terminal::input_routine() {
    std::string buffer{};
    while ( shell_is_alive() ) {
        auto read_input = m_screen->read_input();

        /* default line-buffered input */
        if ( m_shell_input_mode == IO::Shell::MODE_DEFAULT ) {
            if ( read_input.m_key == "KEY_ENTER" && read_input.m_is_pressed ) {
                if ( m_do_echo ) {
                    Tasking::LockGuard lock_guard{ m_screen_lock };
                    m_screen->write_char('\n');
                }

                /* write out the line to the shell */
                buffer += '\n';
                write_string_to_shell(buffer);

                /* clear the buffer */
                buffer.clear();
            } else if ( (read_input.m_ctrl && read_input.m_key == "KEY_C")
                        || (read_input.m_key == "KEY_ESC") ) {
                if ( m_current_proc_id )
                    s_raise_signal(m_current_proc_id, SIGINT);
            } else if ( read_input.m_key == "KEY_BACKSPACE" && read_input.m_is_pressed ) {
                buffer = !buffer.empty() ? buffer.substr(0, buffer.size() - 1) : buffer;
                m_screen->backspace();
            } else {
                auto c = IO::Keyboard::instance().char_for_key(read_input);
                if ( c != -1 ) {
                    buffer += c;

                    if ( m_do_echo ) {
                        Tasking::LockGuard lock_guard{ m_screen_lock };
                        m_screen->write_char(c);
                    }
                }
            }
        } else if ( m_shell_input_mode == IO::Shell::MODE_RAW ) {
            if ( read_input.m_key == "KEY_ENTER" && read_input.m_is_pressed ) {
                write_shellkey_to_shell(SHELLKEY_ENTER);

                if ( m_do_echo ) {
                    Tasking::LockGuard lock_guard{ m_screen_lock };
                    m_screen->write_char('\n');
                }
            } else if ( read_input.m_key == "KEY_BACKSPACE" && read_input.m_is_pressed ) {
                write_shellkey_to_shell(SHELLKEY_BACKSPACE);

                if ( m_do_echo ) {
                    Tasking::LockGuard lock_guard{ m_screen_lock };
                    m_screen->backspace();
                }
            } else if ( read_input.m_key == "KEY_ARROW_LEFT" && read_input.m_is_pressed )
                write_shellkey_to_shell(SHELLKEY_LEFT);
            else if ( read_input.m_key == "KEY_ARROW_RIGHT" && read_input.m_is_pressed )
                write_shellkey_to_shell(SHELLKEY_RIGHT);
            else if ( read_input.m_key == "KEY_ARROW_UP" && read_input.m_is_pressed )
                write_shellkey_to_shell(SHELLKEY_UP);
            else if ( read_input.m_key == "KEY_ARROW_DOWN" && read_input.m_is_pressed )
                write_shellkey_to_shell(SHELLKEY_DOWN);
            else {
                char c = IO::Keyboard::instance().char_for_key(read_input);
                if ( c != -1 ) {
                    write(m_shell_in, &c, 1);

                    if ( m_do_echo ) {
                        Tasking::LockGuard lock_guard{ m_screen_lock };
                        m_screen->write_char(c);
                    }
                }
            }
        }
    }

    return EXIT_SUCCESS;
}
