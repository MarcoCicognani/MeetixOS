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

#include <LibTC/Cxx.hh>
#include <LibTC/IntTypes.hh>
#include <LibTC/Memory/NonNullRef.hh>
#include <LibUnitTest/Assertions.hh>
#include <LibUnitTest/Case.hh>

using namespace TC;

namespace Object {

class Shared {
public:
    Shared() = default;
    explicit Shared(usize value, usize value2)
        : m_value{ value } {
    }

    ~Shared() {
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

private:
    usize m_value{ 0 };

    static usize s_no_ref_count_called_count;
    static bool  s_destructor_was_called;
};

usize Shared::s_no_ref_count_called_count = 0;
bool  Shared::s_destructor_was_called     = false;

} /* namespace Object */

TEST_CASE(construct_nonnull_ref) {
    NonNullRef<Object::Shared> object_ref{ FromArgs, 512uL, 64uL };
    VERIFY_EQUAL(object_ref->value(), 512);
    VERIFY_EQUAL(object_ref.strong_ref_count(), 1);

    object_ref.as_ref().inc_value(512);
    VERIFY_EQUAL(object_ref->value(), 1024);
}

TEST_CASE(try_construct_nonnull_ref) {
    auto error_or_object_ref = NonNullRef<Object::Shared>::try_construct_from_args(4096uL, 64uL);
    VERIFY(error_or_object_ref.is_value());
    VERIFY_EQUAL(error_or_object_ref.value()->value(), 4096);
    VERIFY_EQUAL(error_or_object_ref.value().strong_ref_count(), 1);

    auto object_ref = error_or_object_ref.unwrap_value();
    VERIFY_EQUAL(object_ref->value(), 4096);
    VERIFY_EQUAL(object_ref.strong_ref_count(), 1);
}

TEST_CASE(ref_count_copy) {
    Object::Shared::reset_no_ref_count_called_count();
    Object::Shared::reset_destructor_was_called();

    {
        NonNullRef<Object::Shared> object_ref_512_1{ FromArgs, 512uL, 64uL };
        VERIFY_EQUAL(object_ref_512_1->value(), 512);
        VERIFY_EQUAL(object_ref_512_1.strong_ref_count(), 1);

        NonNullRef object_ref_512_2{ object_ref_512_1 };
        VERIFY_EQUAL(object_ref_512_1->value(), 512);
        VERIFY_EQUAL(object_ref_512_2->value(), 512);
        VERIFY_EQUAL(object_ref_512_1.strong_ref_count(), 2);
        VERIFY_EQUAL(object_ref_512_2.strong_ref_count(), 2);
        VERIFY_EQUAL(object_ref_512_1.as_ptr(), object_ref_512_2.as_ptr());

        NonNullRef object_ref_512_3{ object_ref_512_1 };
        NonNullRef object_ref_512_4{ object_ref_512_2 };

        VERIFY_EQUAL(object_ref_512_1->value(), 512);
        VERIFY_EQUAL(object_ref_512_2->value(), 512);
        VERIFY_EQUAL(object_ref_512_3->value(), 512);
        VERIFY_EQUAL(object_ref_512_4->value(), 512);

        VERIFY_EQUAL(object_ref_512_1.strong_ref_count(), 4);
        VERIFY_EQUAL(object_ref_512_2.strong_ref_count(), 4);
        VERIFY_EQUAL(object_ref_512_3.strong_ref_count(), 4);
        VERIFY_EQUAL(object_ref_512_4.strong_ref_count(), 4);

        VERIFY_EQUAL(object_ref_512_1.as_ptr(), object_ref_512_2.as_ptr());
        VERIFY_EQUAL(object_ref_512_2.as_ptr(), object_ref_512_3.as_ptr());
        VERIFY_EQUAL(object_ref_512_3.as_ptr(), object_ref_512_4.as_ptr());
    }
    VERIFY_EQUAL(Object::Shared::no_ref_count_called_count(), 1);
    VERIFY(Object::Shared::destructor_was_called());
}

TEST_CASE(swap) {
    NonNullRef<Object::Shared> object_ref_512{ FromArgs, 512uL, 64uL };
    VERIFY_EQUAL(object_ref_512->value(), 512);
    VERIFY_EQUAL(object_ref_512.strong_ref_count(), 1);

    NonNullRef<Object::Shared> object_ref_768{ FromArgs, 768uL, 64uL };
    VERIFY_EQUAL(object_ref_768->value(), 768);
    VERIFY_EQUAL(object_ref_768.strong_ref_count(), 1);

    NonNullRef<Object::Shared> object_ref_768_ref{ object_ref_768 };

    object_ref_512.swap(object_ref_768);

    VERIFY_EQUAL(object_ref_512->value(), 768);
    VERIFY_EQUAL(object_ref_512.strong_ref_count(), 2);
    VERIFY_EQUAL(object_ref_768->value(), 512);
    VERIFY_EQUAL(object_ref_768.strong_ref_count(), 1);
}

TEST_CASE(ref_count_into_vector) {
    Object::Shared::reset_no_ref_count_called_count();

    {
        NonNullRef<Object::Shared> object_ref{ FromArgs, 512uL, 64uL };

        Vector<NonNullRef<Object::Shared>> vector_1{};
        Vector<NonNullRef<Object::Shared>> vector_2{};
        Vector<NonNullRef<Object::Shared>> vector_3{};
        Vector<NonNullRef<Object::Shared>> vector_4{};

        vector_1.append(object_ref);
        vector_2.append(object_ref);
        vector_3.append(object_ref);
        vector_4.append(object_ref);
        VERIFY_EQUAL(object_ref.strong_ref_count(), 5);

        vector_1.append(object_ref);
        VERIFY_EQUAL(object_ref.strong_ref_count(), 6);

        vector_2.append(move(object_ref));
        VERIFY_EQUAL(vector_2.last().strong_ref_count(), 6);
    }
    VERIFY_EQUAL(Object::Shared::no_ref_count_called_count(), 1);
}