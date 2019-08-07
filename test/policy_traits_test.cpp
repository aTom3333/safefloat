#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <boost/mpl/at.hpp>
#include <boost/mpl/bind.hpp>
#include <boost/mpl/list.hpp>
#include <boost/mpl/protect.hpp>
#include <boost/mpl/quote.hpp>
#include <boost/mpl/vector.hpp>


#include <cmath>

#include <boost/safe_float.hpp>
#include <boost/safe_float/convenience.hpp>
#include <boost/safe_float/policy/policy_traits.hpp>

// types to be tested
using test_types = boost::mpl::list<float, double, long double>;

using namespace boost::safe_float;


BOOST_AUTO_TEST_SUITE(safe_float_policy_test_suite)
BOOST_AUTO_TEST_SUITE(safe_float_policy_traits_test_suite)

BOOST_AUTO_TEST_SUITE(safe_float_policy_traits_base_policy_test_suite)
BOOST_AUTO_TEST_CASE_TEMPLATE(safe_float_policy_traits_base_policy, FPT, test_types)
{
    // define policy
    using p = policy::check_policy<FPT>;

    BOOST_CHECK((!policy::policy_traits<FPT, p>::has_pre_addition_check()));
    BOOST_CHECK((!policy::policy_traits<FPT, p>::has_post_addition_check()));
    BOOST_CHECK((!policy::policy_traits<FPT, p>::has_pre_subtraction_check()));
    BOOST_CHECK((!policy::policy_traits<FPT, p>::has_post_subtraction_check()));
    BOOST_CHECK((!policy::policy_traits<FPT, p>::has_pre_multiplication_check()));
    BOOST_CHECK((!policy::policy_traits<FPT, p>::has_post_multiplication_check()));
    BOOST_CHECK((!policy::policy_traits<FPT, p>::has_pre_division_check()));
    BOOST_CHECK((!policy::policy_traits<FPT, p>::has_post_division_check()));
}

BOOST_AUTO_TEST_SUITE_END() // policy_traits base policy

BOOST_AUTO_TEST_SUITE(safe_float_policy_traits_addition_overflow_test_suite)
BOOST_AUTO_TEST_CASE_TEMPLATE(safe_float_policy_traits_addition_overflow, FPT, test_types)
{
    // define policy
    using p = policy::check_addition_overflow<FPT>;

    BOOST_CHECK((policy::policy_traits<FPT, p>::has_pre_addition_check()));
    BOOST_CHECK((policy::policy_traits<FPT, p>::has_post_addition_check()));
    BOOST_CHECK((!policy::policy_traits<FPT, p>::has_pre_subtraction_check()));
    BOOST_CHECK((!policy::policy_traits<FPT, p>::has_post_subtraction_check()));
    BOOST_CHECK((!policy::policy_traits<FPT, p>::has_pre_multiplication_check()));
    BOOST_CHECK((!policy::policy_traits<FPT, p>::has_post_multiplication_check()));
    BOOST_CHECK((!policy::policy_traits<FPT, p>::has_pre_division_check()));
    BOOST_CHECK((!policy::policy_traits<FPT, p>::has_post_division_check()));
}

BOOST_AUTO_TEST_SUITE_END() // policy_traits addition overflow

BOOST_AUTO_TEST_SUITE(safe_float_policy_traits_full_policy_test_suite)
BOOST_AUTO_TEST_CASE_TEMPLATE(safe_float_policy_traits_full_policy, FPT, test_types)
{
    // define policy
    using p = policy::check_all<FPT>;

    BOOST_CHECK((policy::policy_traits<FPT, p>::has_pre_addition_check()));
    BOOST_CHECK((policy::policy_traits<FPT, p>::has_post_addition_check()));
    BOOST_CHECK((policy::policy_traits<FPT, p>::has_pre_subtraction_check()));
    BOOST_CHECK((policy::policy_traits<FPT, p>::has_post_subtraction_check()));
    BOOST_CHECK((policy::policy_traits<FPT, p>::has_pre_multiplication_check()));
    BOOST_CHECK((policy::policy_traits<FPT, p>::has_post_multiplication_check()));
    BOOST_CHECK((policy::policy_traits<FPT, p>::has_pre_division_check()));
    BOOST_CHECK((policy::policy_traits<FPT, p>::has_post_division_check()));
}

BOOST_AUTO_TEST_SUITE_END() // policy_traits full policy

BOOST_AUTO_TEST_SUITE(safe_float_policy_subset_test_suite)
BOOST_AUTO_TEST_CASE_TEMPLATE(safe_float_policy_subset, FPT, test_types)
{
    BOOST_CHECK((policy::is_subset<policy::check_addition_overflow<FPT>, policy::check_addition_overflow<FPT>>::value));
    BOOST_CHECK((policy::is_subset<policy::check_addition_overflow<FPT>, policy::check_all<FPT>>::value));
    BOOST_CHECK((policy::is_subset<policy::check_underflow<FPT>, policy::check_all<FPT>>::value));
    BOOST_CHECK((policy::is_subset<policy::check_all<FPT>, policy::check_all<FPT>>::value));
    BOOST_CHECK((not policy::is_subset<policy::check_all<FPT>, policy::check_bothflow<FPT>>::value));
    BOOST_CHECK((not policy::is_subset<policy::check_invalid_result<FPT>, policy::check_addition_invalid_result<FPT>>::value));
    
    BOOST_CHECK((policy::is_subset<
        policy::compose_check<policy::check_addition_overflow, policy::check_subtraction_overflow>::policy<FPT>, 
        policy::compose_check<policy::check_subtraction_overflow, policy::check_addition_overflow>::policy<FPT>
        >::value));
}

BOOST_AUTO_TEST_SUITE_END() // policy subset

BOOST_AUTO_TEST_SUITE_END() // policy_traits
BOOST_AUTO_TEST_SUITE_END() // policy