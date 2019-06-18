#ifndef BOOST_SAFE_FLOAT_POLICY_TRAITS_HPP
#define BOOST_SAFE_FLOAT_POLICY_TRAITS_HPP


#include <type_traits>


namespace boost
{
namespace safe_float
{
namespace policy
{
#define BOOST_SAFE_FLOAT_EVERY_PRE_CHECK(OP) \
    OP(pre_addition_check)                   \
    OP(pre_subtraction_check)                \
    OP(pre_multiplication_check)             \
    OP(pre_division_check)

#define BOOST_SAFE_FLOAT_EVERY_POST_CHECK(OP) \
    OP(post_addition_check)                   \
    OP(post_subtraction_check)                \
    OP(post_multiplication_check)             \
    OP(post_division_check)

#define BOOST_SAFE_FLOAT_EVERY_CHECK(OP) \
    BOOST_SAFE_FLOAT_EVERY_PRE_CHECK(OP) \
    BOOST_SAFE_FLOAT_EVERY_POST_CHECK(OP)

namespace detection
{
template<typename FP, typename Policy, template<typename, typename> typename Capacity, typename = std::void_t<>>
struct detect : std::false_type
{};

template<typename FP, typename Policy, template<typename, typename> typename Capacity>
struct detect<FP, Policy, Capacity, std::void_t<Capacity<FP, Policy>>> : std::true_type
{};

#define BOOST_SAFE_FLOAT_TEST_PRE_CHECK_CAPACITY_TEMPLATE(capacity) \
    template<typename FP, typename Policy>                          \
    using has_##capacity = decltype(std::declval<Policy>().capacity(std::declval<FP>(), std::declval<FP>()));

BOOST_SAFE_FLOAT_EVERY_PRE_CHECK(BOOST_SAFE_FLOAT_TEST_PRE_CHECK_CAPACITY_TEMPLATE)

#undef BOOST_SAFE_FLOAT_TEST_PRE_CHECK_CAPACITY_TEMPLATE

#define BOOST_SAFE_FLOAT_TEST_POST_CHECK_CAPACITY_TEMPLATE(capacity) \
    template<typename FP, typename Policy>                           \
    using has_##capacity = decltype(std::declval<Policy>().capacity(std::declval<FP>()));

BOOST_SAFE_FLOAT_EVERY_POST_CHECK(BOOST_SAFE_FLOAT_TEST_POST_CHECK_CAPACITY_TEMPLATE)

#undef BOOST_SAFE_FLOAT_TEST_POST_CHECK_CAPACITY_TEMPLATE

} // namespace detection


template<typename Fp, typename Policy, bool specialized = true>
class policy_traits
{
public:
#define BOOST_SAFE_FLOAT_TEST_POLICY_CAPACITY(capacity)                         \
    static constexpr bool has_##capacity() noexcept                             \
    {                                                                           \
        return detection::detect<Fp, Policy, detection::has_##capacity>::value; \
    }

    BOOST_SAFE_FLOAT_EVERY_CHECK(BOOST_SAFE_FLOAT_TEST_POLICY_CAPACITY)

#undef BOOST_SAFE_FLOAT_TEST_POLICY_CAPACITY

#define BOOST_SAFE_FLOAT_POLICY_DO_PRE_CHECK(capacity)                                         \
    static bool pre_##capacity##_check(Policy& p, Fp const& lhs, Fp const& rhs)                \
    {                                                                                          \
        if constexpr (has_pre_##capacity##_check()) return p.pre_##capacity##_check(lhs, rhs); \
        return true;                                                                           \
    }

    BOOST_SAFE_FLOAT_POLICY_DO_PRE_CHECK(addition)
    BOOST_SAFE_FLOAT_POLICY_DO_PRE_CHECK(subtraction)
    BOOST_SAFE_FLOAT_POLICY_DO_PRE_CHECK(multiplication)
    BOOST_SAFE_FLOAT_POLICY_DO_PRE_CHECK(division)

#undef BOOST_SAFE_FLOAT_POLICY_DO_PRE_CHECK

#define BOOST_SAFE_FLOAT_POLICY_DO_POST_CHECK(capacity)                                       \
    static bool post_##capacity##_check(Policy& p, Fp const& value)                           \
    {                                                                                         \
        if constexpr (has_post_##capacity##_check()) return p.post_##capacity##_check(value); \
        return true;                                                                          \
    }

    BOOST_SAFE_FLOAT_POLICY_DO_POST_CHECK(addition)
    BOOST_SAFE_FLOAT_POLICY_DO_POST_CHECK(subtraction)
    BOOST_SAFE_FLOAT_POLICY_DO_POST_CHECK(multiplication)
    BOOST_SAFE_FLOAT_POLICY_DO_POST_CHECK(division)

#undef BOOST_SAFE_FLOAT_POLICY_DO_POST_CHECK

#define BOOST_SAFE_FLOAT_POLICY_REPORT_PRE_CHECK_ERROR(operation)                                        \
    template<typename ERROR_HANDLING>                                                                    \
    static void report_pre_##operation(Policy& p, Fp const& lhs, Fp const& rhs, ERROR_HANDLING& e)       \
    {                                                                                                    \
        if constexpr (has_pre_##operation##_check())                                                     \
        {                                                                                                \
            if (!p.pre_##operation##_check(lhs, rhs)) e.report_failure(p.operation##_failure_message()); \
        }                                                                                                \
    }

    BOOST_SAFE_FLOAT_POLICY_REPORT_PRE_CHECK_ERROR(addition)
    BOOST_SAFE_FLOAT_POLICY_REPORT_PRE_CHECK_ERROR(subtraction)
    BOOST_SAFE_FLOAT_POLICY_REPORT_PRE_CHECK_ERROR(multiplication)
    BOOST_SAFE_FLOAT_POLICY_REPORT_PRE_CHECK_ERROR(division)

#undef BOOST_SAFE_FLOAT_POLICY_REPORT_PRE_CHECK_ERROR

#define BOOST_SAFE_FLOAT_POLICY_REPORT_POST_CHECK_ERROR(operation)                                     \
    template<typename ERROR_HANDLING>                                                                  \
    static void report_post_##operation(Policy& p, Fp const& value, ERROR_HANDLING& e)                 \
    {                                                                                                  \
        if constexpr (has_post_##operation##_check())                                                  \
        {                                                                                              \
            if (!p.post_##operation##_check(value)) e.report_failure(p.operation##_failure_message()); \
        }                                                                                              \
    }

    BOOST_SAFE_FLOAT_POLICY_REPORT_POST_CHECK_ERROR(addition)
    BOOST_SAFE_FLOAT_POLICY_REPORT_POST_CHECK_ERROR(subtraction)
    BOOST_SAFE_FLOAT_POLICY_REPORT_POST_CHECK_ERROR(multiplication)
    BOOST_SAFE_FLOAT_POLICY_REPORT_POST_CHECK_ERROR(division)

#undef BOOST_SAFE_FLOAT_POLICY_REPORT_POST_CHECK_ERROR
};

#undef BOOST_SAFE_FLOAT_EVERY_CHECK

} // namespace policy
} // namespace safe_float
} // namespace boost


#endif // BOOST_SAFE_FLOAT_POLICY_TRAITS_HPP