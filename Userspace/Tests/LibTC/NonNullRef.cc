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

TEST_CASE(construct) {
    NonNullRef<i32> ref_i32{ FromArgs, 64u };

    VERIFY_EQUAL(ref_i32.strong_ref_count(), 1);
    VERIFY_EQUAL(ref_i32.as_ref(), 64u);
}

TEST_CASE(copy) {
    NonNullRef<i32> ref_i32{ FromArgs, 64u };

    NonNullRef<i32> ref_i32_2{ ref_i32 };

    VERIFY_EQUAL(ref_i32.strong_ref_count(), 2);
    VERIFY_EQUAL(ref_i32_2.strong_ref_count(), 2);

    VERIFY_EQUAL(ref_i32.as_ref(), 64u);
    VERIFY_EQUAL(ref_i32_2.as_ref(), 64u);

    VERIFY_EQUAL(ref_i32.as_ptr(), ref_i32_2.as_ptr());
}

TEST_CASE(with_object) {
    static bool s_destructor_was_called = false;

    struct Object {
    public:
        ~Object() {
            s_destructor_was_called = true;
        }
    };

    {
        NonNullRef<Object> ref_object_0{ FromArgs };

        {
            NonNullRef<Object> ref_object_1{ ref_object_0 };

            VERIFY_EQUAL(ref_object_1.strong_ref_count(), 2);
        }

        VERIFY_FALSE(s_destructor_was_called);
    }

    VERIFY(s_destructor_was_called);
}

TEST_CASE(reset_with_another_object) {
    static bool s_destructor_was_called = false;

    struct Object {
    public:
        usize m_value{ 0 };

        ~Object() {
            s_destructor_was_called = true;
        }
    };

    NonNullRef<Object> ref_object_32{ FromArgs, 32 };
    VERIFY_EQUAL(ref_object_32.as_ref().m_value, 32);

    NonNullRef<Object> ref_object_64{ FromArgs, 64 };
    VERIFY_EQUAL(ref_object_64.as_ref().m_value, 64);

    ref_object_64 = ref_object_32;
    VERIFY_EQUAL(ref_object_64.as_ref().m_value, 32);
    VERIFY(s_destructor_was_called);
}

TEST_CASE(swap) {
    NonNullRef<i32> ref_i32_512{ FromArgs, 512u };
    VERIFY_EQUAL(ref_i32_512.as_ref(), 512u);
    VERIFY_EQUAL(ref_i32_512.strong_ref_count(), 1);

    NonNullRef<i32> ref_i32_768{ FromArgs, 768u };
    VERIFY_EQUAL(ref_i32_768.as_ref(), 768u);
    VERIFY_EQUAL(ref_i32_768.strong_ref_count(), 1);

    NonNullRef<i32> ref_i32_768_ref{ ref_i32_768 };

    ref_i32_512.swap(ref_i32_768);

    VERIFY_EQUAL(ref_i32_512.as_ref(), 768u);
    VERIFY_EQUAL(ref_i32_512.strong_ref_count(), 2);
    VERIFY_EQUAL(ref_i32_768.as_ref(), 512u);
    VERIFY_EQUAL(ref_i32_768.strong_ref_count(), 1);
}

TEST_CASE(ref_count_into_vector) {
    static i32 s_destructor_called_count = 0;

    struct Object {
    public:
        usize m_value{ 0 };

        ~Object() {
            ++s_destructor_called_count;
        }
    };

    {
        NonNullRef<Object> ref_object{ FromArgs, 64u };

        Vector<NonNullRef<Object>> vector_1{ ref_object };
        Vector<NonNullRef<Object>> vector_2{ ref_object };
        Vector<NonNullRef<Object>> vector_3{ ref_object };
        Vector<NonNullRef<Object>> vector_4{ ref_object };
        VERIFY_EQUAL(ref_object.strong_ref_count(), 5);

        vector_1.append(ref_object);
        VERIFY_EQUAL(ref_object.strong_ref_count(), 6);

        vector_2.append(move(ref_object));
        VERIFY_EQUAL(vector_2.last().strong_ref_count(), 6);
    }

    VERIFY_EQUAL(s_destructor_called_count, 1);
}