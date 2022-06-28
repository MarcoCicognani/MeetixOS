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

#include <LibTC/Functional/Option.hh>
#include <LibUnitTest/Assertions.hh>
#include <LibUnitTest/Case.hh>

using namespace TC;

Option<int> return_value_2() {
    return 100;
}

Option<int> return_value() {
    auto v = TRY(return_value_2());

    return v + 100;
}

void value_a() {
    auto v = return_value();
    if ( v.is_present() ) {
        auto i = v.unwrap();
        i      = 100;
    }
}

TEST_CASE(may_produce_value) {
    auto may_produce_value = [](int value) -> Option<int> {
        if ( value <= 10 )
            return value * 10;
        else
            return {};
    };

    value_a();

    auto must_be_none_option = may_produce_value(20);
    VERIFY_FALSE(must_be_none_option.is_present());

    auto must_be_value_option = may_produce_value(5);
    VERIFY(must_be_value_option.is_present());
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

    Option<Object> option{ Object{ 0xdeadbeef } };
    VERIFY(option.is_present());

    auto const& object = option.value();
    VERIFY_EQUAL(object.value(), 0xdeadbeef);

    auto object_v = option.unwrap();
    VERIFY_EQUAL(object_v.value(), 0xdeadbeef);
    VERIFY_FALSE(option.is_present());

    VERIFY_EQUAL(option.unwrap_or(Object{ 0xcafebabe }).value(), 0xcafebabe);
}

TEST_CASE(option_with_reference) {
    auto* int_ptr = new usize{ 0xdeadbeef };
    auto& int_ref = *int_ptr;

    Option<usize&> option{ int_ref };

    VERIFY(option.is_present());

    auto& int_ref_from_option = option.value();
    VERIFY_EQUAL(int_ref_from_option, 0xdeadbeef);

    int_ref_from_option = 0xcafebabe;
    VERIFY_EQUAL(*int_ptr, 0xcafebabe);
    VERIFY_EQUAL(int_ref, 0xcafebabe);

    auto& int_ref_value = option.unwrap();
    VERIFY_EQUAL(int_ref_value, 0xcafebabe);
    VERIFY_FALSE(option.is_present());

    delete int_ptr;
}

TEST_CASE(assignment_operator) {
    Option<char> option{ 'a' };
    VERIFY(option.is_present());
    VERIFY_EQUAL(option.value(), 'a');

    option = 'b';
    VERIFY(option.is_present());
    VERIFY_EQUAL(option.value(), 'b');

    option = nullptr;
    VERIFY_FALSE(option.is_present());

    auto* int_ptr = new usize{ 0xcafebabe };
    auto& int_ref = *int_ptr;

    Option<usize&> option2 = int_ref;
    VERIFY(option2.is_present());
    VERIFY_EQUAL(option2.value(), 0xcafebabe);

    delete int_ptr;
}