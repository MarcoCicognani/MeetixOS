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

#include <TC/Cxx.hh>
#include <TC/DenyCopy.hh>
#include <TC/DenyMove.hh>
#include <TC/IntTypes.hh>
#include <TC/Memory/NonNullRef.hh>
#include <TC/Memory/Shareable.hh>
#include <UnitTest/Assertions.hh>
#include <UnitTest/Case.hh>

using namespace TC;

namespace Object {

class Shared : public Shareable {
    TC_DENY_COPY(Shared);
    TC_DENY_MOVE(Shared);

public:
    Shared() = default;
    explicit Shared(usize value)
        : m_value{ value } {
    }

    ~Shared() override {
        s_destructor_was_called = true;
    }

    void inc_value(usize inc) {
        m_value += inc;
    }
    [[nodiscard]] usize value() const {
        return m_value;
    }

    static void reset_no_ref_count_called_count() {
        s_no_ref_count_called_count = 0;
    }
    static usize no_ref_count_called_count() {
        return s_no_ref_count_called_count;
    }

    static void reset_destructor_was_called() {
        s_destructor_was_called = false;
    }
    static bool destructor_was_called() {
        return s_destructor_was_called;
    }

protected:
    void on_no_ref_count() override {
        ++s_no_ref_count_called_count;
    }

private:
    usize m_value{ 0 };

    static usize s_no_ref_count_called_count;
    static bool  s_destructor_was_called;
};

usize Shared::s_no_ref_count_called_count = 0;
bool  Shared::s_destructor_was_called     = false;

} /* namespace Object */

TEST_CASE(make_ref) {
    auto object_ref = make_ref<Object::Shared>(512);
    VERIFY_EQUAL(object_ref->value(), 512);
    VERIFY_EQUAL(object_ref->ref_count(), 1);

    object_ref.as_ref().inc_value(512);
    VERIFY_EQUAL(object_ref->value(), 1024);
}

TEST_CASE(try_make_ref) {
    auto error_or_object_ref = try_make_ref<Object::Shared>(4096);
    VERIFY(error_or_object_ref.is_value());
    VERIFY_EQUAL(error_or_object_ref.value()->value(), 4096);
    VERIFY_EQUAL(error_or_object_ref.value()->ref_count(), 1);

    auto object_ref = error_or_object_ref.unwrap_value();
    VERIFY_EQUAL(object_ref->value(), 4096);
    VERIFY_EQUAL(object_ref->ref_count(), 1);
}

TEST_CASE(ref_count_copy) {
    Object::Shared::reset_no_ref_count_called_count();
    Object::Shared::reset_destructor_was_called();

    {
        auto object_ref_512_1 = make_ref<Object::Shared>(512);
        VERIFY_EQUAL(object_ref_512_1->value(), 512);
        VERIFY_EQUAL(object_ref_512_1->ref_count(), 1);

        NonNullRef object_ref_512_2{ object_ref_512_1 };
        VERIFY_EQUAL(object_ref_512_1->value(), 512);
        VERIFY_EQUAL(object_ref_512_2->value(), 512);
        VERIFY_EQUAL(object_ref_512_1->ref_count(), 2);
        VERIFY_EQUAL(object_ref_512_2->ref_count(), 2);
        VERIFY_EQUAL(object_ref_512_1.as_ptr(), object_ref_512_2.as_ptr());

        NonNullRef object_ref_512_3{ object_ref_512_1 };
        NonNullRef object_ref_512_4{ object_ref_512_2 };

        VERIFY_EQUAL(object_ref_512_1->value(), 512);
        VERIFY_EQUAL(object_ref_512_2->value(), 512);
        VERIFY_EQUAL(object_ref_512_3->value(), 512);
        VERIFY_EQUAL(object_ref_512_4->value(), 512);

        VERIFY_EQUAL(object_ref_512_1->ref_count(), 4);
        VERIFY_EQUAL(object_ref_512_2->ref_count(), 4);
        VERIFY_EQUAL(object_ref_512_3->ref_count(), 4);
        VERIFY_EQUAL(object_ref_512_4->ref_count(), 4);

        VERIFY_EQUAL(object_ref_512_1.as_ptr(), object_ref_512_2.as_ptr());
        VERIFY_EQUAL(object_ref_512_2.as_ptr(), object_ref_512_3.as_ptr());
        VERIFY_EQUAL(object_ref_512_3.as_ptr(), object_ref_512_4.as_ptr());
    }
    VERIFY_EQUAL(Object::Shared::no_ref_count_called_count(), 1);
    VERIFY(Object::Shared::destructor_was_called());
}

TEST_CASE(swap) {
    auto object_ref_512 = make_ref<Object::Shared>(512);
    VERIFY_EQUAL(object_ref_512->value(), 512);
    VERIFY_EQUAL(object_ref_512->ref_count(), 1);

    auto object_ref_768 = make_ref<Object::Shared>(768);
    VERIFY_EQUAL(object_ref_768->value(), 768);
    VERIFY_EQUAL(object_ref_768->ref_count(), 1);

    object_ref_512.swap(object_ref_768);

    VERIFY_EQUAL(object_ref_512->value(), 768);
    VERIFY_EQUAL(object_ref_512->ref_count(), 1);
    VERIFY_EQUAL(object_ref_768->value(), 512);
    VERIFY_EQUAL(object_ref_768->ref_count(), 1);
}

TEST_CASE(ref_count_into_vector) {
    Object::Shared::reset_no_ref_count_called_count();

    {
        auto object_ref = make_ref<Object::Shared>(512);

        Vector<NonNullRef<Object::Shared>> vector_1{};
        Vector<NonNullRef<Object::Shared>> vector_2{};
        Vector<NonNullRef<Object::Shared>> vector_3{};
        Vector<NonNullRef<Object::Shared>> vector_4{};

        vector_1.append(object_ref);
        vector_2.append(object_ref);
        vector_3.append(object_ref);
        vector_4.append(object_ref);
        VERIFY_EQUAL(object_ref->ref_count(), 5);

        vector_1.append(object_ref);
        VERIFY_EQUAL(object_ref->ref_count(), 6);

        vector_2.append(move(object_ref));
        VERIFY_EQUAL(vector_2.last()->ref_count(), 6);
    }
    VERIFY_EQUAL(Object::Shared::no_ref_count_called_count(), 1);
}