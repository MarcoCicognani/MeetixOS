// clang-format off
#include <TC/Prelude.hh>
// clang-format on

#include <errno.h>
#include <stdio.h>

using TC::Collection::Vector;
using TC::Functional::ErrorOr;
using TC::Functional::Option;
using TC::Functional::Result;

class Object {
public:
    Object() = default;

    explicit Object(usize value)
        : m_value{ value } {
    }

    Object(Object const& rhs) = default;

    ~Object() {
        m_value = 0;
    }

    bool operator==(usize value) const {
        return m_value == value;
    }

    bool operator<(Object const& rhs) const {
        return m_value < rhs.m_value;
    }

    usize m_value{ 0 };
};

Option<int> may_produce_value(int value) {
    if ( value < 10 )
        return 10;
    else
        return {};
}

ErrorOr<Object> perform_computation() {
    if ( may_produce_value(9).is_present() )
        return Object{ 10 };
    else
        return ENOENT;
}

int do_computation() {
    auto object = TRY(perform_computation());
    printf("A = %lu\n", object.m_value);
    return 0;
}

int main(int argc, char** argv) {
    auto empty_option = may_produce_value(100);
    VERIFY(!empty_option.is_present());

    auto filled_option = may_produce_value(5);
    VERIFY(filled_option.is_present());

    printf("filled_option.unwrap() = %d\n", filled_option.unwrap());

    do_computation();

    Vector vector{ Object{ 1 }, Object{ 2 }, Object{ 3 }, Object{ 4 }, Object{ 5 }, Object{ 6 }, Object{ 7 } };

    VERIFY(vector.count() == 7);

    vector.emplace_first(10);
    vector.emplace_first(30);
    vector.emplace_last(40);
    vector.emplace_last(50);

    VERIFY(vector.count() == 11);

    printf("\n");
    for ( auto const& o : vector ) {
        printf("Object{ %lu }\n", o.m_value);
    }
    fflush(stdout);

    VERIFY(vector[0] == 30);
    VERIFY(vector[1] == 10);

    VERIFY(vector[2] == 1);
    VERIFY(vector[3] == 2);

    VERIFY(vector.first() == 30);
    VERIFY(vector.last() == 50);

    vector.sort([](auto const& a, auto const& b) { return !(a < b); });

    printf("\n");
    for ( auto const& o : vector ) {
        printf("Object{ %lu }\n", o.m_value);
    }
    fflush(stdout);

    vector.prepend(Object{ 70 });
    vector.prepend(Object{ 80 });
    vector.append(Object{ 90 });
    vector.append(Object{ 95 });

    vector.insert_at(5, Object{ 1000 });

    MUST(vector.try_append(Object{ 300 }));

    printf("\n");
    for ( auto const& o : vector ) {
        printf("Object{ %lu }\n", o.m_value);
    }
    fflush(stdout);

    Vector vector2{ 1, 2,   3,   4,  5,   6,   7,  8, 8,  9,  90,  0,  23, 4, 45, 657, 87, 98,
                    9, 345, 234, 23, 123, 345, 56, 7, 87, 98, 435, 23, 5,  7, 4,  88,  11 };
    vector2.sort([](auto const& a, auto const& b) { return a > b; });
    for ( auto const& value : vector2 ) {
        printf("%d ", value);
    }
    printf("\n");
    fflush(stdout);

    return 0;
}