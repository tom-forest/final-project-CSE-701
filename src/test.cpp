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
    bigint(const string& initial_value);

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
static uint64_t STEP_32 = (uint64_t) log10(UINT32_MAX) - 1;

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

    result = "";
    if (number == "") {
        return 0;
    }

    size_t len = number.size();
    uint64_t remainder = 0;
    uint64_t j = 0;

    for (uint64_t i = 1; i <= len; i++) {

        /*  The divisions are made by "blocs" of just under 32 bits.
            This guarantees that a possible large remainder of a previous division concatenated to the new
            dividend will not exceed 64 bits.
            Better efficiency could be achieved using the special 128 or even 512 bits instructions but old computers
            can't always do that.
        */ 
        if ((i - j > STEP_32) or (i == len)) {

            remainder *= (uint64_t) pow(10, i - j);
            remainder += stoull(number.substr(j, i - j));
            string quotient = to_string(remainder / 0x100000000ULL);

            /*  The 0s in the beginning of the number that builtin types discard as useless information
                are needed for concatenation with the beginning of the quotient, so they have to be added
                back.
            */
            for (uint64_t k = j; k < i - quotient.length(); k++) {
                quotient = "0" + quotient;
            }

            result += quotient;
            remainder %= 0x100000000ULL;

            j = i;
        }
    }

    uint64_t i = 0, result_size = result.length();

    while (i < result_size and result[i] == '0') {
        i += 1;
    }

    result = result.substr(i, result_size - i);

    return remainder;
}

/**
 * @brief Divides the provided number by 2^64 and returns the remainder.
 * 
 * @param number A digit string representing the input number.
 * @param result A digit string representing the result. Will be overwritten by the function.
 * @return uint64_t Remainder.
 */
static uint64_t string_euclid_64(const string& number, string& result) {
    string quo32 = "";
    uint64_t mod32 = string_euclid_32(number, quo32);
    return 0x100000000ULL * string_euclid_32(quo32, result) + mod32;
}

/**
 * @brief Checks wether a character is a digit or not.
 * 
 * @param c character to test
 */
static bool isdigit(const char& c) {
    int ascii_code = (int) c;
    return ascii_code < 58 and ascii_code > 47;
}


bigint::bigint(const string& number_string) {
    string value_buffer = number_string;
    string result_buffer = "";

    //  Checking for incorrect characters in the string.
    for (char c: value_buffer) {
        if (!isdigit(c)) {
            throw invalid_argument("Number string should contain digits only.");
        }
    }

    // Decomposing the number string in base 2^64.
    while (value_buffer != "") {
        values.push_back(string_euclid_64(value_buffer, result_buffer));
        value_buffer = result_buffer;
    }
}

int main() {
    cout << (int) '0';
    cout << "STEP32: " << STEP_32 << "\n";
    string res = "";
    uint64_t mod = string_euclid_64("18446744073709551615", res);
    cout << res << " | " << mod << "\n";
}