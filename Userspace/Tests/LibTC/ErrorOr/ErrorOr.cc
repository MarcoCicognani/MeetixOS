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
#include <UnitTest/Macros/VerifyEq.hh>
#include <UnitTest/Macros/VerifyFalse.hh>

using TC::Functional::ErrorOr;

TEST_CASE(may_produce_value) {
    auto may_produce_value = [](int value) -> ErrorOr<int> {
        if ( value == 100 )
            return 110;
        else
            return EINVAL;
    };

    auto must_be_value_result = may_produce_value(100);
    VERIFY(must_be_value_result.is_value());
    VERIFY_EQ(must_be_value_result.value(), 110);

    auto must_be_error_result = may_produce_value(0);
    VERIFY(must_be_error_result.is_error());
    VERIFY_EQ(must_be_error_result.error(), EINVAL);
}

TEST_CASE(unwrap_reset_value) {
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
    VERIFY_EQ(object.value(), 0xcafebabe);

    auto object_v = error_or_object.unwrap_value();
    VERIFY_EQ(object_v.value(), 0xcafebabe);
    VERIFY_FALSE(error_or_object.is_value());
    VERIFY_FALSE(error_or_object.is_error());
}

TEST_CASE(unwrap_reset_error) {
    ErrorOr<int> error_or_int{ ENOENT };
    VERIFY(error_or_int.is_error());

    auto const& error = error_or_int.error();
    VERIFY_EQ(error, ENOENT);

    auto error_v = error_or_int.unwrap_error();
    VERIFY_EQ(error_v, ENOENT);
    VERIFY_FALSE(error_or_int.is_error());
    VERIFY_FALSE(error_or_int.is_value());
}

TEST_CASE(error_or_with_value_reference) {
    auto  int_ptr = new usize{ 0xdeadbeef };
    auto& int_ref = *int_ptr;

    ErrorOr<usize&> error_or_usize{ int_ref };

    VERIFY(error_or_usize.is_value());

    auto& int_ref_from_error_or = error_or_usize.value();
    VERIFY(int_ref_from_error_or == 0xdeadbeef);

    int_ref_from_error_or = 0xcafebabe;
    VERIFY_EQ(*int_ptr, 0xcafebabe);
    VERIFY_EQ(int_ref, 0xcafebabe);

    auto& int_ref_value = error_or_usize.unwrap_value();
    VERIFY_EQ(int_ref_value, 0xcafebabe);
    VERIFY_FALSE(error_or_usize.is_value());
    VERIFY_FALSE(error_or_usize.is_error());

    delete int_ptr;
}