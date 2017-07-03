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


#define __MULTI_ARRAY_OP_DEF__(OP_SYMBOL, FUNCTOR_NAME, SCALAR_NAME)\
template<std::size_t DIM, class E, class T>\
inline BinaryViewExpressionScalarFirst<DIM, E, T, SCALAR_NAME, FUNCTOR_NAME<T, SCALAR_NAME> >\
operator OP_SYMBOL ( const SCALAR_NAME & number,const ViewExpression<DIM, E, T> & expression){\
  return BinaryViewExpressionScalarFirst<DIM, E, T, SCALAR_NAME, FUNCTOR_NAME<T, SCALAR_NAME> >(number, expression);\
}\
template<std::size_t DIM, class E, class T>\
inline BinaryViewExpressionScalarSecond<DIM, E, T, SCALAR_NAME, FUNCTOR_NAME<T, SCALAR_NAME> >\
operator OP_SYMBOL (const ViewExpression<DIM, E, T> & expression,const SCALAR_NAME & number){\
    return BinaryViewExpressionScalarSecond<DIM, E, T, SCALAR_NAME, FUNCTOR_NAME<T, SCALAR_NAME> >(expression, number);\
}
#define __MULTI_ARRAY_OP_DEF_CALLER__(SCALAR_NAME)  \
    __MULTI_ARRAY_OP_DEF__(+, functors::Plus,       SCALAR_NAME); \
    __MULTI_ARRAY_OP_DEF__(-, functors::Minus,      SCALAR_NAME); \
    __MULTI_ARRAY_OP_DEF__(*, functors::Times,      SCALAR_NAME); \
    __MULTI_ARRAY_OP_DEF__(/, functors::DividedBy,  SCALAR_NAME); \
    __MULTI_ARRAY_OP_DEF__(||, functors::Or,        SCALAR_NAME); \
    __MULTI_ARRAY_OP_DEF__(&&, functors::And,       SCALAR_NAME); 
__MULTI_ARRAY_OP_DEF_CALLER__(float);
__MULTI_ARRAY_OP_DEF_CALLER__(double);
__MULTI_ARRAY_OP_DEF_CALLER__(bool);
__MULTI_ARRAY_OP_DEF_CALLER__(uint8_t);
__MULTI_ARRAY_OP_DEF_CALLER__(uint16_t);
__MULTI_ARRAY_OP_DEF_CALLER__(uint32_t);
__MULTI_ARRAY_OP_DEF_CALLER__(uint64_t);
__MULTI_ARRAY_OP_DEF_CALLER__(int16_t);
__MULTI_ARRAY_OP_DEF_CALLER__(int32_t);
__MULTI_ARRAY_OP_DEF_CALLER__(int64_t);
// __MULTI_ARRAY_OP_DEF_CALLER__(std::complex<float>);
// __MULTI_ARRAY_OP_DEF_CALLER__(std::complex<double>);
// __MULTI_ARRAY_OP_DEF_CALLER__(std::complex<bool>);
// __MULTI_ARRAY_OP_DEF_CALLER__(std::complex<uint8_t>);
// __MULTI_ARRAY_OP_DEF_CALLER__(std::complex<uint16_t>);
// __MULTI_ARRAY_OP_DEF_CALLER__(std::complex<uint32_t>);
// __MULTI_ARRAY_OP_DEF_CALLER__(std::complex<uint64_t>);
// __MULTI_ARRAY_OP_DEF_CALLER__(std::complex<int16_t>);
// __MULTI_ARRAY_OP_DEF_CALLER__(std::complex<int32_t>);
// __MULTI_ARRAY_OP_DEF_CALLER__(std::complex<int64_t>);
#undef __MULTI_ARRAY_OP_DEF__
#undef __MULTI_ARRAY_OP_DEF_CALLER__

#define __MULTI_ARRAY_OP_DEF__(OP_SYMBOL, FUNCTOR_NAME)\
template<std::size_t DIM, class E, class T, \
    typename std::enable_if<!std::is_arithmetic<T>::value ,int>::type = 0\
>\
inline BinaryViewExpressionScalarFirst<DIM, E, T, T, FUNCTOR_NAME<T, T> >\
operator OP_SYMBOL (const T & number,const ViewExpression<DIM, E, T> & expression){\
   return BinaryViewExpressionScalarFirst<DIM, E, T, T, FUNCTOR_NAME<T, T> >(number, expression);\
}\
template<std::size_t DIM, class E,class T, \
    typename std::enable_if< !std::is_arithmetic<T>::value ,int>::type = 0\
>\
inline BinaryViewExpressionScalarSecond<DIM, E, T, T, FUNCTOR_NAME<T, T> >\
operator OP_SYMBOL (const ViewExpression<DIM, E, T> & expression,const T & number){\
    return BinaryViewExpressionScalarSecond<DIM, E, T, T, FUNCTOR_NAME<T, T> >(expression, number);\
}\
template<\
    std::size_t DIM, \
    class EA, class TA,\
    class EB, class TB\
>\
inline BinaryViewExpression<DIM, EA, TA, EB, TB, FUNCTOR_NAME<TA, TB> >\
operator OP_SYMBOL (const ViewExpression<DIM, EA, TA> & expressionA,const ViewExpression<DIM, EB, TB> & expressionB){\
    return BinaryViewExpression<DIM, EA, TA, EB, TB, FUNCTOR_NAME<TA, TB> >(expressionA, expressionB);\
}
__MULTI_ARRAY_OP_DEF__(+, functors::Plus); 
__MULTI_ARRAY_OP_DEF__(-, functors::Minus); 
__MULTI_ARRAY_OP_DEF__(*, functors::Times); 
__MULTI_ARRAY_OP_DEF__(/, functors::DividedBy); 
__MULTI_ARRAY_OP_DEF__(||, functors::Or); 
__MULTI_ARRAY_OP_DEF__(&&, functors::And); 
#undef __MULTI_ARRAY_OP_DEF__



//////////////////////////////////////////
//  += *= /= etc. for scalars
#define __MULTI_ARRAY_OP_DEF__(OP_SYMBOL, SCALAR_NAME)\
template<class T, std::size_t DIM>\
inline SmartMultiArray<T, DIM, false> &     \
operator OP_SYMBOL (SmartMultiArray<T, DIM, false> & rhs, const SCALAR_NAME & lhs){  \
    typedef typename SmartMultiArray<T, DIM, false>::reference reference;\
    rhs.forEach([&](reference val){val OP_SYMBOL  lhs;});\
    return rhs;\
}

#define __MULTI_ARRAY_OP_DEF_CALLER__(SCALAR_NAME)  \
    __MULTI_ARRAY_OP_DEF__(+=,  SCALAR_NAME); \
    __MULTI_ARRAY_OP_DEF__(-=,  SCALAR_NAME); \
    __MULTI_ARRAY_OP_DEF__(*=,  SCALAR_NAME); \
    __MULTI_ARRAY_OP_DEF__(/=,  SCALAR_NAME); \
    __MULTI_ARRAY_OP_DEF__(|=,  SCALAR_NAME); \
    __MULTI_ARRAY_OP_DEF__(&=,  SCALAR_NAME); 
__MULTI_ARRAY_OP_DEF_CALLER__(float);
__MULTI_ARRAY_OP_DEF_CALLER__(double);
__MULTI_ARRAY_OP_DEF_CALLER__(bool);
__MULTI_ARRAY_OP_DEF_CALLER__(uint8_t);
__MULTI_ARRAY_OP_DEF_CALLER__(uint16_t);
__MULTI_ARRAY_OP_DEF_CALLER__(uint32_t);
__MULTI_ARRAY_OP_DEF_CALLER__(uint64_t);
__MULTI_ARRAY_OP_DEF_CALLER__(int16_t);
__MULTI_ARRAY_OP_DEF_CALLER__(int32_t);
__MULTI_ARRAY_OP_DEF_CALLER__(int64_t);
// __MULTI_ARRAY_OP_DEF_CALLER__(std::complex<float>);
// __MULTI_ARRAY_OP_DEF_CALLER__(std::complex<double>);
// __MULTI_ARRAY_OP_DEF_CALLER__(std::complex<bool>);
// __MULTI_ARRAY_OP_DEF_CALLER__(std::complex<uint8_t>);
// __MULTI_ARRAY_OP_DEF_CALLER__(std::complex<uint16_t>);
// __MULTI_ARRAY_OP_DEF_CALLER__(std::complex<uint32_t>);
// __MULTI_ARRAY_OP_DEF_CALLER__(std::complex<uint64_t>);
// __MULTI_ARRAY_OP_DEF_CALLER__(std::complex<int16_t>);
// __MULTI_ARRAY_OP_DEF_CALLER__(std::complex<int32_t>);
// __MULTI_ARRAY_OP_DEF_CALLER__(std::complex<int64_t>);
#undef __MULTI_ARRAY_OP_DEF__
#undef __MULTI_ARRAY_OP_DEF__
#undef __MULTI_ARRAY_OP_DEF_CALLER__


//////////////////////////////////////////
//  += *= /= etc. for Array::ValueType
#define __MULTI_ARRAY_OP_DEF__(OP_SYMBOL)\
template<class T, std::size_t DIM,\
    typename std::enable_if<!std::is_arithmetic<T>{} ,int>::type = 0\
>\
inline SmartMultiArray<T, DIM, false> &     \
operator OP_SYMBOL (SmartMultiArray<T, DIM, false> & rhs, const T & lhs){  \
    typedef typename SmartMultiArray<T, DIM, false>::reference reference;\
    rhs.forEach([&](reference val){val OP_SYMBOL lhs;});\
    return rhs;\
}
__MULTI_ARRAY_OP_DEF__(+=);
__MULTI_ARRAY_OP_DEF__(-=);
__MULTI_ARRAY_OP_DEF__(*=);
__MULTI_ARRAY_OP_DEF__(/=);
__MULTI_ARRAY_OP_DEF__(|=);
__MULTI_ARRAY_OP_DEF__(&=); 
#undef __MULTI_ARRAY_OP_DEF__


//////////////////////////////////////////
// += *= /= etc. for other arrays and
// expression types
#define __MULTI_ARRAY_OP_DEF__(OP_SYMBOL)\
template<class TA, std::size_t DIM,class TB,bool IS_CONST>\
inline SmartMultiArray<TA, DIM, false> &     \
operator OP_SYMBOL (\
    SmartMultiArray<TA, DIM, false>    & lhs,\
    const SmartMultiArray<TB, DIM, IS_CONST> & rhs\
){  \
    lhs.operate(rhs,\
    [&](\
        typename SmartMultiArray<TA, DIM, false>::reference va,\
        typename SmartMultiArray<TB, DIM, IS_CONST>::const_reference vb\
    ){\
        va OP_SYMBOL vb;\
    });\
    return lhs;\
}\
template<class TA, std::size_t DIM,class TB,class E>\
inline SmartMultiArray<TA, DIM, false> &     \
operator OP_SYMBOL (\
    SmartMultiArray<TA, DIM, false>    & lhs,\
    const ViewExpression<DIM,E,TB> &    rhs\
){  \
    lhs.operate(rhs,\
    [&](\
        typename SmartMultiArray<TA, DIM, false>::reference  va,\
        const TB vb\
    ){\
        va OP_SYMBOL vb;\
    });\
    return lhs;\
}
__MULTI_ARRAY_OP_DEF__(+=);
__MULTI_ARRAY_OP_DEF__(-=);
__MULTI_ARRAY_OP_DEF__(*=);
__MULTI_ARRAY_OP_DEF__(/=);
__MULTI_ARRAY_OP_DEF__(|=);
__MULTI_ARRAY_OP_DEF__(&=); 
#undef __MULTI_ARRAY_OP_DEF__



}