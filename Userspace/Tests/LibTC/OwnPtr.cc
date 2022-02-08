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

#include <TC/Cxx/Move.hh>
#include <TC/IntTypes.hh>
#include <TC/Memory/OwnPtr.hh>
#include <UnitTest/Case.hh>
#include <UnitTest/Macros/Verify.hh>

using namespace TC;

TEST_CASE(is_null_on_null_ptr) {
    OwnPtr<char> own_ptr_default{};
    VERIFY(own_ptr_default.is_null());

    OwnPtr<char> own_ptr_nullptr{ nullptr };
    VERIFY(own_ptr_nullptr.is_null());

    OwnPtr<char> own_ptr_eq = nullptr;
    VERIFY(own_ptr_eq.is_null());

    OwnPtr<char> own_ptr_moved = move(own_ptr_default);
    VERIFY(own_ptr_moved.is_null());
    VERIFY(own_ptr_default.is_null());
}

TEST_CASE(make_own_ptr) {
    static bool s_destructor_called = false;

    class Object {
    public:
        Object() = default;
        explicit Object(usize value)
            : m_value{ value } {
        }

        ~Object() {
            s_destructor_called = true;
        }

        [[nodiscard]] usize value() const {
            return m_value;
        }

    private:
        usize m_value{ 0 };
    };

    auto own_ptr = make_own_ptr<Object>(512);
    VERIFY_FALSE(own_ptr.is_null());
    VERIFY_EQUAL(own_ptr->value(), 512);

    own_ptr.clear();
    VERIFY(own_ptr.is_null());
    VERIFY(s_destructor_called);

    s_destructor_called = false;

    {
        auto own_ptr_2 = make_own_ptr<Object>(1024);
        VERIFY_FALSE(own_ptr_2.is_null());
        VERIFY_EQUAL(own_ptr_2->value(), 1024);
    }

    VERIFY(s_destructor_called);
}

template<usize SIZE>
class Object {
public:
    Object() = default;

    [[nodiscard]] u64 const* values() const {
        return m_values;
    }

private:
    u64 m_values[SIZE]{ 0 };
};

TEST_CASE(try_make_own_ptr) {
    auto error_or_own_ptr = try_make_own_ptr<Object<0xfffffff>>();
    VERIFY(error_or_own_ptr.is_error());

    auto error_or_own_ptr_2 = try_make_own_ptr<Object<0x1000>>();
    VERIFY(error_or_own_ptr_2.is_value());

    auto own_ptr = error_or_own_ptr_2.unwrap_value();
    VERIFY_EQUAL(own_ptr->values()[0], 0);
}

TEST_CASE(move) {
    auto own_ptr_original = make_own_ptr<int>(0xcafebabe);
    VERIFY_FALSE(own_ptr_original.is_null());
    VERIFY_EQUAL(*own_ptr_original, 0xcafebabe);

    OwnPtr own_ptr_moved{ move(own_ptr_original) };
    VERIFY(own_ptr_original.is_null());
    VERIFY_FALSE(own_ptr_moved.is_null());
    VERIFY_EQUAL(*own_ptr_moved, 0xcafebabe);

    OwnPtr<int> own_ptr_moved_assign{};
    VERIFY(own_ptr_moved_assign.is_null());

    own_ptr_moved_assign = move(own_ptr_moved);
    VERIFY(own_ptr_moved.is_null());
    VERIFY_FALSE(own_ptr_moved_assign.is_null());
    VERIFY_EQUAL(*own_ptr_moved_assign, 0xcafebabe);
}