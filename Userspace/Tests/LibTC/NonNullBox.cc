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

#include <LibTC/Collection/Vector.hh>
#include <LibTC/Cxx.hh>
#include <LibTC/IntTypes.hh>
#include <LibTC/Memory/NonNullBox.hh>
#include <UnitTest/Assertions.hh>
#include <UnitTest/Case.hh>

using namespace TC;

TEST_CASE(make_box) {
    static bool s_destructor_called = false;

    class Object {
    public:
        Object() = default;
        explicit Object(usize value)
            : m_value{ value } {}

        ~Object() { s_destructor_called = true; }

        [[nodiscard]] usize value() const { return m_value; }

    private:
        usize m_value{ 0 };
    };

    {
        auto object_box = make_box<Object>(512);
        VERIFY_EQUAL(object_box->value(), 512);
    }
    VERIFY(s_destructor_called);

    auto int_box = make_box<int>(0xcafebabe);
    VERIFY_EQUAL(int_box.as_ref(), 0xcafebabe);
}

template<usize SIZE>
class Array {
public:
    Array() {
        m_values[0] = 0xcafebabe;
        m_values[1] = 0xdeadbeef;
    }

    [[nodiscard]] u64 const* values() const { return m_values; }

private:
    u64 m_values[SIZE]{ 0 };
};

TEST_CASE(try_make_box) {
    auto error_or_object_box_4096 = try_make_box<Array<0x1000>>();
    VERIFY(error_or_object_box_4096.is_value());

    auto object_box = error_or_object_box_4096.unwrap_value();
    VERIFY_EQUAL(object_box->values()[0], 0xcafebabe);
    VERIFY_EQUAL(object_box->values()[1], 0xdeadbeef);

    auto error_or_object_box_0xfffffff = try_make_box<Array<0xfffffff>>();
    VERIFY(error_or_object_box_0xfffffff.is_error());
    VERIFY_EQUAL(error_or_object_box_0xfffffff.unwrap_error(), ENOMEM);
}

TEST_CASE(swap) {
    auto int_box_256 = make_box<int>(256);
    VERIFY_EQUAL(int_box_256.as_ref(), 256);

    auto int_box_512 = make_box<int>(512);
    VERIFY_EQUAL(int_box_512.as_ref(), 512);

    int_box_256.swap(int_box_512);
    VERIFY_EQUAL(int_box_256.as_ref(), 512);
    VERIFY_EQUAL(int_box_512.as_ref(), 256);
}

TEST_CASE(move) {
    class Object {
    public:
        Object() = default;
        explicit Object(usize value)
            : m_value{ value } {}

        [[nodiscard]] usize value() const { return m_value; }

        static NonNullBox<int> make_int_box() {
            auto int_box = make_box<int>(512);
            VERIFY_EQUAL(int_box.as_ref(), 512);
            return int_box;
        }

    private:
        usize m_value{ 0 };
    };

    auto object_box = make_box<Object>(512);
    VERIFY_EQUAL(object_box->value(), 512);

    NonNullBox object_box_2 = move(object_box);
    VERIFY_EQUAL(object_box_2->value(), 512);

    auto int_box = Object::make_int_box();
    VERIFY_EQUAL(int_box.as_ref(), 512);
}

TEST_CASE(into_vector) {
    Vector<NonNullBox<int>> box_vector{};

    box_vector.append(make_box<int>(256));
    box_vector.append(make_box<int>(512));
    box_vector.append(make_box<int>(1024));

    auto int_box_4096 = make_box<int>(4096);
    box_vector.append(move(int_box_4096));

    VERIFY_EQUAL(box_vector[0].as_ref(), 256);
    VERIFY_EQUAL(box_vector[1].as_ref(), 512);
    VERIFY_EQUAL(box_vector[2].as_ref(), 1024);
    VERIFY_EQUAL(box_vector[3].as_ref(), 4096);
}