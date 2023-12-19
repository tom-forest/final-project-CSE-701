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
     * @brief Stores the value of bigint represented in base 2^64. Index 0 of the vector is the least significant component.
     * 
     */
    vector<uint64_t> values;

};

bigint::bigint() : values({0}) {}

bigint::bigint(uint64_t initial_value) : values({initial_value}) {}

/**
 * @brief Divides the provided number by 2^32 and returns the remainder.
 * 
 * @param number A digit string representing the number. Will be modified.
 * @return uint64_t Remainder.
 */
static uint64_t string_euclid_32(string& number) {
    size_t len = number.size();
    uint64_t remainder = 0;
    for (int i = 0; i < len; i++) {

    }
}

bigint::bigint(string initial_values) {
    
}

#endif