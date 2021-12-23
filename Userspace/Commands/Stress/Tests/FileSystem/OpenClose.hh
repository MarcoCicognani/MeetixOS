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

namespace Tests::FileSystem {

class OpenClose : public Test {
public:
    OpenClose()           = default;
    ~OpenClose() override = default;

    [[nodiscard]] const char* category() const override;
    [[nodiscard]] const char* name() const override;

protected:
    bool run() override;
};

} /* namespace Tests::FileSystem */