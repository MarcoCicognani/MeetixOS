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

#include "Tests/Test.hh"

namespace Memory {

class Heap : public Test {
public:
    explicit Heap(bool is_verbose);

protected:
    bool run() override;

    [[nodiscard]] const char* name() const override;
};

} /* namespace Memory */