#ifndef BOOST_SAFE_FLOAT_POLICY_CHECK_COMPOSERS_HPP
#define BOOST_SAFE_FLOAT_POLICY_CHECK_COMPOSERS_HPP

#include<algorithm>

#include <boost/safe_float/policy/check_base_policy.hpp>
#include <boost/safe_float/policy/policy_traits.hpp>
//this file define composers for policies

namespace boost {
namespace safe_float{
namespace policy{

//check_composer
template<class FP, template<class> class... As>
class compose_check : public check_policy<FP>, private As<FP>...
{
                //TODO add static check for As to be va;id check Policies.
public:
    //operator+
    bool pre_addition_check(const FP& lhs, const FP& rhs)
    {
        return (policy_traits<FP, As<FP>>::pre_addition_check(static_cast<As<FP>&>(*this), lhs, rhs) && ... && true);
    }

    bool post_addition_check(const FP& value)
    {
        return (policy_traits<FP, As<FP>>::post_addition_check(static_cast<As<FP>&>(*this), value) && ... && true);
    }

    std::string addition_failure_message()
    {
        //TODO: find out the message from the policy broken
        return std::string("Policy broken when adding");
    }

    //operator-
    bool pre_subtraction_check(const FP& lhs, const FP& rhs)
    {
        return (policy_traits<FP, As<FP>>::pre_subtraction_check(static_cast<As<FP>&>(*this), lhs, rhs) && ... && true);
    }

    bool post_subtraction_check(const FP& value)
    {
        return (policy_traits<FP, As<FP>>::post_subtraction_check(static_cast<As<FP>&>(*this), value) && ... && true);
    }

    std::string subtraction_failure_message()
    {
        //TODO: find out the message from the policy broken
        return std::string("Policy broken when subtracting");
    }

    //operator*
    bool pre_multiplication_check(const FP& lhs, const FP& rhs)
    {
        return (policy_traits<FP, As<FP>>::pre_multiplication_check(static_cast<As<FP>&>(*this), lhs, rhs) && ...
                && true);
    }

    bool post_multiplication_check(const FP& value)
    {
        return (policy_traits<FP, As<FP>>::post_multiplication_check(static_cast<As<FP>&>(*this), value) && ...
                && true);
    }

    std::string multiplication_failure_message()
    {
        //TODO: find out the message from the policy broken
        return std::string("Policy broken when multiplying");
    }

    //operator/
    bool pre_division_check(const FP& lhs, const FP& rhs)
    {
        return (policy_traits<FP, As<FP>>::pre_division_check(static_cast<As<FP>&>(*this), lhs, rhs) && ... && true);
    }

    bool post_division_check(const FP& value)
    {
        return (policy_traits<FP, As<FP>>::post_division_check(static_cast<As<FP>&>(*this), value) && ... && true);
    }

    std::string division_failure_message()
    {
        //TODO: find out the message from the policy broken
        return std::string("Policy broken when dividing");
    }

};

//on_fail_composer

//cast_composer

}
}
}




template<class FP>
class check_policy {
public:
    //operator+
    bool pre_addition_check(const FP& lhs, const FP& rhs) { return true; }
    bool post_addition_check(const FP& value) { return true; }
    std::string addition_failure_message() { return std::string("Failed to add"); }
    //operator-
    bool pre_subtraction_check(const FP& lhs, const FP& rhs) { return true; }
    bool post_subtraction_check(const FP& value) { return true; }
    std::string subtraction_failure_message() { return std::string("Failed to subtract"); }
    //operator*
    bool pre_multiplication_check(const FP& lhs, const FP& rhs) { return true; }
    bool post_multiplication_check(const FP& value) { return true; }
    std::string multiplication_failure_message() { return std::string("Failed to multiply"); }
    //operator/
    bool pre_division_check(const FP& lhs, const FP& rhs) { return true; }
    bool post_division_check(const FP& value) { return true; }
    std::string division_failure_message() { return std::string("Failed to divide"); }

    ~check_policy() {}
};

#endif // BOOST_SAFE_FLOAT_POLICY_CHECK_COMPOSERS_HPP

