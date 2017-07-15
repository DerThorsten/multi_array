#pragma once


#include <cmath>


#include "multi_array/meta/promote_real.hxx"
#include "multi_array/meta.hxx"
#include "multi_array/misc.hxx"
#include "multi_array/expr/view_expression.hxx"
#include "multi_array/runtime_check.hxx"
#include "multi_array/functors.hxx"

namespace multi_array{











#define GEN_UNARY_MATH_FUNCTION(FUNCTOR_NAME, FUNCTION_NAME)\
template<std::size_t DIM, class EXPRESSION_TYPE, class EXPRESSION_VALUE_TYPE>\
typename UnaryExprOpt< DIM, EXPRESSION_TYPE, EXPRESSION_VALUE_TYPE, FUNCTOR_NAME<EXPRESSION_VALUE_TYPE>>::type \
FUNCTION_NAME(\
    const ViewExpression<DIM, EXPRESSION_TYPE, EXPRESSION_VALUE_TYPE> & expression\
){\
    typedef FUNCTOR_NAME<EXPRESSION_VALUE_TYPE> FunctorType;\
    return UnaryExprOpt<\
        DIM, \
        EXPRESSION_TYPE, \
        EXPRESSION_VALUE_TYPE, \
        FunctorType\
    >::op((expression));\
}\
    
GEN_UNARY_MATH_FUNCTION(functors::Cos,           cos);
GEN_UNARY_MATH_FUNCTION(functors::Sin,           sin);
GEN_UNARY_MATH_FUNCTION(functors::Tan,           tan);
GEN_UNARY_MATH_FUNCTION(functors::Acos,          acos);
GEN_UNARY_MATH_FUNCTION(functors::Asin,          asin);
GEN_UNARY_MATH_FUNCTION(functors::Atan,          atan);
// Hyperbolic functions
GEN_UNARY_MATH_FUNCTION(functors::Cosh,           cosh);
GEN_UNARY_MATH_FUNCTION(functors::Sinh,           sinh);
GEN_UNARY_MATH_FUNCTION(functors::Tanh,           tanh);
GEN_UNARY_MATH_FUNCTION(functors::Acosh,          acosh);
GEN_UNARY_MATH_FUNCTION(functors::Asinh,          asinh);
GEN_UNARY_MATH_FUNCTION(functors::Atanh,          atanh);
// Exponential and logarithmic functions
GEN_UNARY_MATH_FUNCTION(functors::Exp,       exp);
GEN_UNARY_MATH_FUNCTION(functors::Log,       log);
GEN_UNARY_MATH_FUNCTION(functors::Log10,     log10);
GEN_UNARY_MATH_FUNCTION(functors::Exp2,      exp2);
//Power functions
GEN_UNARY_MATH_FUNCTION(functors::Sqrt,      sqrt);
//GEN_UNARY_MATH_FUNCTION(functors::Cqrt,      cqrt);
//Error and gamma functions
GEN_UNARY_MATH_FUNCTION(functors::Erf,       erf);
GEN_UNARY_MATH_FUNCTION(functors::Erfc,      erfc);
GEN_UNARY_MATH_FUNCTION(functors::Tgamma,    tgamma);
GEN_UNARY_MATH_FUNCTION(functors::Lgamma,    lgamma);
//Rounding and remainder functions
GEN_UNARY_MATH_FUNCTION(functors::Ceol,      ceil);
GEN_UNARY_MATH_FUNCTION(functors::Floor,     floor);
GEN_UNARY_MATH_FUNCTION(functors::Trunc,     trunc);
GEN_UNARY_MATH_FUNCTION(functors::Round,     round);
GEN_UNARY_MATH_FUNCTION(functors::Lround,    lround);
GEN_UNARY_MATH_FUNCTION(functors::Rint,      rint);
//Other functions
GEN_UNARY_MATH_FUNCTION(functors::Abs,      abs);
GEN_UNARY_MATH_FUNCTION(functors::Fabs,     fabs);




}
