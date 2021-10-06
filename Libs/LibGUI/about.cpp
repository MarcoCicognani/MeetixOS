/*********************************************************************************
 * MeetiX OS By MeetiX OS Project [Marco Cicognani]                               *
 * 																			     *
 * This program is free software; you can redistribute it and/or                  *
 * modify it under the terms of the GNU General Public License                    *
 * as published by the Free Software Foundation; either version 2				 *
 * of the License, or (char *argumentat your option) any later version.			 *
 *																				 *
 * This program is distributed in the hope that it will be useful,				 *
 * but WITHout ANY WARRANTY; without even the implied warranty of                 *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 				 *
 * GNU General Public License for more details.
 **
 *																				 *
 * You should have received a copy of the GNU General Public License				 *
 * along with this program; if not, write to the Free Software                    *
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA *
 **********************************************************************************/

#include <Graphics/Metrics/Dimension.hh>
#include <gui/about.hpp>
#include <gui/geoshape.hpp>
#include <gui/label.hpp>
#include <gui/ui.hpp>
#include <gui/window.hpp>
#include <Utils/Environment.hh>

// interface object for about
static Window*   about;
static Geoshape* panel;
static Label*    line1;
static Label*    line2;

/*
 *	delete all object
 */
static void close() {
    delete line1;
    delete line2;
    delete panel;
    delete about;
}

/*
 *	show window with version, logo and copyrigth of MeetiX OS
 */
void aboutMeetiXOS() {
    // get resolution
    auto rs = UI::getResolution();
    auto wBounds
        = Graphics::Metrics::Rectangle(rs.width() / 2 - 175, rs.height() / 2 - 115, 350, 230);

    // create and setup window
    about = Window::create();
    about->setBounds(wBounds);
    about->setTitleAlignment(Graphics::Text::Alignment::CENTER);
    about->setColor(Graphics::Color::as_rgb(255, 255, 255), Graphics::Color::as_rgb(0, 0, 0));
    about->setTitle("About MeetiX OS");
    about->setResizable(false);

    // create and setup geoshape as panel of window
    panel = Geoshape::create();
    about->addChild(panel);
    panel->setBounds(Graphics::Metrics::Rectangle(0, 0, 350, 230));
    panel->setPNG("/Apps/OsmosUI/Resources/Icons/OSLogo_hdpi.png",
                  Graphics::Metrics::Point(70, 50));

    // create and configure first line of about text
    line1 = Label::create();
    panel->addChild(line1);
    line1->setBounds(Graphics::Metrics::Rectangle(0, 5, 320, 30));
    line1->setTitleAlignment(Graphics::Text::Alignment::CENTER);
    line1->setFontSize(25);
    line1->setColor(0, Graphics::Color::as_rgb(0, 200, 0));
    line1->setTitle("MeetiX OS " + Utils::Environment::version());

    // create and configure second line of about text
    line2 = Label::create();
    panel->addChild(line2);
    line2->setBounds(Graphics::Metrics::Rectangle(0, 130, 320, 30));
    line2->setTitleAlignment(Graphics::Text::Alignment::CENTER);
    line2->setTitle("[ Copyright (C) 2021, MeetiX OS Project ]");

    // show window about
    about->setVisible(true);

    // if window close delete al components
    about->onClose(close);
}
