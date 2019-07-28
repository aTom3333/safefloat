#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <boost/mpl/vector.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/quote.hpp>
#include <boost/mpl/protect.hpp>
#include <boost/mpl/bind.hpp>
#include <boost/mpl/list.hpp>


#include <cmath>
#include <limits>


#include <boost/safe_float.hpp>
#include <boost/safe_float/convenience.hpp>

//types to be tested
using test_types=boost::mpl::list<
    float, double, long double
>;

using namespace boost::safe_float;

/**
  This test suite checks the methods providing casting.
  */

BOOST_AUTO_TEST_SUITE( safe_float_casting_test_suite )

BOOST_AUTO_TEST_CASE_TEMPLATE( safe_float_get_stored_value, FPT, test_types){
    //set initial values
    FPT a = std::numeric_limits<FPT>::max();
    FPT b = std::numeric_limits<FPT>::min();
    FPT c = std::numeric_limits<FPT>::infinity();
    FPT d = 0;
    FPT e = 1;
    //create safe version of the values
    safe_float<FPT> sa(std::numeric_limits<FPT>::max());
    safe_float<FPT> sb(std::numeric_limits<FPT>::min());
    safe_float<FPT> sc(std::numeric_limits<FPT>::infinity());
    safe_float<FPT> sd(d);
    safe_float<FPT> se(e);
    //check the values match when obtaining the stored value
    BOOST_CHECK_EQUAL(a, sa.get_stored_value());
    BOOST_CHECK_EQUAL(b, sb.get_stored_value());
    BOOST_CHECK_EQUAL(c, sc.get_stored_value());
    BOOST_CHECK_EQUAL(d, sd.get_stored_value());
    BOOST_CHECK_EQUAL(e, se.get_stored_value());
}

template<typename FP, template<typename> typename CAST>
using sf_test_cast = safe_float<FP, policy::check_all, policy::on_fail_throw, CAST>;

BOOST_AUTO_TEST_CASE(safe_float_cast_from_primitive_none) {
    // Cannot be converted from any primitive but is explicitly constructible from float
    using sf = sf_test_cast<float, policy::cast_from_primitive::none>;

    BOOST_CHECK((not std::is_convertible_v<float, sf>));
    BOOST_CHECK((not std::is_convertible_v<double, sf>));
    BOOST_CHECK((not std::is_convertible_v<long double, sf>));
    BOOST_CHECK((std::is_constructible_v<sf, float>));
    BOOST_CHECK((not std::is_constructible_v<sf, double>));
    BOOST_CHECK((not std::is_constructible_v<sf, long double>));


    // Cannot be converted from any primitive but is explicitly constructible from double
    using sd = sf_test_cast<double, policy::cast_from_primitive::none>;

    BOOST_CHECK((not std::is_convertible_v<float, sd>));
    BOOST_CHECK((not std::is_convertible_v<double, sd>));
    BOOST_CHECK((not std::is_convertible_v<long double, sd>));
    BOOST_CHECK((not std::is_constructible_v<sd, float>));
    BOOST_CHECK((std::is_constructible_v<sd, double>));
    BOOST_CHECK((not std::is_constructible_v<sd, long double>));
    
    
    // Cannot be converted from any primitive but is explicitly constructible from long double
    using sld = sf_test_cast<long double, policy::cast_from_primitive::none>;

    BOOST_CHECK((not std::is_convertible_v<float, sld>));
    BOOST_CHECK((not std::is_convertible_v<double, sld>));
    BOOST_CHECK((not std::is_convertible_v<long double, sld>));
    BOOST_CHECK((not std::is_constructible_v<sld, float>));
    BOOST_CHECK((not std::is_constructible_v<sld, double>));
    BOOST_CHECK((std::is_constructible_v<sld, long double>));
}

BOOST_AUTO_TEST_CASE(safe_float_cast_from_primitive_none_explicit) {
    // Cannot be converted from any primitive but is explicitly constructible from float
    using sf = sf_test_cast<float, policy::cast_from_primitive::none_explicit>;

    BOOST_CHECK((not std::is_convertible_v<float, sf>));
    BOOST_CHECK((not std::is_convertible_v<double, sf>));
    BOOST_CHECK((not std::is_convertible_v<long double, sf>));
    BOOST_CHECK((std::is_constructible_v<sf, float>));
    BOOST_CHECK((not std::is_constructible_v<sf, double>));
    BOOST_CHECK((not std::is_constructible_v<sf, long double>));


    // Cannot be converted from any primitive but is explicitly constructible from double
    using sd = sf_test_cast<double, policy::cast_from_primitive::none_explicit>;

    BOOST_CHECK((not std::is_convertible_v<float, sd>));
    BOOST_CHECK((not std::is_convertible_v<double, sd>));
    BOOST_CHECK((not std::is_convertible_v<long double, sd>));
    BOOST_CHECK((not std::is_constructible_v<sd, float>));
    BOOST_CHECK((std::is_constructible_v<sd, double>));
    BOOST_CHECK((not std::is_constructible_v<sd, long double>));


    // Cannot be converted from any primitive but is explicitly constructible from long double
    using sld = sf_test_cast<long double, policy::cast_from_primitive::none_explicit>;

    BOOST_CHECK((not std::is_convertible_v<float, sld>));
    BOOST_CHECK((not std::is_convertible_v<double, sld>));
    BOOST_CHECK((not std::is_convertible_v<long double, sld>));
    BOOST_CHECK((not std::is_constructible_v<sld, float>));
    BOOST_CHECK((not std::is_constructible_v<sld, double>));
    BOOST_CHECK((std::is_constructible_v<sld, long double>));
}

BOOST_AUTO_TEST_CASE(safe_float_cast_from_primitive_same) {
    // Can be converted from float only
    using sf = sf_test_cast<float, policy::cast_from_primitive::same>;

    BOOST_CHECK((std::is_convertible_v<float, sf>));
    BOOST_CHECK((not std::is_convertible_v<double, sf>));
    BOOST_CHECK((not std::is_convertible_v<long double, sf>));
    BOOST_CHECK((std::is_constructible_v<sf, float>));
    BOOST_CHECK((not std::is_constructible_v<sf, double>));
    BOOST_CHECK((not std::is_constructible_v<sf, long double>));


    // Can be converted from double only
    using sd = sf_test_cast<double, policy::cast_from_primitive::same>;

    BOOST_CHECK((not std::is_convertible_v<float, sd>));
    BOOST_CHECK((std::is_convertible_v<double, sd>));
    BOOST_CHECK((not std::is_convertible_v<long double, sd>));
    BOOST_CHECK((not std::is_constructible_v<sd, float>));
    BOOST_CHECK((std::is_constructible_v<sd, double>));
    BOOST_CHECK((not std::is_constructible_v<sd, long double>));


    // Can be converted from long double only
    using sld = sf_test_cast<long double, policy::cast_from_primitive::same>;

    BOOST_CHECK((not std::is_convertible_v<float, sld>));
    BOOST_CHECK((not std::is_convertible_v<double, sld>));
    BOOST_CHECK((std::is_convertible_v<long double, sld>));
    BOOST_CHECK((not std::is_constructible_v<sld, float>));
    BOOST_CHECK((not std::is_constructible_v<sld, double>));
    BOOST_CHECK((std::is_constructible_v<sld, long double>));
}

BOOST_AUTO_TEST_CASE(safe_float_cast_from_primitive_same_explicit) {
    // Cannot be converted from any primitive but is explicitly constructible from float
    using sf = sf_test_cast<float, policy::cast_from_primitive::same_explicit>;

    BOOST_CHECK((not std::is_convertible_v<float, sf>));
    BOOST_CHECK((not std::is_convertible_v<double, sf>));
    BOOST_CHECK((not std::is_convertible_v<long double, sf>));
    BOOST_CHECK((std::is_constructible_v<sf, float>));
    BOOST_CHECK((not std::is_constructible_v<sf, double>));
    BOOST_CHECK((not std::is_constructible_v<sf, long double>));


    // Cannot be converted from any primitive but is explicitly constructible from double
    using sd = sf_test_cast<double, policy::cast_from_primitive::same_explicit>;

    BOOST_CHECK((not std::is_convertible_v<float, sd>));
    BOOST_CHECK((not std::is_convertible_v<double, sd>));
    BOOST_CHECK((not std::is_convertible_v<long double, sd>));
    BOOST_CHECK((not std::is_constructible_v<sd, float>));
    BOOST_CHECK((std::is_constructible_v<sd, double>));
    BOOST_CHECK((not std::is_constructible_v<sd, long double>));


    // Cannot be converted from any primitive but is explicitly constructible from long double
    using sld = sf_test_cast<long double, policy::cast_from_primitive::same_explicit>;

    BOOST_CHECK((not std::is_convertible_v<float, sld>));
    BOOST_CHECK((not std::is_convertible_v<double, sld>));
    BOOST_CHECK((not std::is_convertible_v<long double, sld>));
    BOOST_CHECK((not std::is_constructible_v<sld, float>));
    BOOST_CHECK((not std::is_constructible_v<sld, double>));
    BOOST_CHECK((std::is_constructible_v<sld, long double>));
}

BOOST_AUTO_TEST_CASE(safe_float_cast_from_primitive_more_precise) {
    // Can be converted from float, double and long double
    using sf = sf_test_cast<float, policy::cast_from_primitive::more_precise>;

    BOOST_CHECK((std::is_convertible_v<float, sf>));
    BOOST_CHECK((std::is_convertible_v<double, sf>));
    BOOST_CHECK((std::is_convertible_v<long double, sf>));
    BOOST_CHECK((std::is_constructible_v<sf, float>));
    BOOST_CHECK((std::is_constructible_v<sf, double>));
    BOOST_CHECK((std::is_constructible_v<sf, long double>));


    // Can be converted from double and long double
    using sd = sf_test_cast<double, policy::cast_from_primitive::more_precise>;

    BOOST_CHECK((not std::is_convertible_v<float, sd>));
    BOOST_CHECK((std::is_convertible_v<double, sd>));
    BOOST_CHECK((std::is_convertible_v<long double, sd>));
    BOOST_CHECK((not std::is_constructible_v<sd, float>));
    BOOST_CHECK((std::is_constructible_v<sd, double>));
    BOOST_CHECK((std::is_constructible_v<sd, long double>));


    // Can be converted from long double only
    using sld = sf_test_cast<long double, policy::cast_from_primitive::more_precise>;

    BOOST_CHECK((not std::is_convertible_v<float, sld>));
    BOOST_CHECK((not std::is_convertible_v<double, sld>));
    BOOST_CHECK((std::is_convertible_v<long double, sld>));
    BOOST_CHECK((not std::is_constructible_v<sld, float>));
    BOOST_CHECK((not std::is_constructible_v<sld, double>));
    BOOST_CHECK((std::is_constructible_v<sld, long double>));
}

BOOST_AUTO_TEST_CASE(safe_float_cast_from_primitive_more_precise_explicit) {
    // Can be explicitly constructed from float, double and long double
    using sf = sf_test_cast<float, policy::cast_from_primitive::more_precise_explicit>;

    BOOST_CHECK((not std::is_convertible_v<float, sf>));
    BOOST_CHECK((not std::is_convertible_v<double, sf>));
    BOOST_CHECK((not std::is_convertible_v<long double, sf>));
    BOOST_CHECK((std::is_constructible_v<sf, float>));
    BOOST_CHECK((std::is_constructible_v<sf, double>));
    BOOST_CHECK((std::is_constructible_v<sf, long double>));


    // Can be explicitly constructed from double and long double
    using sd = sf_test_cast<double, policy::cast_from_primitive::more_precise_explicit>;

    BOOST_CHECK((not std::is_convertible_v<float, sd>));
    BOOST_CHECK((not std::is_convertible_v<double, sd>));
    BOOST_CHECK((not std::is_convertible_v<long double, sd>));
    BOOST_CHECK((not std::is_constructible_v<sd, float>));
    BOOST_CHECK((std::is_constructible_v<sd, double>));
    BOOST_CHECK((std::is_constructible_v<sd, long double>));


    // Can be explicitly constructed from long double only
    using sld = sf_test_cast<long double, policy::cast_from_primitive::more_precise_explicit>;

    BOOST_CHECK((not std::is_convertible_v<float, sld>));
    BOOST_CHECK((not std::is_convertible_v<double, sld>));
    BOOST_CHECK((not std::is_convertible_v<long double, sld>));
    BOOST_CHECK((not std::is_constructible_v<sld, float>));
    BOOST_CHECK((not std::is_constructible_v<sld, double>));
    BOOST_CHECK((std::is_constructible_v<sld, long double>));
}

BOOST_AUTO_TEST_CASE(safe_float_cast_from_primitive_less_precise) {
    // Can be converted from float
    using sf = sf_test_cast<float, policy::cast_from_primitive::less_precise>;

    BOOST_CHECK((std::is_convertible_v<float, sf>));
    BOOST_CHECK((not std::is_convertible_v<double, sf>));
    BOOST_CHECK((not std::is_convertible_v<long double, sf>));
    BOOST_CHECK((std::is_constructible_v<sf, float>));
    BOOST_CHECK((not std::is_constructible_v<sf, double>));
    BOOST_CHECK((not std::is_constructible_v<sf, long double>));


    // Can be converted from float and double
    using sd = sf_test_cast<double, policy::cast_from_primitive::less_precise>;

    BOOST_CHECK((std::is_convertible_v<float, sd>));
    BOOST_CHECK((std::is_convertible_v<double, sd>));
    BOOST_CHECK((not std::is_convertible_v<long double, sd>));
    BOOST_CHECK((std::is_constructible_v<sd, float>));
    BOOST_CHECK((std::is_constructible_v<sd, double>));
    BOOST_CHECK((not std::is_constructible_v<sd, long double>));


    // Can be converted from float, double and long double
    using sld = sf_test_cast<long double, policy::cast_from_primitive::less_precise>;

    BOOST_CHECK((std::is_convertible_v<float, sld>));
    BOOST_CHECK((std::is_convertible_v<double, sld>));
    BOOST_CHECK((std::is_convertible_v<long double, sld>));
    BOOST_CHECK((std::is_constructible_v<sld, float>));
    BOOST_CHECK((std::is_constructible_v<sld, double>));
    BOOST_CHECK((std::is_constructible_v<sld, long double>));
}

BOOST_AUTO_TEST_CASE(safe_float_cast_from_primitive_less_precise_explicit) {
    // Can be explicitly constructed from float
    using sf = sf_test_cast<float, policy::cast_from_primitive::less_precise_explicit>;

    BOOST_CHECK((not std::is_convertible_v<float, sf>));
    BOOST_CHECK((not std::is_convertible_v<double, sf>));
    BOOST_CHECK((not std::is_convertible_v<long double, sf>));
    BOOST_CHECK((std::is_constructible_v<sf, float>));
    BOOST_CHECK((not std::is_constructible_v<sf, double>));
    BOOST_CHECK((not std::is_constructible_v<sf, long double>));


    // Can be explicitly constructed from float and double
    using sd = sf_test_cast<double, policy::cast_from_primitive::less_precise_explicit>;

    BOOST_CHECK((not std::is_convertible_v<float, sd>));
    BOOST_CHECK((not std::is_convertible_v<double, sd>));
    BOOST_CHECK((not std::is_convertible_v<long double, sd>));
    BOOST_CHECK((std::is_constructible_v<sd, float>));
    BOOST_CHECK((std::is_constructible_v<sd, double>));
    BOOST_CHECK((not std::is_constructible_v<sd, long double>));


    // Can be explicitly constructed from float, double and long double
    using sld = sf_test_cast<long double, policy::cast_from_primitive::less_precise_explicit>;

    BOOST_CHECK((not std::is_convertible_v<float, sld>));
    BOOST_CHECK((not std::is_convertible_v<double, sld>));
    BOOST_CHECK((not std::is_convertible_v<long double, sld>));
    BOOST_CHECK((std::is_constructible_v<sld, float>));
    BOOST_CHECK((std::is_constructible_v<sld, double>));
    BOOST_CHECK((std::is_constructible_v<sld, long double>));
}

BOOST_AUTO_TEST_CASE(safe_float_cast_from_primitive_all) {
    // Can be converted from float, double and long double
    using sf = sf_test_cast<float, policy::cast_from_primitive::all>;

    BOOST_CHECK((std::is_convertible_v<float, sf>));
    BOOST_CHECK((std::is_convertible_v<double, sf>));
    BOOST_CHECK((std::is_convertible_v<long double, sf>));
    BOOST_CHECK((std::is_constructible_v<sf, float>));
    BOOST_CHECK((std::is_constructible_v<sf, double>));
    BOOST_CHECK((std::is_constructible_v<sf, long double>));


    // Can be converted from float, double and long double
    using sd = sf_test_cast<double, policy::cast_from_primitive::all>;

    BOOST_CHECK((std::is_convertible_v<float, sd>));
    BOOST_CHECK((std::is_convertible_v<double, sd>));
    BOOST_CHECK((std::is_convertible_v<long double, sd>));
    BOOST_CHECK((std::is_constructible_v<sd, float>));
    BOOST_CHECK((std::is_constructible_v<sd, double>));
    BOOST_CHECK((std::is_constructible_v<sd, long double>));


    // Can be converted from float, double and long double
    using sld = sf_test_cast<long double, policy::cast_from_primitive::all>;

    BOOST_CHECK((std::is_convertible_v<float, sld>));
    BOOST_CHECK((std::is_convertible_v<double, sld>));
    BOOST_CHECK((std::is_convertible_v<long double, sld>));
    BOOST_CHECK((std::is_constructible_v<sld, float>));
    BOOST_CHECK((std::is_constructible_v<sld, double>));
    BOOST_CHECK((std::is_constructible_v<sld, long double>));
}

BOOST_AUTO_TEST_CASE(safe_float_cast_from_primitive_all_explicit) {
    // Can be explicitly constructed from float, double and long double
    using sf = sf_test_cast<float, policy::cast_from_primitive::all_explicit>;

    BOOST_CHECK((not std::is_convertible_v<float, sf>));
    BOOST_CHECK((not std::is_convertible_v<double, sf>));
    BOOST_CHECK((not std::is_convertible_v<long double, sf>));
    BOOST_CHECK((std::is_constructible_v<sf, float>));
    BOOST_CHECK((std::is_constructible_v<sf, double>));
    BOOST_CHECK((std::is_constructible_v<sf, long double>));


    // Can be explicitly constructed from float, double and long double
    using sd = sf_test_cast<double, policy::cast_from_primitive::all_explicit>;

    BOOST_CHECK((not std::is_convertible_v<float, sd>));
    BOOST_CHECK((not std::is_convertible_v<double, sd>));
    BOOST_CHECK((not std::is_convertible_v<long double, sd>));
    BOOST_CHECK((std::is_constructible_v<sd, float>));
    BOOST_CHECK((std::is_constructible_v<sd, double>));
    BOOST_CHECK((std::is_constructible_v<sd, long double>));


    // Can be explicitly constructed from float, double and long double
    using sld = sf_test_cast<long double, policy::cast_from_primitive::all_explicit>;

    BOOST_CHECK((not std::is_convertible_v<float, sld>));
    BOOST_CHECK((not std::is_convertible_v<double, sld>));
    BOOST_CHECK((not std::is_convertible_v<long double, sld>));
    BOOST_CHECK((std::is_constructible_v<sld, float>));
    BOOST_CHECK((std::is_constructible_v<sld, double>));
    BOOST_CHECK((std::is_constructible_v<sld, long double>));
}

BOOST_AUTO_TEST_CASE_TEMPLATE( safe_float_cast_from_int, FPT, test_types){
    //set initial values
    int a = 0;
    int b = 1;
    int c = -284554;
    int d = 684654;
    //create safe version of the values
    using sf = safe_float<FPT, policy::check_all, policy::on_fail_throw, policy::allow_cast_from<int>::policy>;
    sf sa = a;
    sf sb = b;
    sf sc = c;
    sf sd = d;
    //check the values match when obtaining the stored value
    BOOST_CHECK_EQUAL(a, sa.get_stored_value());
    BOOST_CHECK_EQUAL(b, sb.get_stored_value());
    BOOST_CHECK_EQUAL(c, sc.get_stored_value());
    BOOST_CHECK_EQUAL(d, sd.get_stored_value());
}

BOOST_AUTO_TEST_CASE_TEMPLATE( safe_float_explicit_cast_from_int, FPT, test_types){
    //set initial values
    int a = 0;
    int b = 1;
    int c = -284554;
    int d = 684654;
    //create safe version of the values
    using sf = safe_float<FPT, policy::check_all, policy::on_fail_throw, policy::allow_explicit_cast_from<int>::policy>;
    sf sa(a);
    sf sb(b);
    sf sc(c);
    sf sd(d);
    //check the values match when obtaining the stored value
    BOOST_CHECK_EQUAL(a, sa.get_stored_value());
    BOOST_CHECK_EQUAL(b, sb.get_stored_value());
    BOOST_CHECK_EQUAL(c, sc.get_stored_value());
    BOOST_CHECK_EQUAL(d, sd.get_stored_value());
}

BOOST_AUTO_TEST_SUITE_END()


