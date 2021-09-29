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

#include <stdlib.h>

void* bsearch(A_UNUSED const void* value,
              A_UNUSED const void* array,
              A_UNUSED usize       num_elements,
              A_UNUSED usize       size,
              A_UNUSED int (*comparator)(const void*, const void*)) {
    __NOT_IMPLEMENTED(bsearch);
    return NULL;
}
