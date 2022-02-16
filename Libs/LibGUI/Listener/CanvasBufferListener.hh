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

/**
 *
 */
class CanvasBufferListener {
public:
    virtual ~CanvasBufferListener() = default;

    virtual void handle_buffer_changed() = 0;
};
