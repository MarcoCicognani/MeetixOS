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

#include <LibTC/Collection/Map.hh>
#include <LibTC/Cxx.hh>
#include <LibUnitTest/Assertions.hh>
#include <LibUnitTest/Case.hh>

using namespace TC;

TEST_CASE(initializer_list) {
    Map<int, int> map{ { 1, 10 }, { 2, 20 }, { 3, 30 }, { 4, 40 }, { 5, 50 } };

    VERIFY_FALSE(map.is_empty());
    VERIFY_EQUAL(map.count(), 5);
}

TEST_CASE(capacity) {
    class Object {};
    Map<Object, Object> map{ 512 };

    VERIFY_EQUAL(map.count(), 0);
    VERIFY(map.is_empty());
    VERIFY_EQUAL(map.capacity(), 512);
}

TEST_CASE(clear) {
    Map<int, int> map{ { 1, 10 }, { 2, 20 }, { 3, 30 }, { 4, 40 }, { 5, 50 } };

    map.clear();
    VERIFY_EQUAL(map.count(), 0);
    VERIFY(map.is_empty());
    VERIFY_GREATER(map.capacity(), 0);
}

TEST_CASE(insert_and_access) {
    class Object {
    public:
        Object() = default;
        explicit Object(usize value)
            : m_value{ value } {}

        [[nodiscard]] usize value() const { return m_value; }

        bool operator==(Object const&) const = default;

    private:
        usize m_value{ 0 };
    };

    Map<int, Object> map{};

    auto error_or_old_value_10 = map.try_insert(10, Object{ 512 });
    VERIFY(error_or_old_value_10.is_value());
    VERIFY_FALSE(error_or_old_value_10.value().is_present());

    auto error_or_old_value_20 = map.try_insert(20, Object{ 256 });
    VERIFY(error_or_old_value_20.is_value());
    VERIFY_FALSE(error_or_old_value_20.value().is_present());

    auto object_or_none_10 = map[10];
    VERIFY(object_or_none_10.is_present());
    VERIFY_EQUAL(object_or_none_10.value(), Object{ 512 });

    auto error_or_old_value_10_new = map.try_insert(10, Object{ 4096 });
    VERIFY(error_or_old_value_10_new.is_value());
    VERIFY_EQUAL(error_or_old_value_10_new.value().value(), Object{ 512 });

    auto object_or_none_20 = map[20];
    VERIFY(object_or_none_20.is_present());
    VERIFY_EQUAL(object_or_none_20.value(), Object{ 256 });

    auto object_or_none_10_new = map[10];
    VERIFY(object_or_none_10_new.is_present());
    VERIFY_EQUAL(object_or_none_10_new.value(), Object{ 4096 });
}

TEST_CASE(insert_and_access_unordered) {
    class Object {
    public:
        Object() = default;
        explicit Object(usize value)
            : m_value{ value } {}

        [[nodiscard]] usize value() const { return m_value; }

        bool operator==(Object const&) const = default;

    private:
        usize m_value{ 0 };
    };

    Map<int, Object, false> map{};

    auto error_or_old_value_10 = map.try_insert(10, Object{ 512 });
    VERIFY(error_or_old_value_10.is_value());
    VERIFY_FALSE(error_or_old_value_10.value().is_present());

    auto error_or_old_value_20 = map.try_insert(20, Object{ 256 });
    VERIFY(error_or_old_value_20.is_value());
    VERIFY_FALSE(error_or_old_value_20.value().is_present());

    auto object_or_none_10 = map[10];
    VERIFY(object_or_none_10.is_present());
    VERIFY_EQUAL(object_or_none_10.value(), Object{ 512 });

    auto error_or_old_value_10_new = map.try_insert(10, Object{ 4096 });
    VERIFY(error_or_old_value_10_new.is_value());
    VERIFY_EQUAL(error_or_old_value_10_new.value().value(), Object{ 512 });

    auto object_or_none_20 = map[20];
    VERIFY(object_or_none_20.is_present());
    VERIFY_EQUAL(object_or_none_20.value(), Object{ 256 });

    auto object_or_none_10_new = map[10];
    VERIFY(object_or_none_10_new.is_present());
    VERIFY_EQUAL(object_or_none_10_new.value(), Object{ 4096 });
}

TEST_CASE(erase_key) {
    Map<int, int> map{ { 1, 10 }, { 2, 20 }, { 3, 30 }, { 4, 40 }, { 5, 50 } };

    auto error_or_void_2 = map.erase_key(2);
    VERIFY(error_or_void_2.is_value());
    VERIFY_EQUAL(map.count(), 4);

    auto value_or_none_2 = map[2];
    VERIFY_FALSE(value_or_none_2.is_present());

    auto error_or_void_5 = map.erase_key(5);
    VERIFY(error_or_void_5.is_value());
    VERIFY_EQUAL(map.count(), 3);

    auto value_or_none_5 = map[5];
    VERIFY_FALSE(value_or_none_5.is_present());
}

TEST_CASE(erase_value) {
    Map<int, int> map{ { 23, 50 }, { 1, 10 }, { 8, 40 }, { 2, 10 }, { 3, 50 }, { 4, 40 }, { 5, 50 } };

    auto error_or_erased_count_40 = map.erase_value(40);
    VERIFY(error_or_erased_count_40.is_value());
    VERIFY_EQUAL(error_or_erased_count_40.value(), 2);

    auto error_or_erased_count_50 = map.erase_value(50);
    VERIFY(error_or_erased_count_50.is_value());
    VERIFY_EQUAL(error_or_erased_count_50.value(), 3);
}

TEST_CASE(has_key) {
    Map<int, int> map{ { 23, 50 }, { 1, 10 }, { 8, 40 }, { 2, 10 }, { 3, 50 }, { 4, 40 }, { 5, 50 } };

    VERIFY(map.has_key(23));
    VERIFY(map.has_key(8));
    VERIFY(map.has_key(4));
}

TEST_CASE(has_value) {
    Map<int, int> map{ { 23, 50 }, { 1, 10 }, { 8, 40 }, { 2, 10 }, { 3, 50 }, { 4, 40 }, { 5, 50 } };

    VERIFY(map.has_value(10));
    VERIFY(map.has_value(40));
    VERIFY(map.has_value(50));
}

TEST_CASE(iterator) {
    Map<int, int> map{ { 0, 10 }, { 1, 20 }, { 2, 40 }, { 3, 80 }, { 4, 160 }, { 5, 320 }, { 6, 640 } };

    usize loop_counter = 0;
    for ( auto const& pair : map ) {
        VERIFY(pair.value() >= 10);
        loop_counter += 1;
    }

    VERIFY_EQUAL(loop_counter, 7);
}

TEST_CASE(iterator_mut) {
    Map<int, int> map{ { 0, 10 }, { 1, 20 }, { 2, 40 }, { 3, 80 }, { 4, 160 }, { 5, 320 }, { 6, 640 } };

    for ( auto& pair : map ) {
        pair.value() = 512;
    }

    VERIFY_EQUAL(map[0].value(), 512);
    VERIFY_EQUAL(map[1].value(), 512);
    VERIFY_EQUAL(map[2].value(), 512);
    VERIFY_EQUAL(map[3].value(), 512);
    VERIFY_EQUAL(map[4].value(), 512);
    VERIFY_EQUAL(map[5].value(), 512);
    VERIFY_EQUAL(map[6].value(), 512);
}

TEST_CASE(assignment_operator) {
    Map<int, int> map{};
    map = { { 1, 10 }, { 2, 20 }, { 3, 30 }, { 4, 40 }, { 5, 50 } };
    VERIFY_FALSE(map.is_empty());
    VERIFY_EQUAL(map.count(), 5);

    Map<int, int> map2 = map;
    VERIFY_FALSE(map2.is_empty());
    VERIFY_EQUAL(map2.count(), 5);
    VERIFY_FALSE(map.is_empty());
    VERIFY_EQUAL(map.count(), 5);

    Map<int, int> map3 = move(map2);
    VERIFY(map2.is_empty());
    VERIFY_EQUAL(map2.count(), 0);
    VERIFY_FALSE(map3.is_empty());
    VERIFY_EQUAL(map3.count(), 5);

    map3 = { { 0, 1 }, { 2, 3 }, { 4, 5 }, { 6, 7 } };
    VERIFY_EQUAL(map3.count(), 4);
}