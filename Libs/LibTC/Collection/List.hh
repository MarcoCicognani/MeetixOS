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

#include <LibTC/Assertions.hh>
#include <LibTC/Collection/ReverseIteratorSupport.hh>
#include <LibTC/Cxx.hh>
#include <LibTC/DenyCopy.hh>
#include <LibTC/Forward.hh>
#include <LibTC/Functional/ErrorOr.hh>
#include <LibTC/Functional/Must.hh>
#include <LibTC/Functional/Option.hh>
#include <LibTC/Functional/Try.hh>
#include <LibTC/IntTypes.hh>

namespace TC {
namespace Collection {
namespace Details {

template<typename Collection, typename T, bool IsReverse>
class ListIterator {
public:
    /**
     * @brief Constructors
     */
    explicit ListIterator(Collection& collection)
        : m_collection{ &collection } {
    }
    ListIterator(Collection& collection, typename Collection::Node* node)
        : m_collection{ &collection }
        , m_current_node{ node } {
    }
    ListIterator(ListIterator const&) = default;

    auto operator=(ListIterator const&) -> ListIterator& = default;

    /**
     * @brief Increment operators
     */
    auto operator++() -> ListIterator& {
        if ( m_current_node != nullptr ) {
            if constexpr ( IsReverse ) {
                m_current_node = m_current_node->m_prev_node;
            } else {
                m_current_node = m_current_node->m_next_node;
            }
        }
        return *this;
    }
    auto operator++(int) -> ListIterator {
        ListIterator it{ *this };

        operator++();
        return it;
    }

    /**
     * @brief ValueReference access operators
     */
    auto operator*() -> T& {
        VERIFY_NOT_NULL(m_current_node);
        return m_current_node->m_value;
    }
    auto operator*() const -> T const& {
        VERIFY_NOT_NULL(m_current_node);
        return m_current_node->m_value;
    }

    /**
     * @brief Pointer access operators
     */
    auto operator->() -> T* {
        return &operator*();
    }
    auto operator->() const -> T const* {
        return &operator*();
    }

    /**
     * @brief Removes the node from the list
     */
    auto erase() -> ListIterator {
        ListIterator it{ *this };
        ++it;

        m_collection->erase(*this);
        return it;
    }

    /**
     * @brief Getters
     */
    [[nodiscard]] auto is_end() const -> bool {
        return m_current_node == nullptr;
    }

    /**
     * @brief Comparison operators
     */
    auto operator==(ListIterator const& rhs) const -> bool {
        return m_current_node == rhs.m_current_node;
    }
    auto operator!=(ListIterator const& rhs) const -> bool {
        return m_current_node != rhs.m_current_node;
    }
    auto operator<=>(ListIterator const& rhs) const -> bool = default;

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
public:
    T         m_value;
    ListNode* m_next_node{ nullptr };
    ListNode* m_prev_node{ nullptr };
};

} /* namespace Details */

template<typename T>
class List {
    TC_DENY_COPY(List);

public:
    using Iterator                    = Details::ListIterator<List, T, false>;
    using ConstIterator               = Details::ListIterator<List const, T const, false>;
    using ReverseIterator             = Details::ListIterator<List, T, true>;
    using ConstReverseIterator        = Details::ListIterator<List const, T const, true>;
    using ReverseIteratorWrapper      = ReverseIteratorSupport::Wrapper<List<T>>;
    using ConstReverseIteratorWrapper = ReverseIteratorSupport::Wrapper<List<T> const>;

    using Node = Details::ListNode<T>;

public:
    /**
     * @brief Non-error safe Factory functions
     */
    [[nodiscard]] static constexpr auto construct_empty() -> List<T> {
        return List<T>{};
    }
    [[nodiscard]] static auto construct_from_other(List<T> const& rhs) -> List<T> {
        return MUST(try_construct_from_other(rhs));
    }
    [[nodiscard]] static auto construct_from_list(Cxx::initializer_list<T> initializer_list) -> List<T> {
        return MUST(try_construct_from_list(initializer_list));
    }

    /**
     * @brief Error safe Factory functions
     */
    [[nodiscard]] static auto try_construct_from_other(List<T> const& rhs) -> ErrorOr<List<T>> {
        auto list = construct_empty();
        for ( auto const& element : rhs ) {
            if constexpr ( TryCloneable<T, ErrorOr<T>> ) {
                TRY(list.try_append(TRY(element.try_clone())));
            } else if constexpr ( Cloneable<T> ) {
                TRY(list.try_append(element.clone()));
            } else if constexpr ( CopyConstructible<T> ) {
                TRY(list.try_append(element));
            }
        }

        return list;
    }
    [[nodiscard]] static auto try_construct_from_list(Cxx::initializer_list<T> initializer_list) -> ErrorOr<List<T>> {
        auto list = construct_empty();
        for ( auto const& element : initializer_list ) /* even with auto initializer_list exposes only T const& */
            TRY(list.try_append(Cxx::move(const_cast<T&>(element))));

        return list;
    }

    /**
     * @brief Move constructor and move assignment
     */
    List(List<T>&& rhs) noexcept
        : m_head_node{ Cxx::exchange(rhs.m_head_node, nullptr) }
        , m_tail_node{ Cxx::exchange(rhs.m_tail_node, nullptr) }
        , m_values_count{ Cxx::exchange(rhs.m_values_count, 0) } {
    }
    auto operator=(List<T>&& rhs) noexcept -> List<T>& {
        List list{ Cxx::move(rhs) };
        swap(list);
        return *this;
    }

    ~List() {
        clear();
    }

    /**
     * @brief Deep cloning
     */
    [[nodiscard]] auto clone() const -> List<T> {
        return MUST(try_clone());
    }
    [[nodiscard]] auto try_clone() const -> ErrorOr<List<T>> {
        return List<T>::try_construct_from_other(*this);
    }

    /**
     * @brief Clears the content of this List and destroys the nodes
     */
    auto clear() {
        for ( auto node = m_head_node; node != nullptr; ) {
            auto const next_node = node->m_next_node;
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
    auto swap(List<T>& rhs) noexcept {
        Cxx::swap(m_head_node, rhs.m_head_node);
        Cxx::swap(m_tail_node, rhs.m_tail_node);
        Cxx::swap(m_values_count, rhs.m_values_count);
    }

    /**
     * @brief Creates a new node and appends it to the last of the list
     */
    auto append(T value) {
        MUST(try_append(Cxx::move(value)));
    }
    auto try_append(T value) -> ErrorOr<void> {
        auto const new_node = new (nothrow) Node{ Cxx::move(value) };
        if ( new_node == nullptr )
            return Error{ ENOMEM };

        if ( m_tail_node != nullptr ) {
            m_tail_node->m_next_node = new_node;
            new_node->m_prev_node    = m_tail_node;
        } else
            m_head_node = new_node;

        m_tail_node = new_node;
        ++m_values_count;
        return {};
    }

    /**
     * @brief Creates a new node and prepends it to the first of the list
     */
    auto prepend(T value) {
        MUST(try_prepend(Cxx::move(value)));
    }
    auto try_prepend(T value) -> ErrorOr<void> {
        auto const new_node = new (nothrow) Node{ Cxx::move(value) };
        if ( new_node == nullptr )
            return Error{ ENOMEM };

        if ( m_head_node != nullptr ) {
            m_head_node->m_prev_node = new_node;
            new_node->m_next_node    = m_head_node;
        } else
            m_tail_node = new_node;

        m_head_node = new_node;
        ++m_values_count;
        return {};
    }

    /**
     * @brief Removes the node referenced by the given iterator
     */
    template<typename TIterator>
    auto erase(TIterator& iterator) {
        VERIFY_FALSE(iterator.is_end());

        auto const node_to_erase = iterator.m_current_node;
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
    auto erase_if(Predicate<T const&> auto predicate) {
        auto it = begin();
        while ( it != end() ) {
            if ( predicate(*it) )
                it = it.erase();
            else
                ++it;
        }
    }

    /**
     * @brief Returns a reference to the element into this List if exists
     */
    auto find(T const& value) -> Option<T&> {
        return find_if([&value](auto const& v) { return value == v; });
    }
    auto find(T const& value) const -> Option<T const&> {
        return find_if([&value](auto const& v) { return value == v; });
    }

    /**
     * @brief Returns a reference to the element if the callback returns true
     */
    auto find_if(Predicate<T&> auto predicate) -> Option<T&> {
        for ( auto& node : *this ) {
            if ( predicate(node) )
                return node;
        }
        return {};
    }
    auto find_if(Predicate<T const&> auto predicate) const -> Option<T const&> {
        for ( auto const& node : *this ) {
            if ( predicate(node) )
                return node;
        }
        return {};
    }

    /**
     * @brief Returns whether the given element is contained into this List
     */
    auto contains(T const& value) const -> bool {
        return find(value).is_present();
    }

    /**
     * @brief for-each support
     */
    auto begin() -> Iterator {
        return Iterator{ *this, m_head_node };
    }
    auto end() -> Iterator {
        return Iterator{ *this };
    }

    auto begin() const -> ConstIterator {
        return ConstIterator{ *this, m_head_node };
    }
    auto end() const -> ConstIterator {
        return ConstIterator{ *this };
    }

    /**
     * @brief reverse for-each support
     */
    auto rbegin() -> ReverseIterator {
        return ReverseIterator{ *this, m_tail_node };
    }
    auto rend() -> ReverseIterator {
        return ReverseIterator{ *this };
    }

    auto rbegin() const -> ConstReverseIterator {
        return ConstReverseIterator{ *this, m_tail_node };
    }
    auto rend() const -> ConstReverseIterator {
        return ConstReverseIterator{ *this };
    }

    auto reverse_iter() -> ReverseIteratorWrapper {
        return ReverseIteratorSupport::in_reverse(*this);
    }
    auto reverse_iter() const -> ConstReverseIteratorWrapper {
        return ReverseIteratorSupport::in_reverse(*this);
    }

    /**
     * @brief Getters
     */
    [[nodiscard]] auto count() const -> usize {
        return m_values_count;
    }
    [[nodiscard]] auto is_empty() const -> bool {
        return count() == 0;
    }

    auto head() -> Node* {
        return m_head_node;
    }
    auto head() const -> Node const* {
        return m_head_node;
    }

    auto tail() -> Node* {
        return m_tail_node;
    }
    auto tail() const -> Node const* {
        return m_tail_node;
    }

    auto first() -> T& {
        VERIFY_NOT_NULL(m_head_node);
        return m_head_node->m_value;
    }
    auto first() const -> T const& {
        VERIFY_NOT_NULL(m_head_node);
        return m_head_node->m_value;
    }

    auto last() -> T& {
        VERIFY_NOT_NULL(m_tail_node);
        return m_tail_node->m_value;
    }
    auto last() const -> T const& {
        VERIFY_NOT_NULL(m_tail_node);
        return m_tail_node->m_value;
    }

private:
    explicit constexpr List() noexcept = default;

private:
    Node* m_head_node{ nullptr };
    Node* m_tail_node{ nullptr };
    usize m_values_count{ 0 };
};

} /* namespace Collection */

using Collection::List;

} /* namespace TC */
