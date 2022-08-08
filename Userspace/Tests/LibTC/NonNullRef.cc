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

#include <LibTC/Collection/List.hh>
#include <LibTC/Cxx.hh>
#include <LibTC/IntTypes.hh>
#include <LibTC/Memory/NonNullRef.hh>
#include <LibUnitTest/Assertions.hh>
#include <LibUnitTest/Case.hh>

using namespace TC;

TEST_CASE(construct) {
    auto const ref_i32 = NonNullRef<i32>::construct_from_args(64);

    VERIFY_EQUAL(ref_i32.strong_ref_count(), 1);
    VERIFY_EQUAL(ref_i32.as_ref(), 64u);
}

TEST_CASE(copy) {
    auto const ref_i32   = NonNullRef<i32>::construct_from_args(64);
    auto const ref_i32_2 = ref_i32.clone();

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
        auto const ref_object_0 = NonNullRef<Object>::construct_from_args();
        {
            auto const ref_object_1 = ref_object_0.clone();
            VERIFY_EQUAL(ref_object_1.strong_ref_count(), 2);
        }

        VERIFY_FALSE(s_destructor_was_called);
    }

    VERIFY(s_destructor_was_called);
}

TEST_CASE(swap) {
    auto ref_i32_512 = NonNullRef<i32>::construct_from_args(512);
    VERIFY_EQUAL(ref_i32_512.as_ref(), 512u);
    VERIFY_EQUAL(ref_i32_512.strong_ref_count(), 1);

    auto ref_i32_768 = NonNullRef<i32>::construct_from_args(768);
    VERIFY_EQUAL(ref_i32_768.as_ref(), 768u);
    VERIFY_EQUAL(ref_i32_768.strong_ref_count(), 1);

    auto const ref_i32_768_clone = ref_i32_768.clone();

    ref_i32_512.swap(ref_i32_768);

    VERIFY_EQUAL(ref_i32_512.as_ref(), 768);
    VERIFY_EQUAL(ref_i32_512.strong_ref_count(), 2);
    VERIFY_EQUAL(ref_i32_768.as_ref(), 512);
    VERIFY_EQUAL(ref_i32_768.strong_ref_count(), 1);
}

static i32 s_destructor_called_count = 0;

struct Object {
public:
    usize m_value{ 0 };

    ~Object() {
        ++s_destructor_called_count;
    }
};

TEST_CASE(ref_count_into_vector) {
    {
        auto ref_object = NonNullRef<Object>::construct_from_args(64u);

        auto list_1 = List<NonNullRef<Object>>::construct_from_list({ ref_object.clone() });
        auto list_2 = List<NonNullRef<Object>>::construct_from_list({ ref_object.clone() });
        auto const list_3 = List<NonNullRef<Object>>::construct_from_list({ ref_object.clone() });
        auto const list_4 = List<NonNullRef<Object>>::construct_from_list({ ref_object.clone() });
        VERIFY_EQUAL(ref_object.strong_ref_count(), 5);

        list_1.append(ref_object.clone());
        VERIFY_EQUAL(ref_object.strong_ref_count(), 6);

        list_2.append(Cxx::move(ref_object));
        VERIFY_EQUAL(list_2.last().strong_ref_count(), 6);
    }

    VERIFY_EQUAL(s_destructor_called_count, 1);
}