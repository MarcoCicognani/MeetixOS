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

#include <LibTC/Alloc/Box.hh>
#include <LibTC/Lang/Option.hh>
#include <LibUnitTest/Assertions.hh>
#include <LibUnitTest/Case.hh>

TEST_CASE(may_produce_value) {
    auto may_produce_value = [](int value) -> Option<int> {
        if ( value <= 10 )
            return value * 10;
        else
            return {};
    };

    VERIFY_IS_PRESENT(may_produce_value(5));
    VERIFY_IS_NONE(may_produce_value(20));
}

TEST_CASE(unwrap_reset_value) {
    struct USize {
        usize m_value{ 0 };
    };

    Option<USize> option = USize{ 0xdeadbeef };
    VERIFY(option.is_present());

    auto const& object = option.value();
    VERIFY_EQUAL(object.m_value, 0xdeadbeef);

    auto object_v = option.unwrap();
    VERIFY_EQUAL(object_v.m_value, 0xdeadbeef);
    VERIFY_FALSE(option.is_present());

    VERIFY_EQUAL(option.unwrap_or(USize{ 0xcafebabe }).m_value, 0xcafebabe);
}

TEST_CASE(option_with_reference) {
    auto boxed_ptr = Box<u32>::construct_from_emplace(0xdeadbeef);
    auto& int_ref = boxed_ptr.as_ref();

    Option<u32&> option = int_ref;
    VERIFY(option.is_present());

    auto& int_ref_from_option = option.value();
    VERIFY_EQUAL(int_ref_from_option, 0xdeadbeef);

    int_ref_from_option = 0xcafebabe;
    VERIFY_EQUAL(*boxed_ptr.as_ptr(), 0xcafebabe);
    VERIFY_EQUAL(int_ref, 0xcafebabe);

    auto& int_ref_value = option.unwrap();
    VERIFY_EQUAL(int_ref_value, 0xcafebabe);
    VERIFY_FALSE(option.is_present());
}

TEST_CASE(assignment_operator) {
    Option<char> option = 'a';
    VERIFY(option.is_present());
    VERIFY_EQUAL(option.value(), 'a');

    option = 'b';
    VERIFY(option.is_present());
    VERIFY_EQUAL(option.value(), 'b');

    option = OptNone;
    VERIFY_FALSE(option.is_present());

    auto boxed_ptr = Box<u32>::construct_from_emplace(0xcafebabe);
    auto& int_ref = boxed_ptr.as_ref();

    Option<u32&> option2 = int_ref;
    VERIFY(option2.is_present());
    VERIFY_EQUAL(option2.value(), 0xcafebabe);
}