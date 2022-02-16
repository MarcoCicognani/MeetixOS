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

#include "Tests/Test.hh"

namespace Tests::Memory {

class Heap : public Test {
public:
    Heap()           = default;
    ~Heap() override = default;

    [[nodiscard]] const char* category() const override;
    [[nodiscard]] const char* name() const override;

protected:
    bool run() override;
};

} /* namespace Tests::Memory */