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

#include <utils/ipcstream.hpp>

/*========================================= Public =========================================*/

/**
 * default constructor
 */
IPCStream::IPCStream() :
    receiver(-1), ready(false), buffered(false), inPipe(FD_NONE), outPipe(FD_NONE), cache(nullptr), maxCacheSize(0), nodeLength(0) {}

                    /**
                     * filled constructor
                     *
                     * @param _receiver:            the receiver of the ipc
                     * @param _type:                the type of the stream
                     * @param-opt buffered:         set the object to store on buffer the messages provided
                     * @param-opt _size:            the size of a single message or the shared pointer
                     * @param-opt _cacheSize:       number of buffered messages, ignored if buffered is false
                     */
IPCStream::IPCStream(Tid _receiver, Type _type, bool buffered, uint32_t _size, uint32_t _cacheSize) :
    receiver(-1), ready(false), buffered(false), inPipe(FD_NONE), outPipe(FD_NONE), cache(nullptr), maxCacheSize(0), nodeLength(0)
{
    open(receiver, type, buffered, _size, _cacheSize);
}

/**
 * destructor
 */
IPCStream::~IPCStream()
{
    close();
}

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
bool IPCStream::open(Tid _receiver, Type _type, bool _buffered, uint32_t _size, uint32_t _cacheSize)
{
    // copy parameters
    type = _type;
    receiver = _receiver;
    buffered = _buffered;

    // copy cache size if we are in buffered mode
    if (buffered) maxCacheSize = (_cacheSize > 0 && _cacheSize < MAX_CACHE_COUNT ? _cacheSize : MAX_CACHE_COUNT);

    // no accepted negative values
    if (_size > 0)
    {
        // we have to distinct if we are in pipe or message or shared memory mode
        switch (type)
        {
            // in pipe mode there is no limit of length per node
            case Type::PIPE: nodeLength = _size; break;

            // in shared memory pointer there no limit too
            case Type::SH_MEM: nodeLength = _size; break;

            // we have limit only in messages
            case Type::MESSAGE:
                _size = (_size < MESSAGE_MAXIMUM_LENGTH ? _size : 0);
            break;
        }
    }

    else
    {
        close();
        return false;
    }

    // check the alive state of the receiver
    if (!checkReceiverAliveState()) return false;

    // now initialize the remaining data
    return initializeData();
}

/**
 * close the stream and release the resources
 * if flsh flag is set and the stream work in buffered mode
 * before close the stream do the flush
 *
 * @param-opt flsh:     flag that force the flush before close
 */
void IPCStream::close(bool flsh)
{
    // return if we have already close
    if (!ready) return;

    // check flags
    if (buffered && flsh) flush();

    // only pipes need specific finalization
    if (type == Type::PIPE)
    {
        // close the pipes
        Close(inPipe);
        Close(outPipe);

        // reset descriptors
        inPipe = FD_NONE;
        outPipe = FD_NONE;
    }

    // now the stream is to be reopened
    ready = false;
    receiver = -1;
    buffered = false;
    maxCacheSize = 0;
    nodeLength = 0;
}

/**
 * manage the internal properties of the stream
 *
 * @param _size:                the new size of a single message/pointer
 * @param-opt _buffered:        enable disable the buffering
 * @param-opt _cacheSize:       the size of the cache, if is provided 0 the buffering is deactived
 * @return whether the operation success
 */
bool IPCStream::set(uint32_t _size, bool _buffered, uint32_t _cacheSize)
{
    // we have to be ready and with non negative parameters
    if (!ready || _size < 0 || _cacheSize < 0) return false;

    // with messages we have size limit
    if (_size > MESSAGE_MAXIMUM_LENGTH && type == Type::MESSAGE)
        nodeLength = MESSAGE_MAXIMUM_LENGTH;

    // elsewhere no
    else nodeLength = _size;

    // if user want to activate first the buffer we need caches
    if (!buffered && _buffered && !createCache()) return false;

    // if the want to deactive the buffered mode we need to destroy caches
    else if (!_buffered && buffered) destroyCache();

    // copy the new parameter
    buffered = _buffered;

    // assign buffer size
    if (buffered)
        maxCacheSize = (_cacheSize >= 1 && _cacheSize < MAX_CACHE_COUNT ? _cacheSize : MAX_CACHE_COUNT);

    return true;
}

/**
 * flush the message buffer, if success delete the message queue
 *
 * @return whether the flush is success
 */
bool IPCStream::flush()
{
    // we need to be ready and buffered
    if (!ready || !buffered) return false;

    // set buffered flag as false to send unbuffered
    buffered = false;

    // check type of buffer to flush
    bool succes = flushCache();

    // reset as buffer now
    buffered = true;

    return succes;
}

/**
 * write on the input pipe pipe
 *
 * @param message:      the message to write
 * @return whether the send success
 */
bool IPCStream::send(const char *message)
{
    // we have to be ready
    if (!ready || type != Type::PIPE) return false;

    // get message length
    uint32_t msglen = strlen(message);

    // check length validity
    if (msglen > 0 && msglen < nodeLength)
    {
        // write now if we aren't buffered
        if (!buffered)
        {
            // write on pipe now
            FsWriteStatus stat;
            uint32_t writed = WriteS(inPipe, message, msglen, &stat);

            // check write status
            if (stat != FS_WRITE_SUCCESSFUL || writed < msglen) return false;
            return true;
        }

        // store to cache
        return addToCache<char>((void*)message, true, msglen);
    }

    return false;
}

/**
 * receive from output pipe
 *
 * @param buffer:       the buffer where the the stream store the readed characters
 * @return whether the read success
 */
bool IPCStream::receive(char *buffer)
{
    // check stream validity
    if (!ready || !buffer || type != Type::PIPE) return false;

    // create a local buffer
    char tmpBuf[nodeLength];
    FsReadStatus status;

    // read from output stream
    uint32_t length = ReadS(outPipe, tmpBuf, nodeLength, &status);
    if (status == FS_READ_SUCCESSFUL)
    {
        // copy local buffer on out buffer
        memcpy(buffer, tmpBuf, length);
        buffer[length] = '\0';
        return true;
    }

    return false;
}

/**
 * send a message
 *
 * @param message:      the message buffer to send
 * @param size:         the size of the message
 * @return whether the send success
 */
bool IPCStream::send(uint8_t *message, uint32_t size)
{
    // we have to be ready and into ranges
    if (!ready || type != Type::MESSAGE || size < 0 || size > nodeLength) return false;

    // if we not working buffered send immediate the message
    if (!buffered)
    {
        // get transaction id
        MessageTransaction tx = GetMessageTxId();

        // send the message
        if (SendMessageT(receiver, message, size, tx) == MESSAGE_SEND_STATUS_SUCCESSFUL) return true;
        return false;
    }

    // store to cache
    return addToCache<uint8_t>((void*)message, true, size);
}

/**
 * receive a message
 *
 * @param buffer:       the buffer where the the stream store the received message
 * @return whether the operation success
 */
bool IPCStream::receive(uint8_t *buffer)
{
    // check stream validity
    if (!ready || !buffer || type != Type::MESSAGE) return false;

    // create a local buffer
    uint8_t tmpBuf[nodeLength];
    MessageTransaction tx = GetMessageTxId();
    MessageReceiveStatus status = ReceiveMessageT(tmpBuf, nodeLength, tx);

    // check receive status
    if (status == MESSAGE_SEND_STATUS_SUCCESSFUL)
    {
        // copy local on out buffer
        memcpy(buffer, tmpBuf, nodeLength);
        return true;
    }

    return false;
}

/**
 * send a signal
 *
 * @param signal:       the signal to send
 * @return whether the send success
 */
bool IPCStream::send(int32_t signal)
{
    // we have to be ready
    if (!ready || type != Type::SIGNAL || signal < 0) return false;

    // immediate send with non buffered mode
    if (!buffered)
    {
        // send the signal
        if (RaiseSignal(receiver, signal) == RAISE_SIGNAL_STATUS_SUCCESSFUL) return true;
        return false;
    }

    // store to cache
    return addToCache<uint32_t>(&signal);
}

/**
 * share a pointer
 *
 * @param pointer:      the pointer that we want to share
 * @param size:         the size of the pointer
 * @return whether the share success
 */
void *IPCStream::share(void *pointer, uint32_t size)
{
    // we have to be ready
    if (!ready || type != Type::SH_MEM || size < 0) return nullptr;

    // buffering is not avaible for shared memory
    return ShareMem(pointer, size, receiver);
}

/*========================================= Private =========================================*/

/**
 * destroy the buffer cache
 */
void IPCStream::destroyCache()
{
    // do only if exist
    if (cache)
    {
        // destroy it's content if exist
        if (cache->count() > 0)
        {
            // parse with iterator
            for (llist<void*>::iterator it = cache->begin(); it != cache->end(); it++)
            {
                // each type have a different type of data
                switch (type)
                {
                    // pipe buffer
                    case Type::PIPE:
                        delete[] static_cast<char*>(*it);
                    break;

                    // signal buffer
                    case Type::SIGNAL:
                        delete (static_cast<uint32_t*>(*it));
                    break;

                    // message buffer
                    case Type::MESSAGE:
                        delete[] (static_cast<uint8_t*>(*it));
                    break;
                }
            }
        }

        // destroying the list the cache is to recreate
        delete cache;
        cache = nullptr;
    }
}

/**
 * flush the buffer cache while destroyng the sended node
 *
 * @return false on error, true if flush success
 */
bool IPCStream::flushCache()
{
    bool sendStat = true;
    void *current = nullptr;
    while ((current = cache->front()) && sendStat)
    {
        switch (type)
        {
            // write on pipe
            case Type::PIPE:
            {
                // cast the buffer
                char *buffer = static_cast<char*>(current);

                // check send status and destroy it
                if (!send(buffer)) return false;
                delete[] buffer;
            }
            break;

            // send the signal
            case Type::SIGNAL:
            {
                // cast the buffer
                uint32_t *signal = static_cast<uint32_t*>(current);

                // check send status
                if (!send(*signal)) return false;
                delete signal;
            }
            break;

            // send the message
            case Type::MESSAGE:
            {
                // cast the buffer
                uint8_t *buffer = static_cast<uint8_t*>(current);

                // check send status
                if (!send(buffer, sizeof(MESSAGE_CONTENT(buffer)))) return false;
                delete[] buffer;
            }
            break;
        }

        // destroy the current node
        cache->popFront();
    }

    return true;
}

/**
 * open the initial pipes when stream is in PIPE mode
 *
 * @return whether the operation success
 */
bool IPCStream::openPipes()
{
    // open the read pipe
    FsPipeStatus outStat;
    File_t inTmpRead;
    File_t outTmpRead;
    PipeS(&inTmpRead, &outTmpRead, &outStat);
    if (outStat != FS_PIPE_SUCCESSFUL) return false;

    // open the write pipe
    FsPipeStatus inStat;
    File_t inTmpWrite;
    File_t outTmpWrite;
    PipeS(&inTmpWrite, &outTmpWrite, &inStat);
    if (outStat != FS_PIPE_SUCCESSFUL) return false;

    // opens are success
    return true;
}

/**
 * initializes the data from type
 *
 * @return whether the initialization success
 */
bool IPCStream::initializeData()
{
    // pipe need specific initialization
    if (type == Type::PIPE)
        if (!openPipes()) return (ready = false);

    // create the buffer if we need
    if (buffered) return (ready = createCache());

    // check instantiation success
    return (ready = true);
}
