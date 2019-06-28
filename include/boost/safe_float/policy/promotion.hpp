#ifndef PROMOTION_HPP
#define PROMOTION_HPP


#include <boost/safe_float.hpp>

namespace boost
{
namespace safe_float
{
namespace policy
{
    
template<typename, typename>
struct promotion;

template<typename FP, template<typename> typename CHECK, typename ERROR_HANDLING, template<typename> typename CAST>
struct promotion<safe_float<FP, CHECK, ERROR_HANDLING, CAST>, safe_float<FP, CHECK, ERROR_HANDLING, CAST>> {
    using promoted = safe_float<FP, CHECK, ERROR_HANDLING, CAST>;
    
    static constexpr bool accept_promotion = true;
};

}
} // namespace safe_float
} // namespace boost

#endif // PROMOTION_HPP