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

#include <CCLang/Forward.hh>

#include <CCLang/Core/Assertions.hh>
#include <CCLang/Core/ErrorOr.hh>
#include <CCLang/Lang/Cxx.hh>
#include <CCLang/Lang/DenyCopy.hh>
#include <CCLang/Lang/IntTypes.hh>
#include <CCLang/Lang/Must.hh>
#include <CCLang/Lang/Option.hh>
#include <CCLang/Lang/ReverseIteratorSupport.hh>
#include <CCLang/Lang/Try.hh>

namespace Details {

template<typename TList, typename T, bool IsReverse>
class ListIterator final {
public:
    /**
     * @brief Error safe factory functions
     */
    static auto empty() -> ListIterator<TList, T, IsReverse> {
        return ListIterator<TList, T, IsReverse>();
    }
    static auto from_begin(TList* list) -> ListIterator<TList, T, IsReverse> {
        return ListIterator<TList, T, IsReverse>(list, list->m_head_node);
    }
    static auto from_rbegin(TList* list) -> ListIterator<TList, T, IsReverse> {
        return ListIterator<TList, T, IsReverse>(list, list->m_tail_node);
    }

    ListIterator(ListIterator const&)                    = default;
    auto operator=(ListIterator const&) -> ListIterator& = default;

    /**
     * @brief Increment operators
     */
    auto operator++() -> ListIterator& {
        if ( m_current_node != nullptr ) {
            if ( IsReverse ) {
                m_current_node = m_current_node->m_prev_node;
            } else {
                m_current_node = m_current_node->m_next_node;
            }
        }
        return *this;
    }
    auto operator++(int) -> ListIterator {
        auto it = *this;

        operator++();
        return it;
    }

    /**
     * @brief ValueReference access operators
     */
    auto operator*() -> T& {
        verify_not_null$(m_current_node);
        return m_current_node->m_value;
    }
    auto operator*() const -> T const& {
        verify_not_null$(m_current_node);
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
        auto it = *this;
        ++it;

        m_list->erase(*this);
        return it;
    }

    /**
     * @brief Getters
     */
    auto is_end() const -> bool {
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
    friend TList;

    explicit ListIterator() = default;
    explicit ListIterator(TList* list, typename TList::Node* current_node)
        : m_list(list)
        , m_current_node(current_node) {
    }

    void delete_node() {
        delete m_current_node;
        m_current_node = nullptr;
    }

private:
    using TNode = typename TList::Node;

    TList* m_list         = nullptr;
    TNode* m_current_node = nullptr;
};

template<typename T>
struct ListNode final {
public:
    T         m_value;
    ListNode* m_next_node = nullptr;
    ListNode* m_prev_node = nullptr;
};

} /* namespace Details */

template<typename T>
class List final : public DenyCopy {
    template<typename TCollection, typename TT, bool IsReverse>
    friend class Details::ListIterator;

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
     * @brief Non-Error safe factory functions
     */
    static auto empty() -> List<T> {
        return List<T>();
    }
    static auto from_other(List<T> const& rhs) -> List<T> {
        return must$(try_from_other(rhs));
    }
    static auto from_list(Cxx::InitializerList<T> initializer_list) -> List<T> {
        return must$(try_from_list(initializer_list));
    }

    /**
     * @brief Error safe Factory functions
     */
    static auto try_from_other(List<T> const& rhs) -> ErrorOr<List<T>> {
        auto list = List<T>::empty();
        for ( auto const& e : rhs ) {
            if ( TryCloneable<T, ErrorOr<T>> ) {
                try$(list.try_append(try$(e.try_clone())));
            } else if ( Cloneable<T> ) {
                try$(list.try_append(e.clone()));
            } else if ( CopyConstructible<T> ) {
                try$(list.try_append(e));
            }
        }

        return list;
    }
    static auto try_from_list(Cxx::InitializerList<T> initializer_list) -> ErrorOr<List<T>> {
        auto list = List<T>::empty();
        for ( auto const& e : initializer_list ) { /* even with <auto> the <InitializerList> exposes only <T const&> */
            try$(list.try_append(Cxx::move(const_cast<T&>(e))));
        }

        return list;
    }

    /**
     * @brief Move constructor and move assignment
     */
    List(List<T>&& rhs)
        : m_head_node(Cxx::exchange(rhs.m_head_node, nullptr))
        , m_tail_node(Cxx::exchange(rhs.m_tail_node, nullptr))
        , m_values_count(Cxx::exchange(rhs.m_values_count, 0)) {
    }
    auto operator=(List<T>&& rhs) -> List<T>& {
        List<T> list = Cxx::move(rhs);
        swap(list);
        return *this;
    }

    ~List() {
        clear();
    }

    /**
     * @brief Deep cloning
     */
    auto clone() const -> List<T> {
        return must$(try_clone());
    }
    auto try_clone() const -> ErrorOr<List<T>> {
        return List<T>::try_from_other(*this);
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
    auto swap(List<T>& rhs) {
        Cxx::swap(m_head_node, rhs.m_head_node);
        Cxx::swap(m_tail_node, rhs.m_tail_node);
        Cxx::swap(m_values_count, rhs.m_values_count);
    }

    /**
     * @brief Creates a new node and appends it to the last of the list
     */
    auto append(T value) {
        must$(try_append(Cxx::move(value)));
    }
    auto try_append(T value) -> ErrorOr<void> {
        auto const new_node = new (nothrow) Node(Cxx::move(value));
        if ( new_node == nullptr ) [[unlikely]] {
            return Error::from_code(ErrorCode::NoMemory);
        }

        if ( m_tail_node != nullptr ) {
            m_tail_node->m_next_node = new_node;
            new_node->m_prev_node    = m_tail_node;
        } else {
            m_head_node = new_node;
        }

        m_tail_node = new_node;
        ++m_values_count;
        return {};
    }

    /**
     * @brief Creates a new node and prepends it to the first of the list
     */
    auto prepend(T value) {
        must$(try_prepend(Cxx::move(value)));
    }
    auto try_prepend(T value) -> ErrorOr<void> {
        auto const new_node = new (nothrow) Node(Cxx::move(value));
        if ( new_node == nullptr ) [[unlikely]] {
            return Error::from_code(ErrorCode::NoMemory);
        }

        if ( m_head_node != nullptr ) {
            m_head_node->m_prev_node = new_node;
            new_node->m_next_node    = m_head_node;
        } else {
            m_tail_node = new_node;
        }

        m_head_node = new_node;
        ++m_values_count;
        return {};
    }

    /**
     * @brief Erases the elements which makes call_back return true
     */
    auto erase_if(Predicate<T const&> auto predicate) {
        auto it = begin();
        while ( it != end() ) {
            if ( predicate(*it) ) {
                it = it.erase();
            } else {
                ++it;
            }
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
            if ( predicate(node) ) {
                return node;
            }
        }
        return {};
    }
    auto find_if(Predicate<T const&> auto predicate) const -> Option<T const&> {
        for ( auto const& node : *this ) {
            if ( predicate(node) ) {
                return node;
            }
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
        return Iterator::from_begin(this);
    }
    auto end() -> Iterator {
        return Iterator::empty();
    }

    auto begin() const -> ConstIterator {
        return ConstIterator::from_begin(this);
    }
    auto end() const -> ConstIterator {
        return ConstIterator::empty();
    }

    /**
     * @brief reverse for-each support
     */
    auto rbegin() -> ReverseIterator {
        return ReverseIterator::from_rbegin(this);
    }
    auto rend() -> ReverseIterator {
        return ReverseIterator::empty();
    }

    auto rbegin() const -> ConstReverseIterator {
        return ConstReverseIterator::from_rbegin(this);
    }
    auto rend() const -> ConstReverseIterator {
        return ConstReverseIterator::empty();
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
    auto count() const -> usize {
        return m_values_count;
    }
    auto is_empty() const -> bool {
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
        verify_not_null$(m_head_node);
        return m_head_node->m_value;
    }
    auto first() const -> T const& {
        verify_not_null$(m_head_node);
        return m_head_node->m_value;
    }

    auto last() -> T& {
        verify_not_null$(m_tail_node);
        return m_tail_node->m_value;
    }
    auto last() const -> T const& {
        verify_not_null$(m_tail_node);
        return m_tail_node->m_value;
    }

private:
    explicit List() = default;

    template<typename TIterator>
    auto erase(TIterator& iterator) {
        verify_false$(iterator.is_end());

        auto const node_to_erase = iterator.m_current_node;
        if ( m_head_node == node_to_erase ) {
            m_head_node = node_to_erase->m_next_node;
        }
        if ( m_tail_node == node_to_erase ) {
            m_tail_node = node_to_erase->m_prev_node;
        }
        if ( node_to_erase->m_prev_node != nullptr ) {
            node_to_erase->m_prev_node->m_next_node = node_to_erase->m_next_node;
        }
        if ( node_to_erase->m_next_node != nullptr ) {
            node_to_erase->m_next_node->m_prev_node = node_to_erase->m_prev_node;
        }

        iterator.delete_node();
        --m_values_count;
    }

private:
    Node* m_head_node    = nullptr;
    Node* m_tail_node    = nullptr;
    usize m_values_count = 0;
};

namespace Cxx {

template<typename T>
auto swap(List<T>& lhs, List<T>& rhs) -> void {
    lhs.swap(rhs);
}

} /* namespace Cxx */
