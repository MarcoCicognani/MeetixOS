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

#include <LibTC/Collection/Vector.hh>
#include <LibTC/Cxx.hh>
#include <LibTC/IntTypes.hh>
#include <LibTC/Memory/NonNullBox.hh>
#include <LibUnitTest/Assertions.hh>
#include <LibUnitTest/Case.hh>

using namespace TC;

TEST_CASE(construct) {
    NonNullBox<u64> boxed_u64{ FromArgs, 0xcafebabe };
    VERIFY_EQUAL(boxed_u64.as_ref(), 0xcafebabe);
}

TEST_CASE(construct_object) {
    static bool s_destructor_called = false;

    struct USize {
    public:
        usize m_value{ 0 };

        ~USize() {
            s_destructor_called = true;
        }
    };

    {
        NonNullBox<USize> boxed_usize{ FromArgs, 512uL };
        VERIFY_EQUAL(boxed_usize->m_value, 512uL);
    }
    VERIFY(s_destructor_called);
}

template<usize SIZE>
struct Array {
public:
    u64 m_values[SIZE]{ 0 };

    Array() {
        m_values[0] = 0xcafebabe;
        m_values[1] = 0xdeadbeef;
    }
};

TEST_CASE(try_construct_from_args) {
    auto error_or_boxed_array = NonNullBox<Array<0x1000>>::try_construct_from_args();
    VERIFY_IS_VALUE(error_or_boxed_array);

    auto object_box = error_or_boxed_array.unwrap_value();
    VERIFY_EQUAL(object_box->m_values[0], 0xcafebabe);
    VERIFY_EQUAL(object_box->m_values[1], 0xdeadbeef);

    auto error_or_boxed_array_over_limit = NonNullBox<Array<0xfffffff>>::try_construct_from_args();
    VERIFY_IS_ERROR_EQUAL(error_or_boxed_array_over_limit, ENOMEM);
}

TEST_CASE(swap) {
    NonNullBox<i32> boxed_i32_64{ FromArgs, 64 };
    VERIFY_EQUAL(boxed_i32_64.as_ref(), 64);

    NonNullBox<i32> boxed_i32_128{ FromArgs, 128 };
    VERIFY_EQUAL(boxed_i32_128.as_ref(), 128);

    boxed_i32_64.swap(boxed_i32_128);
    VERIFY_EQUAL(boxed_i32_64.as_ref(), 128);
    VERIFY_EQUAL(boxed_i32_128.as_ref(), 64);
}

TEST_CASE(move) {
    struct USizePair {
    public:
        usize m_first_value{ 0 };
        usize m_second_value{ 0 };
    };

    NonNullBox<USizePair> boxed_usize_pair{ FromArgs, 512uL, 1024uL };
    VERIFY_EQUAL(boxed_usize_pair->m_first_value, 512uL);
    VERIFY_EQUAL(boxed_usize_pair->m_second_value, 1024uL);

    NonNullBox boxed_usize_pair_2{ move(boxed_usize_pair) };
    VERIFY_EQUAL(boxed_usize_pair_2->m_first_value, 512uL);
    VERIFY_EQUAL(boxed_usize_pair_2->m_second_value, 1024uL);

    NonNullBox<USizePair> boxed_usize_pair_3{ FromArgs, 0xab, 0xcd };
    VERIFY_EQUAL(boxed_usize_pair_3->m_first_value, 0xab);
    VERIFY_EQUAL(boxed_usize_pair_3->m_second_value, 0xcd);

    boxed_usize_pair = move(boxed_usize_pair_3);
    VERIFY_EQUAL(boxed_usize_pair->m_first_value, 0xab);
    VERIFY_EQUAL(boxed_usize_pair->m_second_value, 0xcd);
}

TEST_CASE(vector_of_boxes) {
    Vector<NonNullBox<i32>> vector_of_boxes{};

    vector_of_boxes.append(NonNullBox<i32>{ FromArgs, 256 });
    vector_of_boxes.append(NonNullBox<i32>{ FromArgs, 512 });
    vector_of_boxes.append(NonNullBox<i32>{ FromArgs, 1024 });

    NonNullBox<i32> boxed_i32{ FromArgs, 4096 };
    vector_of_boxes.append(move(boxed_i32));

    VERIFY_EQUAL(vector_of_boxes[0].as_ref(), 256);
    VERIFY_EQUAL(vector_of_boxes[1].as_ref(), 512);
    VERIFY_EQUAL(vector_of_boxes[2].as_ref(), 1024);
    VERIFY_EQUAL(vector_of_boxes[3].as_ref(), 4096);
}