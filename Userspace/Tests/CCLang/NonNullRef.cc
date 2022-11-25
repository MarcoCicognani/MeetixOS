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

#include <CCLang/Alloc/List.hh>
#include <CCLang/Alloc/NonNullRef.hh>
#include <CCLang/Lang/Cxx.hh>
#include <LibUnitTest/Assertions.hh>
#include <LibUnitTest/Case.hh>

TEST_CASE(construct) {
    auto const ref_i32 = NonNullRef<i32>::from_emplace(64);

    verify_equal$(ref_i32.strong_ref_count(), 1);
    verify_equal$(ref_i32.as_ref(), 64u);
}

TEST_CASE(copy) {
    auto const ref_i32   = NonNullRef<i32>::from_emplace(64);
    auto const ref_i32_2 = ref_i32.clone();

    verify_equal$(ref_i32.strong_ref_count(), 2);
    verify_equal$(ref_i32_2.strong_ref_count(), 2);

    verify_equal$(ref_i32.as_ref(), 64u);
    verify_equal$(ref_i32_2.as_ref(), 64u);

    verify_equal$(ref_i32.as_ptr(), ref_i32_2.as_ptr());
}

TEST_CASE(with_object) {
    static bool s_destructor_was_called = false;

    struct Object {
    public:
        ~Object() {
            s_destructor_was_called = true;
        }
    };

    {
        auto const ref_object_0 = NonNullRef<Object>::from_emplace();
        {
            auto const ref_object_1 = ref_object_0.clone();
            verify_equal$(ref_object_1.strong_ref_count(), 2);
        }

        verify_false$(s_destructor_was_called);
    }

    verify$(s_destructor_was_called);
}

TEST_CASE(swap) {
    auto ref_i32_512 = NonNullRef<i32>::from_emplace(512);
    verify_equal$(ref_i32_512.as_ref(), 512u);
    verify_equal$(ref_i32_512.strong_ref_count(), 1);

    auto ref_i32_768 = NonNullRef<i32>::from_emplace(768);
    verify_equal$(ref_i32_768.as_ref(), 768u);
    verify_equal$(ref_i32_768.strong_ref_count(), 1);

    auto const ref_i32_768_clone = ref_i32_768.clone();

    ref_i32_512.swap(ref_i32_768);

    verify_equal$(ref_i32_512.as_ref(), 768);
    verify_equal$(ref_i32_512.strong_ref_count(), 2);
    verify_equal$(ref_i32_768.as_ref(), 512);
    verify_equal$(ref_i32_768.strong_ref_count(), 1);
}

static i32 s_destructor_called_count = 0;

struct MovableOnlyObject : public DenyCopy {
public:
    usize m_value{ 0 };

    explicit MovableOnlyObject(usize value)
        : m_value{ value } {
    }

    MovableOnlyObject(MovableOnlyObject&& rhs)
        : m_value{ rhs.m_value } {
    }
    auto operator=(MovableOnlyObject&& rhs) -> MovableOnlyObject& {
        m_value = rhs.m_value;
        return *this;
    }

    ~MovableOnlyObject() {
        ++s_destructor_called_count;
    }
};

TEST_CASE(ref_count_into_vector) {
    {
        auto ref_object = NonNullRef<MovableOnlyObject>::from_emplace(64u);

        auto       list_1 = List<NonNullRef<MovableOnlyObject>>::from_list({ ref_object.clone() });
        auto       list_2 = List<NonNullRef<MovableOnlyObject>>::from_list({ ref_object.clone() });
        auto const list_3 = List<NonNullRef<MovableOnlyObject>>::from_list({ ref_object.clone() });
        auto const list_4 = List<NonNullRef<MovableOnlyObject>>::from_list({ ref_object.clone() });
        verify_equal$(ref_object.strong_ref_count(), 5);

        list_1.append(ref_object.clone());
        verify_equal$(ref_object.strong_ref_count(), 6);

        list_2.append(Cxx::move(ref_object));
        verify_equal$(list_2.last().strong_ref_count(), 6);
    }

    verify_equal$(s_destructor_called_count, 1);
}