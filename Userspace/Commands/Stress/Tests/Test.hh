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

#include <ostream>
#include <string>

namespace Tests {

class Test {
public:
    explicit Test() = default;
    virtual ~Test() = default;

    bool execute();
    void enable_verbose();

    [[nodiscard]] virtual const char* category() const = 0;
    [[nodiscard]] virtual const char* name() const     = 0;

    [[nodiscard]] std::string full_name() const;

protected:
    virtual bool run() = 0;

    [[nodiscard]] std::string   tests_home() const;
    [[nodiscard]] std::ostream& logger();

private:
    bool         m_is_verbose{ false };
    std::ostream m_null_logger{ nullptr };
};

} /* namespace Tests */
