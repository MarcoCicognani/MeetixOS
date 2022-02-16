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

#include <stdio.h>
#include <stdlib.h>
#include <LibUnitTest/Suite.hh>

int main(int argc, char const* const* argv) {
    if ( argc == 0 || argv[0][0] == '\0' ) {
        fprintf(stderr, "UnitTest - Bad or null test name, aborting\n");
        return EXIT_FAILURE;
    }

    /* run the test suite */
    auto result = UnitTest::Suite::inst().main(argc, argv);
    UnitTest::Suite::finalize();
    return result;
}