#ifndef BOOST_SAFE_FLOAT_UTILITY_HPP
#define BOOST_SAFE_FLOAT_UTILITY_HPP

#include <type_traits>


namespace boost
{
namespace safe_float
{

template<typename, template<typename> typename, typename, template<typename> typename>
class safe_float;

template<typename T>
struct is_safe_float : std::false_type {};

template<typename SF, template<typename> typename CHECK, typename REPORT, template<typename> typename CAST>
struct is_safe_float<safe_float<SF, CHECK, REPORT, CAST>> : std::true_type {};


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

template<typename>
struct is_composed : std::false_type {};

template<template<typename, template<typename> typename...> typename COMP, typename FP, template<typename> typename... SUBPOLS>
struct is_composed<COMP<FP, SUBPOLS...>> : std::true_type {};

template<typename, typename, typename enabled = void>
struct is_subset : std::false_type {};

template<typename POL>
struct is_subset<POL, POL, std::enable_if_t<!is_composed<POL>::value>> : std::true_type {};

template<typename FP1, typename FP2, template<typename> typename POL>
struct is_subset<POL<FP1>, POL<FP2>, void> : std::true_type {};

template<typename FP1, typename FP2, template<typename> typename POL, template<typename, template<typename> typename...> typename COMP, template<typename> typename... SUBPOLS>
struct is_subset<POL<FP1>, COMP<FP2, POL, SUBPOLS...>, void> : std::true_type {};

template<typename FP1, typename FP2, template<typename> typename POL, template<typename, template<typename> typename...> typename COMP>
struct is_subset<POL<FP1>, COMP<FP2>, void> : std::false_type {};

template<typename FP1, typename FP2, template<typename> typename POL, template<typename, template<typename> typename...> typename COMP, template<typename> typename FIRST, template<typename> typename... REST>
struct is_subset<POL<FP1>, COMP<FP2, FIRST, REST...>, void> : is_subset<POL<FP1>, COMP<FP2, REST...>> {};

template<typename FP, template<typename, template<typename> typename...> typename COMP, template<typename> typename... SUBPOLS, typename OTHER>
struct is_subset<COMP<FP, SUBPOLS...>, OTHER, void> : std::conditional_t<(is_subset<SUBPOLS<FP>, OTHER>::value && ...), std::true_type, std::false_type> {};

template<typename POL1, typename POL2>
using is_equivalent = std::conditional_t<is_subset<POL1, POL2>::value && is_subset<POL2, POL1>::value, std::true_type, std::false_type>;


} // namespace policy
} // namespace safe_float
} // namespace boost

#endif // BOOST_SAFE_FLOAT_UTILITY_HPP