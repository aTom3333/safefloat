#ifndef BOOST_SAFE_FLOAT_CAST_FROM_PRIMITIVE_HPP
#define BOOST_SAFE_FLOAT_CAST_FROM_PRIMITIVE_HPP

#include <limits>
#include <type_traits>


namespace boost
{
namespace safe_float
{
namespace policy
{
namespace cast_from_primitive
{
template<typename FP, bool enable_other_explicitly = false, std::enable_if_t<std::is_floating_point_v<FP>, int> = 0>
struct none_impl
{
    template<typename T>
    static constexpr bool can_cast_from = false;

    template<typename T>
    static constexpr bool can_explicitly_cast_from = enable_other_explicitly;

    template<typename T>
    static constexpr bool can_cast_to = false;

    template<typename T>
    static constexpr bool can_explicitly_cast_to = false;
};

template<typename FP>
using none = none_impl<FP>;
template<typename FP>
using none_with_explicit = none_impl<FP, true>;

template<typename FP, bool enable_other_explicitly = false, std::enable_if_t<std::is_floating_point_v<FP>, int> = 0>
struct same_impl
{
    template<typename T>
    static constexpr bool can_cast_from = std::is_same_v<FP, T>;

    template<typename T>
    static constexpr bool can_explicitly_cast_from =
        std::is_floating_point_v<T>&& enable_other_explicitly && !can_cast_from<T>;

    template<typename T>
    static void cast_from(FP& target, T source)
    {
        target = source;
    }

    template<typename T>
    static constexpr bool can_cast_to = false;

    template<typename T>
    static constexpr bool can_explicitly_cast_to = false;
};

template<typename FP>
using same = same_impl<FP>;
template<typename FP>
using same_with_explicit = same_impl<FP, true>;

template<typename FP, bool enable_other_explicitly = false, std::enable_if_t<std::is_floating_point_v<FP>, int> = 0>
struct more_precise_impl
{
    template<typename T>
    static constexpr bool can_cast_from =
        std::is_floating_point_v<T>&& std::numeric_limits<T>::digits >= std::numeric_limits<FP>::digits;

    template<typename T>
    static constexpr bool can_explicitly_cast_from =
        std::is_floating_point_v<T>&& enable_other_explicitly && !can_cast_from<T>;

    template<typename T>
    static void cast_from(FP& target, T source)
    {
        target = source;
    }

    template<typename T>
    static constexpr bool can_cast_to = false;

    template<typename T>
    static constexpr bool can_explicitly_cast_to = false;
};

template<typename FP>
using more_precise = more_precise_impl<FP>;
template<typename FP>
using more_precise_with_explicit = more_precise_impl<FP, true>;

template<typename FP, bool enable_other_explicitly = false, std::enable_if_t<std::is_floating_point_v<FP>, int> = 0>
struct less_precise_impl
{
    template<typename T>
    static constexpr bool can_cast_from =
        std::is_floating_point_v<T>&& std::numeric_limits<T>::digits <= std::numeric_limits<FP>::digits;

    template<typename T>
    static constexpr bool can_explicitly_cast_from =
        std::is_floating_point_v<T>&& enable_other_explicitly && !can_cast_from<T>;

    template<typename T>
    static void cast_from(FP& target, T source)
    {
        target = source;
    }

    template<typename T>
    static constexpr bool can_cast_to = false;

    template<typename T>
    static constexpr bool can_explicitly_cast_to = false;
};

template<typename FP>
using less_precise = less_precise_impl<FP>;
template<typename FP>
using less_precise_with_explicit = less_precise_impl<FP, true>;

template<typename FP, bool enable_other_explicitly = false, std::enable_if_t<std::is_floating_point_v<FP>, int> = 0>
struct all_impl
{
    template<typename T>
    static constexpr bool can_cast_from = std::is_floating_point_v<T>;

    template<typename T>
    static constexpr bool can_explicitly_cast_from = false;

    template<typename T>
    static void cast_from(FP& target, T source)
    {
        target = source;
    }

    template<typename T>
    static constexpr bool can_cast_to = false;

    template<typename T>
    static constexpr bool can_explicitly_cast_to = false;
};

template<typename FP>
using all = all_impl<FP>;
template<typename FP>
using all_with_explicit = all_impl<FP, true>;
} // namespace cast_from_primitive

} // namespace policy
} // namespace safe_float
} // namespace boost

#endif // BOOST_SAFE_FLOAT_CAST_FROM_PRIMITIVE_HPP