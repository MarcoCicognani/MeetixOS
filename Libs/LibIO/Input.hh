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

#include <cstdint>
#include <LibTasking/Lock.hh>

#define INPUT_DRIVER_IDENTIFIER "Input"

namespace IO {

/* ------------------------------------------ C++ types ----------------------------------------- */

/**
 * @brief Singleton interface to communicate with Input driver
 */
class Input {
public:
    /**
     * @brief Shared memory area used for Input data transferring
     */
    struct SharedArea {
        struct {
            /**
             * This atom remains set, until the driver has data to transfer.
             * The driver delegate must wait until the atom is unset, and then
             * set it again.
             */
            bool m_atom_nothing_queued{ false };

            /**
             * This atom is set by the driver, once it has written data that
             * the delegate is yet to read. Once the data is read, the delegate
             * must unset the atom.
             */
            bool m_atom_unhandled{ false };

            i16 m_move_x{ 0 };
            i16 m_move_y{ 0 };
            u16 m_flags{ 0 };
        } m_mouse;

        struct {
            bool m_atom_nothing_queued{ false };
            bool m_atom_unhandled{ false };

            u8 m_scancode{ 0 };
        } m_keyboard;
    } A_PACKED;

    /**
     * Request sent to register the sender thread as the
     * Input data handler.
     */
    struct RegisterRequest {
        u8 m_unused_byte{ 0 };
    } A_PACKED;

    /**
     * Response sent to a registering thread, containing the
     * shared memory area to use.
     */
    struct RegisterResponse {
        SharedArea* m_shared_area{ nullptr };
    } A_PACKED;

public:
    /**
     * @brief Returns the global instance of the singleton
     */
    static Input& instance();

    /**
     * @brief Registers this process as client of the Input driver
     */
    bool register_self();

    /**
     * @brief Returns whether register_self() was already called
     */
    [[nodiscard]] bool is_registered() const { return m_is_registered; }

    /**
     * @brief Returns the reference to the shared memory area with the Input driver
     */
    [[nodiscard]] SharedArea& shared_area() { return *m_shared_area; }

private:
    Input() = default;

private:
    Tasking::Lock m_lock;
    SharedArea*   m_shared_area{ nullptr };
    bool          m_is_registered{ false };
};

} /* namespace IO */