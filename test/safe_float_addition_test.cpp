#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <boost/mpl/vector.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/quote.hpp>
#include <boost/mpl/protect.hpp>
#include <boost/mpl/bind.hpp>
#include <boost/mpl/list.hpp>

#include <limits>


#include <cmath>
#include <boost/safe_float.hpp>
#include <boost/safe_float/convenience.hpp>
#include <boost/safe_float/policy/check_addition_overflow.hpp>
#include <boost/safe_float/policy/check_addition_underflow.hpp>
#include <boost/safe_float/policy/check_addition_inexact.hpp>
#include <boost/safe_float/policy/check_addition_invalid_result.hpp>

//types to be tested
using test_types=boost::mpl::list<
    float, double, long double
>;

using namespace boost::safe_float;

/**
  This test suite checks different policies on addition operations using default parameters for the other policies.
  */
BOOST_AUTO_TEST_SUITE( safe_float_addition_test_suite )

BOOST_AUTO_TEST_CASE_TEMPLATE( safe_float_addition_throws_on_overflow, FPT, test_types){
    // define two FPT numbers suppose to positive overflow
    FPT a = std::numeric_limits<FPT>::max();
    FPT b = std::numeric_limits<FPT>::max();
    // check FPT overflows to inf after add
    BOOST_CHECK(std::isinf(a+b));

    // construct safe_float version of the same two numbers
    safe_float<FPT, policy::check_addition_overflow> c(std::numeric_limits<FPT>::max());
    safe_float<FPT, policy::check_addition_overflow> d(std::numeric_limits<FPT>::max());

    // check the addition throws
    BOOST_CHECK_THROW(c+d, std::exception);

    // define two FPT numbers suppose to negative overflow
    FPT e = std::numeric_limits<FPT>::lowest();
    FPT f = std::numeric_limits<FPT>::lowest();
    // check FPT overflows to inf after add
    BOOST_CHECK(std::isinf(e+f));

    // construct safe_float version of the same two numbers
    safe_float<FPT, policy::check_addition_overflow> g(std::numeric_limits<FPT>::lowest());
    safe_float<FPT, policy::check_addition_overflow> h(std::numeric_limits<FPT>::lowest());

    // check the addition throws
    BOOST_CHECK_THROW(g+h, std::exception);

}

BOOST_AUTO_TEST_CASE_TEMPLATE( safe_float_addition_inexact_rounding, FPT, test_types){
    // define two FPT numbers suppose to produce an inexact rounded result
    FPT a = 1;
    FPT b = pow(2, std::numeric_limits<FPT>::digits);

    // check adding and substracting b does not obtain a again.
    BOOST_CHECK(a+b-b != a);

    // construct safe_float version of the same two numbers
    safe_float<FPT, policy::check_addition_inexact> c(FPT(1));
    safe_float<FPT, policy::check_addition_inexact> d((FPT)pow(2, std::numeric_limits<FPT>::digits));

    // check the addition throws
    BOOST_CHECK_THROW(c+d, std::exception);
}

BOOST_AUTO_TEST_CASE_TEMPLATE( safe_float_addition_underflow, FPT, test_types){
    // define two FPT numbers suppose to underflow
    FPT a, b, c;
    if(std::is_same<FPT, float>()) {
        a =  4.01254977e-38f;
        b = -4.01254949e-38f;

        BOOST_CHECK(std::isnormal(a));
        BOOST_CHECK(std::isnormal(b));

        //check the addition produces an denormal result (considered underflow)
        c = a + b;
        BOOST_CHECK( std::fpclassify( c ) == FP_SUBNORMAL ) ;

        // construct safe_float version of the same two numbers
        safe_float<FPT, policy::check_addition_underflow> d(a);
        safe_float<FPT, policy::check_addition_underflow> e(b);

        // check the addition throws
        BOOST_CHECK_THROW(d+e, std::exception);
    } else if (std::is_same<FPT, double>()) {
        a =  2.2250738585072019e-308;
        b = -2.2250738585072014e-308;
        
        BOOST_CHECK(std::isnormal(a));
        BOOST_CHECK(std::isnormal(b));
        
        //check the addition produces an denormal result (considered underflow)
        c = a + b;
        BOOST_CHECK( std::fpclassify( c ) == FP_SUBNORMAL ) ;

        // construct safe_float version of the same two numbers
        safe_float<FPT, policy::check_addition_underflow> d(a);
        safe_float<FPT, policy::check_addition_underflow> e(b);

        // check the addition throws
        BOOST_CHECK_THROW(d+e, std::exception);
    } else if(std::is_same<FPT, long double>()) {
        a =  3.40132972460942461217e-4932l;
        b = -3.40132972460942461181e-4932l;

        BOOST_CHECK(std::isnormal(a));
        BOOST_CHECK(std::isnormal(b));

        //check the addition produces an denormal result (considered underflow)
        c = a + b;
        BOOST_CHECK( std::fpclassify( c ) == FP_SUBNORMAL ) ;

        // construct safe_float version of the same two numbers
        safe_float<FPT, policy::check_addition_underflow> d(a);
        safe_float<FPT, policy::check_addition_underflow> e(b);

        // check the addition throws
        BOOST_CHECK_THROW(d+e, std::exception);
    } else {
        BOOST_ERROR("underflow test only implemented for, float, double and long double");
    }
}

BOOST_AUTO_TEST_CASE_TEMPLATE( safe_float_addition_invalid_result, FPT, test_types){
    // define two FPT numbers suppose to produce a NAN
    FPT a = std::numeric_limits<FPT>::infinity();
    FPT b = -(std::numeric_limits<FPT>::infinity());

    // check adding produced NaN
    BOOST_CHECK(std::isnan(a+b));

    // construct safe_float version of the same two numbers
    safe_float<FPT, policy::check_addition_invalid_result> c(std::numeric_limits<FPT>::infinity());
    safe_float<FPT, policy::check_addition_invalid_result> d(-(std::numeric_limits<FPT>::infinity()));

    // check the addition throws
    BOOST_CHECK_THROW(c+d, std::exception);
}


BOOST_AUTO_TEST_SUITE_END()


