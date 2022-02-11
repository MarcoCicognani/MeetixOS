namespace TC::Memory {

template<typename T>
NonNullRef<T>::NonNullRef(NonNullRef::AdoptTag, T& ref)
    : m_shared_ptr{ &ref } {
}

template<typename T>
NonNullRef<T>::NonNullRef(NonNullRef::AdoptTag, T const& ref)
    : m_shared_ptr{ const_cast<T*>(&ref) } {
}

template<typename T>
NonNullRef<T>::NonNullRef(NonNullRef const& rhs)
    : m_shared_ptr{ rhs.m_shared_ptr } {
    if ( m_shared_ptr != nullptr ) [[likely]]
        m_shared_ptr->add_ref();
}

template<typename T>
NonNullRef<T>::NonNullRef(NonNullRef&& rhs) noexcept
    : m_shared_ptr{ exchange(rhs.m_shared_ptr, nullptr) } {
}

template<typename T>
NonNullRef<T>::~NonNullRef() {
    if ( m_shared_ptr != nullptr ) [[likely]] {
        m_shared_ptr->dec_ref();
        m_shared_ptr = nullptr;
    }
}

template<typename T>
NonNullRef<T>& NonNullRef<T>::operator=(T& ref) {
    NonNullRef non_null_ref{ ref };
    swap(non_null_ref);
    return *this;
}

template<typename T>
NonNullRef<T>& NonNullRef<T>::operator=(T const& ref) {
    NonNullRef non_null_ref{ ref };
    swap(non_null_ref);
    return *this;
}

template<typename T>
NonNullRef<T>& NonNullRef<T>::operator=(NonNullRef const& rhs) {
    if ( this == &rhs )
        return *this;

    NonNullRef non_null_ref{ rhs };
    swap(non_null_ref);
    return *this;
}

template<typename T>
NonNullRef<T>& NonNullRef<T>::operator=(NonNullRef&& rhs) noexcept {
    NonNullRef non_null_ref{ move(rhs) };
    swap(non_null_ref);
    return *this;
}

template<typename T>
void NonNullRef<T>::swap(NonNullRef& rhs) noexcept {
    Cxx::swap(m_shared_ptr, rhs.m_shared_ptr);
}

template<typename T>
T* NonNullRef<T>::operator->() {
    return as_ptr();
}

template<typename T>
T const* NonNullRef<T>::operator->() const {
    return as_ptr();
}

template<typename T>
T& NonNullRef<T>::operator*() {
    return as_ref();
}

template<typename T>
T const& NonNullRef<T>::operator*() const {
    return as_ref();
}

template<typename T>
NonNullRef<T>::operator T*() {
    return as_ptr();
}

template<typename T>
NonNullRef<T>::operator T const*() const {
    return as_ptr();
}

template<typename T>
NonNullRef<T>::operator T&() {
    return as_ref();
}

template<typename T>
NonNullRef<T>::operator T const&() const {
    return as_ref();
}

template<typename T>
T* NonNullRef<T>::as_ptr() {
    VERIFY_NOT_NULL(m_shared_ptr);
    return m_shared_ptr;
}

template<typename T>
T const* NonNullRef<T>::as_ptr() const {
    VERIFY_NOT_NULL(m_shared_ptr);
    return m_shared_ptr;
}

template<typename T>
T& NonNullRef<T>::as_ref() {
    VERIFY_NOT_NULL(m_shared_ptr);
    return *m_shared_ptr;
}

template<typename T>
T const& NonNullRef<T>::as_ref() const {
    VERIFY_NOT_NULL(m_shared_ptr);
    return *m_shared_ptr;
}

template<typename T, typename... Args>
inline NonNullRef<T> make_ref(Args&&... args) {
    auto ref_ptr = new (nothrow) T{ forward<Args>(args)... };
    VERIFY_NOT_NULL(ref_ptr);
    return NonNullRef<T>{ NonNullRef<T>::Adopt, *ref_ptr };
}

template<typename T, typename... Args>
inline ErrorOr<NonNullRef<T>> try_make_ref(Args&&... args) {
    auto ref_ptr = new (nothrow) T{ forward<Args>(args)... };
    if ( ref_ptr != nullptr )
        return NonNullRef<T>{ NonNullRef<T>::Adopt, *ref_ptr };
    else
        return ENOMEM;
}

} /* namespace TC::Memory */