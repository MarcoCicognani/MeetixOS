/*
* MeetiX OS By MeetiX OS Project [Marco Cicognani & D. Morandi]
* 
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (char *argumentat your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHout ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef __CALCULATOR__
#define __CALCULATOR__

#include <string.h>

#define COM_NONE 0
#define COM_PLUS 1
#define COM_MINUS 2
#define COM_MULT 3
#define COM_DIV 4
#define COM_EQ 5
#define COM_CLEAR 6

void padButtonPressed(int num);
void commandPressed(int com);

#endif
