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
#include <ST/Lang/Option.hh>
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
    verify$(option.is_present());

    auto const& object = option.value();
    verify_equal$(object.m_value, 0xdeadbeef);

    auto object_v = option.unwrap();
    verify_equal$(object_v.m_value, 0xdeadbeef);
    verify_false$(option.is_present());

    verify_equal$(option.unwrap_or(USize{ 0xcafebabe }).m_value, 0xcafebabe);
}

TEST_CASE(option_with_reference) {
    auto boxed_ptr = Box<u32>::construct_from_emplace(0xdeadbeef);
    auto& int_ref = boxed_ptr.as_ref();

    Option<u32&> option = int_ref;
    verify$(option.is_present());

    auto& int_ref_from_option = option.value();
    verify_equal$(int_ref_from_option, 0xdeadbeef);

    int_ref_from_option = 0xcafebabe;
    verify_equal$(*boxed_ptr.as_ptr(), 0xcafebabe);
    verify_equal$(int_ref, 0xcafebabe);

    auto& int_ref_value = option.unwrap();
    verify_equal$(int_ref_value, 0xcafebabe);
    verify_false$(option.is_present());
}

TEST_CASE(assignment_operator) {
    Option<char> option = 'a';
    verify$(option.is_present());
    verify_equal$(option.value(), 'a');

    option = 'b';
    verify$(option.is_present());
    verify_equal$(option.value(), 'b');

    option = OptNone;
    verify_false$(option.is_present());

    auto boxed_ptr = Box<u32>::construct_from_emplace(0xcafebabe);
    auto& int_ref = boxed_ptr.as_ref();

    Option<u32&> option2 = int_ref;
    verify$(option2.is_present());
    verify_equal$(option2.value(), 0xcafebabe);
}