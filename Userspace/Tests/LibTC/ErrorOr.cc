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

#include <LibTC/Functional/ErrorOr.hh>
#include <LibTC/Memory/NonNullBox.hh>
#include <LibUnitTest/Assertions.hh>
#include <LibUnitTest/Case.hh>

using namespace TC;

TEST_CASE(value_and_error) {
    auto return_110_on_100 = [](i32 value) -> ErrorOr<i32> {
        if ( value == 100 )
            return 110;
        else
            return Error{ EINVAL };
    };

    VERIFY_IS_VALUE_EQUAL(return_110_on_100(100), 110);
    VERIFY_IS_ERROR_EQUAL(return_110_on_100(0), EINVAL);
}

TEST_CASE(unwrap_value) {
    struct USize {
    public:
        usize m_value{ 0 };
    };

    ErrorOr<USize> error_or_usize{ USize{ 0xcafebabe } };
    VERIFY(error_or_usize.is_value());

    auto& usize_ref = error_or_usize.value();
    VERIFY_EQUAL(usize_ref.m_value, 0xcafebabe);

    usize_ref.m_value = 123;

    auto usize_value = error_or_usize.unwrap_value();
    VERIFY_EQUAL(usize_value.m_value, 123);
    VERIFY_FALSE(error_or_usize.is_value());
    VERIFY_FALSE(error_or_usize.is_error());
}

TEST_CASE(unwrap_error) {
    ErrorOr<i32> error_or_i32{ Error{ ENOENT, "No Entry Found" } };
    VERIFY(error_or_i32.is_error());

    auto& error = error_or_i32.error();
    VERIFY_EQUAL(error.os_error(), ENOENT);

    error = Error{ EINVAL };

    auto error_value = error_or_i32.unwrap_error();
    VERIFY_EQUAL(error_value, EINVAL);
    VERIFY_FALSE(error_or_i32.is_error());
    VERIFY_FALSE(error_or_i32.is_value());
}

TEST_CASE(reference_as_value) {
    NonNullBox<i32> boxed_i32{ FromArgs, 123 };

    auto& i32_ref = boxed_i32.as_ref();

    ErrorOr<i32&> error_or_i32{ i32_ref };
    VERIFY(error_or_i32.is_value());

    auto& i32_ref_from_error_or = error_or_i32.value();
    VERIFY_EQUAL(i32_ref_from_error_or, 123);
    VERIFY_EQUAL(&i32_ref_from_error_or, &i32_ref);

    i32_ref_from_error_or = 456;
    VERIFY_EQUAL(*boxed_i32, 456);
    VERIFY_EQUAL(i32_ref, 456);

    auto const& int_ref_value = error_or_i32.unwrap_value();
    VERIFY_EQUAL(int_ref_value, 456);
    VERIFY_FALSE(error_or_i32.is_value());
    VERIFY_FALSE(error_or_i32.is_error());
}

TEST_CASE(assignment_operator) {
    ErrorOr<i32> error_or_i32{ 512 };

    error_or_i32 = Error{ ENOENT };
    VERIFY_IS_ERROR_EQUAL(error_or_i32, ENOENT);

    error_or_i32 = 486;
    VERIFY_IS_VALUE_EQUAL(error_or_i32, 486);

    ErrorOr<void> error_or_void{};

    error_or_void = Error{ EINVAL };
    VERIFY_IS_ERROR_EQUAL(error_or_void, EINVAL);

    ErrorOr<NonNullBox<i32>> error_or_boxed_i32{ NonNullBox<i32>{ FromArgs, 64 } };
    VERIFY(error_or_boxed_i32.is_value());
}