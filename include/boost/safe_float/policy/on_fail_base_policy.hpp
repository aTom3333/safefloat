#ifndef BOOST_SAFE_FLOAT_ON_FAIL_BASE_POLICY_HPP
#define BOOST_SAFE_FLOAT_ON_FAIL_BASE_POLICY_HPP

#include <string>

namespace boost {
namespace safe_float{
namespace policy{

/**
 * @brief Base policy for on_fail handling
 */
class on_fail_policy {
public:
    void report_failure(const std::string& s) {}
};

} //policy
} //safe_float
} //boost
#endif // BOOST_SAFE_FLOAT_ON_FAIL_BASE_POLICY_HPP
