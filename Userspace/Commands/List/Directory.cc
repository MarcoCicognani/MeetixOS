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

#include "Directory.hh"

#include <iomanip>
#include <sstream>

#define VT100_RED     "\033[31m"
#define VT100_GREEN   "\033[32m"
#define VT100_MAGENTA "\033[35m"
#define VT100_LGREEN  "\033[38m"
#define VT100_RESET   "\033[0m"

Directory::Entry::Entry(const std::string& path, FsDirectoryEntry* entry)
    : m_node_id{ entry->m_node_id }
    , m_node_type{ entry->m_node_type }
    , m_name{ entry->m_name } {
    std::stringstream ss{};
    ss << path << '/' << m_name;
    m_full_path = ss.str();
}

std::string Directory::Entry::as_string() const {
    std::stringstream ss{};

    /* print type */
    ss << std::setw(WIDTH_TYPE) << std::setfill(' ') << type_str_by_type() << ' ';

    /* print protection */
    ss << std::setw(WIDTH_PROT) << std::setfill(' ');
    ss << "r" VT100_RED "w" VT100_GREEN "x" VT100_RESET;

    /* print name */
    ss << vt100_color_by_type();
    ss << std::setw(WIDTH_NAME) << std::setfill(' ');
    ss << m_name;
    ss << VT100_RESET;

    /* print INode number */
    ss << std::setw(WIDTH_INODE) << std::setfill(' ') << m_node_id;

    /* for files and pipes obtain the length in bytes */
    if ( m_node_type == FS_NODE_TYPE_FILE || m_node_type == FS_NODE_TYPE_PIPE )
        compose_size(ss);

    /* return the stream as string */
    return ss.str();
}

void Directory::Entry::compose_size(std::stringstream& ss) const {
    /* output preview */
    ss << ' ' << std::setw(WIDTH_SIZE) << std::setfill(' ');

    /* open the file */
    FsOpenStatus open_status{};
    auto         file_handle = s_open_fs(m_full_path.c_str(), FILE_FLAG_MODE_READ, &open_status);
    if ( open_status == FS_OPEN_SUCCESSFUL ) {
        /* obtain the length in bytes of the file */
        FsLengthStatus len_status{};
        auto           file_len = s_length_s(file_handle, &len_status);
        if ( len_status == FS_LENGTH_SUCCESSFUL ) {
            /* print out the value formatted */
            if ( file_len >= 1024 * 1024 * 1024 )
                ss << file_len / 1024 / 1024 / 1024 << " GiB";
            else if ( file_len >= 1024 * 1024 )
                ss << file_len / 1024 / 1024 << " MiB";
            else if ( file_len >= 1024 )
                ss << file_len / 1024 << " KiB";
            else
                ss << file_len << " Bytes";
        } else
            ss << "Unknown";

        /* close the handle */
        s_close(file_handle);
    } else
        ss << '-';
}

std::string_view Directory::Entry::vt100_color_by_type() const {
    switch ( m_node_type ) {
        case FS_NODE_TYPE_NONE:
        case FS_NODE_TYPE_ROOT:
            return { VT100_RED };
        case FS_NODE_TYPE_MOUNTPOINT:
        case FS_NODE_TYPE_FOLDER:
            return { VT100_MAGENTA };
        case FS_NODE_TYPE_FILE:
        case FS_NODE_TYPE_PIPE:
        default:
            return { VT100_LGREEN };
    }
}

std::string_view Directory::Entry::type_str_by_type() const {
    switch ( m_node_type ) {
        case FS_NODE_TYPE_ROOT:
            return { "Root" };
        case FS_NODE_TYPE_MOUNTPOINT:
            return { "Mount" };
        case FS_NODE_TYPE_FOLDER:
            return { "Dir" };
        case FS_NODE_TYPE_FILE:
            return { "File" };
        case FS_NODE_TYPE_PIPE:
            return { "Pipe" };
        case FS_NODE_TYPE_NONE:
        default:
            return { "None" };
    }
}

bool Directory::collect_entries() {
    /* open the directory iterator */
    FsOpenDirectoryStatus open_directory_status{};
    auto                  directory_it = s_open_directory_s(m_path.c_str(), &open_directory_status);
    if ( open_directory_status != FS_OPEN_DIRECTORY_SUCCESSFUL )
        return false;

    /* collect all the entries */
    while ( true ) {
        /* read the next directory entry */
        FsReadDirectoryStatus read_directory_status{};
        auto directory_entry = s_read_directory_s(directory_it, &read_directory_status);
        if ( read_directory_status == FS_READ_DIRECTORY_EOD )
            break;

        /* put the entry into the  */
        Entry entry{ m_path, directory_entry };
        m_directory_entries.push_back(entry);
    }

    /* sort the entries */
    sort_entries();

    /* close the directory iterator */
    s_close_directory(directory_it);
    return true;
}

void Directory::for_each(const std::function<void(const Entry&)>& call_back) const {
    for ( auto& entry : m_directory_entries )
        call_back(entry);
}

void Directory::sort_entries() {
    auto sort_by_name = [](const Entry& a, const Entry& b) { return b.m_name < a.m_name; };
    auto sort_by_type = [](const Entry& a, const Entry& b) {
        return a.m_node_type == FS_NODE_TYPE_ROOT || a.m_node_type == FS_NODE_TYPE_MOUNTPOINT
            || a.m_node_type == FS_NODE_TYPE_FOLDER;
    };

    if ( m_sort_order == SortOrder::ByName )
        std::sort(m_directory_entries.begin(), m_directory_entries.end(), sort_by_name);
    else if ( m_sort_order == SortOrder::ByType )
        std::sort(m_directory_entries.begin(), m_directory_entries.end(), sort_by_type);
    else {
        std::sort(m_directory_entries.begin(), m_directory_entries.end(), sort_by_name);
        std::sort(m_directory_entries.begin(), m_directory_entries.end(), sort_by_type);
    }
}
