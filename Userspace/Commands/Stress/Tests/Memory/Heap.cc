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

#include "Tests/Memory/Heap.hh"

#include <memory>
#include <vector>

namespace Tests::Memory {

const char* Heap::category() const {
    return "Memory";
}

const char* Heap::name() const {
    return "Heap";
}

bool Heap::run() {
    static constexpr auto C_POINTERS_COUNT   = 512;
    static constexpr auto C_REGION_BASE_SIZE = 128;

    {
        std::vector<std::unique_ptr<char>> regions{ C_POINTERS_COUNT };

        for ( auto i = 0; i < C_POINTERS_COUNT; ++i ) {
            /* allocate the region  */
            auto region_size  = C_REGION_BASE_SIZE * i;
            auto region_begin = new char[region_size];
            if ( region_begin == nullptr ) {
                logger() << "Failed to allocate region of size " << region_size << " at iteration " << i << '\n';
                return false;
            }

            /* put into the vector */
            regions.push_back(std::unique_ptr<char>{ region_begin });
        }

        /* write the first 12 bytes for each region */
        for ( auto const& region : regions ) {
            region.get()[0]  = 'H';
            region.get()[1]  = 'e';
            region.get()[2]  = 'l';
            region.get()[3]  = 'l';
            region.get()[4]  = 'o';
            region.get()[5]  = ' ';
            region.get()[6]  = 'W';
            region.get()[7]  = 'o';
            region.get()[8]  = 'r';
            region.get()[10] = 'l';
            region.get()[11] = 'd';
        }
    }

    logger() << "Destroyed all the memory\n";

    return true;
}

} /* namespace Tests::Memory */
