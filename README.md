# Bigint module

This module implements basic operations on arbitrary size signed integers.
Implemented operators are:

* all value comparisons (<, >, <=, >=, ==, !=)
* all addition operators (+=, -=, +, -, unary -)
* product operators (*=, *)
* stream operator <<
* assignment operator =

bigint also comes with two handy methods:

* string bigint::to_string(), to convert any bigint to a string of digits corresponding to its decimal writing.
* int8_t bigint::compare(const bigint&) used by all comparison operators. Useful to define comparison operators for classes that use bigint (arbitrary precision floats someone ?)

and 4 constructors:

* a default constructor that initializes a bigint to 0
* a copy constructor
* an int64_t constructor
* a string constructor

### Implementation

The numerical value of bigint instances is stored in base 2^64 as a vector of 64 bits unsigned integers. The sign is stored separately as an 8 bit signed integer.

### Technical details

* Products are performed by blocs of 32 bits, using 64 bits unsigned integers.
* Products always create a bigint to store the result. Therefore, *= is implemented using * and not the other way around. This makes * more efficient than *= for bigints, which is counter-intuitive and a problem with this implementation.
* Additions are performed by blocs of 64 bits.
* Conversion between string and bigint is not very efficient and to be avoided when possible. It makes use of makeshift string euclidean division and multiplication, which works by blocs of 8-16 decimal digits. See STEP_32 static constant and its uses for more details.

### Future improvements

A better handling of string to bigint conversions has to be implemented, as well as a cleaner bigint addition function. Optimizations to a few static functions could also be made.