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

namespace FileSystem {

class ReadWrite : public Test {
public:
    explicit ReadWrite(bool is_verbose);

protected:
    bool run() override;

    [[nodiscard]] const char* name() const override;
};

} /* namespace FileSystem */