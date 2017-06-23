#pragma once

#include <complex>

#include "multi_array/meta.hxx"
#include "multi_array/functors.hxx"

namespace multi_array{

template<class T, std::size_t DIM, bool IS_CONST>
class SmartMultiArray;


template<std::size_t DIM, class E, class T>
class ViewExpression;



template<std::size_t DIM, class E, class T, class UnaryFunctor>
class UnaryViewExpression;

template<std::size_t DIM, class E1, class T1, class E2, class T2, class BinaryFunctor>
class BinaryViewExpression;

template<std::size_t DIM, class E, class T, class S, class BinaryFunctor>
class BinaryViewExpressionScalarFirst;

template<std::size_t DIM, class E, class T, class S, class BinaryFunctor>
class BinaryViewExpressionScalarSecond;






    // operate 

    template<
        std::size_t DIM,
        class EA, 
        class TA, 
        class EB,
        class TB
    >
    BinaryViewExpression<DIM,EA, TA,EB, TB, functors::Plus<TA,TB> >
    operator+(
        const ViewExpression<DIM, EA, TA> & a,
        const ViewExpression<DIM, EB, TB> & b
    ){
        typedef BinaryViewExpression<DIM,EA, TA,EB, TB, functors::Plus<TA,TB>> ResultType;
        return ResultType(a, b);
    }







    template<class F>
    struct InplaceOpElementWise{

        template<class ARRAY, class ARG>
        static ARRAY & op(ARRAY & array, ARG && arg){
        typedef typename ARRAY::reference reference;
            array.forEach([&](reference val){
                F::op(val, arg);
            });
        }
    };


    struct InplaceOpFallback{
        template<class ARRAY, class ARG>
        static ARRAY & op(ARRAY & array, ARG && arg){
            
        }
    };


        
    #define OP_DEF(OP_SYMBOL, FUNCTOR_NAME)                                                      \
        template<class T, std::size_t DIM, class ARG>                                            \
        SmartMultiArray<T, DIM, false> &                                                         \
        operator OP_SYMBOL (                                                                     \
            SmartMultiArray<T, DIM, false> & array,                                              \
            ARG && arg                                                                           \
        ){                                                                                       \
            typedef std::is_integral<ARG> IsIntegralArg;                                         \
            typedef std::is_integral<ARG> IsIntegralT;                                           \
            typedef meta::IsComplex<ARG> IsComplexArg;                                           \
            typedef SmartMultiArray<T, DIM, false> ArrayType;                                    \
            typedef typename ArrayType::value_type ArrayValueType;                               \
            typedef typename meta::Switch<                                                       \
                IsIntegralArg,      InplaceOpElementWise< FUNCTOR_NAME <ArrayValueType, ARG> >,  \
                IsComplexArg,       InplaceOpElementWise< FUNCTOR_NAME <ArrayValueType, ARG> >,  \
                meta::Bool<true>,    InplaceOpFallback                                           \
            >::type ImplType;                                                                    \
            ImplType::op(array, arg);                                                            \
        }                                                                                        \



    OP_DEF(+=, functors::PlusEqual);
    OP_DEF(-=, functors::MinusEqual);
    OP_DEF(*=, functors::TimesEqual);
    OP_DEF(/=, functors::DividedByEqual);
    OP_DEF(|=, functors::OrEqual);
    OP_DEF(^=, functors::XorEqual);
    OP_DEF(&=, functors::AndEqual);
    #undef OP_DEF
  

}
