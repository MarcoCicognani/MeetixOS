/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * *
 * MeetiX OS By MeetiX OS Project [Marco Cicognani]                                    *
 *                                                                                     *
 * This program is free software; you can redistribute it and/or                       *
 * modify it under the terms of the GNU General Public License                         *
 * as published by the Free Software Foundation; either version 2                      *
 * of the License, or (char *argumentat your option) any later version.                *
 *                                                                                     *
 * This program is distributed in the hope that it will be useful,                     *
 * but WITHout ANY WARRANTY; without even the implied warranty of                      *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                       *
 * GNU General Public License for more details.                                        *
 *                                                                                     *
 * You should have received a copy of the GNU General Public License                   *
 * along with this program; if not, write to the Free Software                         *
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA      *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * */

#include <Api.h>
#include <gui/actionlistener.hpp>
#include <gui/button.hpp>
#include <gui/geoshape.hpp>
#include <gui/label.hpp>
#include <gui/ui.hpp>
#include <gui/window.hpp>
#include <utils/environment.hpp>

// ui style, i change the presentation in base the style
enum class UiStyle_t : uint8_t
{
    KDE,
    GNOME
};

// style
static UiStyle_t style;

// display forms
static Geoshape* backGround;
static Window*   mainWindow;

// labels that contains esplanation text
static Label* lb1;
static Label* lb2;
static Label* lb3;
static Label* lb4;
static Label* lb5;

// button for events
static Button* nextButton;
static Button* PrevButton;
// resolutions
static Dimension resolution;
static Rectangle windowDimension;

// bookmark
int page;

// waiter block
uint8_t blocker = true;

// labels contentainer
static std::string rg1[5];
static std::string rg2[5];
static std::string rg3[5];
static std::string rg4[5];
static std::string rg5[5];

/*
 * fill the presentation lines
 */
void fillPresentation() {
    // fill the first lines
    rg1[0]
        = "Hi " + Environment::get("USER") + " welcome to MeetiX OS " + Environment::get("VERSION");
    rg1[1] = "Okay, we can begin";
    rg1[2] = "";
    rg1[3] = "The graphic terminal CandyShell allows you to interact more deeply with the system";
    rg1[4] = "";

    // fill the second lines
    rg2[0] = "I will guide you with a brief introduction to the main aspects of the graphical "
             "interface";
    if ( style == UiStyle_t::KDE )
        rg2[1] = "The OsmosUI has a KDE style, so you can find a bar on the bottom of the screen,";
    else
        rg2[1] = "The OsmosUI has a GNOME style, so you can find a bar on the top of the screen,";
    rg2[2] = "In version 2.4, OsmosUI comes with a module called OsmosDock,";
    rg2[3] = "To run commands and launch new applications in the background";
    rg2[4] = "Remember: MeetiX OS is owned by the MeetiX OS Project";

    // fill the third lines
    rg3[0] = "so you gain confidence with the system and applications; click the 'X' to exit "
             "whenever you want";
    rg3[1] = "Starting from the left, a menu with the applications list and real-time memory "
             "consumption indicator.";
    rg3[2] = "an integrated dock to quickly run MeetiX OS applications.";
    rg3[3] = "";
    rg3[4] = "whose developer is only the founder Marco Cicognani";

    // fill the fourth lines
    rg4[0] = "MeetiX OS is an open source software under GPL3 licence";
    rg4[1] = "At the center of the bar is the window name manager, which displays the names of "
             "running applications";
    rg4[2] = "Only available if you s_log on as admin.";
    rg4[3] = "";
    rg4[4] = "";

    // fill the fifth lines
    rg5[0] = "https://github/MarcoCicognani/MeetiX-OS-Project";
    rg5[1] = "While at the bottom right the watch clocked in hours:minutes:seconds";
    rg5[2] = "";
    rg5[3] = "";
    rg5[4] = "Try it, and if it stops restart the virtual machine,";
}

/*
 *	show on labels provided index page
 */
static void showPage(int index) {
    if ( index == 1 ) {
        mainWindow->setBounds(windowDimension);
        if ( style == UiStyle_t::GNOME )
            backGround->setPNG("/app/welcome/deps/icon/arrow_up.png",
                               Point(resolution.width / 2 - 53, 0));
        else
            backGround->setPNG("/app/welcome/deps/icon/arrow_down.png",
                               Point(resolution.width / 2 - 53, resolution.height - 230));

        lb1->setTitle(rg1[index]);
        lb2->setTitle(rg2[index]);
        lb3->setTitle(rg3[index]);
        lb4->setTitle(rg4[index]);
        lb5->setTitle(rg5[index]);
    }

    else if ( index == 2 ) {
        mainWindow->setBounds(windowDimension);
        if ( style == UiStyle_t::GNOME )
            backGround->setPNG("/app/welcome/deps/icon/arrow_down.png",
                               Point(resolution.width / 2 - 53, resolution.height - 300));
        lb1->setTitle(rg1[index]);
        lb2->setTitle(rg2[index]);
        lb3->setTitle(rg3[index]);
        lb4->setTitle(rg4[index]);
        lb5->setTitle(rg5[index]);
    }

    else if ( index == 3 ) {
        mainWindow->setBounds(windowDimension);
        if ( style == UiStyle_t::GNOME )
            backGround->setPNG("/app/welcome/deps/icon/arrow_down.png",
                               Point(resolution.width / 2 - 200, resolution.height - 300));
        lb1->setTitle(rg1[index]);
        lb2->setTitle(rg2[index]);
        lb3->setTitle(rg3[index]);
        lb4->setTitle(rg4[index]);
        lb5->setTitle(rg5[index]);

        nextButton->setColor(ARGB(180, 0, 200, 0), RGB(0, 0, 0));
        nextButton->setTitle("Next");
    }

    else if ( index == 4 ) {
        mainWindow->setBounds(windowDimension);
        lb1->setTitle(rg1[index]);
        lb2->setTitle(rg2[index]);
        lb3->setTitle(rg3[index]);
        lb4->setTitle(rg4[index]);

        lb5->setColor(0, RGB(200, 0, 0));
        lb5->setTitle(rg5[index]);
        lb5->setColor(0, RGB(0, 0, 0));

        nextButton->setColor(RGB(200, 0, 0), RGB(0, 0, 0));
        nextButton->setFontSize(20);
        nextButton->setTitle("s_exit");
        nextButton->setFontSize(14);
    }

    else if ( index == 5 )
        blocker = false;
}

/*
 *	handler for button next
 */
class ButtonNextListener : public ActionListener {
public:
    virtual void handleAction() {
        showPage(++page);
    }
};

/*
 *	handler for button previous
 */
class ButtonPrevListener : public ActionListener {
public:
    virtual void handleAction() {
        showPage(--page);
    }
};

/*
 * livewelcome, explain basic information for MeetiX OS GUI
 */
int main(int argc, char* argv[]) {
    // open comunication with the window server
    if ( UI::open() == UI_OPEN_STATUS_SUCCESSFUL ) {
        // get screen resolution and set window resolution
        resolution = UI::getResolution();
        windowDimension
            = Rectangle(resolution.width / 2 - 375, resolution.height / 2 - 225, 750, 450);

        // get the style from environment
        style = Environment::get("UISTYLE") == "KDE" ? UiStyle_t::KDE : UiStyle_t::GNOME;

        // fill lines
        fillPresentation();

        // setting background geoshape
        backGround = Geoshape::create();
        backGround->setBounds(Rectangle(0,
                                        style == UiStyle_t::KDE ? 0 : 30,
                                        resolution.width,
                                        resolution.height - (style == UiStyle_t::KDE ? 30 : 94)));
        backGround->setColor(ARGB(120, 0, 0, 0), RGB(255, 255, 255));

        // set frontend window
        mainWindow = Window::create();
        mainWindow->setBounds(windowDimension);
        mainWindow->setTitleAlignment(TextAlignment::CENTER);
        mainWindow->setTitle("Welcome to the Meetix Operating System");
        mainWindow->setResizable(false);
        mainWindow->onClose([] { blocker = false; });

        // configuring label
        lb1 = Label::create();
        lb1->setBounds(Rectangle(0, 20, windowDimension.width, 30));
        lb1->setTitleAlignment(TextAlignment::CENTER);
        mainWindow->addChild(lb1);

        // configuring label
        lb2 = Label::create();
        lb2->setBounds(Rectangle(0, 50, windowDimension.width, 30));
        lb2->setTitleAlignment(TextAlignment::CENTER);
        mainWindow->addChild(lb2);

        // configuring label
        lb3 = Label::create();
        lb3->setBounds(Rectangle(0, 80, windowDimension.width, 30));
        lb3->setTitleAlignment(TextAlignment::CENTER);
        mainWindow->addChild(lb3);

        // configuring label
        lb4 = Label::create();
        lb4->setBounds(Rectangle(0, 110, windowDimension.width, 30));
        lb4->setTitleAlignment(TextAlignment::CENTER);
        mainWindow->addChild(lb4);

        // configuring label
        lb5 = Label::create();
        lb5->setBounds(Rectangle(0, 140, windowDimension.width, 30));
        lb5->setTitleAlignment(TextAlignment::CENTER);
        mainWindow->addChild(lb5);

        // show the window
        mainWindow->setVisible(true);

        // configuring button
        nextButton = Button::create();
        nextButton->setBounds(Rectangle(windowDimension.width / 2 - 200, 300, 400, 30));
        nextButton->setColor(ARGB(180, 0, 200, 0), RGB(0, 0, 0));

        nextButton->setTitle("Next");
        nextButton->setActionListener(new ButtonNextListener());
        mainWindow->addChild(nextButton);

        // configuring button
        PrevButton = Button::create();
        PrevButton->setBounds(Rectangle(windowDimension.width / 2 - 200, 350, 400, 30));
        PrevButton->setColor(ARGB(180, 0, 200, 0), RGB(0, 0, 0));

        PrevButton->setTitle("Previous");
        PrevButton->setActionListener(new ButtonPrevListener());
        mainWindow->addChild(PrevButton);

        // set page to first
        page = 0;

        // initialize labels
        lb1->setColor(0, RGB(20, 200, 20));
        lb1->setFontSize(30);
        lb1->setTitle(rg1[page]);
        lb1->setColor(0, RGB(0, 0, 0));
        lb1->setFontSize(14);

        lb2->setTitle(rg2[page]);
        lb3->setTitle(rg3[page]);
        lb4->setTitle(rg4[page]);

        lb5->setColor(0, RGB(200, 0, 0));
        lb5->setFontSize(20);
        lb5->setTitle(rg5[page]);
        lb5->setColor(0, RGB(0, 0, 0));
        lb5->setFontSize(14);

        // wait for events
        s_atomic_block(&blocker);

        // remove labels
        delete lb1;
        delete lb2;
        delete lb3;
        delete lb4;
        delete lb5;

        // remove buttons
        delete nextButton;
        delete PrevButton;

        // remove containers
        delete backGround;
        delete mainWindow;

        // close the interface connection
        UI::close();
    }
}
