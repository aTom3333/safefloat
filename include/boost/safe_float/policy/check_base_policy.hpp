#ifndef BOOST_SAFE_FLOAT_CHECK_BASE_POLICY_HPP
#define BOOST_SAFE_FLOAT_CHECK_BASE_POLICY_HPP

#include <string>
#include <cmath>

namespace boost {
namespace safe_float{
namespace policy{

/**
 * Base policy for check
 */
template<class FP>
class check_policy {
public:
    //operator+
    std::string addition_failure_message() { return std::string("Failed to add"); }
    //operator-
    std::string subtraction_failure_message() { return std::string("Failed to subtract"); }
    //operator*
    std::string multiplication_failure_message() { return std::string("Failed to multiply"); }
    //operator/
    std::string division_failure_message() { return std::string("Failed to divide"); }
};

} //policy
} //safe_float
} //boost
#endif // BOOST_SAFE_FLOAT_CHECK_BASE_POLICY_HPP
