#include <stdio.h>
#include <TC/Prelude.hh>

int main(int argc, char** argv) {
    TC::Collections::Vector vector{ '1', '2', '3', '4', '5' };

    vector.push_back('a');
    vector.push_back('b');
    vector.push_back('c');
    vector.push_back('d');
    vector.push_back('e');

    for ( auto const& value : vector ) {
        printf("value = %c", value);
    }

    return 0;
}