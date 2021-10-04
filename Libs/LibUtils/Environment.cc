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

#include <Api.h>
#include <Api/utils/local.hpp>
#include <cassert>
#include <fstream>
#include <sstream>
#include <Utils/Environment.hh>
#include <Utils/PropertyFileParser.hh>
#include <Utils/Utils.hh>

#define SHELL_PATH "/Bins/MxSh"

namespace Utils::Environment {

std::string get(const std::string& key) {
    /* open the global environment variable */
    std::ifstream env_stream{ "/MeetiX/Configs/Env/Global" };
    assert(env_stream.is_open());

    /* read the internal properties and return the value for the key */
    PropertyFileParser parser{ env_stream };
    return parser.properties().at(key);
}

std::string get(const char* key) {
    return get(std::string{ key });
}

void set(const std::string& key, const std::string& value) {
    std::stringstream ss;
    ss << "--setenv " << key << "=" << value;

    /* obtain the working directory */
    auto work_dir = Local{ new char[PATH_MAX] };
    s_get_working_directory(work_dir());

    // exec shell
    auto spawn_status
        = s_spawn(SHELL_PATH, ss.str().c_str(), work_dir(), SECURITY_LEVEL_APPLICATION);
    if ( spawn_status != SPAWN_STATUS_SUCCESSFUL )
        log("setEnvVar: failed to s_spawn mx process");
}

std::string logged_user() {
    return get("USER");
}

std::string hostname() {
    return get("HOSTNAME");
}

std::string theme() {
    return get("THEME");
}

std::string version() {
    return get("VERSION");
}

} /* namespace Utils::Environment */