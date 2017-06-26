#include "doctest.h"

#include <iostream>
#include <memory>
#include <type_traits>

#include "multi_array/multi_array.hxx"
#include "multi_array/multi_array_factories.hxx"

#include "multi_array/meta.hxx"
#include "multi_array/runtime_check.hxx"

TEST_SUITE_BEGIN("SmartMultiArray");


TEST_CASE("[SmartMultiArray] operator+"){
    const auto a = multi_array::ones<int>(2,2);
    const auto b = multi_array::ones<int>(2,2);
    const auto c = multi_array::ones<int>(2,2);
    auto res = multi_array::ones<int>(2,2);
    res = a + b + c;


}

TEST_CASE("[SmartMultiArray] operator+= "){

    SUBCASE("int"){
        auto a = multi_array::zeros<int>(2,2);
        REQUIRE_EQ(a(0,0),0);
        REQUIRE_EQ(a(0,1),0);
        REQUIRE_EQ(a(1,0),0);
        REQUIRE_EQ(a(1,1),0);
        SUBCASE("integral"){
            a += 3;
            REQUIRE_EQ(a(0,0),3);
            REQUIRE_EQ(a(0,1),3);
            REQUIRE_EQ(a(1,0),3);
            REQUIRE_EQ(a(1,1),3);
        }
        SUBCASE("dense view"){
            auto aa = a(0,multi_array::all());
            aa += 3;
            REQUIRE_EQ(a(0,0),3);
            REQUIRE_EQ(a(0,1),3);
            REQUIRE_EQ(a(1,0),0);
            REQUIRE_EQ(a(1,1),0);
        }
        SUBCASE("strided view"){
           auto aa = a(multi_array::all(), 0);
           aa += 3;
           REQUIRE_EQ(a(0,0),3);
           REQUIRE_EQ(a(0,1),0);
           REQUIRE_EQ(a(1,0),3);
           REQUIRE_EQ(a(1,1),0);
        }
    }
    SUBCASE("complex"){
        auto a = multi_array::zeros<std::complex<int> >(3,2,2);
        a += std::complex<int>(0,1);
    }

}




TEST_SUITE_END();