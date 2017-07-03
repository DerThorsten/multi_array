#include "doctest.h"

#include <iostream>
#include <memory>
#include <type_traits>

#include "multi_array/multi_array.hxx"
#include "multi_array/multi_array_factories.hxx"
#include "multi_array/meta.hxx"
#include "multi_array/runtime_check.hxx"

TEST_SUITE_BEGIN("Navigator");


TEST_CASE("[Navigator] operator+"){
   
   auto shape = multi_array::shape(3);
   auto strides = multi_array::detail_multi_array::cOrderStrides(shape);

   multi_array::Navigator<1> navigator(shape, strides);

   REQUIRE_EQ(navigator.offset(),0);
   navigator.increment(0);
   REQUIRE_EQ(navigator.offset(),1);
   navigator.increment(0);
   REQUIRE_EQ(navigator.offset(),2);

}





TEST_SUITE_END();