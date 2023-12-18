#ifndef BIGINT
#define BIGINT

#include <vector>
#include <iostream>
#include <stdexcept>
#include <cstdint>
#include <cmath>

using namespace std;

/**
 * @brief Class for storing arbitrary sized unsigned integers.
 * 
 */
class bigint {

public:
    /**
     * @brief Construct a new bigint object of value 0.
     * 
     */
    bigint();

    /**
     * @brief Construct a new bigint object from a uint64_t.
     * 
     * @param initial_value 
     */
    bigint(uint64_t initial_value);

    /**
     * @brief Construct a new bigint object from a string of numbers.
     * 
     * @param initial_value Should only contain numbers.
     */
    bigint(string initial_value);

private:
    /**
     * @brief Stores the value of the bigint object by slices of 18 digits. Inneficient but easy to implement.
     * 
     */
    vector<uint64_t> values;

    int max_digits = log10(UINT64_MAX) - 1;

};

bigint::bigint() : values({0}) {}

bigint::bigint(uint64_t initial_value) : values({initial_value}) {}

bigint::bigint(string initial_values) {
    
}

#endif