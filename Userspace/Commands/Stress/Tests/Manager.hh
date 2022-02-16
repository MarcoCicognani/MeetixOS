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

#include "Tests/FileSystem/Create.hh"
#include "Tests/FileSystem/OpenClose.hh"
#include "Tests/FileSystem/ReadWrite.hh"
#include "Tests/Memory/Heap.hh"
#include "Tests/Memory/Stack.hh"
#include "Tests/Threads/Native.hh"
#include "Tests/Threads/POSIX.hh"

#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace Tests {

class Manager {
public:
    static Manager& inst();

    void register_all();

    [[nodiscard]] int run_tests(const std::vector<std::string>& tests_names, bool verbose) const;
    [[nodiscard]] int list_to_stdout() const;

private:
    Manager() = default;

    [[nodiscard]] bool run_all_tests_for_category(const std::string_view& test_name, bool verbose) const;
    [[nodiscard]] bool run_test_by_name(const std::string_view& test_name, bool verbose) const;

    [[nodiscard]] bool is_category(const std::string_view& name) const;

private:
    std::vector<std::unique_ptr<Test>> m_all_tests{};
};

} /* namespace Tests */