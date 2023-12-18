#ifndef BIGINT
#define BIGINT

#include <vector>
#include <iostream>
#include <stdexcept>
#include <cstdint>

using namespace std;

class bigint {

public:
    bigint();

    bigint(int64_t initial_value);

    bigint(string initial_value);

private:
    vector<int64_t> values;

};

bigint::bigint() : values({0}) {}

bigint::bigint(int64_t initial_value) : values({initial_value}) {}

bigint::bigint(string initial_values) {
    
}

#endif