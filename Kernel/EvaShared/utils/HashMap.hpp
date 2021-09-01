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

#ifndef EVA_SHARED_UTILS_HASHMAP
#define EVA_SHARED_UTILS_HASHMAP

#include <utils/hashable.hpp>

/**
 * symple map that use hashing
 */
template<typename KeyType, typename ValueType> class HashMap
{
public:

	/**
	 *	map Node
	 */
	class MapNode_t
	{
	public:
		/**
		 * filled constructor
		 *
		 * @param key:		the key for the entry
		 * @param value:	the value contained
		 */
		MapNode_t(const KeyType &key, const ValueType &value) : key(key), value(value), next(nullptr) {}

		/**
		 * internal data
		 */
		KeyType key;		// the key of the node
		ValueType value;	// the value contained into the node
		MapNode_t *next;	// the next linked node
	};

	/**
	 *	basic map iterator
	 */
	class HashMapIterator
	{
	public:

		/**
		 * empty constructor
		 */
		HashMapIterator() : _map(nullptr), _bucketIndex(0), _current(nullptr) {}

		/**
		 * filled constructor
		 *
		 * @param map:	the map to iterate to
		 */
		HashMapIterator(HashMap *map) : _map(map), _bucketIndex(0), _current(0)
		{
			// parse each bucket
			for (uint32_t i = 0; i < _map->_bucketCount; ++i)
			{
				// the first valid bucket is the current
				if (_map->_buckets[i])
				{
					_bucketIndex = i;
					_current = _map->_buckets[i];
					break;
				}
			}
		}

		/**
		 *	increment prefix operator
		 *
		 * @return the object update
		 */
		HashMapIterator &operator++()
		{
			// if the next isn't valid
			if (!_current->next)
			{
				// invalidate current
				_current = nullptr;

				// find the next valid bucket
				while (++_bucketIndex < _map->_bucketCount)
				{
					if (_map->_buckets[_bucketIndex])
					{
						// next valid set as current
						_current = _map->_buckets[_bucketIndex];
						break;
					}
				}
			}

			// the next is a valid object
			else _current = _current->next;

			return *this;
		}

		/**
		 * @return the current object entry
		 */
		MapNode_t *operator->() { return _current; }

		/**
		 * check operators overload
		 */
		friend bool operator==(const HashMapIterator &lhs, const HashMapIterator &rhs) { return lhs._current == rhs._current; }
		friend bool operator!=(const HashMapIterator &lhs, const HashMapIterator &rhs) { return !(lhs == rhs); }

	private:

		/**
		 * internal properties
		 */
		HashMap *_map;			// the map that is to iterate
		uint32_t _bucketIndex;	// the current hash index
		MapNode_t *_current;	// the current node object
	};

	/**
	 * empty constructor
	 */
	HashMap() : _size(0), _bucketCount(64), _buckets(new MapNode_t*[_bucketCount]()) {}

	/**
	 * copy constructor
	 */
	HashMap(const HashMap &rhs) = delete;

	/**
	 * destructor
	 */
	~HashMap()
	{
		// parse each bucket
		for (uint32_t i = 0; i < _bucketCount; ++i)
		{
			// destroy each node for each bucket
			MapNode_t *current = _buckets[i];
			while (current)
			{
				MapNode_t *next = current->next;
				delete current;
				current = next;
			}
		}

		// frees the memory
		delete[] _buckets;
	}

	/**
	 * copy operator
	 */
	HashMap &operator=(const HashMap &rhs) = delete;

	/**
	 * iterator initializer
	 */
	HashMapIterator begin() { return HashMapIterator(this); }
	HashMapIterator end() { return HashMapIterator(); }

	/**
	 * get the map node with the provided key
	 *
	 * @param key:	the key to search
	 * @return the corresponding node object or nullptr
	 */
	MapNode_t *get(const KeyType &key) const
	{
		// calculate the hash code and get the first entry from buckets
		uint32_t bucketIndex = HashAble::hashcode(key) % _bucketCount;
		MapNode_t *entry = _buckets[bucketIndex];

		// parse all the entries from the first
		while (entry)
		{
			// check the key
			if (entry->key == key) return entry;

			// iterate to next
			entry = entry->next;
		}

		return nullptr;
	}

	/**
	 * add a new couple of values
	 *
	 * @param key:		the for node
	 * @param value:	the value to store in
	 */
	void add(const KeyType &key, const ValueType &value)
	{
		// calculate the hash code and get the first entry from buckets
		uint32_t bucketIndex = HashAble::hashcode(key) % _bucketCount;
		MapNode_t *entry = _buckets[bucketIndex];

		// if is a unvalid entry create to
		if (!entry)
		{
			_buckets[bucketIndex] = new MapNode_t(key, value);
			++_size;
		}

		// need to find the next unvalid node
		else
		{
			while (true)
			{
				// the same key overwrite the value
				if (entry->key == key)
				{
					entry->value = value;
					++_size;
					break;
				}

				// create e ned node
				else if (!entry->next)
				{
					entry->next = new MapNode_t(key, value);
					++_size;
					break;
				}

				// iterate to next
				entry = entry->next;
			}
		}
	}

	/**
	 * erase a node from it's key
	 *
	 * @param key:		the key of the node
	 */
	void erase(const KeyType &key)
	{
		// calculate the hash code
		uint32_t bucketIndex = HashAble::hashcode(key) % _bucketCount;

		// get the first node of the corresponding bucket
		MapNode_t *current = _buckets[bucketIndex];
		MapNode_t *previous = nullptr;

		// iterate
		while (current)
		{
			// check key correspondance
			if (current->key == key)
			{
				// link the nodes
				if (!previous) _buckets[bucketIndex] = current->next;
				else previous->next = current->next;

				// destroy the node and exist
				delete current;
				--_size;
				break;
			}

			// link and iterate to next
			previous = current;
			current = current->next;
		}
	}

	/**
	 * @return the map size
	 */
	inline uint32_t size() const { return _size; }

private:

	/**
	 * internal data
	 */
	uint32_t _size;			// node count
	uint32_t _bucketCount;	// bucket count
	MapNode_t **_buckets;	// pointer to node objects
};

#endif
