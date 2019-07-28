#ifndef BOOST_SAFE_FLOAT_CAST_TO_PRIMITIVE_HPP
#define BOOST_SAFE_FLOAT_CAST_TO_PRIMITIVE_HPP

#include <limits>
#include <type_traits>
#include <boost/safe_float/utility.hpp>

namespace boost
{
namespace safe_float
{
namespace policy
{
namespace cast_from_primitive
{
template<typename SF, bool is_explicit = false, std::enable_if_t<is_safe_float<SF>::value, int> = 0>
struct none_impl
{
    template<typename T>
    static constexpr bool can_cast_from = false;

    template<typename T>
    static constexpr bool can_explicitly_cast_from = false;

    template<typename T>
    static constexpr bool can_cast_to = false;

    template<typename T>
    static constexpr bool can_explicitly_cast_to = false;

    template<typename T>
    static T cast_to(typename SF::value_type source)
    {
        return T(source);
    }
};

template<typename SF>
using none = none_impl<SF, true>;
template<typename SF>
using none_explicit = none_impl<SF, false>;

template<typename SF, bool is_explicit = false, std::enable_if_t<is_safe_float<SF>::value, int> = 0>
struct same_impl
{
    template<typename T>
    static constexpr bool can_cast_from = false;

    template<typename T>
    static constexpr bool can_explicitly_cast_from = false;

    template<typename T>
    static constexpr bool can_cast_to = !is_explicit&& std::is_same_v<typename SF::value_type, T>;

    template<typename T>
    static constexpr bool can_explicitly_cast_to = is_explicit&& std::is_same_v<typename SF::value_type, T>;

    template<typename T>
    static T cast_to(typename SF::value_type source)
    {
        return T(source);
    }
};

template<typename SF>
using same = same_impl<SF>;
template<typename SF>
using same_explicit = same_impl<SF, true>;

template<typename SF, bool is_explicit = false, std::enable_if_t<is_safe_float<SF>::value, int> = 0>
struct more_precise_impl
{
    template<typename T>
    static constexpr bool can_cast_from = false;

    template<typename T>
    static constexpr bool can_explicitly_cast_from = false;

    template<typename T>
    static constexpr bool can_cast_to = !is_explicit&& std::is_floating_point_v<T>&& std::numeric_limits<T>::digits
                                                                                    >= std::numeric_limits<typename SF::value_type>::digits;

    template<typename T>
    static constexpr bool can_explicitly_cast_to = is_explicit&& std::is_floating_point_v<T>&& std::numeric_limits<T>::digits
                                                                                                >= std::numeric_limits<typename SF::value_type>::digits;

    template<typename T>
    static T cast_to(typename SF::value_type source)
    {
        return T(source);
    }
};

template<typename SF>
using more_precise = more_precise_impl<SF>;
template<typename SF>
using more_precise_explicit = more_precise_impl<SF, true>;

template<typename SF, bool is_explicit = false, std::enable_if_t<is_safe_float<SF>::value, int> = 0>
struct less_precise_impl
{
    template<typename T>
    static constexpr bool can_cast_from = false;

    template<typename T>
    static constexpr bool can_explicitly_cast_from = false;

    template<typename T>
    static constexpr bool can_cast_to = !is_explicit&& std::is_floating_point_v<T>&& std::numeric_limits<T>::digits
                                                                                    <= std::numeric_limits<typename SF::value_type>::digits;

    template<typename T>
    static constexpr bool can_explicitly_cast_to = is_explicit&& std::is_floating_point_v<T>&& std::numeric_limits<T>::digits
                                                                                                <= std::numeric_limits<typename SF::value_type>::digits;

    template<typename T>
    static T cast_to(typename SF::value_type source)
    {
        return T(source);
    }
};

template<typename SF>
using less_precise = less_precise_impl<SF>;
template<typename SF>
using less_precise_explicit = less_precise_impl<SF, true>;

template<typename SF, bool is_explicit = false, std::enable_if_t<is_safe_float<SF>::value, int> = 0>
struct all_impl
{
    template<typename T>
    static constexpr bool can_cast_from = false;

    template<typename T>
    static constexpr bool can_explicitly_cast_from = false;

    template<typename T>
    static constexpr bool can_cast_to = !is_explicit && std::is_floating_point_v<T>;

    template<typename T>
    static constexpr bool can_explicitly_cast_to = is_explicit && std::is_floating_point_v<T>;
    
    template<typename T>
    static T cast_to(typename SF::value_type source)
    {
        return T(source);
    }
};

template<typename SF>
using all = all_impl<SF>;
template<typename SF>
using all_explicit = all_impl<SF, true>;
} // namespace cast_from_primitive

} // namespace policy
} // namespace safe_float
} // namespace boost

#endif // BOOST_SAFE_FLOAT_CAST_TO_PRIMITIVE_HPP