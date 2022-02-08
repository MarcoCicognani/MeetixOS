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

#include <TC/Collection/Vector.hh>
#include <TC/Cxx/Move.hh>
#include <UnitTest/Case.hh>
#include <UnitTest/Macros/Verify.hh>
#include <UnitTest/Macros/VerifyEqual.hh>
#include <UnitTest/Macros/VerifyFalse.hh>
#include <UnitTest/Macros/VerifyGreaterEqual.hh>

using TC::Collection::Vector;
using TC::Cxx::move;

TEST_CASE(initializer_list) {
    Vector vector{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

    VERIFY_FALSE(vector.is_empty());
    VERIFY_EQUAL(vector.count(), 10);
}

TEST_CASE(capacity) {
    class Object {};
    Vector<Object> vector{ 512 };

    VERIFY_EQUAL(vector.count(), 0);
    VERIFY(vector.is_empty());
    VERIFY_EQUAL(vector.capacity(), 512);
}

TEST_CASE(ensure_capacity) {
    Vector<int> vector{};

    auto error_or_void = vector.try_ensure_capacity(4096);
    VERIFY(error_or_void.is_value());
    VERIFY_GREATER_EQUAL(vector.capacity(), 4096);
}

TEST_CASE(resize) {
    Vector<int> vector{};

    auto error_or_void = vector.try_resize(4096);
    VERIFY(error_or_void.is_value());
    VERIFY_GREATER_EQUAL(vector.capacity(), 4096);
    VERIFY_EQUAL(vector.count(), 4096);
}

TEST_CASE(clear) {
    Vector vector{ 1, 2, 3, 4, 5 };

    vector.clear(Vector<int>::KeepStorageCapacity::Yes);
    VERIFY(vector.is_empty());
    VERIFY_GREATER_EQUAL(vector.capacity(), 5);

    vector.clear(Vector<int>::KeepStorageCapacity::No);
    VERIFY(vector.is_empty());
    VERIFY_EQUAL(vector.capacity(), 0);
}

TEST_CASE(emplace_and_access) {
    Vector<int> vector{};

    auto error_or_void_50 = vector.try_emplace_first(50);
    VERIFY(error_or_void_50.is_value());

    VERIFY_EQUAL(vector.first(), 50);
    VERIFY_EQUAL(vector.last(), 50);

    VERIFY_FALSE(vector.is_empty());
    VERIFY_EQUAL(vector.count(), 1);

    auto error_or_void_40 = vector.try_emplace_first(40);
    VERIFY(error_or_void_40.is_value());
    auto error_or_void_30 = vector.try_emplace_first(30);
    VERIFY(error_or_void_30.is_value());
    auto error_or_void_20 = vector.try_emplace_first(20);
    VERIFY(error_or_void_20.is_value());
    auto error_or_void_10 = vector.try_emplace_first(10);
    VERIFY(error_or_void_10.is_value());

    VERIFY_EQUAL(vector.count(), 5);

    VERIFY_EQUAL(vector[0], 10);
    VERIFY_EQUAL(vector[1], 20);
    VERIFY_EQUAL(vector[2], 30);
    VERIFY_EQUAL(vector[3], 40);
    VERIFY_EQUAL(vector[4], 50);

    auto error_or_void_100 = vector.try_emplace_last(100);
    VERIFY(error_or_void_100.is_value());
    auto error_or_void_200 = vector.try_emplace_last(200);
    VERIFY(error_or_void_200.is_value());
    auto error_or_void_300 = vector.try_emplace_last(300);
    VERIFY(error_or_void_300.is_value());
    auto error_or_void_400 = vector.try_emplace_last(400);
    VERIFY(error_or_void_400.is_value());
    auto error_or_void_500 = vector.try_emplace_last(500);
    VERIFY(error_or_void_500.is_value());

    VERIFY_EQUAL(vector.count(), 10);

    VERIFY_EQUAL(vector[5], 100);
    VERIFY_EQUAL(vector[6], 200);
    VERIFY_EQUAL(vector[7], 300);
    VERIFY_EQUAL(vector[8], 400);
    VERIFY_EQUAL(vector[9], 500);
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

    Vector<Object> vector{ 1 };

    auto error_or_void_10 = vector.try_append(Object{ 10 });
    VERIFY(error_or_void_10.is_value());
    auto error_or_void_20 = vector.try_append(Object{ 20 });
    VERIFY(error_or_void_20.is_value());
    auto error_or_void_30 = vector.try_append(Object{ 30 });
    VERIFY(error_or_void_30.is_value());
    auto error_or_void_40 = vector.try_append(Object{ 40 });
    VERIFY(error_or_void_40.is_value());
    auto error_or_void_50 = vector.try_append(Object{ 50 });
    VERIFY(error_or_void_50.is_value());

    VERIFY_FALSE(vector.is_empty());
    VERIFY_EQUAL(vector.count(), 5);
    VERIFY_GREATER_EQUAL(vector.capacity(), 5);
}

TEST_CASE(prepend) {
    Vector vector{ 5, 6, 7, 8, 9 };

    auto error_or_void_4 = vector.try_prepend(4);
    VERIFY(error_or_void_4.is_value());
    auto error_or_void_3 = vector.try_prepend(3);
    VERIFY(error_or_void_3.is_value());
    auto error_or_void_2 = vector.try_prepend(2);
    VERIFY(error_or_void_2.is_value());
    auto error_or_void_1 = vector.try_prepend(1);
    VERIFY(error_or_void_1.is_value());
    auto error_or_void_0 = vector.try_prepend(0);
    VERIFY(error_or_void_0.is_value());

    VERIFY_EQUAL(vector.count(), 10);
    VERIFY_EQUAL(vector.first(), 0);
    VERIFY_EQUAL(vector.last(), 9);
    VERIFY_EQUAL(vector[0], 0);
    VERIFY_EQUAL(vector[1], 1);
    VERIFY_EQUAL(vector[2], 2);
    VERIFY_EQUAL(vector[3], 3);
    VERIFY_EQUAL(vector[4], 4);
    VERIFY_EQUAL(vector[5], 5);
    VERIFY_EQUAL(vector[6], 6);
    VERIFY_EQUAL(vector[7], 7);
    VERIFY_EQUAL(vector[8], 8);
    VERIFY_EQUAL(vector[9], 9);
}

TEST_CASE(insert_at) {
    Vector vector{ 100, 200, 300, 600 };

    auto error_or_void_400 = vector.try_insert_at(3, 400);
    VERIFY(error_or_void_400.is_value());

    auto error_or_void_700 = vector.try_insert_sorted(700);
    VERIFY(error_or_void_700.is_value());

    VERIFY_EQUAL(vector.count(), 6);
    VERIFY_EQUAL(vector[0], 100);
    VERIFY_EQUAL(vector[1], 200);
    VERIFY_EQUAL(vector[2], 300);
    VERIFY_EQUAL(vector[3], 400);
    VERIFY_EQUAL(vector[4], 600);
    VERIFY_EQUAL(vector[5], 700);

    auto error_or_void_500 = vector.try_insert_sorted(500);
    VERIFY(error_or_void_500.is_value());

    VERIFY_EQUAL(vector[4], 500);
}

TEST_CASE(sort) {
    Vector vector{ 9, 4, 6, 12, 44, 102, 0, 45 };

    VERIFY_EQUAL(vector.count(), 8);

    vector.sort([](auto const& a, auto const& b) { return a > b ? 1 : -1; });

    VERIFY_EQUAL(vector[0], 0);
    VERIFY_EQUAL(vector[1], 4);
    VERIFY_EQUAL(vector[2], 6);
    VERIFY_EQUAL(vector[3], 9);
    VERIFY_EQUAL(vector[4], 12);
    VERIFY_EQUAL(vector[5], 44);
    VERIFY_EQUAL(vector[6], 45);
    VERIFY_EQUAL(vector[7], 102);

    vector.sort([](auto const& a, auto const& b) { return a < b ? 1 : -1; });

    VERIFY_EQUAL(vector[0], 102);
    VERIFY_EQUAL(vector[1], 45);
    VERIFY_EQUAL(vector[2], 44);
    VERIFY_EQUAL(vector[3], 12);
    VERIFY_EQUAL(vector[4], 9);
    VERIFY_EQUAL(vector[5], 6);
    VERIFY_EQUAL(vector[6], 4);
    VERIFY_EQUAL(vector[7], 0);
}

TEST_CASE(erase_at) {
    Vector vector{ 150, 250, 350, 450, 550 };

    auto error_or_void_1 = vector.erase_at(1);
    VERIFY(error_or_void_1.is_value());
    VERIFY_EQUAL(vector.count(), 4);
    VERIFY_EQUAL(vector[1], 350);

    auto error_or_void_3 = vector.erase_at(3);
    VERIFY(error_or_void_3.is_value());
    VERIFY_EQUAL(vector.count(), 3);
    VERIFY_EQUAL(vector[2], 450);
}

TEST_CASE(erase_first_of) {
    class Object {
    public:
        Object() = default;
        explicit Object(usize value)
            : m_value{ value } {
        }

        [[nodiscard]] usize value() const {
            return m_value;
        }

        bool operator==(Object const& rhs) const {
            return m_value == rhs.m_value;
        }

    private:
        usize m_value{ 0 };
    };

    Vector vector{ Object{ 10 }, Object{ 20 }, Object{ 30 }, Object{ 40 }, Object{ 50 } };

    auto error_or_void_30 = vector.erase_first_of(Object{ 30 });
    VERIFY(error_or_void_30.is_value());
    VERIFY_EQUAL(vector.count(), 4);
    VERIFY_EQUAL(vector[2], Object{ 40 });

    auto error_or_void_10 = vector.erase_first_of(Object{ 10 });
    VERIFY(error_or_void_10.is_value());
    VERIFY_EQUAL(vector.count(), 3);
    VERIFY_EQUAL(vector.first(), Object{ 20 });
}

TEST_CASE(erase_all_of) {
    Vector vector{ 45, 67, 88, 90, 34, 45, 78, 45, 99 };

    auto error_or_count_45 = vector.erase_all_of(45);
    VERIFY(error_or_count_45.is_value());
    VERIFY_EQUAL(error_or_count_45.value(), 3);
    VERIFY_EQUAL(vector.count(), 6);
    VERIFY_EQUAL(vector.first(), 67);

    auto error_or_count_100 = vector.erase_all_of(100);
    VERIFY(error_or_count_100.is_error());
    VERIFY_EQUAL(error_or_count_100.error(), ENOENT);
}

TEST_CASE(erase_all_matches) {
    class Object {
    public:
        Object() = default;
        explicit Object(usize value)
            : m_value{ value } {
        }

        [[nodiscard]] usize value() const {
            return m_value;
        }

        bool operator==(Object const&) const = default;

    private:
        usize m_value{ 0 };
    };

    Vector vector{ Object{ 10 }, Object{ 20 }, Object{ 30 }, Object{ 40 }, Object{ 57 } };

    auto error_or_count = vector.erase_all_matches([](auto const& o) { return o.value() % 2 == 0; });
    VERIFY(error_or_count.is_value());
    VERIFY_EQUAL(error_or_count.value(), 4);
    VERIFY_EQUAL(vector.first(), Object{ 57 });
    VERIFY_EQUAL(vector.count(), 1);
}

TEST_CASE(const_iterator) {
    Vector vector{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

    int expected_value = 1;
    for ( auto const& value : vector )
        VERIFY_EQUAL(value, expected_value++);
}

TEST_CASE(mutable_iterator) {
    Vector vector{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

    for ( auto& value : vector )
        value += 1;

    int expected_value = 2;
    for ( auto const& value : vector )
        VERIFY_EQUAL(value, expected_value++);
}

TEST_CASE(contains_and_index_of) {
    Vector vector{ 8, 34, 57, 21, 4, 3, 8, 78, 46 };

    VERIFY(vector.contains(21));
    VERIFY(vector.contains(78));
    VERIFY(vector.contains(3));

    auto index_or_none_21 = vector.index_of(21);
    VERIFY(index_or_none_21.is_present());
    VERIFY_EQUAL(index_or_none_21.value(), 3);

    auto index_or_none_78 = vector.index_of(78);
    VERIFY(index_or_none_78.is_present());
    VERIFY_EQUAL(index_or_none_78.value(), 7);

    auto index_or_none_3 = vector.index_of(3);
    VERIFY(index_or_none_3.is_present());
    VERIFY_EQUAL(index_or_none_3.value(), 5);

    auto index_or_none_99 = vector.index_of(99);
    VERIFY_FALSE(index_or_none_99.is_present());
}

TEST_CASE(index_if) {
    Vector vector{ 8, 34, 57, 21, 4, 3, 8, 78, 46 };

    auto index_or_none_78 = vector.index_if([](auto const& value) { return value == 78; });
    VERIFY(index_or_none_78.is_present());
    VERIFY_EQUAL(index_or_none_78.value(), 7);

    auto index_or_none_first_not_even = vector.index_if([](auto const& value) { return value % 2 != 0; });
    VERIFY(index_or_none_first_not_even.is_present());
    VERIFY_EQUAL(index_or_none_first_not_even.value(), 2);
}

TEST_CASE(find) {
    Vector vector{ 0, 4, 6, 2, 3, 8, 5, 1 };

    auto value_or_none = vector.find(6);
    VERIFY(value_or_none.is_present());

    auto& value_ref = value_or_none.unwrap();
    value_ref       = 512;

    auto updated_value_or_none = vector.find(512);
    VERIFY(updated_value_or_none.is_present());
    VERIFY_EQUAL(updated_value_or_none.value(), 512);

    Vector const vector_const{ 0, 4, 6, 2, 3, 8, 5, 1 };

    auto const_value_or_none = vector_const.find(8);
    VERIFY(const_value_or_none.is_present());

    auto const_value_or_none_200 = vector_const.find(200);
    VERIFY_FALSE(const_value_or_none_200.is_present());
}

TEST_CASE(find_if) {
    class Object {
    public:
        Object() = default;
        Object(usize first, char second)
            : m_first{ first }
            , m_second{ second } {
        }

        [[nodiscard]] usize first() const {
            return m_first;
        }
        [[nodiscard]] char second() const {
            return m_second;
        }

    private:
        usize m_first{ 0 };
        char  m_second{ 'c' };
    };

    Vector vector{ Object{ 4, 'a' }, Object{ 6, 'b' }, Object{ 8, 'c' }, Object{ 12, 'e' } };

    auto object_or_none = vector.find_if([](auto const& o) { return o.first() < 10 && o.second() == 'b'; });
    VERIFY(object_or_none.is_present());

    Vector const vector_const{ 0, 4, 6, 2, 3, 8, 5, 1 };

    auto const_value_or_none = vector_const.find_if([](auto const& i) { return i == 3; });
    VERIFY(const_value_or_none.is_present());
}

TEST_CASE(assignment_operator) {
    Vector vector{ 1, 2, 3, 4, 5 };

    vector = { 6, 7, 8, 9, 10 };
    VERIFY_EQUAL(vector.count(), 5);

    usize expected_value = 6;
    for ( auto const& value : vector )
        VERIFY_EQUAL(value, expected_value++);

    Vector<int> vector2{};
    vector2        = vector;
    expected_value = 6;
    for ( auto const& value : vector2 )
        VERIFY_EQUAL(value, expected_value++);

    Vector vector3{ 8, 5, 2 };
    VERIFY_EQUAL(vector3.count(), 3);
    VERIFY_EQUAL(vector3[0], 8);
    VERIFY_EQUAL(vector3[1], 5);
    VERIFY_EQUAL(vector3[2], 2);

    vector3 = move(vector);
    VERIFY_EQUAL(vector3.count(), 5);

    expected_value = 6;
    for ( auto const& value : vector3 )
        VERIFY_EQUAL(value, expected_value++);
}

BENCHMARK_CASE(one_hundred_thousand_append_with_reallocations) {
    class Object {
    public:
        Object() = default;
        explicit Object(u64 value)
            : m_value{ value } {
        }

        [[nodiscard]] u64 value() const {
            return m_value;
        }

        bool operator==(Object const&) const = default;

    private:
        u64 m_value{ 0 };
    };

    Vector<Object> vector{ 1 };

    for ( usize i = 0; i < 100'000; ++i )
        VERIFY(vector.try_append(Object{ i * 34 }).is_value());

    VERIFY_EQUAL(vector.count(), 100'000);
    VERIFY(vector.capacity() >= 100'000);
}

BENCHMARK_CASE(one_hundred_thousand_append_without_reallocations) {
    class Object {
    public:
        Object() = default;
        explicit Object(u64 value)
            : m_value{ value } {
        }

        [[nodiscard]] u64 value() const {
            return m_value;
        }

        bool operator==(Object const&) const = default;

    private:
        u64 m_value{ 0 };
    };

    Vector<Object> vector{ 100'000 };

    for ( usize i = 0; i < 100'000; ++i )
        VERIFY(vector.try_append(Object{ i * 34 }).is_value());

    VERIFY_EQUAL(vector.count(), 100'000);
    VERIFY_EQUAL(vector.capacity(), 100'000);
}

BENCHMARK_CASE(one_hundred_thousand_append_unchecked_without_reallocations) {
    class Object {
    public:
        Object() = default;
        explicit Object(u64 value)
            : m_value{ value } {
        }

        [[nodiscard]] u64 value() const {
            return m_value;
        }

        bool operator==(Object const&) const = default;

    private:
        u64 m_value{ 0 };
    };

    Vector<Object> vector{ 100'000 };

    for ( usize i = 0; i < 100'000; ++i )
        vector.append_unchecked(Object{ i * 34 });

    VERIFY_EQUAL(vector.count(), 100'000);
    VERIFY_EQUAL(vector.capacity(), 100'000);
}