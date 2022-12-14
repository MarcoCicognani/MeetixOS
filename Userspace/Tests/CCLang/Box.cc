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

#include <CCLang/Alloc/Box.hh>
#include <CCLang/Alloc/Vector.hh>
#include <CCLang/Lang/Cxx.hh>
#include <CCLang/Lang/IntTypes.hh>
#include <LibUnitTest/Assertions.hh>
#include <LibUnitTest/Case.hh>

TEST_CASE(construct) {
    verify_equal$(Box<u64>::from_emplace(0xcafebabe).as_ref(), 0xcafebabe);
}

TEST_CASE(new_object) {
    static bool s_destructor_called = false;

    struct Wrapper {
    public:
        usize m_value = 0;

        static auto from_value(usize value) -> Box<Wrapper> {
            auto error_or_wrapper_box = Box<Wrapper>::try_from_adopt(new (nothrow) Wrapper(value));
            verify$(error_or_wrapper_box.is_value());
            return error_or_wrapper_box.unwrap();
        }

        ~Wrapper() {
            s_destructor_called = true;
        }

    private:
        explicit Wrapper() = default;
        explicit Wrapper(usize value)
            : m_value(value) {
        }
    };

    verify_equal$(Wrapper::from_value(512)->m_value, 512u);
    verify$(s_destructor_called);
}

template<usize::NativeInt SIZE>
class Array {
public:
    Array() {
        m_values[0] = 0xcafebabe;
        m_values[1] = 0xdeadbeef;
    }

    auto at(usize index) const -> u64 const& {
        return m_values[index.unwrap()];
    }

private:
    u64 m_values[SIZE] = { 0 };
};

TEST_CASE(try_new_from_args) {
    auto error_or_array_box = Box<Array<0x1000>>::try_from_emplace();
    verify$(error_or_array_box.is_value());

    auto array_box = error_or_array_box.unwrap();
    verify_equal$(array_box->at(0), 0xcafebabe);
    verify_equal$(array_box->at(1), 0xdeadbeef);

    verify_is_error_equal$(Box<Array<0xfffffff>>::try_from_emplace(), ErrorCode::NoMemory);
}

TEST_CASE(swap) {
    auto i32_box_v64 = Box<i32>::from_emplace(64);
    verify_equal$(i32_box_v64.as_ref(), 64);

    auto i32_box_v128 = Box<i32>::from_emplace(128);
    verify_equal$(i32_box_v128.as_ref(), 128);

    i32_box_v64.swap(i32_box_v128);
    verify_equal$(i32_box_v64.as_ref(), 128);
    verify_equal$(i32_box_v128.as_ref(), 64);
}

TEST_CASE(move) {
    struct Pair {
    public:
        usize m_first_value  = 0;
        usize m_second_value = 0;

        explicit Pair(usize first, usize second)
            : m_first_value(first)
            , m_second_value(second) {
        }
    };

    auto pair_box = Box<Pair>::from_emplace(512, 1024);
    verify_equal$(pair_box->m_first_value, 512);
    verify_equal$(pair_box->m_second_value, 1024);

    auto pair_box_2 = Cxx::move(pair_box);
    verify_equal$(pair_box_2->m_first_value, 512);
    verify_equal$(pair_box_2->m_second_value, 1024);

    auto pair_box_3 = Box<Pair>::from_emplace(0xab, 0xcd);
    verify_equal$(pair_box_3->m_first_value, 0xab);
    verify_equal$(pair_box_3->m_second_value, 0xcd);

    pair_box = Cxx::move(pair_box_3);
    verify_equal$(pair_box->m_first_value, 0xab);
    verify_equal$(pair_box->m_second_value, 0xcd);
}

TEST_CASE(leak) {
    auto i32_box = Box<i32>::from_emplace(512);

    auto& i32_ref_unboxed = i32_box.leak_ref();
    verify$(i32_box.is_null());

    delete &i32_ref_unboxed;
}

TEST_CASE(vector_of_boxes) {
    auto vector_of_boxes = Vector<Box<i32>>::with_capacity(4);

    vector_of_boxes.append(Box<i32>::from_emplace(0x123));
    vector_of_boxes.append(Box<i32>::from_emplace(0x456));
    vector_of_boxes.append(Box<i32>::from_emplace(0xabc));

    auto i32_box = Box<i32>::from_emplace(0xdef);
    vector_of_boxes.append(Cxx::move(i32_box));

    verify_equal$(vector_of_boxes[0].as_ref(), 0x123);
    verify_equal$(vector_of_boxes[1].as_ref(), 0x456);
    verify_equal$(vector_of_boxes[2].as_ref(), 0xabc);
    verify_equal$(vector_of_boxes[3].as_ref(), 0xdef);
}