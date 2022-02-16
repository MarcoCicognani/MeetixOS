/**
 * @brief
 * This file is part of the MeetiX Operating System.
 * Copyright (c) 2017-2021, Marco Cicognani (marco.cicognani@meetixos.org)
 *
 * @developers
 * Marco Cicognani (marco.cicognani@meetixos.org)
 *
 * @license
 * GNU General Public License version 3
 */

#pragma once

#include <initializer_list>
#include <TC/DenyCopy.hh>
#include <TC/Functional/ErrorOr.hh>
#include <TC/Functional/Option.hh>
#include <TC/IntTypes.hh>

namespace TC {
namespace Collection {
namespace Details {

template<typename Collection, typename T>
class ListIterator {
public:
    explicit ListIterator(Collection& collection);
    ListIterator(Collection& collection, typename Collection::Node* node);
    ListIterator(ListIterator const&) = default;

    ListIterator& operator=(ListIterator const&) = default;

    /**
     * @brief Increment operators
     */
    ListIterator& operator++();
    ListIterator  operator++(int);

    /**
     * @brief ValueReference access operators
     */
    T&       operator*();
    T const& operator*() const;

    /**
     * @brief Pointer access operators
     */
    T*       operator->();
    T const* operator->() const;

    /**
     * @brief Removes the node from the list
     */
    ListIterator erase();

    /**
     * @brief Getters
     */
    [[nodiscard]] bool is_end() const;

    /**
     * @brief Comparison operators
     */
    [[nodiscard]] bool operator==(ListIterator const& rhs) const;
    [[nodiscard]] bool operator!=(ListIterator const& rhs) const;
    [[nodiscard]] bool operator<=>(ListIterator const& rhs) const = default;

private:
    friend Collection;

    void delete_node();

private:
    Collection*                m_collection{ nullptr };
    typename Collection::Node* m_current_node{ nullptr };
};

template<typename T>
struct ListNode {
    explicit ListNode(T&& value);

    T         m_value;
    ListNode* m_next_node{ nullptr };
    ListNode* m_prev_node{ nullptr };
};

} /* namespace Details */

template<typename T>
class List {
    TC_DENY_COPY(List);

public:
    using Iterator      = Details::ListIterator<List, T>;
    using ConstIterator = Details::ListIterator<List const, T const>;
    using Node          = Details::ListNode<T>;

public:
    /**
     * @brief Constructors
     */
    List() = default;
    List(List&& rhs) noexcept;
    List(std::initializer_list<T> initializer_list);

    ~List();

    List& operator=(List&& rhs) noexcept;
    List& operator=(std::initializer_list<T> initializer_list);

    void clear();
    void swap(List& rhs) noexcept;

    /**
     * @brief Creates a new node and appends it to the last of the list
     */
    void          append(T const& value);
    void          append(T&& value);
    ErrorOr<void> try_append(T const& value);
    ErrorOr<void> try_append(T&& value);

    /**
     * @brief Creates a new node and prepends it to the first of the list
     */
    void          prepend(T const& value);
    void          prepend(T&& value);
    ErrorOr<void> try_prepend(T const& value);
    ErrorOr<void> try_prepend(T&& value);

    /**
     * @brief Removes the node referenced by the given iterator
     */
    void erase(Iterator& iterator);

    template<typename Callback>
    void erase_if(Callback callback);

    [[nodiscard]] bool contains(T const& value) const;

    Option<T&>       find(T const& value);
    Option<T const&> find(T const& value) const;

    template<typename Callback>
    Option<T&> find_if(Callback callback);
    template<typename Callback>
    Option<T const&> find_if(Callback callback) const;

    /**
     * @brief for-each support
     */
    Iterator begin();
    Iterator end();

    ConstIterator begin() const;
    ConstIterator end() const;

    /**
     * @brief Getters
     */
    [[nodiscard]] usize count() const;
    [[nodiscard]] bool  is_empty() const;

    [[nodiscard]] Node*       head();
    [[nodiscard]] Node const* head() const;

    [[nodiscard]] Node*       tail();
    [[nodiscard]] Node const* tail() const;

    [[nodiscard]] T&       first();
    [[nodiscard]] T const& first() const;

    [[nodiscard]] T&       last();
    [[nodiscard]] T const& last() const;

private:
    Node* m_head_node{ nullptr };
    Node* m_tail_node{ nullptr };
    usize m_values_count{ 0 };
};

} /* namespace Collection */

using Collection::List;

} /* namespace TC */

#include <../LibTC/Collection/List.hhi>
