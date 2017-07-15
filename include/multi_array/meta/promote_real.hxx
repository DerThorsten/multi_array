#pragma once 

#include <type_traits>


#include "multi_array/meta/promote_type.hxx"

namespace multi_array{
namespace meta{


    ///\cond
    namespace detail_meta{


        template<class T, bool IS_ARITHMETIC, bool IS_FLOATING_POINT>
        struct PromoteRealHelper{
            typedef T type;
        }; 

        template<class T>
        struct PromoteRealHelper<T, true, false>{
            typedef double type;
        }; 
        template<class T>
        struct PromoteRealHelper<T, true, true>{
            typedef T type;
        }; 


    }
    ///\endcond

    template<class T>
    struct PromoteReal :
        public detail_meta::PromoteRealHelper<
            T,
            std::is_arithmetic<T>::value, 
            std::is_floating_point<T>::value
        >
    {
    };


    template<class TA, class TB>
    struct PromoteRealType : public 
        PromoteReal<
            typename PromoteType<TA, TB>::type
        >{
    };



}
}