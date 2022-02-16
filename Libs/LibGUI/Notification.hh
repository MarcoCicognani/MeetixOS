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

#include <Api.h>
#include <LibGraphics/Color.hh>
#include <string>

// max length of title
#define MAX_TITLE_LENGTH 512

// max legnth of single message
#define MAX_MESSAGE_LENGTH 1024

// thread identifier of notifierThread
#define NOTIFICATION_THREAD "OsmosUI::notifier"

/*
 * message animation
 */
enum class MessageAnimation_t : uint8_t {
    DARKENING,
    SIDE_SCROLL,
};

/*
 * message descriptor
 */
typedef struct {
    // m_command of application that send message
    char title[MAX_TITLE_LENGTH];

    // message to show
    char message[MAX_MESSAGE_LENGTH];

    // time to display message on screen
    uint32_t displayTime;

    // color of message
    Graphics::Color::ArgbGradient color;

    // animation type
    MessageAnimation_t animation;
} A_PACKED Notification_t;

/*
 * static class to send message notification on user interface
 */
class Notification {
private:
    Notification() = delete;

public:
    /*
     * reduced parameters version
     */
    static void send(const std::string& title, const std::string& message);
    static void send(const std::string& title, const std::string& message, uint32_t sleep);
    static void
    send(const std::string& title, const std::string& message, uint32_t sleep, Graphics::Color::ArgbGradient color);

    /*
     * send message to notification thread of UI
     * @param title: string with title of notification
     * @param message: string to show on popup
     * @param sleep: time that the message remain on screen
     * @prama color: color of message showed
     */
    static void send(const std::string&            title,
                     const std::string&            message,
                     uint32_t                      sleep,
                     Graphics::Color::ArgbGradient color,
                     MessageAnimation_t            animation);
};
