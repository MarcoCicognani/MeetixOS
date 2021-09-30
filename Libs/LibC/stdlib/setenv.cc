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

#include <sstream>
#include <stdlib.h>

/* TODO find a better way to do this */

extern "C" int setenv(const char* key, const char* value, int) {
    std::stringstream ss;
    ss << "/Bins/MxSh --setenv " << key << "=" << value;

    return system(ss.str().c_str());
}
