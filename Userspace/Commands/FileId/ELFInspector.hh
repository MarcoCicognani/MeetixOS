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

class ELFInspector : public Inspector {
public:
    explicit ELFInspector(std::ifstream& stream)
        : Inspector{ stream } {
    }
    ~ELFInspector() override = default;

    std::string inspection() override;
};