/**
 * @brief
 * This file is part of the MeetiX Operating System.
 * Copyright (c) 2017-2021, Marco Cicognani (marco.cicognani@meetixos.org)
 *
 * @developers
 * Marco Cicognani (marco.cicognani@meetixos.org)
 *
 * @license
 * GNU General Public License version 3
 */

#include <TC/Functional/ErrorOr.hh>
#include <UnitTest/Case.hh>
#include <UnitTest/Macros/Verify.hh>
#include <UnitTest/Macros/VerifyEqual.hh>
#include <UnitTest/Macros/VerifyFalse.hh>

using TC::Functional::ErrorOr;

TEST_CASE(value_and_error) {
    auto may_produce_value = [](int value) -> ErrorOr<int> {
        if ( value == 100 )
            return 110;
        else
            return EINVAL;
    };

    auto must_be_value_result = may_produce_value(100);
    VERIFY(must_be_value_result.is_value());
    VERIFY_EQUAL(must_be_value_result.value(), 110);

    auto must_be_error_result = may_produce_value(0);
    VERIFY(must_be_error_result.is_error());
    VERIFY_EQUAL(must_be_error_result.error(), EINVAL);
}

TEST_CASE(unwrap_value) {
    class Object {
    public:
        Object() = default;
        explicit Object(usize value)
            : m_value{ value } {
        }

        [[nodiscard]] usize value() const {
            return m_value;
        }

    private:
        usize m_value{ 0 };
    };

    ErrorOr<Object> error_or_object{ Object{ 0xcafebabe } };
    VERIFY(error_or_object.is_value());

    auto const& object = error_or_object.value();
    VERIFY_EQUAL(object.value(), 0xcafebabe);

    auto object_v = error_or_object.unwrap_value();
    VERIFY_EQUAL(object_v.value(), 0xcafebabe);
    VERIFY_FALSE(error_or_object.is_value());
    VERIFY_FALSE(error_or_object.is_error());
}

TEST_CASE(unwrap_error) {
    ErrorOr<int> error_or_int{ ENOENT };
    VERIFY(error_or_int.is_error());

    auto const& error = error_or_int.error();
    VERIFY_EQUAL(error, ENOENT);

    auto error_v = error_or_int.unwrap_error();
    VERIFY_EQUAL(error_v, ENOENT);
    VERIFY_FALSE(error_or_int.is_error());
    VERIFY_FALSE(error_or_int.is_value());
}

TEST_CASE(reference_as_value) {
    auto  int_ptr = new usize{ 0xdeadbeef };
    auto& int_ref = *int_ptr;

    ErrorOr<usize&> error_or_usize{ int_ref };

    VERIFY(error_or_usize.is_value());

    auto& int_ref_from_error_or = error_or_usize.value();
    VERIFY_EQUAL(int_ref_from_error_or, 0xdeadbeef);

    int_ref_from_error_or = 0xcafebabe;
    VERIFY_EQUAL(*int_ptr, 0xcafebabe);
    VERIFY_EQUAL(int_ref, 0xcafebabe);

    auto& int_ref_value = error_or_usize.unwrap_value();
    VERIFY_EQUAL(int_ref_value, 0xcafebabe);
    VERIFY_FALSE(error_or_usize.is_value());
    VERIFY_FALSE(error_or_usize.is_error());

    delete int_ptr;
}

TEST_CASE(assignment_operator) {
    ErrorOr<int> error_or_int{ 512 };

    error_or_int = ENOENT;
    VERIFY(error_or_int.is_error());
    VERIFY_FALSE(error_or_int.is_value());
    VERIFY_EQUAL(error_or_int.error(), ENOENT);

    error_or_int = 4096;
    VERIFY(error_or_int.is_value());
    VERIFY_FALSE(error_or_int.is_error());
    VERIFY_EQUAL(error_or_int.value(), 4096);

    ErrorOr<void> error_or_void{};

    error_or_void = EINVAL;
    VERIFY(error_or_void.is_error());
    VERIFY_FALSE(error_or_void.is_value());
    VERIFY_EQUAL(error_or_void.error(), EINVAL);

    ErrorOr<int&> error_or_ref = *new int{ 256 };

    VERIFY(error_or_ref.is_value());

    auto* value = &error_or_ref.value();
    delete value;

    error_or_ref = ENOENT;
    VERIFY(error_or_ref.is_error());
    VERIFY_FALSE(error_or_ref.is_value());
    VERIFY_EQUAL(error_or_ref.error(), ENOENT);
}