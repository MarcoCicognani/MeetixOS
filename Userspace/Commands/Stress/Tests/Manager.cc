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

#include "Tests/Manager.hh"

#include <algorithm>
#include <iostream>

namespace Tests {

Manager& Manager::inst() {
    static std::unique_ptr<Manager> s_instance{ nullptr };
    if ( s_instance == nullptr )
        s_instance.reset(new Manager{});

    return *s_instance;
}

void Manager::register_all() {
    /* Thread tests */
    m_all_tests.push_back(std::make_unique<Threads::POSIX>());
    m_all_tests.push_back(std::make_unique<Threads::Native>());

    /* Memory tests */
    m_all_tests.push_back(std::make_unique<Memory::Stack>());
    m_all_tests.push_back(std::make_unique<Memory::Heap>());

    /* FileSystem tests */
    m_all_tests.push_back(std::make_unique<FileSystem::ReadWrite>());
    m_all_tests.push_back(std::make_unique<FileSystem::OpenClose>());
    m_all_tests.push_back(std::make_unique<FileSystem::Create>());
}

int Manager::run_tests(const std::vector<std::string>& tests_names, bool verbose) const {
    int exit_result = EXIT_SUCCESS;
    for ( auto const& test_name : tests_names ) {
        /* run the test(s) */
        bool all_success;
        if ( is_category(test_name) )
            all_success = run_all_tests_for_category(test_name, verbose);
        else
            all_success = run_test_by_name(test_name, verbose);

        /* wen at least one test fails the entire command returns false */
        if ( !all_success )
            exit_result = EXIT_FAILURE;
    }

    return exit_result;
}

int Manager::list_to_stdout() const {
    std::cout << "Available tests:\n";
    for ( auto const& test : m_all_tests ) {
        std::cout << "\t\033[31m" << test->category() << "\033[0m/\033[32m" << test->name() << "\033[0m\n";
    }

    std::cout << '\n';
    std::cout << "You could provide a \033[31mCategory\033[0m to execute all the tests for the category\n";
    std::cout << "Or provide the full name using \033[31mCategory\033[0m/\033[32mName\033[0m" << std::endl;

    return EXIT_SUCCESS;
}

bool Manager::run_all_tests_for_category(const std::string_view& test_name, bool verbose) const {
    bool tests_result = true;
    for ( auto const& test : m_all_tests ) {
        if ( test_name == test->category() ) {
            /* enable verbosity */
            if ( verbose )
                test->enable_verbose();

            /* execute the test */
            tests_result |= test->execute();
        }
    }
    return tests_result;
}

bool Manager::run_test_by_name(const std::string_view& test_name, bool verbose) const {
    for ( auto const& test : m_all_tests ) {
        if ( test->full_name() == test_name ) {
            /* enable verbosity */
            if ( verbose )
                test->enable_verbose();

            /* execute the test */
            return test->execute();
        }
    }
    return true;
}

bool Manager::is_category(const std::string_view& name) const {
    for ( auto const& test : m_all_tests ) {
        if ( name == test->category() )
            return true;
    }
    return false;
}

} /* namespace Tests */