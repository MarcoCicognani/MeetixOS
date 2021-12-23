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

#include "Inspector.hh"

class TextInspector : public Inspector {
public:
    explicit TextInspector(std::ifstream& stream)
        : Inspector{ stream } {
    }
    ~TextInspector() override = default;

    std::string inspection() override;
};