#include <Api/StdInt.h>
#include <assert.h>
#include <stdio.h>
#include <TC/Prelude.hh>

class Object {
public:
    Object() = default;

    explicit Object(usize value)
        : m_value{ value } {
        printf("Object{ %lu } Conversion Constructor\n", m_value);
        fflush(stdout);
    }

    Object(Object const& rhs)
        : m_value{ rhs.m_value } {
        printf("Object{ %lu } Copy Constructor\n", m_value);
        fflush(stdout);
    }

    //    Object(Object&& rhs)
    //        : m_value{ std::exchange(rhs.m_value, 0) } {
    //        printf("Object{ %lu } Move Constructor\n", m_value);
    //        fflush(stdout);
    //    }

    ~Object() {
        printf("Object{ %lu } Destructor\n", m_value);
        fflush(stdout);

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

using TC::Collections::Vector;

int main(int argc, char** argv) {
    Vector<Object> vector{};

    printf("Em-placing Object{ %lu }\n", 10UL);
    vector.emplace(10);

    printf("Em-placing Object{ %lu }\n", 30UL);
    vector.emplace(30);

    printf("Em-placing back Object{ %lu }\n", 40UL);
    vector.emplace_back(40);

    printf("Em-placing back Object{ %lu }\n", 50UL);
    vector.emplace_back(50);

    assert(vector.count() == 4);

    printf("\n");
    for ( auto const& o : vector ) {
        printf("Object{ %lu }\n", o.m_value);
    }
    fflush(stdout);

    assert(vector[0] == 30);
    assert(vector[1] == 10);

    assert(vector[2] == 40);
    assert(vector[3] == 50);

    vector.sort([](auto const& a, auto const& b) { return !(a < b); });

    printf("\n");
    for ( auto const& o : vector ) {
        printf("Object{ %lu }\n", o.m_value);
    }
    fflush(stdout);

    printf("Pushing Object{ %lu }\n", 70UL);
    vector.push(Object{ 70 });

    printf("Pushing Object{ %lu }\n", 80UL);
    vector.push(Object{ 80 });

    printf("Pushing back Object{ %lu }\n", 90UL);
    vector.push_back(Object{ 90 });

    printf("Pushing back Object{ %lu }\n", 95UL);
    vector.push_back(Object{ 95 });

    vector.insert(5, Object{ 1000 });

    printf("\n");
    for ( auto const& o : vector ) {
        printf("Object{ %lu }\n", o.m_value);
    }
    fflush(stdout);

    return EXIT_SUCCESS;
}