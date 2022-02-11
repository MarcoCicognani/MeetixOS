namespace TC::Memory::Details {

usize PlainRefCountTrait::fetch_add(usize volatile* value, usize addition) {
    auto old_value = load(value);

    *value = old_value + addition;
    return old_value;
}

usize PlainRefCountTrait::fetch_sub(usize volatile* value, usize subtraction) {
    auto old_value = load(value);

    *value = old_value - subtraction;
    return old_value;
}

usize PlainRefCountTrait::load(usize volatile* value) {
    return *value;
}

usize AtomicRefCountTrait::fetch_add(usize volatile* value, usize addition) {
    return __atomic_fetch_add(value, addition, __ATOMIC_SEQ_CST);
}

usize AtomicRefCountTrait::fetch_sub(usize volatile* value, usize subtraction) {
    return __atomic_fetch_sub(value, subtraction, __ATOMIC_SEQ_CST);
}

usize AtomicRefCountTrait::load(usize volatile* value) {
    return __atomic_load_n(value, __ATOMIC_SEQ_CST);
}

template<typename RefCountTrait>
Shareable<RefCountTrait>::~Shareable() {
    VERIFY_EQUAL(RefCountTrait::load(&m_ref_count), 0);
}

template<typename RefCountTrait>
void Shareable<RefCountTrait>::add_ref() const {
    auto old_ref_count = RefCountTrait::fetch_add(&m_ref_count, 1);
    VERIFY_GREATER(old_ref_count, 0);
}

template<typename RefCountTrait>
bool Shareable<RefCountTrait>::dec_ref() const {
    auto old_ref_count = RefCountTrait::fetch_sub(&m_ref_count, 1);
    VERIFY_GREATER_EQUAL(old_ref_count, 1);

    if ( old_ref_count - 1 == 0 ) {
        auto* self = const_cast<Shareable*>(this);
        self->on_no_ref_count();
        delete this;
        return true;
    }
    return false;
}

template<typename RefCountTrait>
usize Shareable<RefCountTrait>::ref_count() const {
    return RefCountTrait::load(&m_ref_count);
}

template<typename RefCountTrait>
void Shareable<RefCountTrait>::on_no_ref_count() {
}

} /* namespace TC::Memory::Details */