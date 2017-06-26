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
        auto e =  ma::ones<int>(2,3);
        
        SUBCASE("matching strides simple"){
            auto ab = a + b ;   
            REQUIRE_EQ(ab.matchingStrides(),true);
            REQUIRE_EQ(ab.strides(0) , a.strides(0));
            REQUIRE_EQ(ab.strides(1) , a.strides(1));
        }
        SUBCASE("matching strides advanced A"){
            auto ab  = a + b;
            auto aba  = ab + a;
            
            REQUIRE_EQ(ab.strides(0) , a.strides(0));
            REQUIRE_EQ(ab.strides(1) , a.strides(1));
            REQUIRE_EQ(ab.matchingStrides(),true);

            REQUIRE_EQ(aba.strides(0) , a.strides(0));
            REQUIRE_EQ(aba.strides(1) , a.strides(1));
            REQUIRE_EQ(aba.matchingStrides(),true);
            
        }
        SUBCASE("matching strides advanced B"){
            auto ab  = a + b;
            auto aba = a + ab;
            REQUIRE_EQ(ab.strides(0) , a.strides(0));
            REQUIRE_EQ(ab.strides(1) , a.strides(1));
            REQUIRE_EQ(ab.matchingStrides(),true);
            REQUIRE_EQ(aba.strides(0) , a.strides(0));
            REQUIRE_EQ(aba.strides(1) , a.strides(1));
            REQUIRE_EQ(aba.matchingStrides(),true);    
        }
        SUBCASE("matching strides advanced C"){
            //auto ab  = (a + b);
            auto aba  = ((a + b)) + e;

            //REQUIRE_EQ(a.smartHandle().useCount(),1);
            REQUIRE_EQ(aba.strides(0) , 3);
            REQUIRE_EQ(aba.strides(1) , 1);
            REQUIRE_EQ(aba.matchingStrides(),true);

        }
        SUBCASE("matching strides advanced D"){
            //auto ab  = (a + b);
            auto thesum  = e + ((a + b + (a+b+(a+b)))) + e;
            REQUIRE_EQ(thesum.strides(0) , 3);
            REQUIRE_EQ(thesum.strides(1) , 1);
            REQUIRE_EQ(thesum.matchingStrides(),true);

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

TEST_CASE("[BinaryViewExpression] BinaryViewExpression::unsafeAccess"){

    namespace ma = multi_array;

    SUBCASE("2D"){

        auto a =  ma::ones<int>(2,3);
        auto b =  ma::ones<int>(2,3);
        auto c =  ma::ones<int>(2,3);
        
        SUBCASE("Simple"){
            auto ab = a + b ;   
            REQUIRE_EQ(ab.matchingStrides(),true);
            REQUIRE_EQ(ab.contiguous(),true);
            REQUIRE_EQ(ab.strides(0) , a.strides(0));
            REQUIRE_EQ(ab.strides(1) , a.strides(1));

            REQUIRE_EQ(ab.unsafeAccess(0),2);
            REQUIRE_EQ(ab.unsafeAccess(1),2);
            REQUIRE_EQ(ab.unsafeAccess(2),2);
            REQUIRE_EQ(ab.unsafeAccess(3),2);
            REQUIRE_EQ(ab.unsafeAccess(4),2);
            REQUIRE_EQ(ab.unsafeAccess(5),2);
        }
        SUBCASE("Advanced"){
            auto theSum = a + b + c + (a+b);   
            REQUIRE_EQ(theSum.matchingStrides(),true);
            REQUIRE_EQ(theSum.contiguous(),true);
            REQUIRE_EQ(theSum.strides(0) , a.strides(0));
            REQUIRE_EQ(theSum.strides(1) , a.strides(1));

            REQUIRE_EQ(theSum.unsafeAccess(0),5);
            REQUIRE_EQ(theSum.unsafeAccess(1),5);
            REQUIRE_EQ(theSum.unsafeAccess(2),5);
            REQUIRE_EQ(theSum.unsafeAccess(3),5);
            REQUIRE_EQ(theSum.unsafeAccess(4),5);
            REQUIRE_EQ(theSum.unsafeAccess(5),5);
        }
    }
}


TEST_CASE("[BinaryViewExpression] BinaryViewExpression::unsafeAccess"){

    namespace ma = multi_array;

    SUBCASE("2D"){

        auto a =  ma::ones<int>(2,3);
        auto b =  ma::ones<int>(2,3);
        auto c =  ma::ones<int>(2,3);
        
        SUBCASE("Simple"){
            auto ab = a + b ;   
            REQUIRE_EQ(ab.matchingStrides(),true);
            REQUIRE_EQ(ab.contiguous(),true);
            REQUIRE_EQ(ab.strides(0) , a.strides(0));
            REQUIRE_EQ(ab.strides(1) , a.strides(1));
            REQUIRE_EQ(ab.unsafeAccess(0),2);
            REQUIRE_EQ(ab.unsafeAccess(1),2);
            REQUIRE_EQ(ab.unsafeAccess(2),2);
            REQUIRE_EQ(ab.unsafeAccess(3),2);
            REQUIRE_EQ(ab.unsafeAccess(4),2);
            REQUIRE_EQ(ab.unsafeAccess(5),2);
        }
        SUBCASE("Advanced"){
            auto theSum = a + b + c + (a+b);   
            REQUIRE_EQ(theSum.matchingStrides(),true);
            REQUIRE_EQ(theSum.contiguous(),true);
            REQUIRE_EQ(theSum.strides(0) , a.strides(0));
            REQUIRE_EQ(theSum.strides(1) , a.strides(1));
            REQUIRE_EQ(theSum.unsafeAccess(0),5);
            REQUIRE_EQ(theSum.unsafeAccess(1),5);
            REQUIRE_EQ(theSum.unsafeAccess(2),5);
            REQUIRE_EQ(theSum.unsafeAccess(3),5);
            REQUIRE_EQ(theSum.unsafeAccess(4),5);
            REQUIRE_EQ(theSum.unsafeAccess(5),5);
        }
    }
}

TEST_CASE("[BinaryViewExpression] assign to array"){

    namespace ma = multi_array;

    SUBCASE("2D"){

        auto a =  ma::ones<int>(2,3);
        auto b =  ma::ones<int>(2,3);
        auto c =  ma::ones<int>(2,3);
        
        SUBCASE("Simple matching types"){
            auto ab = a + b ;   
            REQUIRE_EQ(ab.matchingStrides(),true);
            REQUIRE_EQ(ab.contiguous(),true);
            REQUIRE_FALSE(ab.overlaps(c));
            REQUIRE(ab.overlaps(a));
            REQUIRE(ab.overlaps(b));
            c = a + b;
        }
    }
}



TEST_SUITE_END();