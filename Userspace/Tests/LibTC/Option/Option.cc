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

#include <TC/Functional/Option.hh>
#include <UnitTest/Case.hh>
#include <UnitTest/Macros/VerifyFalse.hh>

TEST_CASE(may_produce_value) {
    auto may_produce_value = [](int value) -> TC::Functional::Option<int> {
        if ( value <= 10 )
            return value * 10;
        else
            return {};
    };

    auto must_be_none_option = may_produce_value(20);
    VERIFY_FALSE(must_be_none_option.is_present());

    auto must_be_value_option = may_produce_value(5);
    VERIFY(must_be_value_option.is_present());
}