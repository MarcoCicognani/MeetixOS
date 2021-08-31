/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * *
* MeetiX OS By MeetiX OS Project [Marco Cicognani]                                    *
*                                                                                     *
*         DERIVED FROM THE GHOST OPERATING SYSTEM                                     *
*         This software is derived from the Ghost operating system project,           *
*         written by Max Schlüssel <lokoxe@gmail.com>. Copyright 2012-2017            *
*         https://ghostkernel.org/                                                    *
*         https://github.com/maxdev1/ghost                                            *
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

#ifndef __CANVAS__
#define __CANVAS__

#include <components/component.hpp>

/**
 *
 */
struct BufferInfo
{
	uint8_t *localMapping;
	uint8_t *remoteMapping;
	uint16_t pages;
	bool acknowledged;
};

/**
 *
 */
class Canvas_t : public Component_t
{
public:
	Pid partnerProcess;
	Tid partnerThread;

	BufferInfo currentBuffer;
	BufferInfo nextBuffer;

	bool mustCheckAgain;

	Canvas_t(Tid partnerThread);
	virtual ~Canvas_t();

	/*
	 * Components
	 */
	virtual void paint();
	virtual bool handle() { return false; }
	virtual void handleBoundChange(Rectangle oldBounds);

	void createNewBuffer(uint16_t requiredPages);
	void clientHasAcknowledgedCurrentBuffer();
	void requestClientToAcknowledgeNewBuffer();
	void blit();

private:
	void checkBuffer();
};

#endif
