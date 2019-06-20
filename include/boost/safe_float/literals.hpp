#ifndef BOOST_SAFE_FLOAT_LITERALS_HPP
#define BOOST_SAFE_FLOAT_LITERALS_HPP

#include <boost/safe_float.hpp>


namespace boost
{
namespace safe_float
{
namespace literals
{
namespace detail
{
#define HEX_PARSE_ERROR "Not a valid hex floating point number literal"
#define DEC_PARSE_ERROR "Not a valid dec floating point number literal"
#define VALUE_ERROR "Floating point number isn't a positive power of 0.5"

// A struct to be used in static_assert
template<typename T>
struct always_false
{
    static constexpr bool value = false;
};

// A constexpr version of isxdigit working on ASCII
constexpr bool isxdigit(char c)
{
    return ((c >= '0' and c <= '9') or (c >= 'a' and c <= 'f') or (c >= 'A' and c <= 'F'));
}

// A constexpr version of isdigit working on ASCII
constexpr bool isdigit(char c)
{
    return (c >= '0' and c <= '9');
}

// constexpr function to get the represented value of a hex digit
constexpr int hexvalue(char xdigit)
{
    return (xdigit >= '0' and xdigit <= '9') ?
               xdigit - '0' :
               (xdigit >= 'a' and xdigit <= 'f') ? xdigit - 'a' + 10 :
                                                   (xdigit >= 'A' and xdigit <= 'F') ? xdigit - 'A' + 10 : 0;
}

// constexpr function to get the represented value of a digit
constexpr int decvalue(char digit)
{
    return (digit >= '0' and digit <= '9') ? digit - '0' : 0;
}

// constexpr function that count bit equal to 1
constexpr int count_bit(unsigned long long a)
{
    int count = 0;
    while (a != 0)
    {
        if (a & 0x1) count++;
        a >>= 1;
    }

    return count;
}

// A constexpr function that count the number of bit with value 0 before a bit with 1
constexpr int count_shift(unsigned long long a)
{
    int count = 0;
    while (a != 0)
    {
        if (a & 0x1) return count;
        count++;
        a >>= 1;
    }

    return count;
}

template<typename T>
constexpr T trivial_pow(T x, int e)
{
    return e < 0 ? 1 / trivial_pow(x, -e) : e == 0 ? 1 : x * trivial_pow(x, e - 1);
}

// A function to know is a literal begins with an hex prefix
template<char FIRST>
constexpr bool has_hex_prefix()
{
    return false;
}

template<char FIRST, char SECOND, char... REST>
constexpr bool has_hex_prefix()
{
    return FIRST == '0' and (SECOND == 'x' or SECOND == 'X');
}

/*
 * Hex parsing related functions (better understanding by reading from bottom to top)
 */

// Function used to validate the number and returning its value
template<typename RT, unsigned long long MANTISSA, int POW, bool SIGN, int EXPONENT>
constexpr RT validate_hex()
{
    // Is it a power of two ?
    static_assert(count_bit(MANTISSA) == 1, VALUE_ERROR);
    // Is the exponent negative (positive power of 0.5 means negative power of 2)
    static_assert(count_shift(MANTISSA) + POW + (SIGN ? EXPONENT : -EXPONENT) < 0, VALUE_ERROR);
    return static_cast<RT>(MANTISSA) * trivial_pow(2.0l, POW + (SIGN ? EXPONENT : -EXPONENT));
}


// Function used to parse the digit of the exponent
template<typename RT, unsigned long long MANTISSA, int POW, bool SIGN, int EXPONENT>
constexpr RT parse_hex_exponent()
{
    return validate_hex<RT, MANTISSA, POW, SIGN, EXPONENT>();
}

template<typename RT, unsigned long long MANTISSA, int POW, bool SIGN, int EXPONENT, char FIRST, char... REST>
constexpr RT parse_hex_exponent()
{
    static_assert(isdigit(FIRST), HEX_PARSE_ERROR);
    return parse_hex_exponent<RT, MANTISSA, POW, SIGN, EXPONENT * 10 + decvalue(FIRST), REST...>();
}


// This function parses the first character after the 'p'
template<typename RT, unsigned long long MANTISSA, int POW>
constexpr RT parse_hex_exponent_sign()
{
    static_assert(always_false<RT>::value, HEX_PARSE_ERROR);
    return RT{};
}

template<typename RT, unsigned long long MANTISSA, int POW, char FIRST, char... REST>
constexpr RT parse_hex_exponent_sign()
{
    static_assert(isxdigit(FIRST) or FIRST == '+' or FIRST == '-', HEX_PARSE_ERROR);
    if constexpr (isxdigit(FIRST))
        return parse_hex_exponent<RT, MANTISSA, POW, true, hexvalue(FIRST), REST...>();
    else
    {
        // If we have a sign, we need to make there is at least one other character after
        static_assert(sizeof...(REST) > 0, HEX_PARSE_ERROR);
        if constexpr (FIRST == '+')
            return parse_hex_exponent<RT, MANTISSA, POW, true, 0, REST...>();
        else
            return parse_hex_exponent<RT, MANTISSA, POW, false, 0, REST...>();
    }
}


// Function used to parse the characters of a hex float literal that appear after the period
template<typename RT, unsigned long long MANTISSA, int POW>
constexpr RT parse_hex_post_period()
{
    static_assert(always_false<RT>::value, HEX_PARSE_ERROR);
    return RT{};
}

template<typename RT, unsigned long long MANTISSA, int POW, char FIRST, char... REST>
constexpr RT parse_hex_post_period()
{
    static_assert(isxdigit(FIRST) or FIRST == 'p' or FIRST == 'P', HEX_PARSE_ERROR);
    if constexpr (isxdigit(FIRST))
        return parse_hex_post_period<RT, MANTISSA * 16 + hexvalue(FIRST), POW - 4, REST...>();
    else
        return parse_hex_exponent_sign<RT, MANTISSA, POW, REST...>();
}


// Function used to parse the characters of a hex float literal that appear before the period
template<typename RT, unsigned long long MANTISSA>
constexpr RT parse_hex_pre_period()
{
    // If they are no more characters and we're here it means that is not a good hex float literal
    static_assert(always_false<RT>::value, HEX_PARSE_ERROR);
    return RT{};
}

template<typename RT, unsigned long long MANTISSA, char FIRST, char... REST>
constexpr RT parse_hex_pre_period()
{
    static_assert(isxdigit(FIRST) or FIRST == '.' or FIRST == 'p' or FIRST == 'P', HEX_PARSE_ERROR);
    if constexpr (isxdigit(FIRST))
        return parse_hex_pre_period<RT, MANTISSA * 16 + hexvalue(FIRST), REST...>();
    else if constexpr (FIRST == '.')
        return parse_hex_post_period<RT, MANTISSA, 0, REST...>();
    else
        return parse_hex_exponent_sign<RT, MANTISSA, 0, REST...>();
}


template<typename RT, char FIRST, char SECOND, char... REST>
constexpr RT test_hex()
{
    // Discard 2 first char as they are the hex marker
    return parse_hex_pre_period<RT, 0, REST...>();
}


// Validate value and returns it
template<typename RT, unsigned long long MANTISSA, int POW, bool SIGN, int EXPONENT>
constexpr RT validate_dec()
{
    constexpr RT value =
        (POW + (SIGN ? EXPONENT : -EXPONENT) < 0 ? MANTISSA / trivial_pow(10.0l, -POW - (SIGN ? EXPONENT : -EXPONENT)) :
                                                   MANTISSA * trivial_pow(10.0l, POW + (SIGN ? EXPONENT : -EXPONENT)));
    return value;
}


// This function parses the exponent past the first character
template<typename RT, unsigned long long MANTISSA, int POW, bool SIGN, int EXPONENT>
constexpr RT parse_dec_exponent()
{
    return validate_dec<RT, MANTISSA, POW, SIGN, EXPONENT>();
}

template<typename RT, unsigned long long MANTISSA, int POW, bool SIGN, int EXPONENT, char FIRST, char... REST>
constexpr RT parse_dec_exponent()
{
    static_assert(isdigit(FIRST), DEC_PARSE_ERROR);
    return parse_dec_exponent<RT, MANTISSA, POW, SIGN, EXPONENT * 10 + decvalue(FIRST), REST...>();
}


// This function parses the first character of the exponent
template<typename RT, unsigned long long MANTISSA, int POW>
constexpr RT parse_dec_exponent_sign()
{
    static_assert(always_false<RT>::value, DEC_PARSE_ERROR);
    return RT{};
}

template<typename RT, unsigned long long MANTISSA, int POW, char FIRST, char... REST>
constexpr RT parse_dec_exponent_sign()
{
    static_assert(isdigit(FIRST) or ((FIRST == '+' or FIRST == '-') and sizeof...(REST) > 0), DEC_PARSE_ERROR);
    if constexpr (isdigit(FIRST))
        return parse_dec_exponent<RT, MANTISSA, POW, true, decvalue(FIRST), REST...>();
    else if constexpr (FIRST == '+')
        return parse_dec_exponent<RT, MANTISSA, POW, true, 0, REST...>();
    else
        return parse_dec_exponent<RT, MANTISSA, POW, false, 0, REST...>();
}


// This function parses the characters that are after the period
template<typename RT, unsigned long long MANTISSA, int POW>
constexpr RT parse_dec_post_period()
{
    return validate_dec<RT, MANTISSA, POW, true, 0>();
}

template<typename RT, unsigned long long MANTISSA, int POW, char FIRST, char... REST>
constexpr RT parse_dec_post_period()
{
    static_assert(isdigit(FIRST) or FIRST == 'e' or FIRST == 'E', DEC_PARSE_ERROR);
    if constexpr (isdigit(FIRST))
        return parse_dec_post_period<RT, MANTISSA * 10 + decvalue(FIRST), POW - 1, REST...>();
    else
        return parse_dec_exponent_sign<RT, MANTISSA, POW, REST...>();
}


// This character parses the first characters before the period
template<typename RT, unsigned long long MANTISSA>
constexpr RT parse_dec_pre_period()
{
    return validate_dec<RT, MANTISSA, 0, true, 0>();
}

template<typename RT, unsigned long long MANTISSA, char FIRST, char... REST>
constexpr RT parse_dec_pre_period()
{
    static_assert(isdigit(FIRST) or FIRST == '.' or FIRST == 'e' or FIRST == 'E', DEC_PARSE_ERROR);
    if constexpr (isdigit(FIRST))
        return parse_dec_pre_period<RT, MANTISSA * 10 + decvalue(FIRST), REST...>();
    else if constexpr (FIRST == '.')
        return parse_dec_post_period<RT, MANTISSA, 0, REST...>();
    else
        return parse_dec_exponent_sign<RT, MANTISSA, 0, REST...>();
}


template<typename RT, char... STR>
constexpr RT test_dec()
{
    return parse_dec_pre_period<RT, 0, STR...>();
}

// Don't pollute global namespace
#undef HEX_PARSE_ERROR
#undef DEC_PARSE_ERROR
#undef VALUE_ERROR


template<typename RT, char... STR>
constexpr RT parse_float_literal()
{
    if constexpr (has_hex_prefix<STR...>())
        return test_hex<RT, STR...>();
    else
        return test_dec<RT, STR...>();
}
} // namespace detail


template<char... STR>
boost::safe_float::safe_float<double> operator""_sd()
{
    return boost::safe_float::safe_float<double>(detail::parse_float_literal<double, STR...>());
}

} // namespace literals
} // namespace safe_float
} // namespace boost

#endif // BOOST_SAFE_FLOAT_LITERALS_HPP