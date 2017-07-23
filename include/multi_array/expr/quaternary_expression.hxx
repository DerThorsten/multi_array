#pragma once

#include <iostream>
#include <memory>
#include <type_traits>
#include <utility>

#include "multi_array/misc.hxx"
#include "multi_array/expr/view_expression.hxx"

namespace multi_array{




template<std::size_t DIM, class E, class T>
class ViewExpression;

template<bool NEEDS_COORDINATE, bool IS_SHAPELESS>
struct ViewExpressionMeta;





}