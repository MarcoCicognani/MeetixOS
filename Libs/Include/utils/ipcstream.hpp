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

#ifndef MEETIX_LIBRARY_UTILS_IPCSTREAM
#define MEETIX_LIBRARY_UTILS_IPCSTREAM

#include <Api.h>
#include <Api/utils/llist.hpp>
#include <stdint.h>
#include <string.h>
#include <string>

/**
 * maximum storable cache
 */
#define MAX_CACHE_COUNT 10

/**
 * class to manage high level interprocess comunication
 */
class IPCStream {
public:
    /**
     * struct to store message pointer and it's size
     */
    template<typename T>
    struct Packet {
    public:
        /**
         * default constructor
         */
        Packet() : bufptr(nullptr), size(0) {
        }

        /**
         * constructor
         *
         * @param pointer
         *      the pointer to the message buffer
         *
         * @param size
         *      the size of the buffer
         */
        Packet(T* pointer, uint32_t size) : bufptr(pointer), size(size) {
        }

        /**
         * internal data
         */
        T*       bufptr; // the pointer to the buffer
        uint32_t size;   // the size of the buffer
    } A_PACKED;

    /**
     * ipc stream type
     */
    enum class Type : uint8_t
    {
        PIPE,
        SH_MEM,
        SIGNAL,
        MESSAGE,
    };

    /**
     * default constructor
     */
    IPCStream();

    /**
     * filled constructor
     *
     * @param _receiver:            the receiver of the ipc
     * @param _type:                the type of the stream
     * @param-opt buffered:         set the object to store on buffer the messages provided
     * @param-opt _size:            the size of a single message or the shared pointer
     * @param-opt _cacheSize:       number of buffered messages, ignored if buffered is false
     */
    IPCStream(Tid      _receiver,
              Type     _type,
              bool     buffered   = false,
              uint32_t _size      = 0,
              uint32_t _cacheSize = 0);

    /**
     * no copy
     */
    IPCStream(const IPCStream& stream) = delete;

    /**
     * destructor
     */
    ~IPCStream();

    /**
     * open an IPCStream of provided type
     *
     * @param _receiver:            the receiver of the ipc
     * @param _type:                the type of the stream
     * @param-opt _size:            the size of a single input
     * @param-opt _buffered:        set the object to store on buffer the inputs provided
     * @param-opt _cacheSize:       number of bufferable messages, ignored if buffered flag is false
     * @return whether the operation success
     */
    bool open(Tid      _receiver,
              Type     _type,
              bool     _buffered  = false,
              uint32_t _size      = 0,
              uint32_t _cacheSize = 0);

    /**
     * close the stream and release the resources
     * if flsh flag is set and the stream work in buffered mode
     * before close the stream do the flush
     *
     * @param-opt flsh:     flag that force the flush before close
     */
    void close(bool flsh = false);

    /**
     * manage the internal properties of the stream
     *
     * @param _size:                the new size of a single message/pointer
     * @param-opt _buffered:        enable disable the buffering
     * @param-opt _cacheSize:       the size of the cache, if is provided 0 the buffering is
     * deactived
     * @return whether the operation success
     */
    bool set(uint32_t _size, bool _buffered = false, uint32_t _cacheSize = 0);

    /**
     * flush the message buffer, if success delete the message queue
     *
     * @return whether the flush is success
     */
    bool flush();

    /**
     * write on the input pipe pipe
     *
     * @param message:      the message to write
     * @return whether the send success
     */
    bool send(const char* message);

    /**
     * pipe << operator overload
     *
     * @param message:      the string to be writed
     * @return whether the send success
     */
    inline bool operator<<(const char* message) {
        return send(message);
    }
    inline bool operator<<(const std::string& message) {
        return send(message.c_str());
    }

    /**
     * receive from output pipe
     *
     * @param buffer:       the buffer where the the stream store the readed characters
     * @return whether the read success
     */
    bool receive(char* buffer);

    /**
     * pipe >> operator overload
     *
     * @param out:      the reference to the output buffer
     * @return whether the operation success
     */
    inline bool operator>>(char* buffer) {
        return receive(buffer);
    }
    inline bool operator>>(std::string& out) {
        return receive((char*)out.c_str());
    }

    /**
     * send a message
     *
     * @param message:      the message buffer to send
     * @param size:         the size of the message
     * @return whether the send success
     */
    bool send(uint8_t* message, uint32_t size);

    /**
     * packet message << operator overload
     *
     * @param pack:     the pack to be sended
     * @return whether the send success
     */
    inline bool operator<<(const Packet<uint8_t>& pack) {
        return send((uint8_t*)pack.bufptr, pack.size);
    }

    /**
     * receive a message
     *
     * @param buffer:       the buffer where the the stream store the received message
     * @return whether the operation success
     */
    bool receive(uint8_t* buffer);

    /**
     * message >> operator overload
     *
     * @param buffer:       the buffer where the the stream store the received message
     * @return whether the operation success
     */
    inline bool operator>>(uint8_t* buffer) {
        return receive(buffer);
    }

    /**
     * send a signal
     *
     * @param signal:       the signal to send
     * @return whether the send success
     */
    bool send(int32_t signal);

    /**
     * signal << operator overload
     *
     * @param signal:       the signal to send
     * @return whether the operation success
     */
    inline bool operator<<(uint32_t signal) {
        return send(signal);
    }

    /**
     * share a pointer
     *
     * @param pointer:      the pointer that we want to share
     * @param size:         the size of the pointer
     * @return whether the share success
     */
    void* share(void* pointer, uint32_t size);

    /**
     * packet message << operator overload
     *
     * @param pack:     the pack to be sended
     * @return whether the send success
     */
    inline void* operator<<(const Packet<void>& pack) {
        return share(pack.bufptr, pack.size);
    }

    /**
     * check the stream type
     *
     * @return the type of the stream
     */
    inline Type getType() const {
        return type;
    }

    /**
     * check the ready state
     *
     * @return the ready flag
     */
    inline bool isOpen() const {
        return ready;
    }

    /**
     * check the configured buffer cache node length
     *
     * @return the cache node size
     */
    inline uint32_t getNodeLength() const {
        return nodeLength;
    }

    /**
     * check the size of stored nodes into cache
     *
     * @return the effective node count
     */
    inline uint32_t getCacheSize() const {
        return cache->count();
    }

    /**
     * check the max cache size storable
     *
     * @return the max node count of cache
     */
    inline uint32_t getMaxCacheSize() const {
        return maxCacheSize;
    }

private:
    /**
     * stream data
     */
    Type    type;     // type of ipc
    Tid     receiver; // the receiver of the ipc
    uint8_t ready;    // ready flag
    uint8_t buffered; // buffered flag

    /**
     * cache data, used by the stream when buffered flag is true
     */
    llist<void*>* cache;        // cache for buffered mode
    uint32_t      maxCacheSize; // max storable buffers into cache
    uint32_t      nodeLength;   // length of a single buffer inot cache

    /**
     * pipe descriptors
     * opened when stream is in PIPE mode
     */
    FileHandle inPipe;  // pipe where stream write
    FileHandle outPipe; // pipe where stream read

    /**
     * instantiate the cache object
     * and return whether the allocation success
     *
     * @return whether the operation success
     */
    inline bool createCache() {
        // instantiate the object
        cache = new llist<void*>();
        return (cache != nullptr);
    }

    /**
     * add to cache and return the flushing status if the cache have to bec flushed
     *
     * @template T:        the type to use for pointer creation
     * @param buffer:           the buffer to copy into created new buffer
     * @param-opt vector:       flag to determine if pointer to create is a buffer
     * @param-opt size:         the size of the buffer to create, ignored if vector flag is false
     * @return the operation status
     */
    template<typename T>
    addToCache(void* buffer, bool vector = false, uint32_t size = 0) {
        // create type pointer
        T* newBuffer;

        // create as a vector
        if ( vector && size > 0 )
            newBuffer = new T[size];
        else
            newBuffer = new T();

        // check pointer validity
        if ( newBuffer ) {
            // copy memory
            if ( vector )
                memcpy(newBuffer, buffer, size);
            else
                *newBuffer = *((T*)buffer);

            // add to cache
            cache->add((void*)newBuffer);
            if ( cache->count() > maxCacheSize )
                return flush();
            return true;
        }

        // no memory
        return false;
    }

    /**
     * destroy the buffer cache
     */
    void destroyCache();

    /**
     * flush the buffer cache while destroyng the sended node
     *
     * @return false on error, true if flush success
     */
    bool flushCache();

    /**
     * check if the receiver is alive
     */
    inline bool checkReceiverAliveState() {
        return (s_get_pid_for_tid(receiver) != -1 ? true : false);
    }

    /**
     * open the initial pipes when stream is in PIPE mode
     *
     * @return  whether the operation success
     */
    bool openPipes();

    /**
     * initializes the data from type
     *
     * @return whether the initialization success
     */
    bool initializeData();
};

#endif
