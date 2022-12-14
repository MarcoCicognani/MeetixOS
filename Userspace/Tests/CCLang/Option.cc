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
#include <CCLang/Lang/Option.hh>
#include <LibUnitTest/Assertions.hh>
#include <LibUnitTest/Case.hh>

TEST_CASE(may_produce_value) {
    auto may_produce_value = [](i32 value) -> Option<i32> {
        if ( value <= 10 )
            return value * 10;
        else
            return {};
    };

    verify_is_present$(may_produce_value(5));
    verify_is_none$(may_produce_value(20));
}

TEST_CASE(unwrap_reset_value) {
    struct Wrapper {
        usize m_value =0;
    };

    Option<Wrapper> wrapper_or_none = Wrapper{ 0xdeadbeef };
    verify$(wrapper_or_none.is_present());

    auto object_v = wrapper_or_none.unwrap();
    verify_equal$(object_v.m_value, 0xdeadbeef);
    verify_false$(wrapper_or_none.is_present());

    verify_equal$(wrapper_or_none.unwrap_or(Wrapper{ 0xcafebabe }).m_value, 0xcafebabe);
}

TEST_CASE(option_with_reference) {
    auto  u32_box = Box<u32>::from_emplace(0xdeadbeef);
    auto& u32_ref = u32_box.as_ref();

    Option<u32&> u32_or_none = u32_ref;
    verify$(u32_or_none.is_present());

    auto& int_ref_value = u32_or_none.unwrap();
    verify_equal$(int_ref_value, 0xcafebabe);
    verify_false$(u32_or_none.is_present());
}

TEST_CASE(assignment_operator) {
    Option<char> char_or_none = 'a';
    verify_is_present_equal$(char_or_none, 'a');

    char_or_none = 'b';
    verify_is_present_equal$(char_or_none, 'b');

    char_or_none = OptionNone;
    verify_is_none$(char_or_none);

    auto  u32_box = Box<u32>::from_emplace(0xcafebabe);
    auto& u32_ref = u32_box.as_ref();

    Option<u32&> ref_or_none = u32_ref;
    verify_is_present_equal$(ref_or_none, 0xcafebabe);
}