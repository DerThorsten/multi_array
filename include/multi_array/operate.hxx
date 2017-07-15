#pragma once

#include <complex>
#include <utility>

#include "multi_array/meta.hxx"
#include "multi_array/functors.hxx"
#include "multi_array/expr/view_expression.hxx"



namespace multi_array{

template<class T, std::size_t DIM, bool IS_CONST>
class SmartMultiArray;

template<std::size_t DIM, class E, class T>
class ViewExpression;

template<std::size_t DIM, class E, class T, class UnaryFunctor>
class UnaryViewExpression;

template<std::size_t DIM, class E1, class T1, class E2, class T2, class BinaryFunctor>
class BinaryViewExpression;







#define __MULTI_ARRAY_OP_DEF__(OP_SYMBOL, FUNCTOR_NAME, SCALAR_NAME)\
template<std::size_t DIM, class E, class T>\
inline typename UnaryExprOpt<DIM, E, T, functors::BindScalarLeft< SCALAR_NAME, FUNCTOR_NAME<SCALAR_NAME,T>>>::type \
operator OP_SYMBOL ( const SCALAR_NAME & number,const ViewExpression<DIM, E, T> & expression){\
    typedef functors::BindScalarLeft<SCALAR_NAME, FUNCTOR_NAME<SCALAR_NAME,T> > UnaryFunctor;\
    return UnaryExprOpt<DIM, E, T, UnaryFunctor>::op(expression, UnaryFunctor(number));\
}\
template<std::size_t DIM, class E, class T>\
inline typename UnaryExprOpt<DIM, E, T, functors::BindScalarRight<SCALAR_NAME, FUNCTOR_NAME<T,SCALAR_NAME>>>::type \
operator OP_SYMBOL (const ViewExpression<DIM, E, T> & expression,const SCALAR_NAME & number){\
    typedef functors::BindScalarRight<SCALAR_NAME, FUNCTOR_NAME<T,SCALAR_NAME> > UnaryFunctor;\
    return UnaryExprOpt<DIM, E, T, UnaryFunctor>::op(expression, UnaryFunctor(number));\
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
inline typename UnaryExprOpt<DIM, E, T, functors::BindScalarLeft<T, FUNCTOR_NAME<T,T>>>::type \
operator OP_SYMBOL (const T & number,const ViewExpression<DIM, E, T> & expression){\
   typedef functors::BindScalarLeft<T, FUNCTOR_NAME<T,T> > UnaryFunctor;\
   return UnaryExprOpt<DIM, E, T, UnaryFunctor>::op(expression, UnaryFunctor(number));\
}\
template<std::size_t DIM, class E,class T, \
    typename std::enable_if< !std::is_arithmetic<T>::value ,int>::type = 0\
>\
inline typename UnaryExprOpt<DIM, E, T, functors::BindScalarRight<T, FUNCTOR_NAME<T,T>>>::type \
operator OP_SYMBOL (const ViewExpression<DIM, E, T> & expression,const T & number){\
    typedef functors::BindScalarRight<T, FUNCTOR_NAME<T,T> > UnaryFunctor;\
    return UnaryExprOpt<DIM, E, T, UnaryFunctor>::op(expression, UnaryFunctor(number));\
}\
template<\
    std::size_t DIM, \
    class EA, class TA,\
    class EB, class TB\
>\
inline typename BinaryExprOpt<DIM, EA, TA, EB, TB, FUNCTOR_NAME<TA, TB>>::type \
operator OP_SYMBOL (const ViewExpression<DIM, EA, TA> & expressionA,const ViewExpression<DIM, EB, TB> & expressionB){\
    return BinaryExprOpt<DIM, EA, TA, EB, TB, FUNCTOR_NAME<TA, TB>>::op(expressionA, expressionB);\
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


// the old: std::enable_if<!std::is_arithmetic<T>::value ,int>::type = 0
// the old: std::enable_if<!std::is_arithmetic<T>{} ,int>::type = 0

//////////////////////////////////////////
//  += *= /= etc. for Array::ValueType
#define __MULTI_ARRAY_OP_DEF__(OP_SYMBOL)\
template<class T, std::size_t DIM,\
    typename std::enable_if<!std::is_arithmetic<T>::value ,int>::type = 0\
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