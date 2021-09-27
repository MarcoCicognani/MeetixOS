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

#include "filesystem/pipes.hpp"

#include "logger/logger.hpp"
#include "utils/HashMap.hpp"

/**
 *
 */
static PipeID                  pipeNextID = 0;
static HashMap<PipeID, Pipe*>* pipes;

/**
 *
 */
void Pipes::initialize() {
    pipes = new HashMap<PipeID, Pipe*>();
}

/**
 *
 */
PipeID Pipes::create() {
    PipeID id = -1;

    auto pipe        = new Pipe();
    pipe->buffer     = new uint8_t[PIPE_DEFAULT_CAPACITY];
    pipe->write      = pipe->buffer;
    pipe->read       = pipe->buffer;
    pipe->size       = 0;
    pipe->capacity   = PIPE_DEFAULT_CAPACITY;
    pipe->references = 0;

    if ( pipe->buffer != 0 ) {
        id = pipeNextID++;
        pipes->add(id, pipe);
    }

    return id;
}

/**
 *
 */
Pipe* Pipes::get(PipeID id) {
    HashMap<PipeID, Pipe*>::MapNode_t* entry = pipes->get(id);
    if ( entry )
        return entry->value;
    return nullptr;
}

/**
 *
 */
void Pipes::addReference(PipeID id, Pid pid) {
    HashMap<PipeID, Pipe*>::MapNode_t* pipeEntry = pipes->get(id);
    if ( pipeEntry ) {
        auto pipe = pipeEntry->value;

        auto entry       = new ListEntry<Pid>;
        entry->value     = pid;
        entry->next      = pipe->references;
        pipe->references = entry;
    }
}

/**
 *
 */
bool Pipes::hasReferenceFromOtherProcess(Pipe* pipe, Pid pid) {
    ListEntry<Pid>* entry = pipe->references;
    while ( entry ) {
        if ( entry->value != pid )
            return true;
        entry = entry->next;
    }
    return false;
}

/**
 *
 */
void Pipes::removeReference(PipeID id, Pid pid) {
    HashMap<PipeID, Pipe*>::MapNode_t* pipeEntry = pipes->get(id);
    if ( pipeEntry ) {
        auto pipe = pipeEntry->value;

        // find entry and remove
        ListEntry<Pid>* prev  = 0;
        ListEntry<Pid>* entry = pipe->references;
        while ( entry ) {
            if ( entry->value == pid ) {
                if ( prev == 0 )
                    pipe->references = entry->next;
                else
                    prev->next = entry->next;

                delete entry;
                break;
            }
            prev  = entry;
            entry = entry->next;
        }

        // no entry left?
        if ( pipe->references == 0 ) {
            pipes->erase(id);

            logDebug("%! removing non-referenced pipe %i", "pipes", id);
            delete pipe->buffer;
            delete pipe;
        }
    }
}
