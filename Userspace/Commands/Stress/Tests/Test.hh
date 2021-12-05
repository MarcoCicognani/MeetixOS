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

class Test {
public:
    explicit Test(bool is_verbose);

    bool execute();

protected:
    virtual bool run() = 0;

    [[nodiscard]] virtual const char* name() const = 0;

    [[nodiscard]] bool is_verbose() const {
        return m_is_verbose;
    }

private:
    bool m_is_verbose{ false };
};
