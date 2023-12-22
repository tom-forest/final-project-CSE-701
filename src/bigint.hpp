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

/**
 * @brief Constant used for division by 2^32. Bad practice but computing it everytime would be a waste.
 * 
 */
static uint64_t STEP_32 = (uint64_t) log(UINT32_MAX) - 1;

bigint::bigint() : values({0}) {}

bigint::bigint(uint64_t initial_value) : values({initial_value}) {}

/**
 * @brief Divides the provided number by 2^32 and returns the remainder.
 * 
 * @param number A digit string representing the input number.
 * @param result A digit string representing the result. Will be overwritten by the function.
 * @return uint64_t Remainder.
 */
static uint64_t string_euclid_32(const string& number, string& result) {
    size_t len = number.size();
    uint64_t remainder = 0;
    result = "";
    uint64_t j = 0;

    for (uint64_t i = 1; i <= len; i++) {
        if ((i - j > STEP_32) or (i == len)) {
            remainder *= (uint64_t) pow(10, i - j);
            remainder += stoull(number.substr(j, i - j));
            string quotient = to_string(remainder / 0x100000000ULL);

            for (uint64_t k = j; k < i; k++) {
                quotient = "0" + quotient;
            }

            result += quotient;
            remainder %= 0x100000000ULL;

            j = i;
        }
    }

    return remainder;
}

bigint::bigint(string initial_values) {
    
}

int main() {
    string res = "";
    uint64_t mod = string_euclid_32("18446744073709551616", res);
    cout << res << " | " << mod << "\n";
}

#endif