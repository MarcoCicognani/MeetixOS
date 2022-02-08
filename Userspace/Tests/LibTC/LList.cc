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

#include <TC/Collection/List.hh>
#include <TC/Cxx/Move.hh>
#include <UnitTest/Case.hh>
#include <UnitTest/Macros/Verify.hh>
#include <UnitTest/Macros/VerifyEqual.hh>
#include <UnitTest/Macros/VerifyFalse.hh>

using namespace TC;

TEST_CASE(initializer_list) {
    List list{ 1, 2, 3, 4, 5 };

    VERIFY_FALSE(list.is_empty());
    VERIFY_EQUAL(list.count(), 5);
}

TEST_CASE(clear) {
    List list{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

    list.clear();
    VERIFY(list.is_empty());
    VERIFY_EQUAL(list.count(), 0);
}

TEST_CASE(append) {
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

    List<Object> list{};
    VERIFY(list.is_empty());

    list.append(Object{ 1 });
    list.append(Object{ 10 });
    list.append(Object{ 100 });
    list.append(Object{ 1000 });
    list.append(Object{ 10'000 });

    VERIFY_FALSE(list.is_empty());
    VERIFY_EQUAL(list.count(), 5);
}

TEST_CASE(prepend) {
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

    List<Object> list{};
    VERIFY(list.is_empty());

    list.prepend(Object{ 1 });
    list.prepend(Object{ 10 });
    list.prepend(Object{ 100 });
    list.prepend(Object{ 1000 });
    list.prepend(Object{ 10'000 });

    VERIFY_FALSE(list.is_empty());
    VERIFY_EQUAL(list.count(), 5);
}

TEST_CASE(erase) {
    List list{ 9, 5, 4, 6, 3, 2 };

    auto it = list.begin();
    while ( it != list.end() ) {
        if ( *it % 2 == 0 )
            it = it.erase();
        else
            ++it;
    }

    VERIFY_FALSE(list.is_empty());
    VERIFY_EQUAL(list.count(), 3);
}

TEST_CASE(erase_if) {
    class Object {
    public:
        Object() = default;
        explicit Object(usize value, char cc)
            : m_value{ value }
            , m_cc{ cc } {
        }

        [[nodiscard]] usize value() const {
            return m_value;
        }

        [[nodiscard]] char cc() const {
            return m_cc;
        }

    private:
        usize m_value{ 0 };
        char  m_cc{ 'a' };
    };

    List list{ Object{},         Object{ 1, 'b' }, Object{ 2, 'c' }, Object{ 3, 'd' },
               Object{ 3, 'x' }, Object{ 3, 'k' }, Object{ 4, 'e' }, Object{ 5, 'f' } };

    list.erase_if([](auto const& o) { return o.value() == 3 && o.cc() != 'd'; });

    VERIFY_EQUAL(list.count(), 6);
}

TEST_CASE(contains) {
    List list{ 5, 1, 3, 78, 99, 76 };

    VERIFY(list.contains(3));
    VERIFY(list.contains(5));
    VERIFY(list.contains(99));
    VERIFY(list.contains(78));
    VERIFY_FALSE(list.contains(1000));
}

TEST_CASE(find) {
    List list{ 5, 1, 3, 78, 99, 76 };

    auto value_or_none_78 = list.find(78);
    VERIFY(value_or_none_78.is_present());

    auto& value = value_or_none_78.value();
    value       = 1000;

    auto value_or_none_1000 = list.find(1000);
    VERIFY(value_or_none_1000.is_present());
    VERIFY_EQUAL(value_or_none_1000.value(), 1000);
    VERIFY_EQUAL(value_or_none_78.value(), 1000);

    auto value_or_none_4 = list.find(4);
    VERIFY_FALSE(value_or_none_4.is_present());

    List const list2{ 1, 2, 3, 4 };

    auto value_or_none_2 = list2.find(2);
    VERIFY(value_or_none_2.is_present());
}

TEST_CASE(find_if) {
    class Object {
    public:
        Object() = default;
        explicit Object(usize value)
            : m_value{ value } {
        }

        [[nodiscard]] usize value() const {
            return m_value;
        }

        void set_value(usize new_value) {
            m_value = new_value;
        }

        [[nodiscard]] bool operator==(Object const&) const = default;

    private:
        usize m_value{ 0 };
    };

    List list{ Object{ 100 }, Object{ 200 }, Object{ 300 }, Object{ 400 }, Object{ 500 } };

    auto value_or_none = list.find_if([](auto const& o) { return o.value() == 400; });
    VERIFY(value_or_none.is_present());
    VERIFY_EQUAL(value_or_none.value(), Object{ 400 });

    auto& object = value_or_none.value();
    object.set_value(3000);

    auto value_or_none_3000 = list.find_if([](auto const& o) { return o.value() == 3000; });
    VERIFY(value_or_none_3000.is_present());
    VERIFY_EQUAL(value_or_none_3000.value(), Object{ 3000 });
    VERIFY_EQUAL(value_or_none.value(), Object{ 3000 });

    List const list2{ Object{ 100 }, Object{ 200 }, Object{ 500 } };

    auto value_or_none_500 = list2.find_if([](auto const& o) { return o.value() == 500; });
    VERIFY_EQUAL(value_or_none_500.value(), Object{ 500 });
}

TEST_CASE(first_and_last) {
    List list{ 1 };

    VERIFY_EQUAL(list.first(), 1);
    VERIFY_EQUAL(list.last(), 1);

    list.append(2);

    VERIFY_EQUAL(list.first(), 1);
    VERIFY_EQUAL(list.last(), 2);

    list.prepend(0);

    VERIFY_EQUAL(list.first(), 0);
    VERIFY_EQUAL(list.last(), 2);
}

TEST_CASE(const_iterator) {
    List list{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };

    usize expected_value = 1;
    for ( auto const& value : list )
        VERIFY_EQUAL(value, expected_value++);
}

TEST_CASE(mutable_iterator) {
    List list{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

    for ( auto& value : list )
        value += 1;

    int expected_value = 2;
    for ( auto const& value : list )
        VERIFY_EQUAL(value, expected_value++);
}

TEST_CASE(assignment_operator) {
    List list = { 1, 2, 3, 4, 5, 6 };
    VERIFY_FALSE(list.is_empty());
    VERIFY_EQUAL(list.count(), 6);

    usize expected_value = 1;
    for ( auto const& value : list )
        VERIFY_EQUAL(value, expected_value++);

    list = { 7, 8, 9, 10, 11, 12, 13, 14 };
    VERIFY_FALSE(list.is_empty());
    VERIFY_EQUAL(list.count(), 8);
    for ( auto const& value : list )
        VERIFY_EQUAL(value, expected_value++);

    List other{ 64, 65, 66, 67, 68 };
    list = move(other);
    VERIFY(other.is_empty());
    VERIFY_EQUAL(other.count(), 0);
    VERIFY_FALSE(list.is_empty());
    VERIFY_EQUAL(list.count(), 5);

    expected_value = 64;
    for ( auto const& value : list )
        VERIFY_EQUAL(value, expected_value++);
}