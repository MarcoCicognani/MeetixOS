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

#define VIDEO_DRIVER_IDENTIFIER "Video"

namespace Graphics::Video {

/* ------------------------------------ C++ Type Definitions ------------------------------------ */

/**
 * @brief Video driver commands
 */
enum Command { COMMAND_SET_MODE, };

/**
 * @brief Video driver requests statuses
 */
enum SetModeStatus {
    SET_MODE_STATUS_SUCCESS,
    SET_MODE_STATUS_FAILED
};

/**
 * @brief Video Mode setting-information
 */
struct ModeInfo {
public:
    u16   m_width{ 0 };
    u16   m_height{ 0 };
    u16   m_bit_per_pixel{ 0 };
    u16   m_bytes_per_scanline{ 0 };
    usize m_linear_framebuffer{ 0 };

    /**
     * @brief Constructors
     */
    ModeInfo() = default;
    ModeInfo(u16 width, u16 height, u16 bit_per_pixel, u16 bytes_per_scanline, usize linear_framebuffer)
        : m_width{ width }
        , m_height{ height }
        , m_bit_per_pixel{ bit_per_pixel }
        , m_bytes_per_scanline{ bytes_per_scanline }
        , m_linear_framebuffer{ linear_framebuffer } {}
} A_PACKED;

/**
 * @brief Video driver request header
 */
struct RequestHeader {
public:
    Command m_command{ COMMAND_SET_MODE };

    /**
     * @brief Constructors
     */
    RequestHeader() = default;
    RequestHeader(Command command)
        : m_command{ command } {}
} A_PACKED;

/**
 * @brief Video driver set-mode request payload
 */
struct SetModeRequest {
public:
    RequestHeader m_request_header;
    u16           m_width{ 0 };
    u16           m_height{ 0 };
    u8            m_bit_per_pixel{ 0 };

    /**
     * @brief Constructors
     */
    SetModeRequest() = default;
    SetModeRequest(Command command, u16 width, u16 height, u8 bit_per_pixel)
        : m_request_header{ command }
        , m_width{ width }
        , m_height{ height }
        , m_bit_per_pixel{ bit_per_pixel } {}
} A_PACKED;

/**
 * @brief Video driver set-mode response
 */
struct SetModeResponse {
    SetModeStatus m_mode_status{};
    ModeInfo      m_mode_info{};

    /**
     * @brief Constructors
     */
    SetModeResponse() = default;
} A_PACKED;

/* ----------------------------------- C++ function prototypes ---------------------------------- */

/**
 * @brief Requests to the Video driver to set the given video-mode
 */
bool set_mode(u16 width, u16 height, u8 bpp, ModeInfo& mode_info);

} /* namespace Graphics::Video */