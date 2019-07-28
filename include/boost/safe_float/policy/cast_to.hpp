#ifndef BOOST_SAFE_FLOAT_CAST_TO_HPP
#define BOOST_SAFE_FLOAT_CAST_TO_HPP

#include <type_traits>

namespace boost
{
namespace safe_float
{
namespace policy
{
    template<typename T>
    struct allow_cast_to {
        template<typename SF>
        struct policy {
            template<typename U>
            static constexpr bool can_cast_from = false;

            template<typename U>
            static constexpr bool can_explicitly_cast_from = false;

            template<typename U>
            static constexpr bool can_cast_to = std::is_same_v<T, U>;

            template<typename U>
            static constexpr bool can_explicitly_cast_to = false;

            template<typename U, std::enable_if_t<std::is_same_v<T, U>, int> = 0>
            static T cast_to(typename SF::value_type source)
            {
                return T(source);
            }
        };
    };


    template<typename T>
    struct allow_explicit_cast_to {
        template<typename SF>
        struct policy {
            template<typename U>
            static constexpr bool can_cast_from = false;

            template<typename U>
            static constexpr bool can_explicitly_cast_from = false;

            template<typename U>
            static constexpr bool can_cast_to = false;

            template<typename U>
            static constexpr bool can_explicitly_cast_to = std::is_same_v<T, U>;

            template<typename U, std::enable_if_t<std::is_same_v<T, U>, int> = 0>
            static T cast_to(typename SF::value_type source)
            {
                return T(source);
            }
        };
    };
}
} // namespace safe_float
} // namespace boost


#endif // BOOST_SAFE_FLOAT_CAST_TO_HPP