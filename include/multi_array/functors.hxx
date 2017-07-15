#pragma once

#include <cmath>

#include "multi_array/meta/promote_real.hxx"
#include "multi_array/meta/promote_type.hxx"



namespace multi_array{

    namespace functors{

    // unary in-place functors
    template<class T>
    struct Negative { 
        void operator()(T& x) { x = -x; } 
    };

    template<class T>
    struct PrefixIncrement { 
        void operator()(T& x) { ++x; } 
    };

    template<class T>
    struct PostfixIncrement { 
        void operator()(T& x) { x++; } 
    };

    template<class T>
    struct PrefixDecrement { 
        void operator()(T& x) { --x; } 
    };

    template<class T>
    struct PostfixDecrement { 
        void operator()(T& x) { x--; } 
    };

    // binary in-place functors
    template<class T1, class T2>
    struct Assign { 
        void operator()(T1& x, const T2& y) { x = y; } 
    };

    template<class T1, class T2>
    struct PlusEqual { 
        static void op(T1& x, const T2& y) { x += y; } 
        void operator()(T1& x, const T2& y) { x += y; } 
    };

    template<class T1, class T2>
    struct MinusEqual { 
        static void op(T1& x, const T2& y) { x -= y; } 
        void operator()(T1& x, const T2& y) { x -= y; } 
    };

    template<class T1, class T2>
    struct TimesEqual { 
        static void op(T1& x, const T2& y) { x *= y; } 
        void operator()(T1& x, const T2& y) { x *= y; } 
    };

    template<class T1, class T2>
    struct DividedByEqual { 
        static void op(T1& x, const T2& y) { x /= y; } 
        void operator()(T1& x, const T2& y) { x /= y; } 
    };

    template<class T1, class T2>
    struct OrEqual { 
        static void op(T1& x, const T2& y) { x |= y; } 
        void operator()(T1& x, const T2& y) { x |= y; } 
    };


    template<class T1, class T2>
    struct AndEqual { 
        static void op(T1& x, const T2& y) { x &= y; } 
        void operator()(T1& x, const T2& y) { x &= y; } 
    };

    // binary functors
    template<class T1, class T2>
    struct Plus { 
        typedef typename meta::PromoteType<T1, T2>::type type;
        static type op(const T1& x, const T2& y){ return x + y; } 
        type operator()(const T1& x, const T2& y) const { return x + y; } 
    };

    template<class T1, class T2>
    struct Minus { 
        typedef typename meta::PromoteType<T1, T2>::type type;
        static type op(const T1& x, const T2& y){ return x - y; } 
        type operator()(const T1& x, const T2& y) const { return x - y; } 
    };

    template<class T1, class T2>
    struct Times { 
        typedef typename meta::PromoteType<T1, T2>::type type;
        static type op(const T1& x, const T2& y){ return x * y; } 
        type operator()(const T1& x, const T2& y) const { return x * y; } 
    };

    template<class T1, class T2>
    struct DividedBy {
        typedef typename meta::PromoteType<T1, T2>::type type;
        static type op(const T1& x, const T2& y){ return x / y; } 
        type operator()(const T1& x, const T2& y) const { return x / y; } 
    };

    template<class T1, class T2>
    struct Or { 
        typedef typename meta::PromoteType<T1, T2>::type type;
        static type op(const T1& x, const T2& y){ return x || y; } 
        type operator()(const T1& x, const T2& y) const { return x || y; } 
    };

    template<class T1, class T2>
    struct And { 
        typedef typename meta::PromoteType<T1, T2>::type type;
        static type op(const T1& x, const T2& y){ return x && y; } 
        type operator()(const T1& x, const T2& y) const { return x && y; } 
    };



    template<class T, class EXPONENT_TYPE>
    struct Pow { 
        typedef typename multi_array::meta::PromoteRealType<T,EXPONENT_TYPE>::type type;
        typedef Pow<T,EXPONENT_TYPE> SelfType;

        static type op(const T& x, const EXPONENT_TYPE& exponent){ 
            return std::pow(x, exponent);
        } 
        type operator()(const T& x, const EXPONENT_TYPE& exponent) const { 
            return SelfType::op(x, exponent);
        } 
    };

    template<class T, int64_t EXPONENT>
    struct PowN { 
        typedef typename multi_array::meta::PromoteRealType<T,int64_t>::type type;
        typedef PowN<T,EXPONENT> SelfType;

        static type op(const T& x){ 
            return std::pow(x, EXPONENT);
        } 
        type operator()(const T& x) const { 
            return SelfType::op(x, EXPONENT);
        } 
    };

    template<class T>
    using Pow2 = PowN<T,2>;
    template<class T>
    using Pow3 = PowN<T,3>;




    template<class T>
    struct Cast{
        typedef T type;

        template<class U>
        static type op(U && val){ return static_cast<U>(val); } 

        template<class U>
        type operator()(U && val) const { return static_cast<U>(val); } 
    };


    // UNARY MATH FUNCTIONS WICH HAVE
    // ALL THE SAME RESULT TYPE
    #define GEN_MATH_FUNCTOR(CLASS_NAME, FUNCTION_NAME)\
    template<class T>\
    struct CLASS_NAME{\
        typedef typename meta::PromoteReal<T>::type type;\
        type operator()(const T& value)const{\
            return FUNCTION_NAME(value);\
        }\
    };

    // Trigonometric functions
    GEN_MATH_FUNCTOR(Cos,           std::cos);
    GEN_MATH_FUNCTOR(Sin,           std::sin);
    GEN_MATH_FUNCTOR(Tan,           std::tan);
    GEN_MATH_FUNCTOR(Acos,          std::acos);
    GEN_MATH_FUNCTOR(Asin,          std::asin);
    GEN_MATH_FUNCTOR(Atan,          std::atan);
    // Hyperbolic functions
    GEN_MATH_FUNCTOR(Cosh,           std::cosh);
    GEN_MATH_FUNCTOR(Sinh,           std::sinh);
    GEN_MATH_FUNCTOR(Tanh,           std::tanh);
    GEN_MATH_FUNCTOR(Acosh,          std::acosh);
    GEN_MATH_FUNCTOR(Asinh,          std::asinh);
    GEN_MATH_FUNCTOR(Atanh,          std::atanh);
    // Exponential and logarithmic functions
    GEN_MATH_FUNCTOR(Exp,       std::exp);
    GEN_MATH_FUNCTOR(Log,       std::log);
    GEN_MATH_FUNCTOR(Log10,     std::log10);
    GEN_MATH_FUNCTOR(Exp2,      std::exp2);
    //Power functions
    GEN_MATH_FUNCTOR(Sqrt,      std::sqrt);
    //GEN_MATH_FUNCTOR(Cqrt,      std::cqrt);
    //Error and gamma functions
    GEN_MATH_FUNCTOR(Erf,       std::erf);
    GEN_MATH_FUNCTOR(Erfc,      std::erfc);
    GEN_MATH_FUNCTOR(Tgamma,    std::tgamma);
    GEN_MATH_FUNCTOR(Lgamma,    std::lgamma);
    //Rounding and remainder functions
    GEN_MATH_FUNCTOR(Ceol,      std::ceil);
    GEN_MATH_FUNCTOR(Floor,     std::floor);
    GEN_MATH_FUNCTOR(Trunc,     std::trunc);
    GEN_MATH_FUNCTOR(Round,     std::round);
    GEN_MATH_FUNCTOR(Lround,    std::lround);
    GEN_MATH_FUNCTOR(Rint,      std::rint);
    //Other functions
    GEN_MATH_FUNCTOR(Abs,      std::abs);
    GEN_MATH_FUNCTOR(Fabs,     std::fabs);
    #undef GEN_MATH_FUNCTOR




    template<class SCALAR_LIKE, class BINARY_FUNCTOR>
    struct BindScalarLeft{
        typedef typename BINARY_FUNCTOR::type type;
        BindScalarLeft(
            const SCALAR_LIKE & scalarLike = SCALAR_LIKE(),
            const BINARY_FUNCTOR & binaryFunctor = BINARY_FUNCTOR()
        )
        :   scalarLike_(scalarLike),
            binaryFunctor_(binaryFunctor){
        }
        template<class VALUE>
        type operator()(VALUE && value)const{
            return binaryFunctor_(scalarLike_, value);
        }

        SCALAR_LIKE scalarLike_;
        BINARY_FUNCTOR binaryFunctor_;
    };

    template<class SCALAR_LIKE, class BINARY_FUNCTOR>
    struct BindScalarRight{
        typedef typename BINARY_FUNCTOR::type type;
        BindScalarRight(
            const SCALAR_LIKE & scalarLike = SCALAR_LIKE(),
            const BINARY_FUNCTOR & binaryFunctor = BINARY_FUNCTOR()
        )
        :   scalarLike_(scalarLike),
            binaryFunctor_(binaryFunctor){
        }
        template<class VALUE>
        type operator()(VALUE && value)const{
            return binaryFunctor_(value, scalarLike_);
        }

        SCALAR_LIKE scalarLike_;
        BINARY_FUNCTOR binaryFunctor_;
    };



    template< template<class, class> class FUNCTOR>
    struct FunctorGenerator{
        template<class T1, class T2>
        struct Generate{
            typedef FUNCTOR<T1, T2> type;
        };
    };





    template<
        class OUTER_UNARY_FUNCTOR,
        class INNER_UNARY_FUNCTOR
    >
    struct ChainedUnaryFunctors{
        ChainedUnaryFunctors(
            const OUTER_UNARY_FUNCTOR outerUnaryFunctor,
            const INNER_UNARY_FUNCTOR innerUnaryFunctor
        )
        :   outerUnaryFunctor_(outerUnaryFunctor),
            innerUnaryFunctor_(innerUnaryFunctor){
        }
        typedef typename OUTER_UNARY_FUNCTOR::type type;
        template<class ARG>
        type operator()(
            ARG && arg
        )const{

        }
        const OUTER_UNARY_FUNCTOR outerUnaryFunctor_;
        const INNER_UNARY_FUNCTOR innerUnaryFunctor_;
    };

    




    }
}