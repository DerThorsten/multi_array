#pragma once

#include <cmath>

#include "multi_array/meta/promote_real.hxx"
#include "multi_array/meta/promote_type.hxx"
#include "multi_array/misc.hxx"




namespace multi_array{

    namespace functors{

    // unary in-place functors
    template<class T>
    struct Negative { 
        very_inline void operator()(T& x) { x = -x; } 
    };

    template<class T>
    struct PrefixIncrement { 
        very_inline void operator()(T& x) { ++x; } 
    };

    template<class T>
    struct PostfixIncrement { 
        very_inline void operator()(T& x) { x++; } 
    };

    template<class T>
    struct PrefixDecrement { 
        very_inline void operator()(T& x) { --x; } 
    };

    template<class T>
    struct PostfixDecrement { 
        very_inline void operator()(T& x) { x--; } 
    };

    // binary in-place functors
    template<class T1, class T2>
    struct Assign { 
        very_inline void operator()(T1& x, const T2& y) { x = y; } 
    };

    template<class T1, class T2>
    struct PlusEqual { 
        very_inline static void op(T1& x, const T2& y) { x += y; } 
        very_inline void operator()(T1& x, const T2& y) { x += y; } 
    };

    template<class T1, class T2>
    struct MinusEqual { 
        very_inline static void op(T1& x, const T2& y) { x -= y; } 
        very_inline void operator()(T1& x, const T2& y) { x -= y; } 
    };

    template<class T1, class T2>
    struct TimesEqual { 
        very_inline static void op(T1& x, const T2& y) { x *= y; } 
        very_inline void operator()(T1& x, const T2& y) { x *= y; } 
    };

    template<class T1, class T2>
    struct DividedByEqual { 
        very_inline static void op(T1& x, const T2& y) { x /= y; } 
        very_inline void operator()(T1& x, const T2& y) { x /= y; } 
    };

    template<class T1, class T2>
    struct OrEqual { 
        very_inline static void op(T1& x, const T2& y) { x |= y; } 
        very_inline void operator()(T1& x, const T2& y) { x |= y; } 
    };


    template<class T1, class T2>
    struct AndEqual { 
        very_inline static void op(T1& x, const T2& y) { x &= y; } 
        very_inline void operator()(T1& x, const T2& y) { x &= y; } 
    };

    // binary functors
    template<class T1, class T2>
    struct Plus { 
        typedef typename meta::PromoteType<T1, T2>::type type;
        very_inline static type op(const T1& x, const T2& y){ return x + y; } 
        very_inline type operator()(const T1& x, const T2& y) const { return x + y; } 
    };

    template<class T1, class T2>
    struct Minus { 
        typedef typename meta::PromoteType<T1, T2>::type type;
        very_inline static type op(const T1& x, const T2& y){ return x - y; } 
        very_inline type operator()(const T1& x, const T2& y) const { return x - y; } 
    };

    template<class T1, class T2>
    struct Times { 
        typedef typename meta::PromoteType<T1, T2>::type type;
        very_inline static type op(const T1& x, const T2& y){ return x * y; } 
        very_inline type operator()(const T1& x, const T2& y) const { return x * y; } 
    };

    template<class T1, class T2>
    struct DividedBy {
        typedef typename meta::PromoteType<T1, T2>::type type;
        very_inline static type op(const T1& x, const T2& y){ return x / y; } 
        very_inline type operator()(const T1& x, const T2& y) const { return x / y; } 
    };

    template<class T1, class T2>
    struct Or { 
        typedef typename meta::PromoteType<T1, T2>::type type;
        very_inline static type op(const T1& x, const T2& y){ return x || y; } 
        very_inline type operator()(const T1& x, const T2& y) const { return x || y; } 
    };

    template<class T1, class T2>
    struct And { 
        typedef typename meta::PromoteType<T1, T2>::type type;
        very_inline static type op(const T1& x, const T2& y){ return x && y; } 
        very_inline type operator()(const T1& x, const T2& y) const { return x && y; } 
    };



    template<class T, class EXPONENT_TYPE>
    struct Pow { 
        typedef typename multi_array::meta::PromoteRealType<T,EXPONENT_TYPE>::type type;
        typedef Pow<T,EXPONENT_TYPE> SelfType;

        very_inline static type op(const T& x, const EXPONENT_TYPE& exponent){ 
            return std::pow(x, exponent);
        } 
        very_inline type operator()(const T& x, const EXPONENT_TYPE& exponent) const { 
            return SelfType::op(x, exponent);
        } 
    };

    template<class T, int64_t EXPONENT>
    struct PowN { 
        typedef typename multi_array::meta::PromoteRealType<T,int64_t>::type type;
        typedef PowN<T,EXPONENT> SelfType;

        very_inline static type op(const T& x){ 
            return std::pow(x, EXPONENT);
        } 
        very_inline type operator()(const T& x) const { 
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
        very_inline static type op(U && val){ return static_cast<U>(val); } 

        template<class U>
        very_inline type operator()(U && val) const { return static_cast<U>(val); } 
    };


    // UNARY MATH FUNCTIONS WICH HAVE
    // ALL THE SAME RESULT TYPE
    #define GEN_MATH_FUNCTOR(CLASS_NAME, FUNCTION_NAME)\
    template<class T>\
    struct CLASS_NAME{\
        typedef typename meta::PromoteReal<T>::type type;\
        very_inline type operator()(const T& value)const{\
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
        very_inline type operator()(VALUE && value)const{
            return binaryFunctor_(scalarLike_, value);
        }

        const SCALAR_LIKE scalarLike_;
        const BINARY_FUNCTOR binaryFunctor_;
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
        very_inline type operator()(VALUE && value)const{
            return binaryFunctor_(value, scalarLike_);
        }

        const SCALAR_LIKE scalarLike_;
        const BINARY_FUNCTOR binaryFunctor_;
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
        very_inline type operator()(
            ARG && arg
        )const{

        }
        const OUTER_UNARY_FUNCTOR outerUnaryFunctor_;
        const INNER_UNARY_FUNCTOR innerUnaryFunctor_;
    };

    




    template<class OUTER_BINARY_FUNCTOR, class INNER_BINARY_FUNCTOR_0, class INNER_BINARY_FUNCTOR_1>
    struct BinaryFunctorOfBinaryFunctorAndBinaryFunctor
    {
        BinaryFunctorOfBinaryFunctorAndBinaryFunctor(
            const OUTER_BINARY_FUNCTOR  & binaryFunctor = OUTER_BINARY_FUNCTOR(),
            const INNER_BINARY_FUNCTOR_0 & binaryFunctor1 = INNER_BINARY_FUNCTOR_0(),
            const INNER_BINARY_FUNCTOR_1 & binaryFunctor2 = INNER_BINARY_FUNCTOR_1()
        )
        :   binaryFunctor_(binaryFunctor),
            binaryFunctor1_(binaryFunctor1),
            binaryFunctor2_(binaryFunctor2){
        }
        typedef typename OUTER_BINARY_FUNCTOR::type type;
        template<class A, class B, class C, class D>
        very_inline type operator()(const A & a, const B & b, const C & c , const D & d)const{
            return binaryFunctor_(binaryFunctor1_(a,b), binaryFunctor2_(c,d));
        }
        const OUTER_BINARY_FUNCTOR  binaryFunctor_;
        const INNER_BINARY_FUNCTOR_0 binaryFunctor1_;
        const INNER_BINARY_FUNCTOR_1 binaryFunctor2_;
    };


    // just an experiment
    // (a+b) + (c+d)   

    #if 1
    template<class T0_A, class T0_B, class T1_A, class T1_B>
    struct BinaryFunctorOfBinaryFunctorAndBinaryFunctor<
        Plus<typename Plus<T0_A, T0_B>::type, typename Plus<T1_A, T1_B>::type >,
        Plus<T0_A,T0_B>,
        Plus<T1_A, T1_B>
    >{
        typedef typename meta::PromoteType<T0_A, T0_B>::type T0AB;
        typedef typename meta::PromoteType<T1_A, T1_B>::type T1AB;
        typedef typename meta::PromoteType<T0AB, T1AB>::type type;

        BinaryFunctorOfBinaryFunctorAndBinaryFunctor(
            const Plus<typename Plus<T0_A, T0_B>::type, typename Plus<T1_A, T1_B>::type > &,
            const Plus<T0_A,T0_B> & ,
            const Plus<T1_A,T1_B> & 
        )
        {

        }
        template<class A, class B, class C, class D>
        very_inline type operator()(const A & a, const B & b, const C & c , const D & d)const{
            return a+b+c+d;
        }

    };




    #endif









    }
}