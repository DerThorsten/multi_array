#include "doctest.h"

#include <iostream>
#include <memory>
#include <type_traits>
#include "multi_array/multi_array_factories.hxx"
#include "multi_array/multi_array.hxx"
#include "multi_array/meta.hxx"
#include "multi_array/runtime_check.hxx"



TEST_SUITE_BEGIN("Misc");

TEST_CASE( "[Factories]  -- arange]" ) {
    namespace ma = multi_array;
    SUBCASE("integer a"){
        auto a = ma::arange(3);
        REQUIRE_EQ(a.dimension(),1);
        REQUIRE_EQ(a.size(),3);
        REQUIRE_EQ(a(0),0);
        REQUIRE_EQ(a(1),1);
        REQUIRE_EQ(a(2),2);
    }
    SUBCASE("integer b"){
        auto a = ma::arange(3,7);
        REQUIRE_EQ(a.dimension(),1);
        REQUIRE_EQ(a.size(),4);
        REQUIRE_EQ(a(0),3);
        REQUIRE_EQ(a(1),4);
        REQUIRE_EQ(a(2),5);
        REQUIRE_EQ(a(3),6);
    }
    SUBCASE("integer c"){
        auto a = ma::arange(3,7,2);
        REQUIRE_EQ(a.dimension(),1);
        REQUIRE_EQ(a.size(),2);
        REQUIRE_EQ(a(0),3);
        REQUIRE_EQ(a(1),5);
    }
    SUBCASE("float a"){
        auto a = ma::arange(3.0f);
        REQUIRE_EQ(a.dimension(),1);
        REQUIRE_EQ(a.size(),3);
        REQUIRE_EQ(a(0),doctest::Approx(0.0f));
        REQUIRE_EQ(a(1),doctest::Approx(1.0f));
        REQUIRE_EQ(a(2),doctest::Approx(2.0f));
    }
    SUBCASE("float b"){
        auto a = ma::arange(3.0, 7.0);
        REQUIRE_EQ(a.dimension(),1);
        REQUIRE_EQ(a.size(),4);
        REQUIRE_EQ(a(0),doctest::Approx(3.0));
        REQUIRE_EQ(a(1),doctest::Approx(4.0));
        REQUIRE_EQ(a(2),doctest::Approx(5.0));
        REQUIRE_EQ(a(3),doctest::Approx(6.0));
    }
    SUBCASE("float c"){
        auto a = ma::arange(3.0, 7.0, 2.0);
        REQUIRE_EQ(a.dimension(),1);
        REQUIRE_EQ(a.size(),2);
        REQUIRE_EQ(a(0),doctest::Approx(3.0));
        REQUIRE_EQ(a(1),doctest::Approx(5.0));
    }
    SUBCASE("float d"){
        auto a = ma::arange(0.0, 1.0, 0.3);
        REQUIRE_EQ(a.dimension(),1);
        REQUIRE_EQ(a.size(),4);
        REQUIRE_EQ(a(0),doctest::Approx(0.0));
        REQUIRE_EQ(a(1),doctest::Approx(0.3));
        REQUIRE_EQ(a(2),doctest::Approx(0.6));
        REQUIRE_EQ(a(3),doctest::Approx(0.9));
    }
}

TEST_CASE( "[Shape]  -- [detail_multi_array::dot()]" ) {

    SUBCASE("0D Shape"){
        auto s = multi_array::shape();
        REQUIRE_EQ(s.size(),0);
    }
    SUBCASE("1D Shape"){
        auto s = multi_array::shape(2);
        REQUIRE_EQ(s.size(),1);
        REQUIRE_EQ(s[0],2);
    }
    SUBCASE("3D Shape mixed types rrefs"){
        auto s = multi_array::shape(int(2),char(4),uint16_t(6));
        REQUIRE_EQ(s.size(),3);
        REQUIRE_EQ(s[0],2);
        REQUIRE_EQ(s[1],4);
        REQUIRE_EQ(s[2],6);
    }
    SUBCASE("3D Shape same type rrefs"){
        auto s = multi_array::shape(2,4,6);
        REQUIRE_EQ(s.size(),3);
        REQUIRE_EQ(s[0],2);
        REQUIRE_EQ(s[1],4);
        REQUIRE_EQ(s[2],6);
    }
    SUBCASE("4D Shape mixed types lref/cref"){
        const auto s0 = int(4);
        auto s2 = uint16_t(1);
        auto s = multi_array::shape(s0,char(4), s2);

        REQUIRE_EQ(s.size(),3);
        REQUIRE_EQ(s[0],4);
        REQUIRE_EQ(s[1],4);
        REQUIRE_EQ(s[2],1);
    }
}
TEST_CASE( "[Shape]  -- [Shape::countNegativeEntries()]" ) {

    SUBCASE("1D Shape"){
        auto s = multi_array::shape(-1);
        REQUIRE_EQ(s.size(),1);
        const auto nNeg = s.countNegativeEntries();
        REQUIRE_EQ(nNeg,1);
    }
}
TEST_CASE( "[Shape]  -- [Shape::makeShape()]" ) {

    SUBCASE("1D Shape"){
        auto sa = multi_array::shape(-1);
        auto s = sa.makeShape(6);
        REQUIRE_EQ(s.size(),1);
        REQUIRE_EQ(s[0], 6);
    }
}
TEST_CASE( "[Strides]  " ) {

    SUBCASE("copy strides"){

        auto shape = multi_array::shape(3);
        auto strides = multi_array::detail_multi_array::cOrderStrides(shape);

        REQUIRE_EQ(strides[0],1);

        multi_array::Strides<1> s = strides;
        REQUIRE_EQ(s[0],1);
    }
}

TEST_CASE( "2D dot  -- [detail_multi_array::dot()]" ) {
    multi_array::Shape<2> shape;
    shape[0] = 2;
    shape[1] = 3;
    auto strides = multi_array::detail_multi_array::cOrderStrides(shape);
    
    {
        auto i = multi_array::detail_multi_array::dot(strides, 0,0);
        REQUIRE(i==0);
    }
    {
        auto i = multi_array::detail_multi_array::dot(strides, 0,1);
        REQUIRE(i==1);
    }   
    {
        auto i = multi_array::detail_multi_array::dot(strides, 0,2);
        REQUIRE(i==2);
    }

    {
        auto i = multi_array::detail_multi_array::dot(strides, 1,0);
        REQUIRE(i==3);
    }
    {
        auto i = multi_array::detail_multi_array::dot(strides, 1,1);
        REQUIRE(i==4);
    }   
    {
        auto i = multi_array::detail_multi_array::dot(strides, 1,2);
        REQUIRE(i==5);
    }
}

TEST_CASE( "2D cOrderStrides  -- [detail_multi_array::cOrderStrides()]" ) {
    multi_array::Shape<2> shape;
    shape[0] = 20;
    shape[1] = 30;
    auto strides = multi_array::detail_multi_array::cOrderStrides(shape);
    REQUIRE(strides[0] == 30);
    REQUIRE(strides[1] == 1);
}

TEST_CASE( "3D cOrderStrides  -- [detail_multi_array::cOrderStrides()]" ) {
    multi_array::Shape<3> shape;
    shape[0] = 2;
    shape[1] = 3;
    shape[2] = 4;
    auto strides = multi_array::detail_multi_array::cOrderStrides(shape);
    REQUIRE(strides[0] == 12);
    REQUIRE(strides[1] == 4);
    REQUIRE(strides[2] == 1);
}

TEST_CASE( "4D cOrderStrides with Singleton  -- [detail_multi_array::cOrderStrides()]" ) {
    
    SUBCASE("1 Singleton")
    {
        multi_array::Shape<4> shape;
        shape[0] = 2;
        shape[1] = 1;
        shape[2] = 4;
        shape[3] = 5;
        auto strides = multi_array::detail_multi_array::cOrderStrides(shape);
        REQUIRE(strides[0] == 20);
        REQUIRE(strides[1] == 20);
        REQUIRE(strides[2] == 5);
        REQUIRE(strides[3] == 1);
    }

    SUBCASE("Begin Singleton")
    {
        multi_array::Shape<4> shape;
        shape[0] = 1;
        shape[1] = 2;
        shape[2] = 3;
        shape[3] = 4;
        auto strides = multi_array::detail_multi_array::cOrderStrides(shape);
        REQUIRE(strides[0] == 24);
        REQUIRE(strides[1] == 12);
        REQUIRE(strides[2] == 4);
        REQUIRE(strides[3] == 1);
    }
    SUBCASE("Begin Singletons")
    {
        multi_array::Shape<4> shape;
        shape[0] = 1;
        shape[1] = 1;
        shape[2] = 3;
        shape[3] = 4;
        auto strides = multi_array::detail_multi_array::cOrderStrides(shape);
        REQUIRE(strides[0] == 12);
        REQUIRE(strides[1] == 12);
        REQUIRE(strides[2] == 4);
        REQUIRE(strides[3] == 1);
    }
    SUBCASE("Middle Singletons")
    {
        multi_array::Shape<4> shape;
        shape[0] = 2;
        shape[1] = 1;
        shape[2] = 1;
        shape[3] = 4;
        auto strides = multi_array::detail_multi_array::cOrderStrides(shape);
        REQUIRE(strides[0] == 4);
        REQUIRE(strides[1] == 4);
        REQUIRE(strides[2] == 4);
        REQUIRE(strides[3] == 1);
    }
    SUBCASE("End Singletons")
    {
        multi_array::Shape<4> shape;
        shape[0] = 3;
        shape[1] = 2;
        shape[2] = 1;
        shape[3] = 1;
        auto strides = multi_array::detail_multi_array::cOrderStrides(shape);
        REQUIRE(strides[0] == 2);
        REQUIRE(strides[1] == 1);
        REQUIRE(strides[2] == 1);
        REQUIRE(strides[3] == 1);
    }
    SUBCASE("Only Singletons")
    {
        multi_array::Shape<4> shape;
        shape[0] = 1;
        shape[1] = 1;
        shape[2] = 1;
        shape[3] = 1;
        auto strides = multi_array::detail_multi_array::cOrderStrides(shape);
        REQUIRE(strides[0] == 1);
        REQUIRE(strides[1] == 1);
        REQUIRE(strides[2] == 1);
        REQUIRE(strides[3] == 1);
    }
}

TEST_SUITE_END();