#include "doctest.h"

#include <iostream>
#include <memory>
#include <type_traits>

#include "multi_array/multi_array.hxx"
#include "multi_array/multi_array_factories.hxx"

#include "multi_array/meta.hxx"
#include "multi_array/runtime_check.hxx"

TEST_SUITE_BEGIN("ViewExpression");

TEST_CASE("[BinaryViewExpression] BinaryViewExpression::matchingStrides"){

    namespace ma = multi_array;

    SUBCASE("2D"){

        auto a =  ma::ones<int>(2,3);
        auto b =  ma::ones<int>(2,3);
        auto c =  ma::ones<int>(3,2);
        auto d =  c.transposedView();

        
        SUBCASE("matching strides simple"){
            auto ab = a + b ;   
            REQUIRE_EQ(ab.matchingStrides(),true);
            REQUIRE_EQ(ab.strides(0) , a.strides(0));
            REQUIRE_EQ(ab.strides(1) , a.strides(1));
        }

        SUBCASE("matching strides advanced"){
            auto aba  = a + b + a;
            REQUIRE_EQ(aba.matchingStrides(),true);
            REQUIRE_EQ(aba.strides(0) , a.strides(0));
            REQUIRE_EQ(aba.strides(1) , a.strides(1));
        }

        SUBCASE("not matching strides simple"){
            auto ad = a + d ;   
            REQUIRE_EQ(ad.matchingStrides(),false);
        }

        SUBCASE("matching strides advanced"){
            auto abd  = a + b + d;
            REQUIRE_EQ(abd.matchingStrides(),false);
        }
    }

}

TEST_SUITE_END();