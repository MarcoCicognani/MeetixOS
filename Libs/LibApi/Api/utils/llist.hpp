/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * *
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

#ifndef _LINKED_LIST_
#define _LINKED_LIST_

#include <stdint.h>

/**
 * template list with internal linked list
 */
template<class Type_t>
class llist {
public:
    /**
     * node descriptor
     */
    struct Node_t {
        // constructor
        Node_t(Type_t value) : value(value), next(0), prev(0) {
        }

        // value
        Type_t value;

        // pointers
        Node_t* next;
        Node_t* prev;
    };

    /**
     * internal iterator of class
     */
    class iterator {
    public:
        /**
         *	empty constructor
         */
        iterator() : list(0), _current(0), _index(0) {
        }

        /**
         *	initialized constructor
         *
         * @param list:     		pointer to an initialized list
         * @param reverse:  		set the iterator direction
         * @param-opt first:		used by the find property to provide the finded node
         * @param-opt firstIndex:	used by the find property to provide the finded node index
         */
        iterator(llist* list, bool reverse, Node_t* first = 0, uint32_t firstIndex = 0)
            : list(list), _current(0), _index(firstIndex) {
            if ( first )
                _current = first;
            else if ( !reverse && list->_firstNode )
                _current = list->_firstNode;
            else if ( reverse && list->_lastNode ) {
                _current = list->_lastNode;
                _index   = list->_size;
            }
        }

        /**
         *	copy constructor
         *
         * @param it:   reference to an initialized constructor
         */
        iterator(const iterator& it) : list(it.list), _current(it._current), _index(it._index) {
        }

        /**
         *	increment prefix operator
         *
         * @return the object update
         */
        iterator& operator++() {
            // if there is the next set it
            if ( (_current->next != 0) && (_index < list->_size) ) {
                _current = _current->next;
                _index++;
            }

            // reset current entry
            else
                _current = 0;

            // update object
            return *this;
        }

        /**
         * increment postfix operator
         *
         * @return the object update
         */
        iterator& operator++(int) {
            return operator++();
        }

        /**
         * decrement prefix operator
         *
         * @return the object update
         */
        iterator& operator--() {
            // if there is the previous set it
            if ( (_current->prev != 0) && (_index > 0) ) {
                _current = _current->prev;
                _index--;
            }

            // reset current entry
            else
                _current = 0;

            // update object
            return *this;
        }

        /**
         * decrement postfix operator
         *
         * @return the object update
         */
        iterator& operator--(int) {
            return operator--();
        }

        /**
         * increment current entry of provided size
         *
         * @param size:   integer value of increment
         * @return the object update
         */
        iterator& operator+(uint32_t size) {
            return increment(size);
        }

        /**
         * increment and assign current entry of provided size
         *
         * @param size:  integer value of increment
         * @return the object update
         */
        iterator& operator+=(uint32_t size) {
            return (*this = increment(size));
        }

        /**
         * increment current entry of provided size
         *
         * @param size:   integer size of decrement
         * @return the object update
         */
        iterator& operator-(uint32_t size) {
            return decrement(size);
        }

        /**
         * increment and assign current entry of provided size
         *
         * @param size:   integer size of decrement
         * @return object update
         */
        iterator& operator-=(uint32_t size) {
            return (*this = decrement(size));
        }

        /**
         * arrow operator
         *
         * @return the Node_t instance on current position
         */
        Node_t* operator->() const {
            return _current;
        }

        /**
         * asterisk operator
         *
         * @return the Node_t value on current position
         */
        Type_t operator*() const {
            return (_current->value);
        }

        /**
         * check operators overload
         */
        friend bool operator==(const iterator& lhs, const iterator& rhs) {
            return lhs._current == rhs._current;
        }
        friend bool operator!=(const iterator& lhs, const iterator& rhs) {
            return !(lhs == rhs);
        }
        friend bool operator!(const iterator& it) {
            return ((it.list == 0) || (it._current == 0));
        }

    private:
        /**
         * internal properties
         */
        llist*   list;     // pointer to list to iterate
        Node_t*  _current; // current node on iterator
        uint32_t _index;   // current list index

        /**
         * increment of size position the iterator
         *
         * @param size:     the units of iterator increment
         * @return the object update
         */
        iterator& increment(uint32_t size) {
            // increment of size
            uint32_t i = 0;
            while ( (_current != 0) && (_current->next != 0) && (_index < list->_size)
                    && (i <= size) ) {
                _current = _current->next;
                _index++;
                i++;
            }

            // update the object
            return *this;
        }

        /**
         * decrement of size position the iterator
         *
         * @param size:     the units of iterator decrement
         * @return the object update
         */
        iterator& decrement(uint32_t size) {
            // decrement of size
            uint32_t i = 0;
            while ( (_current != 0) && (_current->next != 0) && (_index < list->_size)
                    && (i <= size) ) {
                _current = _current->prev;
                _index++;
                i++;
            }

            // update the object
            return *this;
        }

        // declare as friend the list class to use it's properties
        friend class llist;

    } A_PACKED;

    /**
     * empty constructor
     */
    llist() : _size(0), _firstNode(0), _lastNode(0), _currentNode(0), _lastIndex(0) {
    }

    /**
     * destructor delete all nodes
     */
    ~llist() {
        clear();
    }

    /**
     *	insert a value to back of the llist
     *
     * @param value:     the new value to be added
     */
    void add(Type_t value) {
        // try to add to back of list
        if ( !_firstNode ) {
            _firstNode = new Node_t(value);
            _lastNode  = _firstNode;
            _size++;
        }

        // link to the end of the list
        else if ( !_lastNode->next ) {
            _lastNode->next       = new Node_t(value); // create new node
            _lastNode->next->prev = _lastNode;         // link previous of new node previous

            _lastNode = _lastNode->next; // save as last node the new node
            _size++;                     // increment list size
        }
    }

    /**
     * insert a value on the front of the list
     *
     * @param value:     the new value to be added
     */
    void addFront(Type_t value) {
        // no first node, add first
        if ( !_firstNode ) {
            _firstNode = new Node_t(value);
            _lastNode  = _firstNode;
        }

        // add to the front of the first node
        else {
            // create a new node
            Node_t* newNode = new Node_t(value);

            // link to the first node
            newNode->next = _firstNode;
            newNode->prev = _lastNode;

            // adjust linkage
            _firstNode->prev = newNode;
            _lastNode->next  = newNode;

            // set the new node as first
            _firstNode = newNode;
        }

        // increase size
        _size++;
    }

    /**
     * get a value from llist
     *
     * @param position:     the positon of the node that we want
     * @return the value of the node requested if exist
     */
    Type_t get(uint32_t position) {
        // if is requested first index and list have almost one node return value
        if ( !position && _firstNode ) {
            _currentNode = _firstNode;
            return _currentNode->value;
        }

        // find the value from the last accessed node
        else if ( position < _size ) {
            // start from last accessed node and index
            Node_t*  current = _currentNode;
            uint32_t _start  = position > _lastIndex ? _lastIndex : position;
            uint32_t _end    = position > _lastIndex ? position : _lastIndex;
            uint8_t  reverse = position > _lastIndex ? false : true;

            // find node
            while ( _start < _end ) {
                // iterate to next / prev
                if ( !reverse && current && current->next )
                    current = current->next;
                else if ( current && current->prev )
                    current = current->prev;
                _start++;
            }

            // save last node and it's index
            _currentNode = current;
            _lastIndex   = position;

            // return the finded node values
            return _currentNode->value;
        }

        // return empty constructor of type elsewhere
        return Type_t();
    }

    /**
     *	get a value from llist
     *
     * @param position:     the positon of the node that we want
     * @return the value of the node requested if exist
     */
    inline Type_t operator[](uint32_t position) {
        return get(position);
    }

    /**
     * find an element into the list and return an iterator
     *
     * @param value:    the value to be finded
     * @return an iterator positioned on the node with the value requested
     */
    iterator find(Type_t value) {
        // sort need at least 2 elements
        if ( _size >= 2 ) {
            // start from begin
            bool     found   = false;
            uint32_t index   = 0;
            Node_t*  current = _firstNode;

            // find the element
            while ( current != 0 && index < _size ) {
                // compare the value of the node
                if ( current->value == value ) {
                    found = true;
                    break;
                }

                // iterate to next
                current = current->next;
                index++;
            }

            // create an iterator filled with current situation
            if ( found )
                return iterator(this, false, current, index);
        }

        // return and empty iterator
        return iterator();
    }

    /**
     * return the value of the first added node
     *
     * @return the value of the first node if exist
     */
    inline Type_t front() {
        // if exist return first node value
        if ( _firstNode )
            return _firstNode->value;

        // return empty constructor of type elsewhere
        return Type_t();
    }

    /**
     * remove the first node of the list
     */
    inline void popFront() {
        remove(_firstNode);
    }

    /**
     * return the value of the last added node
     *
     * @return the value of the last node if exist
     */
    inline Type_t back() {
        // if exist return the last node value
        if ( _lastNode )
            return _lastNode->value;

        // return empty constructor of type elsewhere
        return Type_t();
    }

    /**
     * remove the last node of the list
     */
    inline void popBack() {
        remove(_lastNode);
    }

    /**
     *	remove the node pointed by the iterator
     *
     * @param it:     an initialized object iterator
     */
    inline void erase(const iterator& it) {
        remove(it._current);
    }

    /**
     * remove node from value
     *
     * @param value:    the value to be removed from llist
     */
    inline void erase(Type_t value) {
        remove(find(value)._current);
    }

    /**
     *	remove node from position
     *
     * @param position:    the node position
     */
    void erase(uint32_t position) {
        // find node
        Node_t* toErase = _firstNode;

        // if position is major than 0 find node, else the node to erase is the first
        if ( position > 0 ) {
            uint32_t i = 0;
            while ( (position < _size || i != position) && (toErase != 0) ) {
                toErase = toErase->next;
                i++;
            }
        }

        // remove node
        remove(toErase);
    }

    /**
     *	delete all nodes
     */
    void clear() {
        for ( Node_t* current = _firstNode; current != 0; ) {
            Node_t* next = current->next;
            delete current;
            current = next;
        }

        _size = 0;
    }

    /**
     * sort the nodes with comparison method provided
     *
     * @param comparison:    function pointer to compare the values into the llist
     */
    void sort(bool (*compare)(const Type_t& first, const Type_t& second)) {
        for ( Node_t* first = _firstNode; first != 0; first = first->next )
            for ( Node_t* second = first->next; second != 0; second = second->next )
                if ( compare(first->value, second->value) )
                    swap(first, second);
    }

    /**
     * @return llist nodes count
     */
    inline uint32_t count() const {
        return _size;
    }

    /**
     *	iterator initializer
     */
    iterator begin() {
        return iterator(this, false);
    }
    iterator rbegin() {
        return iterator(this, true);
    }
    iterator end() {
        return iterator();
    }
    iterator rend() {
        return iterator();
    }

private:
    /**
     * list informations
     */
    uint32_t _size;      // number of nodes into this llist
    Node_t*  _firstNode; // first added node
    Node_t*  _lastNode;  // last added node

    /**
     * cache informations
     */
    Node_t*  _currentNode; // pointer to last requested nodes
    uint32_t _lastIndex;   // index to the last requested index

    /**
     *	remove provided node
     *
     * @param node:    the node to be removed
     */
    void remove(Node_t* node) {
        // first check if it is a valid node
        if ( node ) {
            // get next and previous if exists
            Node_t* next = node->next != 0 ? node->next : 0;
            Node_t* prev = node->prev != 0 ? node->prev : 0;

            // check if node to erase is the first or the last node
            if ( node == _firstNode )
                _firstNode = next;
            else if ( node == _lastNode )
                _lastNode = prev;
            else // else link next and prev
            {
                prev->next = next;
                next->prev = prev;
            }

            delete node; // erase node

            _size--; // decrement size
        }
    }

    /**
     * swap method, invert the two provided node's value
     *
     * @param first:    the first iterator
     * @param second:   the second iterator
     */
    inline void swap(Node_t* first, Node_t* second) {
        Type_t tmp    = first->value;
        first->value  = second->value;
        second->value = tmp;
    }
} A_PACKED;

#endif
