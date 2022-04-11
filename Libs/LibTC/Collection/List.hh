/**
 * @brief
 * This file is part of the MeetiX Operating System.
 * Copyright (c) 2017-2022, Marco Cicognani (marco.cicognani@meetixos.org)
 *
 * @developers
 * Marco Cicognani (marco.cicognani@meetixos.org)
 *
 * @license
 * GNU General Public License version 3
 */

#pragma once

#include <initializer_list>
#include <LibTC/Assertions.hh>
#include <LibTC/Cxx.hh>
#include <LibTC/DenyCopy.hh>
#include <LibTC/Functional/ErrorOr.hh>
#include <LibTC/Functional/Must.hh>
#include <LibTC/Functional/Option.hh>
#include <LibTC/IntTypes.hh>

namespace TC {
namespace Collection {
namespace Details {

template<typename Collection, typename T>
class ListIterator {
public:
    explicit ListIterator(Collection& collection)
        : m_collection{ &collection } {
    }
    ListIterator(Collection& collection, typename Collection::Node* node)
        : m_collection{ &collection }
        , m_current_node{ node } {
    }
    ListIterator(ListIterator const&) = default;

    ListIterator& operator=(ListIterator const&) = default;

    /**
     * @brief Increment operators
     */
    ListIterator& operator++() {
        if ( m_current_node != nullptr )
            m_current_node = m_current_node->m_next_node;
        return *this;
    }
    ListIterator operator++(int) {
        ListIterator it{ *this };

        operator++();
        return it;
    }

    /**
     * @brief ValueReference access operators
     */
    T& operator*() {
        VERIFY_NOT_NULL(m_current_node);
        return m_current_node->m_value;
    }
    T const& operator*() const {
        VERIFY_NOT_NULL(m_current_node);
        return m_current_node->m_value;
    }

    /**
     * @brief Pointer access operators
     */
    T* operator->() {
        return &operator*();
    }
    T const* operator->() const {
        return &operator*();
    }

    /**
     * @brief Removes the node from the list
     */
    ListIterator erase() {
        ListIterator it{ *this };
        ++it;

        m_collection->erase(*this);
        return it;
    }

    /**
     * @brief Getters
     */
    bool is_end() const {
        return m_current_node == nullptr;
    }

    /**
     * @brief Comparison operators
     */
    bool operator==(ListIterator const& rhs) const {
        return m_current_node == rhs.m_current_node;
    }
    bool operator!=(ListIterator const& rhs) const {
        return m_current_node != rhs.m_current_node;
    }
    bool operator<=>(ListIterator const& rhs) const = default;

private:
    friend Collection;

    void delete_node() {
        delete m_current_node;
        m_current_node = nullptr;
    }

private:
    Collection*                m_collection{ nullptr };
    typename Collection::Node* m_current_node{ nullptr };
};

template<typename T>
struct ListNode {
    explicit ListNode(T&& value)
        : m_value{ move(value) } {
    }

    T         m_value;
    ListNode* m_next_node{ nullptr };
    ListNode* m_prev_node{ nullptr };
};

} /* namespace Details */

template<typename T>
class List {
public:
    using Iterator      = Details::ListIterator<List, T>;
    using ConstIterator = Details::ListIterator<List const, T const>;
    using Node          = Details::ListNode<T>;

public:
    /**
     * @brief Constructors
     */
    List() = default;
    List(List const& rhs) {
        for ( auto const& element : rhs )
            append(element);
    }
    List(List&& rhs) noexcept
        : m_head_node{ exchange(rhs.m_head_node, nullptr) }
        , m_tail_node{ exchange(rhs.m_tail_node, nullptr) }
        , m_values_count{ exchange(rhs.m_values_count, 0) } {
    }
    List(std::initializer_list<T> initializer_list) {
        for ( auto const& value : initializer_list )
            append(value);
    }

    ~List() {
        clear();
    }

    List& operator=(List const& rhs) noexcept {
        if ( this == &rhs )
            return *this;

        List list{ rhs };
        swap(list);
        return *this;
    }
    List& operator=(List&& rhs) noexcept {
        List list{ move(rhs) };
        swap(list);
        return *this;
    }
    List& operator=(std::initializer_list<T> initializer_list) {
        List list{ initializer_list };
        swap(list);
        return *this;
    }

    /**
     * @brief Clears the content of this List and destroys the nodes
     */
    void clear() {
        for ( auto* node = m_head_node; node != nullptr; ) {
            auto* next_node = node->m_next_node;
            delete node;
            node = next_node;
        }
        m_head_node    = nullptr;
        m_tail_node    = nullptr;
        m_values_count = 0;
    }

    /**
     * @brief Swaps in O(1) the content of this List with another
     */
    void swap(List& rhs) noexcept {
        Cxx::swap(m_head_node, rhs.m_head_node);
        Cxx::swap(m_tail_node, rhs.m_tail_node);
        Cxx::swap(m_values_count, rhs.m_values_count);
    }

    /**
     * @brief Creates a new node and appends it to the last of the list
     */
    void append(T const& value) {
        MUST(try_append(T{ value }));
    }
    void append(T&& value) {
        MUST(try_append(move(value)));
    }

    ErrorOr<void> try_append(T const& value) {
        return try_append(T{ value });
    }
    ErrorOr<void> try_append(T&& value) {
        auto new_node = new (nothrow) Node{ move(value) };
        if ( new_node == nullptr )
            return Error{ ENOMEM };

        if ( m_tail_node == nullptr )
            m_head_node = new_node;
        else {
            m_tail_node->m_next_node = new_node;
            new_node->m_prev_node    = m_tail_node;
        }

        m_tail_node = new_node;
        ++m_values_count;
        return {};
    }

    /**
     * @brief Creates a new node and prepends it to the first of the list
     */
    void prepend(T const& value) {
        MUST(try_prepend(T{ value }));
    }
    void prepend(T&& value) {
        MUST(try_prepend(move(value)));
    }

    ErrorOr<void> try_prepend(T const& value) {
        return try_prepend(T{ value });
    }
    ErrorOr<void> try_prepend(T&& value) {
        auto new_node = new (nothrow) Node{ move(value) };
        if ( new_node == nullptr )
            return Error{ ENOMEM };

        if ( m_head_node == nullptr )
            m_tail_node = new_node;
        else {
            m_head_node->m_prev_node = new_node;
            new_node->m_next_node    = m_head_node;
        }

        m_head_node = new_node;
        ++m_values_count;
        return {};
    }

    /**
     * @brief Removes the node referenced by the given iterator
     */
    void erase(Iterator& iterator) {
        VERIFY_FALSE(iterator.is_end());

        auto* node_to_erase = iterator.m_current_node;
        if ( m_head_node == node_to_erase )
            m_head_node = node_to_erase->m_next_node;
        if ( m_tail_node == node_to_erase )
            m_tail_node = node_to_erase->m_prev_node;
        if ( node_to_erase->m_prev_node != nullptr )
            node_to_erase->m_prev_node->m_next_node = node_to_erase->m_next_node;
        if ( node_to_erase->m_next_node != nullptr )
            node_to_erase->m_next_node->m_prev_node = node_to_erase->m_prev_node;

        iterator.delete_node();
        --m_values_count;
    }

    /**
     * @brief Erases the elements which makes call_back return true
     */
    template<typename Callback>
    void erase_if(Callback call_back) {
        auto it = begin();
        while ( it != end() ) {
            if ( call_back(*it) )
                it = it.erase();
            else
                ++it;
        }
    }

    /**
     * @brief Returns whether the given element is contained into this List
     */
    bool contains(T const& value) const {
        return find(value).is_present();
    }

    /**
     * @brief Returns a reference to the element into this List if exists
     */
    Option<T&> find(T const& value) {
        return find_if([&value](auto const& v) { return value == v; });
    }
    Option<T const&> find(T const& value) const {
        return find_if([&value](auto const& v) { return value == v; });
    }

    /**
     * @brief Returns a reference to the element if the callback returns true
     */
    template<typename Callback>
    Option<T&> find_if(Callback callback) {
        for ( auto& node : *this ) {
            if ( callback(node) )
                return node;
        }
        return {};
    }
    template<typename Callback>
    Option<T const&> find_if(Callback callback) const {
        for ( auto const& node : *this ) {
            if ( callback(node) )
                return node;
        }
        return {};
    }

    /**
     * @brief for-each support
     */
    Iterator begin() {
        return Iterator{ *this, m_head_node };
    }
    Iterator end() {
        return Iterator{ *this };
    }

    ConstIterator begin() const {
        return ConstIterator{ *this, m_head_node };
    }
    ConstIterator end() const {
        return ConstIterator{ *this };
    }

    /**
     * @brief Getters
     */
    [[nodiscard]] usize count() const {
        return m_values_count;
    }
    [[nodiscard]] bool is_empty() const {
        return count() == 0;
    }

    Node* head() {
        return m_head_node;
    }
    Node const* head() const {
        return m_head_node;
    }

    Node* tail() {
        return m_tail_node;
    }
    Node const* tail() const {
        return m_tail_node;
    }

    T& first() {
        VERIFY_NOT_NULL(m_head_node);
        return m_head_node->m_value;
    }
    T const& first() const {
        VERIFY_NOT_NULL(m_head_node);
        return m_head_node->m_value;
    }

    T& last() {
        VERIFY_NOT_NULL(m_tail_node);
        return m_tail_node->m_value;
    }
    T const& last() const {
        VERIFY_NOT_NULL(m_tail_node);
        return m_tail_node->m_value;
    }

private:
    Node* m_head_node{ nullptr };
    Node* m_tail_node{ nullptr };
    usize m_values_count{ 0 };
};

} /* namespace Collection */

using Collection::List;

} /* namespace TC */
