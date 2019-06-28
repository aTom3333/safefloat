#ifndef BOOST_SAFE_FLOAT_UTILITY_HPP
#define BOOST_SAFE_FLOAT_UTILITY_HPP

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

template<template<typename, template<typename> typename...> typename COMPOSED>
struct flattener
{
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

    template<template<typename> typename... FLAT, typename FP, template<typename> typename... CONTENT, typename... REST>
    struct flatten<flattened<FLAT...>, COMPOSED<FP, CONTENT...>, REST...>
    {
        using type = typename flatten<flattened<FLAT...>, CONTENT<FP>..., REST...>::type;
    };

    template<typename T>
    struct flatten_composed
    {
        using type = T;
    };

    template<typename FP, template<typename> typename... As>
    struct flatten_composed<COMPOSED<FP, As...>>
    {
        using type = typename flatten<flattened<>, As<FP>...>::type;
    };

    template<typename, typename>
    struct composed_helper;

    template<typename FP, template<typename> typename... As>
    struct composed_helper<FP, flattened<As...>>
    {
        using type = COMPOSED<FP, As...>;
    };

    template<typename FP, template<typename> typename... As>
    using compose_flat = typename composed_helper<FP, typename flatten_composed<COMPOSED<FP, As...>>::type>::type;
};


} // namespace policy
} // namespace safe_float
} // namespace boost

#endif // BOOST_SAFE_FLOAT_UTILITY_HPP