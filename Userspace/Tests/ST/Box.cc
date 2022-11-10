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

#include <ST/Alloc/Box.hh>
#include <ST/Alloc/Vector.hh>
#include <ST/Lang/Cxx.hh>
#include <ST/Lang/IntTypes.hh>
#include <LibUnitTest/Assertions.hh>
#include <LibUnitTest/Case.hh>

TEST_CASE(construct) {
    auto const boxed_u64 = Box<u64>::construct_from_emplace(0xcafebabe);
    verify_equal$(boxed_u64.as_ref(), 0xcafebabe);
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
        auto const boxed_usize = Box<USize>::construct_from_emplace(512u);
        verify_equal$(boxed_usize->m_value, 512u);
    }
    verify$(s_destructor_called);
}

static bool s_destructor_private_called = false;

struct USizePrivate {
    TCBoxConstructible$(USizePrivate);

public:
    usize m_value{ 0 };

    ~USizePrivate() {
        s_destructor_private_called = true;
    }

private:
    explicit constexpr USizePrivate(usize value)
        : m_value{ value } {
    }
};

TEST_CASE(construct_object_with_private_constructor) {
    {
        auto const boxed_usize = Box<USizePrivate>::construct_from_emplace(512u);
        verify_equal$(boxed_usize->m_value, 512u);
    }
    verify$(s_destructor_private_called);
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
    auto error_or_boxed_array = Box<Array<0x1000>>::try_construct_from_emplace();
    verify$(error_or_boxed_array.is_value());

    auto object_box = error_or_boxed_array.unwrap_value();
    verify_equal$(object_box->m_values[0], 0xcafebabe);
    verify_equal$(object_box->m_values[1], 0xdeadbeef);

    VERIFY_IS_ERROR_EQUAL(Box<Array<0xfffffff>>::try_construct_from_emplace(), ENOMEM);
}

TEST_CASE(swap) {
    auto boxed_i32_64 = Box<i32>::construct_from_emplace(64);
    verify_equal$(boxed_i32_64.as_ref(), 64);

    auto boxed_i32_128 = Box<i32>::construct_from_emplace(128);
    verify_equal$(boxed_i32_128.as_ref(), 128);

    boxed_i32_64.swap(boxed_i32_128);
    verify_equal$(boxed_i32_64.as_ref(), 128);
    verify_equal$(boxed_i32_128.as_ref(), 64);
}

TEST_CASE(move) {
    struct USizePair {
    public:
        usize m_first_value{ 0 };
        usize m_second_value{ 0 };
    };

    auto boxed_usize_pair = Box<USizePair>::construct_from_emplace(512u, 1024u);
    verify_equal$(boxed_usize_pair->m_first_value, 512u);
    verify_equal$(boxed_usize_pair->m_second_value, 1024u);

    auto boxed_usize_pair_2 = Cxx::move(boxed_usize_pair);
    verify_equal$(boxed_usize_pair_2->m_first_value, 512uL);
    verify_equal$(boxed_usize_pair_2->m_second_value, 1024uL);

    auto boxed_usize_pair_3 = Box<USizePair>::construct_from_emplace(0xab, 0xcd);
    verify_equal$(boxed_usize_pair_3->m_first_value, 0xab);
    verify_equal$(boxed_usize_pair_3->m_second_value, 0xcd);

    boxed_usize_pair = Cxx::move(boxed_usize_pair_3);
    verify_equal$(boxed_usize_pair->m_first_value, 0xab);
    verify_equal$(boxed_usize_pair->m_second_value, 0xcd);
}

TEST_CASE(leak) {
    auto boxed_i32 = Box<i32>::construct_from_emplace(512);

    auto const& unmanaged_i32_ref = boxed_i32.leak_ref();
    verify$(boxed_i32.is_null());

    delete &unmanaged_i32_ref;
}

TEST_CASE(vector_of_boxes) {
    auto vector_of_boxes = Vector<Box<i32>>::construct_with_capacity(4);

    vector_of_boxes.append(Box<i32>::construct_from_emplace(256));
    vector_of_boxes.append(Box<i32>::construct_from_emplace(512));
    vector_of_boxes.append(Box<i32>::construct_from_emplace(1024));

    auto boxed_i32 = Box<i32>::construct_from_emplace(4096);
    vector_of_boxes.append(Cxx::move(boxed_i32));

    verify_equal$(vector_of_boxes[0].as_ref(), 256);
    verify_equal$(vector_of_boxes[1].as_ref(), 512);
    verify_equal$(vector_of_boxes[2].as_ref(), 1024);
    verify_equal$(vector_of_boxes[3].as_ref(), 4096);
}