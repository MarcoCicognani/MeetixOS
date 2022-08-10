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

#include <LibMain/Main.hh>
#include <LibTC/Collection/Vector.hh>
#include <LibTC/Cxx.hh>
#include <LibUnitTest/Suite.hh>

/**
 * @brief Test suite entry point, it only calls the run method
 */
auto entry(Vector<StringView> args) -> ErrorOr<void> {
    TRY(UnitTest::Suite::inst().run(Cxx::move(args)));
    return {};
}
