#
# @brief
# This file is part of the MeetiX Operating System.
# Copyright (c) 2017-2021, Marco Cicognani (marco.cicognani@meetixos.org)
#
# @developers
# Marco Cicognani (marco.cicognani@meetixos.org)
#
# @license
# GNU General Public License version 3
#

add_library(LibGUI
        STATIC
        Component/ActionComponent.cc
        Component/Geoshape.cc
        Component/BoundsEventComponent.cc
        Component/Component.cc
        Component/ComponentRegistry.cc
        Component/Button.cc
        Application.cc
        Component/TitledComponent.cc
        Component/Label.cc
        ButtonList.cc
        Component/ImageComponent.cc
        MessageBox.cc
        About.cc
        Component/TextField.cc
        Component/ColoredComponent.cc
        Component/Window.cc
        ButtonMenu.cc
        Notification.cc
        Component/Canvas.cc)