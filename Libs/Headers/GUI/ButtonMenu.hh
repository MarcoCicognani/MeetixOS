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

#pragma once

#include "../../../Toolchain/Local/i686-pc-meetix/include/c++/11.2.0/vector"
#include "ButtonList.hh"

/*
 *	this class provide a more complex list for button, is a simple way
 *	to create structurate menu list with button and icons
 */
class ButtonMenu {
private:
    // local list for button list
    ButtonList buttons;

    // local configuration
    std::map<std::string, std::string> cfg;

    // local vector that content paths to buttons icon
    std::vector<std::string> pngPaths;

    // standard button heigth
    const int buttonHeight = 30;

    // first heigth
    int height = 30;

    // local bounds
    Graphics::Metrics::Rectangle bds;

    // used for dimension of each button
    Graphics::Metrics::Rectangle buttonBounds;

    // copy into cfg map the configuration in file on path
    void getMapFromPath(const std::string& path);

    // configure internal map
    void configure();

    // process each pair of key/value in cfg map
    std::string parseCurrent(const std::pair<std::string, std::string>& current);

    // configure current button on button list
    void configureCurrent(const std::pair<std::string, std::string>& current, int index);

public:
    // empty constructor
    ButtonMenu(){};

    // constructor, espected path to configuration file
    ButtonMenu(const std::string& pathToConfiguration);

    // constructor with map of configuration, bounds and geoshape base espected
    ButtonMenu(const std::map<std::string, std::string>& configuration,
               const Graphics::Metrics::Rectangle&       bounds,
               Geoshape*                                 where);

    // constructor with map of configuration, bounds and window base espected
    ButtonMenu(const std::map<std::string, std::string>& configuration,
               const Graphics::Metrics::Rectangle&       bounds,
               Window*                                   where);

    // create menu from file configuration
    void create(const std::string& pathToConfiguration, const Graphics::Metrics::Rectangle& bounds);

    // create menu from map configuration
    void create(const std::map<std::string, std::string>& configuration,
                const Graphics::Metrics::Rectangle&       bounds);

    // set size of menu
    inline void setBounds(const Graphics::Metrics::Rectangle& bounds);

    // menu on desire Geoshape form
    void show(Geoshape* where);

    // menu on desire Window form
    inline void show(Window* where) {
        buttons.show(where);
    }
};
