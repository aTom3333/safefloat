#ifndef BOOST_SAFE_FLOAT_UTILITY_HPP
#define BOOST_SAFE_FLOAT_UTILITY_HPP

#include <boost/safe_float/policy/policy_composers.hpp>

#include <type_traits>


namespace boost
{
namespace safe_float
{
namespace policy
{
template<template<typename...> typename A, template<typename...> typename B>
struct is_same_template : std::false_type
{};

template<template<typename> typename A>
struct is_same_template<A, A> : std::true_type
{};


template<template<typename> typename... Ts>
struct flattened
{
    template<template<typename> typename T>
    using append = std::conditional_t<(is_same_template<Ts, T>::value || ...), flattened, flattened<Ts..., T>>;
};

template<typename FP, typename T>
struct compose_helper;

template<typename FP, template<typename> typename... As>
struct compose_helper<FP, flattened<As...>> : compose_check<FP, As...>
{};

template<typename, typename...>
struct flatten;

template<typename FLAT>
struct flatten<FLAT>
{
    using type = FLAT;
};

template<template<typename> typename... FLAT, typename FP, template<typename> typename FIRST, typename... REST>
struct flatten<flattened<FLAT...>, FIRST<FP>, REST...>
{
    using type = typename flatten<typename flattened<FLAT...>::template append<FIRST>, REST...>::type;
};

template<template<typename> typename... POLICIES>
struct composer_helper
{
    template<typename FP>
    using composed_type = compose_check<FP, POLICIES...>;
};

template<template<typename> typename... FLAT, typename FP, template<typename> typename... CONTENT, typename... REST>
struct flatten<flattened<FLAT...>, compose_check<FP, CONTENT...>, REST...>
{
    using type = typename flatten<flattened<FLAT...>, CONTENT<FP>..., REST...>::type;
};

template<typename T>
struct flatten_composed : T
{};


template<typename FP, template<typename> typename... As>
struct flatten_composed<compose_check<FP, As...>>
{
    using type = typename flatten<flattened<>, As<FP>...>::type;
};


} // namespace policy
} // namespace safe_float
} // namespace boost

#endif // BOOST_SAFE_FLOAT_UTILITY_HPP