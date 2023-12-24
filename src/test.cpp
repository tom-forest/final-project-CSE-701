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
    bigint(int64_t initial_value);

    /**
     * @brief Construct a new bigint object from a string of numbers.
     * 
     * @param initial_value Should only contain numbers.
     */
    bigint(const string& initial_value);

    /**
     * @brief Outputs the number in base 10.
     * 
     * @param os 
     * @param number 
     * @return ostream& 
     */
    friend ostream& operator<<(ostream& os, const bigint& number);

    /**
     * @brief Returns the number in base 10.
     * 
     * @return string 
     */
    string to_string() const;

    /**
     * @brief Copies the r_value bigint to the l_value bigint.
     * 
     * @param int_to_copy 
     */
    void operator=(const bigint& r_value);
    
    /**
     * @brief Assigns the r_value string of digits to the l_value bigint.
     * 
     * @param r_value String of digits.
     */
    void operator=(const string& r_value);

    /**
     * @brief Compares both bigints. Returns 1 if caller is greater, 0 if it is equal, -1 else.
     * 
     * @param second_int 
     * @return int 
     */
    int compare(const bigint& second_int) const;

    /**
     * @brief Compares the numerical values of the caller and second_int.
     * 
     * @param second_int 
     * @return true 
     * @return false 
     */
    bool operator<(const bigint& second_int) const;

    /**
     * @brief Compares the numerical values of the caller and second_int.
     * 
     * @param second_int 
     * @return true 
     * @return false 
     */
    bool operator>(const bigint& second_int) const;

    /**
     * @brief Compares the numerical values of the caller and second_int.
     * 
     * @param second_int 
     * @return true 
     * @return false 
     */
    bool operator==(const bigint& second_int) const;

    /**
     * @brief Compares the numerical values of the caller and second_int.
     * 
     * @param second_int 
     * @return true 
     * @return false 
     */
    bool operator<=(const bigint& second_int) const;

    /**
     * @brief Compares the numerical values of the caller and second_int.
     * 
     * @param second_int 
     * @return true 
     * @return false 
     */
    bool operator>=(const bigint& second_int) const;


private:
    /**
     * @brief Stores the value of bigint represented in base 2^64. Index 0 of the vector is the least significant component.
     * 
     */
    vector<uint64_t> values;

    /**
     * @brief Stores the sign of the bigint. True is positive. Because reasons.
     * 
     */
    bool sign;

    /**
     * @brief Assigns the value represented by a string of digits to the bigint.
     * 
     * @param number String of digits.
     */
    void assign_string(const string& number);
};

/**
 * @brief Constant used for computations by blocs of ~32 bits. Bad practice but computing it everytime would be a waste.
 * 
 */
static uint64_t STEP_32 = (uint64_t) log10(UINT32_MAX) - 1;


/**
 * @brief Power function that uses uint64_t variables internally. Does not check for overflow.
 * 
 * @param base 
 * @param exp 
 * @return uint64_t 
 */
static uint64_t pow_64(uint64_t base, uint64_t exp) {
    uint64_t res = 1;
    for (uint64_t i = 0; i < exp; i++) {
        res *= base;
    }
    return res;
}


/**
 * @brief Removes unnecessary 0s at the beginning of a string of digits representing a number.
 * 
 * @param number 
 */
static void truncate_0s(string& number) {
    uint64_t l = number.length();
    uint64_t i = 0;
    while (number[i] == '0' and i < l) {
        i ++;
    }

    number = number.substr(i, l);
}


/**
 * @brief Adds unnecessary 0s at the beginning of a string of digits representing a number.
 * 
 * @param number String of digits to modify.
 * @param n_0s Number of 0s to add.
 */
static void add_0s(string& number, uint64_t n_0s) {
    for (uint64_t i = 0; i < n_0s; i++) {
        number = "0" + number;
    }
}


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

            remainder *= pow_64(10, i - j);
            remainder += stoull(number.substr(j, i - j));
            string quotient = to_string(remainder / 0x100000000ULL);

            /*  The 0s in the beginning of the number that builtin types discard as useless information
                are needed for concatenation with the beginning of the quotient, so they have to be added
                back.
            */
            add_0s(quotient, i - quotient.length() - j);

            result += quotient;
            remainder %= 0x100000000ULL;

            j = i;
        }
    }

    truncate_0s(result);

    return remainder;
}


/**
 * @brief Divides the provided number by 2^64 and returns the remainder.
 * 
 * @param number A digit string representing the input number.
 * @param result A digit string representing the result. Will be overwritten by the function.
 * @return uint64_t remainder of the euclidean division.
 */
static uint64_t string_euclid_64(const string& number, string& result) {
    string quo32 = "";
    uint64_t mod32 = string_euclid_32(number, quo32);
    return 0x100000000ULL * string_euclid_32(quo32, result) + mod32;
}

/**
 * @brief Retrieves a uint64_t from a string of digits. Returns 0 if pos is out of range.
 * 
 * @param number Source string.
 * @param pos Position of the number in the string.
 * @param len Number of digits to extract.
 * @return uint64_t 
 */
static uint64_t string_to_64(const string& number, uint64_t pos, uint64_t len) {
    if (len == 0) {
        return 0;
    }
    if (pos < number.length()) {
        return stoull(number.substr(pos, len));
    }
    return 0;
}


/**
 * @brief   Avoid using this when possible.
 *          Adjust given index and reach to fit inside given length.
 *          Used for exploring strings of digits from right to left by blocs.
 * 
 * @param i Index.
 * @param b Reach.
 * @param l Length.
 */
static void adjust_index(uint64_t& i, uint64_t& b, uint64_t l) {
    //  Change this from <= to < for a sort of subtle and long to fix bug.
    if (i <= l) {
            b = 2 * STEP_32;
            i = l - i;
        }
        else if (i - l < 2 * STEP_32){
            b = l % (2 * STEP_32);
            i = 0;
        }
        else {
            b = 0, i = 0;
        }
}


/**
 * @brief   Adds together the numbers represented by two strings of digits.
 *          Does not work with negative numbers.
 * 
 * @param number1 
 * @param number2 
 * @param result A string to store the result of the addition.
 */
static void string_add(const string& number1, const string& number2, string& result) {
    result = "";
    string temp;

    uint64_t l1 = number1.length(), l2 = number2.length();
    uint64_t long_length = max(l1, l2);
    uint64_t numbuffer = 0, carry = 0, i = 0, bias = 0, i1, i2, b1, b2;

    /*  Numbers are added together from right to left (of the string) by blocs of a little under 64 bits.
        Bloc size could be closer to 64 bits for optimization but reusing STEP_32 in a simple way felt cleaner.
    */
    while (i < long_length or carry != 0) {
        i += 2 * STEP_32;

        i1 = i, i2 = i;

        adjust_index(i1, b1, l1);
        adjust_index(i2, b2, l2);

        numbuffer = string_to_64(number1, i1, b1);
        numbuffer += string_to_64(number2, i2, b2);
        numbuffer += carry;

        carry = numbuffer / pow_64(10, 2 * STEP_32);
        numbuffer %= pow_64(10, 2 * STEP_32);

        temp = to_string(numbuffer);
        bias = temp.length();

        add_0s(temp, 2 * STEP_32 - bias);

        result = temp + result;
    }

    truncate_0s(result);
}


/**
 * @brief Multiplies the number represented by a string of digits by 2^32.
 * 
 * @param number Number to be multiplied.
 * @param result String to store the result.
 */
static void string_mult_32(const string& number, string& result) {
    result = "";
    uint64_t len = number.length();

    uint64_t buffer_int;
    string buffer_string, buffer_result;

    /*  Multiplication is performed by blocs of just under 32 bits so that
        the result still fits inside a uint64_t. Could have been plain 32 bits for better performance. 
    */
    for (uint64_t i = 0; i < len; i += STEP_32) {
        buffer_int = string_to_64(number, i, STEP_32);
        buffer_int *= 0x100000000ULL;

        buffer_string = "";

        uint64_t number_of_0s = len - i - STEP_32;

        //  Overflow management. May or may not cause bugs for numbers more than 2^64 - 8 digits long.
        if (number_of_0s > len) {
            number_of_0s = 0;
        }

        add_0s(buffer_string, number_of_0s);

        buffer_string = to_string(buffer_int) + buffer_string;
        string_add(buffer_string, result, buffer_result);

        result = buffer_result;
    }
} 


/**
 * @brief   Multiplies the number represented by a string of digits by 2^64.
 *          Using blocs of 32 is necessary since 64 blocs would overflow.
 * 
 * @param number Number to be multiplied.
 * @param result String to store the result.
 */
static void string_mult_64(const string& number, string& result) {
    string buffer_string;
    string_mult_32(number, buffer_string);
    string_mult_32(buffer_string, result);
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


/**
 * @brief   Compare function for uint64_t since I couldn't find the original one.
 *          Returns -1 if v1 is smaller, 0 if equal, 1 else.
 * 
 * @param v1 
 * @param v2 
 * @return int 
 */
static int compare_64(const uint64_t& v1, const uint64_t& v2) {
    if (v1 < v2) {
        return -1;
    }
    if (v1 > v2) {
        return 1;
    }
    return 0;
}


void bigint::assign_string(const string& number) {
    string value_buffer = number;
    string result_buffer = "";

    if (value_buffer.length() > 0 and value_buffer[0] == '-') {
        sign = false;
        value_buffer.erase(0, 1);
    }
    else {
        sign = true;
    }

    //  Checking for incorrect characters in the string.
    for (char c: value_buffer) {
        if (!isdigit(c)) {
            throw invalid_argument("Number string should contain digits only.");
        }
    }

    //  Resetting the values vector prior to filling it again.
    values.clear();

    // Decomposing the number string in base 2^64.
    while (value_buffer != "") {
        values.push_back(string_euclid_64(value_buffer, result_buffer));
        value_buffer = result_buffer;
    }
}


bigint::bigint() : values({0}), sign(true) {}

bigint::bigint(int64_t initial_value) {
    sign = initial_value >= 0;
    uint64_t n_val = sign ? initial_value : -initial_value;
    values.push_back(n_val);
}

bigint::bigint(const string& number_string) {
    assign_string(number_string);
}

string bigint::to_string() const{
    string value_string = "";
    string buffer_result;
    uint64_t val_size = values.size();
    for (uint64_t i = 0; i < values.size(); i++) {
        string_mult_64(value_string, buffer_result);
        value_string = buffer_result;

        string_add(value_string, std::to_string(values[val_size - i - 1]), buffer_result);
        value_string = buffer_result;
    }

    if (!sign) {
        value_string = "-" + value_string;
    }

    return value_string;
}

ostream& operator<<(ostream& os, const bigint& number) {
    os << number.to_string();
    return os;
}

void bigint::operator=(const bigint& r_value) {
    values = r_value.values;
    sign = r_value.sign;
}

void bigint::operator=(const string& r_value) {
    assign_string(r_value);
}

int bigint::compare(const bigint& second_int) const {
    int sign_int = sign ? 1 : -1;

    if (sign != second_int.sign) {
        return sign_int;
    }

    uint64_t values_size = values.size();
    uint64_t second_size = second_int.values.size();
    if (values_size != second_size) {
        return sign_int * (values_size > second_size ? 1 : -1);
    }

    int comparisson = 0;
    for (uint64_t i = 0; i < values_size; i++) {
        comparisson += compare_64(values[values_size - i - 1], second_int.values[values_size - i - 1]);
        if (comparisson != 0) {
            return comparisson * sign_int;
        }
    }
    return 0;
}

bool bigint::operator<(const bigint& second_int) const {
    return compare(second_int) < 0;
}

bool bigint::operator>(const bigint& second_int) const {
    return compare(second_int) > 0;
}

bool bigint::operator==(const bigint& second_int) const {
    return compare(second_int) == 0;
}

bool bigint::operator<=(const bigint& second_int) const {
    return compare(second_int) <= 0;
}

bool bigint::operator>=(const bigint& second_int) const {
    return compare(second_int) >= 0;
}

int main() {
    bigint a("-99999999999999999999999999999999");
    bigint b = a;
    a = "091872390817908712309487";
    cout << a << "\n" << b << "\n";
}