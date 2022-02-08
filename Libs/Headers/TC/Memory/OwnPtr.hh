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

#include <TC/Cxx/Exchange.hh>
#include <TC/Cxx/Forward.hh>
#include <TC/Cxx/New.hh>
#include <TC/Cxx/NullPtr.hh>
#include <TC/DenyCopy.hh>
#include <TC/Functional/ErrorOr.hh>

namespace TC {
namespace Memory {

template<typename T>
class OwnPtr {
    TC_DENY_COPY(OwnPtr);

public:
    /**
     * @brief Constructors
     */
    OwnPtr() = default;
    OwnPtr(T* data_pointer);
    OwnPtr(Cxx::nullptr_t);
    OwnPtr(OwnPtr&& rhs) noexcept;
    ~OwnPtr();

    OwnPtr& operator=(T* data_pointer);
    OwnPtr& operator=(Cxx::nullptr_t);
    OwnPtr& operator=(OwnPtr&& rhs) noexcept;

    /**
     * @brief Reset the OwnPtr to a nullptr value
     */
    void clear();

    /**
     * @brief Access operators
     */
    T*       operator->();
    T const* operator->() const;

    T&       operator*();
    T const& operator*() const;

    /**
     * @brief Conversion operators
     */
    explicit operator T*();
    explicit operator T const*() const;

    /**
     * @brief Reset this OwnPtr and returns the stored pointer
     */
    [[nodiscard]] T* leak();

    /**
     * @brief Getters
     */
    [[nodiscard]] T*       ptr();
    [[nodiscard]] T const* ptr() const;

    [[nodiscard]] bool is_null() const;

private:
    T* m_data_pointer{ nullptr };
};

template<typename T, typename... Args>
inline OwnPtr<T> make_own_ptr(Args&&... args) {
    OwnPtr own_ptr{ new (Cxx::nothrow) T{ Cxx::forward<Args>(args)... } };
    VERIFY_FALSE(own_ptr.is_null());
    return own_ptr;
}

template<typename T, typename... Args>
inline Functional::ErrorOr<OwnPtr<T>> try_make_own_ptr(Args&&... args) {
    auto ptr = new (Cxx::nothrow) T{ Cxx::forward<Args>(args)... };
    if ( ptr != nullptr )
        return OwnPtr{ ptr };
    else
        return ENOMEM;
}

template<typename T>
OwnPtr<T>::OwnPtr(T* data_pointer)
    : m_data_pointer{ data_pointer } {
}

template<typename T>
OwnPtr<T>::OwnPtr(Cxx::nullptr_t) {
}

template<typename T>
OwnPtr<T>::OwnPtr(OwnPtr&& rhs) noexcept
    : m_data_pointer{ Cxx::exchange(rhs.m_data_pointer, nullptr) } {
}

template<typename T>
OwnPtr<T>::~OwnPtr() {
    clear();
}

template<typename T>
OwnPtr<T>& OwnPtr<T>::operator=(T* data_pointer) {
    clear();
    m_data_pointer = data_pointer;
    return *this;
}

template<typename T>
OwnPtr<T>& OwnPtr<T>::operator=(Cxx::nullptr_t) {
    clear();
    return *this;
}

template<typename T>
OwnPtr<T>& OwnPtr<T>::operator=(OwnPtr&& rhs) noexcept {
    if ( m_data_pointer == rhs.m_data_pointer )
        return *this;

    clear();
    m_data_pointer = Cxx::exchange(rhs.m_data_pointer, nullptr);
    return *this;
}

template<typename T>
void OwnPtr<T>::clear() {
    delete m_data_pointer;
    m_data_pointer = nullptr;
}

template<typename T>
T* OwnPtr<T>::operator->() {
    VERIFY_NOT_NULL(m_data_pointer);
    return m_data_pointer;
}

template<typename T>
T const* OwnPtr<T>::operator->() const {
    VERIFY_NOT_NULL(m_data_pointer);
    return m_data_pointer;
}

template<typename T>
T& OwnPtr<T>::operator*() {
    VERIFY_NOT_NULL(m_data_pointer);
    return *m_data_pointer;
}

template<typename T>
T const& OwnPtr<T>::operator*() const {
    VERIFY_NOT_NULL(m_data_pointer);
    return *m_data_pointer;
}

template<typename T>
OwnPtr<T>::operator T*() {
    return m_data_pointer;
}

template<typename T>
OwnPtr<T>::operator T const*() const {
    return m_data_pointer;
}

template<typename T>
T* OwnPtr<T>::leak() {
    auto* data_pointer = m_data_pointer;
    m_data_pointer     = nullptr;
    return data_pointer;
}

template<typename T>
T* OwnPtr<T>::ptr() {
    return m_data_pointer;
}

template<typename T>
T const* OwnPtr<T>::ptr() const {
    return m_data_pointer;
}

template<typename T>
bool OwnPtr<T>::is_null() const {
    return m_data_pointer == nullptr;
}

} /* namespace Memory */

using Memory::make_own_ptr;
using Memory::OwnPtr;
using Memory::try_make_own_ptr;

} /* namespace TC */