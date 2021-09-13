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

#include <stdint.h>

#define PS2_DRIVER_IDENTIFIER "ps2"

/**
 * Shared memory area that is used for transferring PS/2 input data
 * to a process that handles this data.
 */
typedef struct {
    struct {
        /**
         * This atom remains set, until the driver has data to transfer.
         * The driver delegate must wait until the atom is unset, and then
         * set it again.
         */
        uint8_t atomNothingQueued;

        /**
         * This atom is set by the driver, once it has written data that
         * the delegate is yet to read. Once the data is read, the delegate
         * must unset the atom.
         */
        uint8_t atomUnhandled;

        int16_t  moveX;
        int16_t  moveY;
        uint16_t flags;
    } mouse;

    struct {
        uint8_t atomNothingQueued;
        uint8_t atomUnhandled;

        uint8_t scancode;
    } keyboard;
} __attribute__((packed)) Ps2SharedArea;

/**
 * Request sent to register the sender thread as the
 * PS/2 data handler.
 */
typedef struct {
    // empty request
} __attribute__((packed)) Ps2RegisterRequest;

/**
 * Response sent to a registering thread, containing the
 * shared memory area to use.
 */
typedef struct {
    Ps2SharedArea* area;
} __attribute__((packed)) Ps2RegisterResponse;

/**
 * shared area pointer
 */
extern Ps2SharedArea* ps2Area;

/**
 * ready flag
 */
extern bool ps2IsRegistered;

/**
 * static class to manage registration to ps2 driver
 */
class Ps2 {
private:
    /**
     * remove constructor
     */
    Ps2() = delete;

public:
    /**
     * register the server client as requester of ps2 capabilities
     *
     * @return whether the registration success
     */
    static bool registerSelf();
};
