#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <boost/mpl/vector.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/quote.hpp>
#include <boost/mpl/protect.hpp>
#include <boost/mpl/bind.hpp>
#include <boost/mpl/list.hpp>


#include <cmath>
#include <boost/safe_float.hpp>
#include <boost/safe_float/convenience.hpp>
#include <boost/safe_float/policy/check_multiplication_overflow.hpp>
#include <boost/safe_float/policy/check_multiplication_underflow.hpp>
#include <boost/safe_float/policy/check_multiplication_inexact.hpp>
#include <boost/safe_float/policy/check_multiplication_invalid_result.hpp>

//types to be tested
using test_types=boost::mpl::list<
    float, double, long double
>;

using namespace boost::safe_float;

/**
  This test suite checks different policies on multiplication operations using default parameters for the other policies.
  */
BOOST_AUTO_TEST_SUITE( safe_float_multiplication_test_suite )

BOOST_AUTO_TEST_CASE_TEMPLATE( safe_float_multiplication_throws_on_overflow, FPT, test_types){
    // define two FPT numbers suppose to positive overflow
    FPT a = std::numeric_limits<FPT>::max();
    FPT b = 2;
    // check FPT overflows to inf after add
    BOOST_CHECK(std::isinf(a*b));

    // construct safe_float version of the same two numbers
    safe_float<FPT, policy::check_multiplication_overflow> c(std::numeric_limits<FPT>::max());
    safe_float<FPT, policy::check_multiplication_overflow> d(FPT(2));

    // check the multiplication throws
    BOOST_CHECK_THROW(c*d, std::exception);

    // define two FPT numbers suppose to negative overflow
    FPT e = std::numeric_limits<FPT>::lowest();
    FPT f = 2;
    // check FPT overflows to inf after add
    BOOST_CHECK(std::isinf(e*f));

    // construct safe_float version of the same two numbers
    safe_float<FPT, policy::check_multiplication_overflow> g(std::numeric_limits<FPT>::lowest());
    safe_float<FPT, policy::check_multiplication_overflow> h(FPT(2));

    // check the multiplication throws
    BOOST_CHECK_THROW(g*h, std::exception);
}

BOOST_AUTO_TEST_CASE_TEMPLATE( safe_float_multiplication_inexact_rounding, FPT, test_types){
    // define two FPT numbers suppose to produce inexact rounding
    FPT a, b;
    
    if(std::is_same<FPT, float>()) {
        a = 1.64005529880523681640625f;
        b = 3.1559422016143798828125f;
    } else if(std::is_same<FPT, double>()){
        a = 1.200941392190915113502569511183537542819976806640625;
        b = 1.7035518365272823704259508303948678076267242431640625;
    } else if(std::is_same<FPT, long double>()) {
        a = 1.48057361058650153290937312444697226965217851102352142333984375L;
        b = 1.8352666822131742060432435525996197611675597727298736572265625L;
    } else {
        BOOST_ERROR("Test implemented only for float, double and long double");
    }

    // check multiplying and dividing gives the same number back.
    BOOST_CHECK((a*b)/b != a);

    // construct safe_float version of the same two numbers
    safe_float<FPT, policy::check_multiplication_inexact> c(a);
    safe_float<FPT, policy::check_multiplication_inexact> d(b);

    // check the multiplication throws
    BOOST_CHECK_THROW(c*d, std::exception);
}

BOOST_AUTO_TEST_CASE_TEMPLATE( safe_float_multiplication_underflow, FPT, test_types){
    // define two FPT numbers suppose to produce underflow
    FPT a = std::numeric_limits<FPT>::min();
    FPT b = 0.5;

    // check FPT underflow to denormalized result after multiply
    BOOST_CHECK(std::fpclassify(a*b) == FP_SUBNORMAL);

    // construct safe_float version of the same two numbers
    safe_float<FPT, policy::check_multiplication_underflow> c(a);
    safe_float<FPT, policy::check_multiplication_underflow> d(b);

    // check the multiplication throws
    BOOST_CHECK_THROW(c*d, std::exception);
}

BOOST_AUTO_TEST_CASE_TEMPLATE( safe_float_multiplication_invalid_result, FPT, test_types){
    // define two FPT numbers suppose to produce a NAN
    FPT a = std::numeric_limits<FPT>::infinity();
    FPT b = 0;

    // check adding produced NaN
    BOOST_CHECK(std::isnan(a*b));

    // construct safe_float version of the same two numbers
    safe_float<FPT, policy::check_multiplication_invalid_result> c(std::numeric_limits<FPT>::infinity());
    safe_float<FPT, policy::check_multiplication_invalid_result> d(FPT(0));

    // check the multiplication throws
    BOOST_CHECK_THROW(c*d, std::exception);
}


BOOST_AUTO_TEST_SUITE_END()


