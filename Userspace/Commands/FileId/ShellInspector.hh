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

class ShellInspector : public Inspector {
public:
    explicit ShellInspector(std::ifstream& stream)
        : Inspector{ stream } {
    }
    ~ShellInspector() override = default;

    std::string inspection() override;
};