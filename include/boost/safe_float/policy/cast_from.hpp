#ifndef BOOST_SAFE_FLOAT_CAST_FROM_HPP
#define BOOST_SAFE_FLOAT_CAST_FROM_HPP

#include <type_traits>


namespace boost {
namespace safe_float {
namespace policy {

template<typename T>
struct allow_cast_from {
    
    template<typename SF>
    struct policy
    {
        template<typename U>
        static constexpr bool can_cast_from = std::is_same_v<T, U>;

        template<typename U>
        static constexpr bool can_explicitly_cast_from = false;

        template<typename U, std::enable_if_t<std::is_same_v<T, U>, int> = 0>
        static void cast_from(typename SF::value_type& target, T source)
        {
            target = source;
        }

        template<typename U>
        static constexpr bool can_cast_to = false;

        template<typename U>
        static constexpr bool can_explicitly_cast_to = false;
    };
};


template<typename T>
struct allow_explicit_cast_from {

    template<typename SF>
    struct policy
    {
        template<typename U>
        static constexpr bool can_cast_from = false;

        template<typename U>
        static constexpr bool can_explicitly_cast_from = std::is_same_v<T, U>;

        template<typename U, std::enable_if_t<std::is_same_v<T, U>, int> = 0>
        static void cast_from(typename SF::value_type& target, T source)
        {
            target = source;
        }

        template<typename U>
        static constexpr bool can_cast_to = false;

        template<typename U>
        static constexpr bool can_explicitly_cast_to = false;
    };
};

}
}
}


#endif // BOOST_SAFE_FLOAT_CAST_FROM_HPP