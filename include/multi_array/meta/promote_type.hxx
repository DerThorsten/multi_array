#pragma once 

#include <type_traits>

namespace multi_array{
namespace meta{

    /// \cond
    namespace detail_meta{

        template<class T> struct TypeTraits
        { 
            static const unsigned char position = 255; 
        };
        template<> struct TypeTraits<char>
        { 
            static const unsigned char position = 0; 
        };
        template<> struct TypeTraits<unsigned char>
        { 
            static const unsigned char position = 1; 
        };
        template<> struct TypeTraits<short>
        { 
            static const unsigned char position = 2; 
        };
        template<> struct TypeTraits<unsigned short>
        { 
            static const unsigned char position = 3; 
        };
        template<> struct TypeTraits<int>
        { 
            static const unsigned char position = 4; 
        };
        template<> struct TypeTraits<unsigned int>
        { 
            static const unsigned char position = 5; 
        };
        template<> struct TypeTraits<long>
        { 
            static const unsigned char position = 6; 
        };
        template<> struct TypeTraits<unsigned long>
        { 
            static const unsigned char position = 7; 
        };
        template<> struct TypeTraits<float>
        { 
            static const unsigned char position = 8; 
        };
        template<> struct TypeTraits<double>
        { 
            static const unsigned char position = 9; 
        };
        template<> struct TypeTraits<long double>
        { 
            static const unsigned char position = 10; 
        };

    }
    
    ///\endcodn

    template<class A, class B> 
    struct PromoteType
    : public std::conditional<
        detail_meta::TypeTraits<A>::position >= detail_meta::TypeTraits<B>::position,
        A,
        B
    >{ 
 
    };


    template<class A, class B> 
    struct PromoteType<
        std::complex<A>, 
        std::complex<B>

    > : public std::conditional<
        detail_meta::TypeTraits<A>::position >= detail_meta::TypeTraits<B>::position,
        std::complex<A>,
        std::complex<B>
    >{
        
    };


}
}