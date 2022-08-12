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

#include <LibC/stdint.h>
#include <LibC/stdlib.h>

static void swap(u8* x, u8* y, usize element_size) {
    auto a = x;
    auto b = y;
    u8   c;
    while ( element_size-- ) {
        c    = *a;
        *a++ = *b;
        *b++ = c;
    }
}

static void sort(u8* arr, usize el_sz, int (*comparator)(const void*, const void*), usize begin, usize end) {
    if ( end > begin ) {
        auto pivot        = arr + begin;
        auto begin_offset = begin + el_sz;
        auto end_offset   = end;

        while ( begin_offset < end_offset ) {
            if ( comparator(arr + begin_offset, pivot) <= 0 )
                begin_offset += el_sz;
            else if ( comparator(arr + end_offset, pivot) > 0 )
                end_offset -= el_sz;
            else if ( begin_offset < end_offset )
                swap(arr + begin_offset, arr + end_offset, el_sz);
        }

        begin_offset -= el_sz;

        swap(arr + begin, arr + begin_offset, el_sz);
        sort(arr, el_sz, comparator, begin, begin_offset);
        sort(arr, el_sz, comparator, end_offset, end);
    }
}

extern "C" void
qsort(void* array, usize num_elements, usize element_size, int (*comparator)(const void*, const void*)) {
    sort(reinterpret_cast<u8*>(array), element_size, comparator, 0, num_elements * element_size);
}
