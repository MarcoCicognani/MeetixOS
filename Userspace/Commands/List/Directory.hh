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

#pragma once

#include <algorithm>
#include <Api/FileSystem.h>
#include <functional>
#include <string>
#include <string_view>
#include <vector>

#define WIDTH_TYPE  6
#define WIDTH_PROT  5
#define WIDTH_NAME  22
#define WIDTH_INODE 6
#define WIDTH_SIZE  7

class Directory {
public:
    enum class SortOrder {
        ByName,
        ByType,
        ByTypeAndName
    };

    static SortOrder from_string(std::string value) {
        std::ranges::transform(value, value.begin(), ::tolower);

        if ( value == "name" )
            return SortOrder::ByName;
        else if ( value == "Type" )
            return SortOrder::ByType;
        else
            return SortOrder::ByTypeAndName;
    }

    struct Entry {
    public:
        FsVirtID    m_node_id{};
        FsNodeType  m_node_type{};
        std::string m_name{};
        std::string m_full_path{};

        /**
         * @brief Constructors
         */
        Entry() = default;
        Entry(const std::string& path, FsDirectoryEntry* entry);

        /**
         * @brief Returns this entry as List string
         */
        [[nodiscard]] std::string as_string() const;

    private:
        void compose_size(std::stringstream& ss) const;

        [[nodiscard]] std::string_view vt100_color_by_type() const;
        [[nodiscard]] std::string_view type_str_by_type() const;
    };

public:
    /**
     * @brief Constructor
     */
    Directory(const std::string& path, const std::string& order)
        : m_path{ path }
        , m_sort_order{ from_string(order) } {}

    /**
     * @brief Reads all the entries into the directory open
     */
    bool collect_entries();

    /**
     * @brief Calls call_back for each entry
     */
    void for_each(const std::function<void(const Entry&)>& call_back) const;

private:
    void sort_entries();

private:
    std::string        m_path{};
    std::vector<Entry> m_directory_entries{};
    SortOrder          m_sort_order{};
};