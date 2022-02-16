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

#include "Tests/FileSystem/OpenClose.hh"

#include <fcntl.h>
#include <vector>

namespace Tests::FileSystem {

const char* OpenClose::category() const {
    return "FileSystem";
}

const char* OpenClose::name() const {
    return "OpenClose";
}

bool OpenClose::run() {
    static constexpr auto C_MAX_OPEN_CLOSE = 500;

    std::vector<std::string> all_paths{ tests_home() + "/TestFile_1.txt",
                                        tests_home() + "/TestFile_2.txt",
                                        tests_home() + "/Sub/TestFile_1.txt",
                                        tests_home() + "/Sub/TestFile_2.txt" };

    /* initialize the random generator */
    std::srand(s_millis());

    FsOpenStatus open_status{};
    FileHandle   file_handle{};
    for ( auto i = 0; i < C_MAX_OPEN_CLOSE; ++i ) {
        auto const& path = all_paths[std::rand() % all_paths.size()];

        /* open the selected path */
        file_handle = s_open_fms(path.c_str(), O_RDWR, 0, &open_status);
        if ( open_status != FS_OPEN_SUCCESSFUL ) {
            logger() << "Failed to open '" << path << "' at try " << i << '\n';
            return false;
        } else {
            /* close the previously open handle */
            auto close_status = s_close(file_handle);
            if ( close_status != FS_CLOSE_SUCCESSFUL ) {
                logger() << "Failed to close '" << path << "'\n";
                return false;
            }
        }
    }
    return true;
}

} /* namespace Tests::FileSystem */
