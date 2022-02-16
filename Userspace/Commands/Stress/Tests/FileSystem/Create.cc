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

#include "Tests/FileSystem/Create.hh"

#include <Api/FileSystem.h>
#include <cstdio>
#include <fcntl.h>
#include <sstream>

namespace Tests::FileSystem {

const char* Create::category() const {
    return "FileSystem";
}

const char* Create::name() const {
    return "Create";
}

bool Create::run() {
    static constexpr auto C_MAX_CREATE = 120;

    logger() << "Creating Files...\n";

    FsOpenStatus open_status{};
    std::string  path{ tests_home() + "/TestFile" };
    for ( auto i = 0; i < C_MAX_CREATE; ++i ) {
        std::stringstream ss{};
        ss << path << i << ".empty";
        auto full_path = ss.str();

        /* try to create the file */
        logger() << "Creating '" << full_path << "'\n";
        auto file_handle = s_open_fms(ss.str().c_str(), O_CREAT | O_RDWR, 0, &open_status);
        if ( open_status != FS_OPEN_SUCCESSFUL ) {
            logger() << "Failed with error " << open_status << '\n';
            return false;
        } else
            s_close(file_handle);
    }

    return true;
}

} /* namespace Tests::FileSystem */
