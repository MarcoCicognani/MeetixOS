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

#ifndef _Graphic_FrontEnd_H_
#define _Graphic_FrontEnd_H_

#include "FrontEnd.hpp"

#include <cairo/cairo.h>
#include <fstream>
#include <Graphics/Text/Font.hh>
#include <Graphics/Text/FontLoader.hh>
#include <Graphics/Text/Layouter.hh>
#include <GUI/Application.hh>
#include <GUI/Component/Button.hh>
#include <GUI/Component/Canvas.hh>
#include <GUI/Component/Label.hh>
#include <GUI/Component/TextField.hh>
#include <GUI/Component/Window.hh>
#include <GUI/Listener/ActionListener.hh>
#include <GUI/Listener/FocusListener.hh>
#include <GUI/Listener/KeyListener.hpp>
#include <GUI/MessageBox.hh>
#include <list>
#include <LibTasking/Lock.hh>
#include <LibUtils/Utils.hh>

using namespace std;

class GraphicFrontEnd_t : public FrontEnd {
public:
    // constructor
    GraphicFrontEnd_t();

    virtual void clean();

    virtual void end();

    virtual void write(string message, string title);

    virtual IO::Keyboard::Info readInput();

private:
    void initialize();
    void paintEntry();
    void paintLoop();
};

#endif
