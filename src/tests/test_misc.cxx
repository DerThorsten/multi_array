#include "doctest.h"

#include <iostream>
#include <memory>
#include <type_traits>

#include "multi_array/multi_array.hxx"
#include "multi_array/meta.hxx"
#include "multi_array/runtime_check.hxx"

TEST_SUITE_BEGIN("SmartMultiArray");


TEST_CASE( "Empty Constructor 1D -- [SmartMultiArray::SmartMultiArray]" ) {
    multi_array::SmartMultiArray<float, 1> array;
    REQUIRE(array.empty());
}


TEST_CASE( "Empty Constructor 2D -- [SmartMultiArray::SmartMultiArray]" ) {
    multi_array::SmartMultiArray<float, 2> array;
    REQUIRE(array.empty());
}

TEST_CASE( "Empty Constructor 10D -- [SmartMultiArray::SmartMultiArray]" ) {
    multi_array::SmartMultiArray<float, 10> array;
    REQUIRE(array.empty());
}


TEST_CASE( "Non Empty Constructor 1D -- [SmartMultiArray::SmartMultiArray]" ) {
    multi_array::Shape<1> shape;
    shape[0] = 2;
    multi_array::SmartMultiArray<int, 1> array(shape);
    REQUIRE(!array.empty());
    REQUIRE_EQ(array.shape()[0], 2);
    REQUIRE_EQ(array.strides()[0], 1);
    REQUIRE(array.data() != nullptr);
}

TEST_CASE( "Non Empty Constructor 2D -- [SmartMultiArray::SmartMultiArray]" ) {
    multi_array::Shape<2> shape;
    shape[0] = 2;
    shape[1] = 3;
    multi_array::SmartMultiArray<int, 2> array(shape);
    REQUIRE(!array.empty());
    REQUIRE(array.shape()[0] == 2);
    REQUIRE(array.shape()[1] == 3);
    REQUIRE(array.strides()[0] == 3);
    REQUIRE(array.strides()[1] == 1);
    REQUIRE(array.data() != nullptr);

}

TEST_CASE( "Non Empty Constructor 3D -- [SmartMultiArray::SmartMultiArray]" ) {
    multi_array::Shape<3> shape;
    shape[0] = 2;
    shape[1] = 3;
    shape[2] = 4;
    multi_array::SmartMultiArray<int, 3> array(shape);
    REQUIRE(!array.empty());

    REQUIRE(array.shape()[0] == 2);
    REQUIRE(array.shape()[1] == 3);
    REQUIRE(array.shape()[2] == 4);
    REQUIRE(array.strides()[0] == 12);
    REQUIRE(array.strides()[1] == 4);
    REQUIRE(array.strides()[2] == 1);
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

TEST_CASE( "1D access Simple -- [SmartMultiArray::operator()]" ) {

    multi_array::Shape<1> shape;
    shape[0] = 3;
    multi_array::SmartMultiArray<int, 1> array(shape);

    {   // write 
        auto c = 0;
        for(auto x0=0; x0<shape[0]; ++x0){
            array(x0) = int(c);
            ++c;
        }
    }

    {   // read 
        auto c = 0;
        for(auto x0=0; x0<shape[0]; ++x0){
            uint8_t x1UInt8 = x0;
            const int  val = array(x1UInt8);
            MULTI_ARRAY_TEST_OP(val,==,c);
            REQUIRE(val == c );
            ++c;
        }
    }
}

TEST_CASE( "2D access Simple -- [SmartMultiArray::operator()]" ) {

    multi_array::Shape<2> shape;
    shape[0] = 20;
    shape[1] = 30;
    multi_array::SmartMultiArray<int, 2> array(shape);

    {   // write 
        auto c = 0;
        for(auto x0=0; x0<shape[0]; ++x0)
        for(auto x1=0; x1<shape[1]; ++x1){
            array(x0, int(x1)) = int(c);
            ++c;
        }
    }

    {   // read 
        auto c = 0;
        for(auto x0=0; x0<shape[0]; ++x0)
        for(auto x1=0; x1<shape[1]; ++x1){
            uint8_t x1UInt8 = x1;
            const int  val = array(x0, x1);
            MULTI_ARRAY_TEST_OP(val,==,c);
            REQUIRE(val == c );
            ++c;
        }
    }
}

TEST_CASE( "3D access array Simple -- [SmartMultiArray::operator()]" ) {

    multi_array::Shape<3> shape;
    shape[0] = 2;
    shape[1] = 3;
    shape[2] = 3;
    multi_array::SmartMultiArray<uint64_t, 3> array(shape);

    {   // write 
        auto c = 0;
        for(auto x0=0; x0<shape[0]; ++x0)
        for(auto x1=0; x1<shape[1]; ++x1)
        for(auto x2=0; x2<shape[2]; ++x2)
        {
            array(x0, int(x1), uint8_t(x2)) = int(c);
            ++c;
        }
    }

    {   // read 
        auto c = 0;
        for(auto x0=0; x0<shape[0]; ++x0)
        for(auto x1=0; x1<shape[1]; ++x1)
        for(auto x2=0; x2<shape[2]; ++x2){
            uint8_t x1UInt8 = x1;
            const int  val = array(x0, x1UInt8, x2);
            MULTI_ARRAY_TEST_OP(val,==,c);
            REQUIRE(val == c );
            ++c;
        }
    }
}



TEST_CASE( "2D access Advanced -- [SmartMultiArray::operator()]" * doctest::description("to long test..") ) {

    multi_array::Shape<2> shape;
    shape[0] = 3;
    shape[1] = 4;
    multi_array::SmartMultiArray<int, 2> array(shape);

    REQUIRE_EQ(array.smartHandle().useCount(),1);

    {   // write 
        auto c = 0;
        for(auto x0=0; x0<shape[0]; ++x0)
        for(auto x1=0; x1<shape[1]; ++x1){
            array(x0, int(x1)) = int(c);
            ++c;
        }
    }
    /* The Array:

        0  4  8
        1  5  9
        2  6  10
        3  7  11

    */
    SUBCASE("Singleton Simple")
    {
        using namespace multi_array;

        REQUIRE_EQ(array.smartHandle().useCount(),1);
        auto subarray = array(Range(0,3), Range(0,4), NewAxis());
        REQUIRE_EQ(array.smartHandle().useCount(),2);
        REQUIRE_EQ(subarray.smartHandle().useCount(),2);
        

        REQUIRE_EQ(subarray.dimension(), 3);
        REQUIRE_EQ(subarray.shape(0), 3);
        REQUIRE_EQ(subarray.shape(1), 4);
        REQUIRE_EQ(subarray.shape(2), 1);
        REQUIRE_EQ(subarray.strides(0), 4);       
        REQUIRE_EQ(subarray.strides(1), 1);   
        REQUIRE_EQ(subarray.strides(2), 0);
        REQUIRE_EQ(subarray.data(),array.data());
        REQUIRE_EQ(subarray.isDense(), true);

        REQUIRE_EQ(&subarray.front() ,&subarray(0,0,0));
        REQUIRE_EQ(subarray.front() ,0);
        REQUIRE_EQ(&subarray.back() ,&subarray(2,3,0));
        REQUIRE_EQ(subarray.back() ,11);

        REQUIRE_EQ(subarray(0,0,0) ,0);
        REQUIRE_EQ(subarray(0,1,0) ,1);
        REQUIRE_EQ(subarray(0,2,0) ,2);
        REQUIRE_EQ(subarray(0,3,0) ,3);
        REQUIRE_EQ(subarray(1,0,0) ,4);
        REQUIRE_EQ(subarray(1,1,0) ,5);
        REQUIRE_EQ(subarray(1,2,0) ,6);
        REQUIRE_EQ(subarray(1,3,0) ,7);
        REQUIRE_EQ(subarray(2,0,0) ,8);
        REQUIRE_EQ(subarray(2,1,0) ,9);
        REQUIRE_EQ(subarray(2,2,0) ,10);
        REQUIRE_EQ(subarray(2,3,0) ,11);
    }
    SUBCASE("Singleton And All Range")
    {
        using namespace multi_array;

        REQUIRE_EQ(array.smartHandle().useCount(),1);
        auto subarray = array(all(), all(), NewAxis());
        REQUIRE_EQ(array.smartHandle().useCount(),2);
        REQUIRE_EQ(subarray.smartHandle().useCount(),2);
        

        REQUIRE_EQ(subarray.dimension(), 3);
        REQUIRE_EQ(subarray.shape(0), 3);
        REQUIRE_EQ(subarray.shape(1), 4);
        REQUIRE_EQ(subarray.shape(2), 1);
        REQUIRE_EQ(subarray.strides(0), 4);       
        REQUIRE_EQ(subarray.strides(1), 1);   
        REQUIRE_EQ(subarray.strides(2), 0);
        REQUIRE_EQ(subarray.data(),array.data());
        REQUIRE_EQ(subarray.isDense(), true);

        REQUIRE_EQ(&subarray.front() ,&subarray(0,0,0));
        REQUIRE_EQ(subarray.front() ,0);
        REQUIRE_EQ(&subarray.back() ,&subarray(2,3,0));
        REQUIRE_EQ(subarray.back() ,11);

        REQUIRE_EQ(subarray(0,0,0) ,0);
        REQUIRE_EQ(subarray(0,1,0) ,1);
        REQUIRE_EQ(subarray(0,2,0) ,2);
        REQUIRE_EQ(subarray(0,3,0) ,3);
        REQUIRE_EQ(subarray(1,0,0) ,4);
        REQUIRE_EQ(subarray(1,1,0) ,5);
        REQUIRE_EQ(subarray(1,2,0) ,6);
        REQUIRE_EQ(subarray(1,3,0) ,7);
        REQUIRE_EQ(subarray(2,0,0) ,8);
        REQUIRE_EQ(subarray(2,1,0) ,9);
        REQUIRE_EQ(subarray(2,2,0) ,10);
        REQUIRE_EQ(subarray(2,3,0) ,11);
    }
    REQUIRE_EQ(array.smartHandle().useCount(),1);

    SUBCASE("Range-Range A")
    {
        using namespace multi_array;

        REQUIRE_EQ(array.smartHandle().useCount(),1);
        auto subarray = array(Range(0,3), Range(0,4));

        REQUIRE_EQ(subarray.dimension(), 2);
        REQUIRE_EQ(subarray.shape(0), 3);
        REQUIRE_EQ(subarray.shape(1), 4);
        REQUIRE_EQ(subarray.strides(0), 4);       
        REQUIRE_EQ(subarray.strides(1), 1);   
        REQUIRE_EQ(subarray.data(),array.data());
        REQUIRE_EQ(subarray(0,0) ,0);
        REQUIRE_EQ(&subarray.front() ,&subarray(0,0));
        REQUIRE_EQ(subarray.front() ,0);
        REQUIRE_EQ(&subarray.back() ,&subarray(2,3));
        REQUIRE_EQ(subarray.back() ,11);
        REQUIRE_EQ(subarray.isDense(), true);

        // read 
        auto c = 0;
        for(auto x0=0; x0<shape[0]; ++x0)
        for(auto x1=0; x1<shape[1]; ++x1){
            uint8_t x1UInt8 = x1;
            const int  val = subarray(x0, x1);
            MULTI_ARRAY_TEST_OP(val,==,c);
            REQUIRE_EQ(val, c );
            ++c;
        }
    }
    SUBCASE("Range-All A")
    {
        using namespace multi_array;

        REQUIRE_EQ(array.smartHandle().useCount(),1);
        auto subarray = array(Range(0,3), All());

        REQUIRE_EQ(subarray.dimension(), 2);
        REQUIRE_EQ(subarray.shape(0), 3);
        REQUIRE_EQ(subarray.shape(1), 4);
        REQUIRE_EQ(subarray.strides(0), 4);       
        REQUIRE_EQ(subarray.strides(1), 1);   
        REQUIRE_EQ(subarray.data(),array.data());
        REQUIRE_EQ(subarray(0,0) ,0);
        REQUIRE_EQ(&subarray.front() ,&subarray(0,0));
        REQUIRE_EQ(subarray.front() ,0);
        REQUIRE_EQ(&subarray.back() ,&subarray(2,3));
        REQUIRE_EQ(subarray.back() ,11);
        REQUIRE_EQ(subarray.isDense(), true);

        // read 
        auto c = 0;
        for(auto x0=0; x0<shape[0]; ++x0)
        for(auto x1=0; x1<shape[1]; ++x1){
            uint8_t x1UInt8 = x1;
            const int  val = subarray(x0, x1);
            MULTI_ARRAY_TEST_OP(val,==,c);
            REQUIRE_EQ(val, c );
            ++c;
        }
    }
    //SUBCASE("Range-Range A")
    {
        using namespace multi_array;
        auto subarray = array(range(1,3), range(2,4));
        REQUIRE_EQ(subarray(0,0) , 6);
        REQUIRE_EQ(subarray(0,1) , 7);
        REQUIRE_EQ(subarray(1,0) , 10);
        REQUIRE_EQ(subarray(1,1) , 11);
        REQUIRE_EQ(&subarray.front() ,&subarray(0,0));
        REQUIRE_EQ(subarray.front() ,6);
        REQUIRE_EQ(&subarray.back() ,&subarray(1,1));
        REQUIRE_EQ(subarray.back() ,11);
        REQUIRE_FALSE(subarray.isDense());
    }
    {
        using namespace multi_array;
        auto subarray = array(range(1,3), 1);
        REQUIRE_EQ(subarray.shape() [0] , 2);
        REQUIRE_EQ(subarray.strides()[0] , 4);
        REQUIRE_EQ(subarray(0) , 5);
        REQUIRE_EQ(subarray(1) , 9);

        REQUIRE_EQ(&subarray.front() ,&subarray(0));
        REQUIRE_EQ(subarray.front() ,5);
        REQUIRE_EQ(&subarray.back() ,&subarray(1));
        REQUIRE_EQ(subarray.back() ,9);
        REQUIRE_FALSE(subarray.isDense());
    }
    {
        using namespace multi_array;
        auto subarray = array(2, range(1,3));
        REQUIRE_EQ(subarray.shape() [0] , 2);
        REQUIRE_EQ(subarray.strides()[0] , 1);
        REQUIRE_EQ(subarray(0) , 9);
        REQUIRE_EQ(subarray(1) , 10);
        REQUIRE(subarray.isDense());

        REQUIRE_EQ(&subarray.front() ,&subarray(0));
        REQUIRE_EQ(subarray.front() ,9);
        REQUIRE_EQ(&subarray.back() ,&subarray(1));
        REQUIRE_EQ(subarray.back() ,10);
    }
    {
        using namespace multi_array;
        auto aRange = range(1,3);
        auto subarray = array(2, aRange);
        REQUIRE_EQ(subarray.shape() [0] , 2);
        REQUIRE_EQ(subarray.strides()[0] , 1);
        REQUIRE_EQ(subarray(0) , 9);
        REQUIRE_EQ(subarray(1) , 10);
        REQUIRE(subarray.isDense());

        REQUIRE_EQ(&subarray.front() ,&subarray(0));
        REQUIRE_EQ(subarray.front() ,9);
        REQUIRE_EQ(&subarray.back() ,&subarray(1));
        REQUIRE_EQ(subarray.back() ,10);
    }
    {
        using namespace multi_array;
        auto subarray = array(2,range(0,4,4));
        REQUIRE_EQ(subarray.dimension(), 1);
        REQUIRE_EQ(subarray.shape(0), 1);
        REQUIRE_EQ(subarray.strides(0), 4);
        REQUIRE_EQ(subarray(0), 8);

        // THIS IS an interesting 
        // corner case.
        // while strides may suggest non-density
        // since there is only 1 element it
        // is always dense
        REQUIRE(subarray.isDense());
    }
    {
        using namespace multi_array;
        auto subarray = array(2,range(0,4,3));
        REQUIRE_EQ(subarray.dimension(), 1);
        REQUIRE_EQ(subarray.shape(0), 2);
        REQUIRE_EQ(subarray.strides(0), 3);
        REQUIRE_EQ(subarray(0), 8);
        REQUIRE_EQ(subarray(1), 11);
        REQUIRE_FALSE(subarray.isDense());
    }
    {
        using namespace multi_array;
        auto subarray = array(range(0,3,3),1);
        REQUIRE_EQ(subarray.dimension(), 1);
        REQUIRE_EQ(subarray.shape(0), 1);
        REQUIRE_EQ(subarray.strides(0), 12);
        REQUIRE_EQ(subarray(0), 1);


        // THIS IS an interesting 
        // corner case.
        // while strides may suggest non-density
        // since there is only 1 element it
        // is always dense
        REQUIRE(subarray.isDense());
    }
    {
        using namespace multi_array;
        auto subarray = array(range(0,3,2),1);
        REQUIRE_EQ(subarray.dimension(), 1);
        REQUIRE_EQ(subarray.shape(0), 2);
        REQUIRE_EQ(subarray.strides(0), 8);
        REQUIRE_EQ(subarray(0), 1);
        REQUIRE_EQ(subarray(1), 9);
        REQUIRE_FALSE(subarray.isDense());
    }
    {
        using namespace multi_array;
        auto subarray = array(0, range(0,4,2));
        REQUIRE_EQ(subarray.dimension(), 1);
        REQUIRE_EQ(subarray.shape(0), 2);
        REQUIRE_EQ(subarray.strides(0), 2);
        REQUIRE_FALSE(subarray.isDense());
    }
}

TEST_SUITE_END();