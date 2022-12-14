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

template<typename T>
struct RefWrapper : public RefCounted {
    T m_value;

    explicit RefWrapper(T value)
        : m_value(value) {
    }

    auto operator==(T const& rhs) const -> bool {
        return m_value == rhs;
    }
};

TEST_CASE(construct) {
    auto const i32_ref = NonNullRef<RefWrapper<i32>>::from_emplace(64);

    verify_equal$(i32_ref.strong_ref_count(), 1);
    verify_equal$(i32_ref.as_ref(), 64);
}

TEST_CASE(copy) {
    auto const i32_ref   = NonNullRef<RefWrapper<i32>>::from_emplace(64);
    auto const i32_ref_2 = i32_ref.clone();

    verify_equal$(i32_ref.strong_ref_count(), 2);
    verify_equal$(i32_ref_2.strong_ref_count(), 2);

    verify_equal$(i32_ref.as_ref(), 64);
    verify_equal$(i32_ref_2.as_ref(), 64);

    verify_equal$(i32_ref.as_ptr(), i32_ref_2.as_ptr());
}

TEST_CASE(with_object) {
    static bool s_destructor_was_called = false;
    struct Object : public RefCounted {
    public:
        ~Object() {
            s_destructor_was_called = true;
        }
    };

    {
        auto const object_ref = NonNullRef<Object>::from_emplace();
        {
            auto const object_ref_1 = object_ref.clone();
            verify_equal$(object_ref_1.strong_ref_count(), 2);
        }

        verify_false$(s_destructor_was_called);
    }

    verify$(s_destructor_was_called);
}

TEST_CASE(swap) {
    auto i32_ref_v512 = NonNullRef<RefWrapper<i32>>::from_emplace(512);
    verify_equal$(i32_ref_v512.as_ref(), 512);
    verify_equal$(i32_ref_v512.strong_ref_count(), 1);

    auto i32_ref_v768 = NonNullRef<RefWrapper<i32>>::from_emplace(768);
    verify_equal$(i32_ref_v768.as_ref(), 768);
    verify_equal$(i32_ref_v768.strong_ref_count(), 1);

    auto const ref_i32_768_clone = i32_ref_v768.clone();

    i32_ref_v512.swap(i32_ref_v768);

    verify_equal$(i32_ref_v512.as_ref(), 768);
    verify_equal$(i32_ref_v512.strong_ref_count(), 2);
    verify_equal$(i32_ref_v768.as_ref(), 512);
    verify_equal$(i32_ref_v768.strong_ref_count(), 1);
}

TEST_CASE(ref_count_into_vector) {
    auto i32_ref = NonNullRef<RefWrapper<i32>>::from_emplace(64u);

    auto       list_1 = List<NonNullRef<RefWrapper<i32>>>::from_list({ i32_ref.clone() });
    auto       list_2 = List<NonNullRef<RefWrapper<i32>>>::from_list({ i32_ref.clone() });
    auto const list_3 = List<NonNullRef<RefWrapper<i32>>>::from_list({ i32_ref.clone() });
    auto const list_4 = List<NonNullRef<RefWrapper<i32>>>::from_list({ i32_ref.clone() });
    verify_equal$(i32_ref.strong_ref_count(), 5);

    list_1.append(i32_ref.clone());
    verify_equal$(i32_ref.strong_ref_count(), 6);

    list_2.append(Cxx::move(i32_ref));
    verify_equal$(list_2.last().strong_ref_count(), 6);
}