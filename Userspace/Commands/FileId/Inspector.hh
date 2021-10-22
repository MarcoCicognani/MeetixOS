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

#include <fstream>

class Inspector {
public:
    static Inspector* inspector_for_file(std::ifstream& stream);

    virtual std::string inspection() = 0;

protected:
    explicit Inspector(std::ifstream& stream) : m_stream{ stream } {
    }

protected:
    std::ifstream& m_stream;
};
