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

#include <LibLS/Alloc/Box.hh>
#include <LibLS/Core/ErrorOr.hh>
#include <LibUnitTest/Assertions.hh>
#include <LibUnitTest/Case.hh>

TEST_CASE(value_and_error) {
    auto return_110_on_100 = [](i32 value) -> ErrorOr<i32> {
        if ( value == 100 )
            return 110;
        else
            return Error::construct_from_code(EINVAL);
    };

    VERIFY_IS_VALUE_EQUAL(return_110_on_100(100), 110);
    VERIFY_IS_ERROR_EQUAL(return_110_on_100(0), EINVAL);
}

TEST_CASE(unwrap_value) {
    struct USize {
        usize m_value{ 0 };
    };

    ErrorOr<USize> error_or_usize = USize{ 0xcafebabe };
    verify$(error_or_usize.is_value());

    auto& usize_ref = error_or_usize.value();
    verify_equal$(usize_ref.m_value, 0xcafebabe);

    usize_ref.m_value = 123;

    auto usize_value = error_or_usize.unwrap_value();
    verify_equal$(usize_value.m_value, 123);
    verify_false$(error_or_usize.is_value());
    verify_false$(error_or_usize.is_error());
}

TEST_CASE(unwrap_error) {
    ErrorOr<i32> error_or_i32 = Error::construct_from_literal("No Entry Found"sv);
    verify$(error_or_i32.is_error());

    auto& error = error_or_i32.error();
    verify_equal$(error.code(), ENOENT);

    error = Error::construct_from_code(EINVAL);

    auto error_value = error_or_i32.unwrap_error();
    verify_equal$(error_value, EINVAL);
    verify_false$(error_or_i32.is_error());
    verify_false$(error_or_i32.is_value());
}

TEST_CASE(reference_as_value) {
    auto boxed_i32 = Box<i32>::construct_from_emplace(123);

    auto& i32_ref = boxed_i32.as_ref();

    ErrorOr<i32&> error_or_i32{ i32_ref };
    verify$(error_or_i32.is_value());

    auto& i32_ref_from_error_or = error_or_i32.value();
    verify_equal$(i32_ref_from_error_or, 123);
    verify_equal$(&i32_ref_from_error_or, &i32_ref);

    i32_ref_from_error_or = 456;
    verify_equal$(*boxed_i32, 456);
    verify_equal$(i32_ref, 456);

    auto const& int_ref_value = error_or_i32.unwrap_value();
    verify_equal$(int_ref_value, 456);
    verify_false$(error_or_i32.is_value());
    verify_false$(error_or_i32.is_error());
}

TEST_CASE(assignment_operator) {
    ErrorOr<i32> error_or_i32{ 512 };

    error_or_i32 = Error::construct_from_code(ENOENT);
    VERIFY_IS_ERROR_EQUAL(error_or_i32, ENOENT);

    error_or_i32 = 486;
    VERIFY_IS_VALUE_EQUAL(error_or_i32, 486);

    ErrorOr<void> error_or_void{};

    error_or_void = Error::construct_from_code(EINVAL);
    VERIFY_IS_ERROR_EQUAL(error_or_void, EINVAL);

    ErrorOr<Box<i32>> error_or_boxed_i32 = Box<i32>::construct_from_emplace(64);
    verify$(error_or_boxed_i32.is_value());
}