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

#include <LibGUI/Application.hh>
#include <LibGUI/Notification.hh>
#include <stdio.h>
#include <string.h>

/*
 * redirect
 */
void Notification::send(const std::string& title, const std::string& message) {
    send(title, message, 5, Graphics::Color::as_rgb(255, 255, 255), MessageAnimation_t::DARKENING);
}

/*
 * redirect
 */
void Notification::send(const std::string& title, const std::string& message, uint32_t sleep) {
    send(title, message, sleep, Graphics::Color::as_rgb(255, 255, 255), MessageAnimation_t::DARKENING);
}

/*
 * redirect
 */
void Notification::send(const std::string&            title,
                        const std::string&            message,
                        uint32_t                      sleep,
                        Graphics::Color::ArgbGradient color) {
    send(title, message, sleep, color, MessageAnimation_t::DARKENING);
}

/*
 * send message to notification thread of UI
 * @param message: string to show on popup
 * @param sleep: seconds of stay of message on screen popup
 */
void Notification::send(const std::string&            title,
                        const std::string&            message,
                        uint32_t                      sleep,
                        Graphics::Color::ArgbGradient color,
                        MessageAnimation_t            animation) {
    // if ui is uninitialized instant return
    if ( !UiInitialized )
        return;

    // get notifier m_command
    Tid notifierTid = s_task_get_id(NOTIFICATION_THREAD);
    if ( notifierTid == -1 )
        return;

    // create message
    MessageTransaction transaction = s_get_message_tx_id();
    Notification_t     request;

    // copy string into char pointer and truncate if exced in length
    const char* ttl    = title.c_str();
    uint32_t    ttllen = title.length();
    if ( ttllen >= MAX_TITLE_LENGTH )
        ttllen = MAX_TITLE_LENGTH - 1;

    // copy string into char pointer and truncate if exced in length
    const char* msg    = message.c_str();
    uint32_t    msglen = message.length();
    if ( msglen >= MAX_MESSAGE_LENGTH )
        msglen = MAX_MESSAGE_LENGTH - 1;

    // copy message string
    strncpy(request.title, ttl, ttllen);
    strncpy(request.message, msg, msglen);
    request.displayTime = sleep;
    request.color       = color;
    request.animation   = animation;

    // send message
    s_send_message_t(notifierTid, &request, sizeof(Notification_t), transaction);
}
