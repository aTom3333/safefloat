#ifndef BOOST_SAFE_FLOAT_POLICY_CHECK_COMPOSERS_HPP
#define BOOST_SAFE_FLOAT_POLICY_CHECK_COMPOSERS_HPP

#include <algorithm>

#include <boost/safe_float/policy/check_base_policy.hpp>
#include <boost/safe_float/policy/policy_traits.hpp>
#include <boost/safe_float/utility.hpp>
// this file define composers for policies

namespace boost
{
namespace safe_float
{
namespace policy
{
// check_composer
template<class FP, template<class> class... As>
class composed_check : private As<FP>...
{
    // TODO add static check for As to be va;id check Policies.
    friend policy_traits<FP, composed_check, true>;

public:
    // operator+
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
        // TODO: find out the message from the policy broken
        return std::string("Policy broken when adding");
    }

    // operator-
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
        // TODO: find out the message from the policy broken
        return std::string("Policy broken when subtracting");
    }

    // operator*
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
        // TODO: find out the message from the policy broken
        return std::string("Policy broken when multiplying");
    }

    // operator/
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
        // TODO: find out the message from the policy broken
        return std::string("Policy broken when dividing");
    }
};

template<template<typename> typename... POLICIES>
struct compose_check {
    template<typename FP>
    using policy = flattener<composed_check>::template compose_flat<FP, POLICIES...>;
};

// Specialization of policy_traits to report the correct message
template<typename FP, template<typename> typename... As>
class policy_traits<FP, composed_check<FP, As...>, true> : public policy_traits<FP, composed_check<FP, As...>, false>
{
    using Policy = composed_check<FP, As...>;
    using parent = policy_traits<FP, composed_check<FP, As...>, false>;

public:
#define BOOST_SAFE_FLOAT_POLICY_REPORT_PRE_CHECK_ERROR(operation)                                  \
    template<typename ERROR_HANDLING>                                                              \
    static void report_pre_##operation(Policy& p, FP const& lhs, FP const& rhs, ERROR_HANDLING& e) \
    {                                                                                              \
        if constexpr (parent::has_pre_##operation##_check())                                       \
        {                                                                                          \
            (                                                                                      \
                [&]() {                                                                            \
                    auto& pol = static_cast<As<FP>&>(p);                                           \
                    policy_traits<FP, As<FP>>::report_pre_##operation(pol, lhs, rhs, e);           \
                }(),                                                                               \
                ...);                                                                              \
        }                                                                                          \
    }

    BOOST_SAFE_FLOAT_POLICY_REPORT_PRE_CHECK_ERROR(addition)
    BOOST_SAFE_FLOAT_POLICY_REPORT_PRE_CHECK_ERROR(subtraction)
    BOOST_SAFE_FLOAT_POLICY_REPORT_PRE_CHECK_ERROR(multiplication)
    BOOST_SAFE_FLOAT_POLICY_REPORT_PRE_CHECK_ERROR(division)

#undef BOOST_SAFE_FLOAT_POLICY_REPORT_PRE_CHECK_ERROR

#define BOOST_SAFE_FLOAT_POLICY_REPORT_POST_CHECK_ERROR(operation)                        \
    template<typename ERROR_HANDLING>                                                     \
    static void report_post_##operation(Policy& p, FP const& value, ERROR_HANDLING& e)    \
    {                                                                                     \
        if constexpr (parent::has_pre_##operation##_check())                              \
        {                                                                                 \
            (                                                                             \
                [&]() {                                                                   \
                    auto& pol = static_cast<As<FP>&>(p);                                  \
                    policy_traits<FP, As<FP>>::report_post_##operation(pol, value, e); \
                }(),                                                                      \
                ...);                                                                     \
        }                                                                                 \
    }

    BOOST_SAFE_FLOAT_POLICY_REPORT_POST_CHECK_ERROR(addition)
    BOOST_SAFE_FLOAT_POLICY_REPORT_POST_CHECK_ERROR(subtraction)
    BOOST_SAFE_FLOAT_POLICY_REPORT_POST_CHECK_ERROR(multiplication)
    BOOST_SAFE_FLOAT_POLICY_REPORT_POST_CHECK_ERROR(division)

#undef BOOST_SAFE_FLOAT_POLICY_REPORT_POST_CHECK_ERROR
};

// on_fail_composer

// cast_composer
template<typename FP, template<typename> typename... As>
struct composed_cast
{
    template<typename T>
    static constexpr bool can_cast_from = (As<FP>::template can_cast_from<T> || ...);

    template<typename T>
    static constexpr bool can_explicitly_cast_from = (As<FP>::template can_explicitly_cast_from<T> || ...)
                                                     && !can_cast_from<T>;

    template<typename T>
    static constexpr bool can_cast_to = (As<FP>::template can_cast_to<T> || ...);

    template<typename T>
    static constexpr bool can_explicitly_cast_to = (As<FP>::template can_explicitly_cast_to<T> || ...)
                                                   && !can_cast_to<T>;
};

template<template<typename> typename... POLICIES>
struct compose_cast {
    template<typename FP>
    using policy = flattener<composed_cast>::template compose_flat<FP, POLICIES...>;
};

namespace helper
{
// TODO Put in order
template<typename FP, template<typename> typename...>
struct construct_implicit;

template<typename FP, template<typename> typename FIRST, template<typename> typename... REST>
struct construct_implicit<FP, FIRST, REST...>
{
    template<typename T>
    static void construct(FP& target, T source)
    {
        if constexpr (FIRST<FP>::template can_cast_from<T>)
        {
            cast_helper<FP, FIRST<FP>>::template construct_implicitly(target, source);
        }
        else
        {
            construct_implicit<FP, REST...>::construct(target, source);
        }
    }
};

template<typename FP>
struct construct_implicit<FP>
{
    template<typename T>
    static void construct(FP& target, T source)
    {}
};


template<typename FP, template<typename> typename...>
struct construct_explicit;

template<typename FP, template<typename> typename FIRST, template<typename> typename... REST>
struct construct_explicit<FP, FIRST, REST...>
{
    template<typename T>
    static void construct(FP& target, T source)
    {
        if constexpr (FIRST<FP>::template can_cast_from<T> || FIRST<FP>::template can_explicitly_cast_from<T>)
        {
            cast_helper<FP, FIRST<FP>>::template construct_explicitly(target, source);
        }
        else
        {
            construct_explicit<FP, REST...>::construct(target, source);
        }
    }
};

template<typename FP>
struct construct_explicit<FP>
{
    template<typename T>
    static void construct(FP& target, T source)
    {}
};
} // namespace helper

template<typename FP, template<typename> typename... As>
struct cast_helper<FP, composed_cast<FP, As...>>
{
    using CAST_POLICY = composed_cast<FP, As...>;

    template<typename T, std::enable_if_t<CAST_POLICY::template can_cast_from<T>, int> = 0>
    static void construct_implicitly(FP& target, T source)
    {
        helper::construct_implicit<FP, As...>::template construct<T>(target, source);
    }
    template<typename T, std::enable_if_t<CAST_POLICY::template can_explicitly_cast_from<T>, int> = 0>
    static void construct_explicitly(FP& target, T source)
    {
        helper::construct_explicit<FP, As...>::template construct<T>(target, source);
    }

    template<typename T, std::enable_if_t<CAST_POLICY::template can_cast_from<T>, int> = 0>
    static T convert_implicitly(FP source)
    {
        CAST_POLICY::template cast_to<T>(source);
    }
    template<typename T, std::enable_if_t<CAST_POLICY::template can_explicitly_cast_from<T>, int> = 0>
    static T convert_explicitly(FP source)
    {
        CAST_POLICY::template cast_to<T>(source);
    }
};

} // namespace policy
} // namespace safe_float
} // namespace boost


#endif // BOOST_SAFE_FLOAT_POLICY_CHECK_COMPOSERS_HPP
