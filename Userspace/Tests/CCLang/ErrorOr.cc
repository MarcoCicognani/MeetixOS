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
#include <CCLang/Core/ErrorOr.hh>
#include <LibUnitTest/Assertions.hh>
#include <LibUnitTest/Case.hh>

TEST_CASE(value_and_error) {
    auto return_110_on_100 = [](i32 value) -> ErrorOr<i32> {
        if ( value == 100 ) {
            return i32(value + 10);
        } else {
            return Error::from_code(ErrorCode::BadParameter);
        }
    };

    verify_is_value_equal$(return_110_on_100(100), 110);
    verify_is_error_equal$(return_110_on_100(0), ErrorCode::BadParameter);
}

TEST_CASE(unwrap_value) {
    struct Wrapper {
        usize m_value = 0;
    };

    ErrorOr<Wrapper> error_or_wrapper = Wrapper{ 0xcafebabe };
    verify$(error_or_wrapper.is_value());

    auto const wrapper = error_or_wrapper.unwrap();
    verify_equal$(wrapper.m_value, 0xcafebabe);

    verify_false$(error_or_wrapper.is_value());
    verify_false$(error_or_wrapper.is_error());
}

TEST_CASE(unwrap_error) {
    ErrorOr<i32> error_or_i32 = Error::from_literal("No Entry Found"sv);
    verify$(error_or_i32.is_error());

    auto const error = error_or_i32.unwrap_error();
    verify_equal$(error, "No Entry Found"sv);

    verify_false$(error_or_i32.is_error());
    verify_false$(error_or_i32.is_value());
}

TEST_CASE(reference_as_value) {
    auto  i32_box = Box<i32>::from_emplace(0x123);
    auto& i32_ref = i32_box.as_ref();

    ErrorOr<i32&> error_or_i32 = i32_ref;
    verify$(error_or_i32.is_value());

    auto const& i32_ref_value = error_or_i32.unwrap();
    verify_equal$(i32_ref_value, 0x123);

    verify_false$(error_or_i32.is_value());
    verify_false$(error_or_i32.is_error());
}

TEST_CASE(assignment_operator) {
    ErrorOr<i32> error_or_i32 = i32(0xabc);
    verify_is_value_equal$(error_or_i32, 0xabc);

    error_or_i32 = Error::from_code(ErrorCode::NotFound);
    verify_is_error_equal$(error_or_i32, ErrorCode::NotFound);
}