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

#include <Api.h>
#include <functional>
#include <LibGUI/Component/Button.hh>
#include <LibGUI/Component/Geoshape.hh>
#include <LibGUI/Component/Window.hh>
#include <string>
#include <unordered_map>

/*
 * container class to create button list
 */
class ButtonList {
public:
    // constructors
    ButtonList() {}
    ButtonList(const std::string& name, Button* button, const std::function<void(void)>& handler) {
        add(name, button, handler);
    }

    // destructor
    ~ButtonList();

    // overloads of add method
    void add(const std::string& name, ActionListener* listener);
    void add(const std::string& name, Button* button, ActionListener* listener);

    void add(const std::string& name, const std::function<void(void)>& handler);
    void add(const std::string& name, Button* button, const std::function<void(void)>& handler);

    // get method, interact with single objects
    Button* get(const std::string& name);
    Button* operator[](const std::string& name);

    // overloaded configure method, each method provide different combination of parameters
    void configure(const std::string& name, const Graphics::Metrics::Rectangle& bounds);
    void configure(const std::string& name, const Graphics::Metrics::Rectangle& bounds, const std::string& title);
    void configure(const std::string&                  name,
                   const Graphics::Metrics::Rectangle& bounds,
                   const std::string&                  title,
                   Graphics::Color::ArgbGradient       buttonColor,
                   Graphics::Color::ArgbGradient       titleColor);
    void configure(const std::string&                  name,
                   const Graphics::Metrics::Rectangle& bounds,
                   const std::string&                  title,
                   const std::string&                  pathToPng,
                   const Graphics::Metrics::Point&     pngPosition);
    void configure(const std::string&                  name,
                   const Graphics::Metrics::Rectangle& bounds,
                   const std::string&                  title,
                   const std::string&                  pathToPng,
                   const Graphics::Metrics::Point&     pngPosition,
                   Graphics::Color::ArgbGradient       buttonColor,
                   Graphics::Color::ArgbGradient       titleColor);

    // method to display on screen buttons of map
    void show(Window* where);
    void show(Geoshape* where);
    void show(const std::string& name, Window* where);
    void show(const std::string& name, Geoshape* where);

private:
    // maps that contains button and handlers
    std::unordered_map<std::string, Button*>         buttons;
    std::unordered_map<std::string, ActionListener*> handlers;
};
