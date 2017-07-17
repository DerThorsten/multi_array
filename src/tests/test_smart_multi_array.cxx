#include "doctest.h"

#include <iostream>
#include <memory>
#include <type_traits>

#include "multi_array/multi_array.hxx"
#include "multi_array/multi_array_factories.hxx"

#include "multi_array/meta.hxx"
#include "multi_array/runtime_check.hxx"

TEST_SUITE_BEGIN("SmartMultiArray");


TEST_CASE("[SmartMultiArray] SmartMultiArray::SmartMultiArray"){


    SUBCASE( "Showcase" ) {
        namespace ma = multi_array;
        auto a = ma::ones<float>(2,5,5,9);
        auto b = a(1, ma::all(), ma::range(0,4,2),ma::newAxis(), ma::range(1,8));
    }

    SUBCASE( "Empty Constructor 1D -- [SmartMultiArray::SmartMultiArray]" ) {
        multi_array::SmartMultiArray<float, 1> array;
        REQUIRE(array.empty());
    }

    SUBCASE( "Empty Constructor 2D -- [SmartMultiArray::SmartMultiArray]" ) {
        multi_array::SmartMultiArray<float, 2> array;
        REQUIRE(array.empty());
    }

    SUBCASE( "Empty Constructor 10D -- [SmartMultiArray::SmartMultiArray]" ) {
        multi_array::SmartMultiArray<float, 10> array;
        REQUIRE(array.empty());
    }

    SUBCASE( "Non Empty Constructor 1D -- [SmartMultiArray::SmartMultiArray]" ) {
        multi_array::Shape<1> shape;
        shape[0] = 2;
        multi_array::SmartMultiArray<int, 1> array(shape);
        REQUIRE(!array.empty());
        REQUIRE_EQ(array.shape()[0], 2);
        REQUIRE_EQ(array.strides()[0], 1);
        REQUIRE(array.data() != nullptr);
    }

    SUBCASE( "Non Empty Constructor 2D -- [SmartMultiArray::SmartMultiArray]" ) {
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

    SUBCASE( "Non Empty Constructor 3D -- [SmartMultiArray::SmartMultiArray]" ) {
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
}


TEST_CASE("[SmartMultiArray] SmartMultiArray::operator"){

    SUBCASE( "1D access Simple -- [SmartMultiArray::operator()]" ) {

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

    SUBCASE( "2D access Simple -- [SmartMultiArray::operator()]" ) {

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

    SUBCASE( "3D access array Simple -- [SmartMultiArray::operator()]" ) {

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

    SUBCASE( "2D access Advanced -- [SmartMultiArray::operator()]") {

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
            auto subarray = array(Range(0,3), Range(0,4), NewAxis<1>());
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
            REQUIRE_EQ(subarray.contiguous(), true);

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
            auto subarray = array(all(), all(), NewAxis<1>());
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
            REQUIRE_EQ(subarray.contiguous(), true);

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
            REQUIRE_EQ(subarray.contiguous(), true);

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
            REQUIRE_EQ(subarray.contiguous(), true);

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
            REQUIRE_FALSE(subarray.contiguous());
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
            REQUIRE_FALSE(subarray.contiguous());
        }
        {
            using namespace multi_array;
            auto subarray = array(2, range(1,3));
            REQUIRE_EQ(subarray.shape() [0] , 2);
            REQUIRE_EQ(subarray.strides()[0] , 1);
            REQUIRE_EQ(subarray(0) , 9);
            REQUIRE_EQ(subarray(1) , 10);
            REQUIRE(subarray.contiguous());

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
            REQUIRE(subarray.contiguous());

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
            REQUIRE(subarray.contiguous());
        }
        {
            using namespace multi_array;
            auto subarray = array(2,range(0,4,3));
            REQUIRE_EQ(subarray.dimension(), 1);
            REQUIRE_EQ(subarray.shape(0), 2);
            REQUIRE_EQ(subarray.strides(0), 3);
            REQUIRE_EQ(subarray(0), 8);
            REQUIRE_EQ(subarray(1), 11);
            REQUIRE_FALSE(subarray.contiguous());
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
            REQUIRE(subarray.contiguous());
        }
        {
            using namespace multi_array;
            auto subarray = array(range(0,3,2),1);
            REQUIRE_EQ(subarray.dimension(), 1);
            REQUIRE_EQ(subarray.shape(0), 2);
            REQUIRE_EQ(subarray.strides(0), 8);
            REQUIRE_EQ(subarray(0), 1);
            REQUIRE_EQ(subarray(1), 9);
            REQUIRE_FALSE(subarray.contiguous());
        }
        {
            using namespace multi_array;
            auto subarray = array(0, range(0,4,2));
            REQUIRE_EQ(subarray.dimension(), 1);
            REQUIRE_EQ(subarray.shape(0), 2);
            REQUIRE_EQ(subarray.strides(0), 2);
            REQUIRE_FALSE(subarray.contiguous());
        }
    }
}


TEST_CASE("[SmartMultiArray] SmartMultiArray::overlaps"){
    SUBCASE("Trivial Non Overlapping"){

   
        auto a = multi_array::ones<float>(2,3,4);
        auto b = multi_array::ones<float>(4,8);
        REQUIRE(a.overlaps(a));
        REQUIRE(b.overlaps(b));
        REQUIRE_FALSE(a.overlaps(b));
        REQUIRE_FALSE(b.overlaps(a));
    }

    SUBCASE("Trivial 1D"){
        auto a = multi_array::ones<float>(10);

        SUBCASE("A"){
            auto aa = a(multi_array::range(0,5));
            auto bb = a(multi_array::range(5,10));
            REQUIRE(aa.overlaps(a));
            REQUIRE(bb.overlaps(a));
            REQUIRE(a.overlaps(aa));
            REQUIRE(a.overlaps(bb));
            REQUIRE(aa.overlaps(aa));
            REQUIRE(bb.overlaps(bb));
            REQUIRE_FALSE(aa.overlaps(bb));
            REQUIRE_FALSE(bb.overlaps(aa));
        }
        SUBCASE("B"){
            auto aa = a(multi_array::range(0,6));
            auto bb = a(multi_array::range(5,10));
            auto cc = a(multi_array::range(5,6));
            auto dd = a(multi_array::range(6,7));
            REQUIRE(aa.overlaps(a));
            REQUIRE(bb.overlaps(a));
            REQUIRE(a.overlaps(aa));
            REQUIRE(a.overlaps(bb));
            REQUIRE(aa.overlaps(aa));
            REQUIRE(bb.overlaps(bb));
            REQUIRE(aa.overlaps(bb));
            REQUIRE(bb.overlaps(aa));
            REQUIRE(cc.overlaps(bb));
            REQUIRE(cc.overlaps(aa));
            REQUIRE(dd.overlaps(bb));
            REQUIRE_FALSE(dd.overlaps(aa));
        }
    }
}


TEST_CASE("[SmartMultiArray] SmartMultiArray::forEach"){
    namespace ma = multi_array;

    SUBCASE("2D"){
        // unstrided
        auto a = ma::ones<int>(2,3);
        // strided
        auto bb = ma::ones<int>(2,5,3);
        auto b =  bb(ma::all(), 3, ma::all());

        auto c = 0;
        for(auto x0=0; x0<2; ++x0)
        for(auto x1=0; x1<3; ++x1){
            a(x0, x1) = c;
            b(x0, x1) = c;
            ++c;
        }
        SUBCASE("unstrided"){

            // check order  (c order is current default)
            c = 0;
            a.forEach([&](int & val){
                REQUIRE_EQ(val, c);
                ++c;
            });

            //  write val
            a.forEach([&](int & val){
                val *= 2;
            });

            // read val
            c = 0;
            a.forEach([&](int & val){
                REQUIRE_EQ(val/2, c);
                ++c;
            });
        }
        SUBCASE("strided"){

            // check order  (c order is current default)
            c = 0;
            b.forEach([&](int & val){
                REQUIRE_EQ(val, c);
                ++c;
            });

            //  write val
            b.forEach([&](int & val){
                val *= 2;
            });

            // read val
            c = 0;
            b.forEach([&](int & val){
                REQUIRE_EQ(val/2, c);
                ++c;
            });
        }
    }


    SUBCASE("11D"){
        // unstrided
        auto a = ma::ones<int>(2,2,2,2,2,2,2,2,2,2,2);
        // strided
        auto bb = ma::ones<int>(2,3,2,2,2,2,2,2,2,2,2,2);

        auto b =  bb(ma::all(),1,ma::all(),ma::all(),ma::all(),ma::all(),ma::all(),ma::all(),ma::all(),ma::all(),ma::all(),ma::all());

        auto c = 0;
        for(auto x0  =0;  x0 <2; ++x0 )
        for(auto x1  =0;  x1 <2; ++x1 )
        for(auto x2  =0;  x2 <2; ++x2 )
        for(auto x3  =0;  x3 <2; ++x3 )
        for(auto x4  =0;  x4 <2; ++x4 )
        for(auto x5  =0;  x5 <2; ++x5 )
        for(auto x6  =0;  x6 <2; ++x6 )
        for(auto x7  =0;  x7 <2; ++x7 )
        for(auto x8  =0;  x8 <2; ++x8 )
        for(auto x9  =0;  x9 <2; ++x9 )
        for(auto x10  =0; x10<2; ++x10)
        {
            a(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10) = c;
            b(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10) = c;
            ++c;
        }
        SUBCASE("unstrided"){

            // check order  (c order is current default)
            c = 0;
            a.forEach([&](int & val){
                REQUIRE_EQ(val, c);
                ++c;
            });

            //  write val
            a.forEach([&](int & val){
                val *= 2;
            });

            // read val
            c = 0;
            a.forEach([&](int & val){
                REQUIRE_EQ(val/2, c);
                ++c;
            });
        }
        SUBCASE("strided"){

            // check order  (c order is current default)
            c = 0;
            b.forEach([&](int & val){
                REQUIRE_EQ(val, c);
                ++c;
            });

            //  write val
            b.forEach([&](int & val){
                val *= 2;
            });

            // read val
            c = 0;
            b.forEach([&](int & val){
                REQUIRE_EQ(val/2, c);
                ++c;
            });
        }
    }
    
}


TEST_CASE("[SmartMultiArray] SmartMultiArray::reshape"){

    namespace ma = multi_array;

    SUBCASE("2D"){
        auto a = ma::ones<int>(2,3);

        auto c = 0;
        for(auto x0=0; x0<2; ++x0)
        for(auto x1=0; x1<3; ++x1){
            a(x0, x1) = c;
            ++c;
        }
        REQUIRE(a.contiguous(ma::Order::C_ORDER));
        REQUIRE_EQ(a.smartHandle().useCount(),1);

        SUBCASE("Explicit Shape"){
            // do the reshape
            auto b = a.reshape(ma::shape(6));
            REQUIRE_EQ(b.size(), 6);
            REQUIRE_EQ(b.shape(0), 6);
            // ensure that no copy has been made
            REQUIRE_EQ(a.smartHandle().useCount(),2);
            REQUIRE_EQ(a.data(),b.data());
        }
        SUBCASE("Incomplete Shape"){
            // do the reshape
            auto b = a.reshape(ma::shape(-1));
            REQUIRE_EQ(b.size(), 6);
            REQUIRE_EQ(b.shape(0), 6);
            // ensure that no copy has been made
            REQUIRE_EQ(a.smartHandle().useCount(), 2);
            REQUIRE_EQ(a.data(),b.data());
        }
    }
}


TEST_CASE("[SmartMultiArray] SmartMultiArray::accumulate"){
    
    namespace ma = multi_array;

    SUBCASE("1D"){

        auto a  = ma::arange(0,4);

        SUBCASE("SUM"){
            auto s = a.sum();
            REQUIRE_EQ(6,s);
        }
        SUBCASE("MEAN"){
            auto m = a.mean();
            REQUIRE_EQ(doctest::Approx(6.0/4),m);
        }
    }
}

TEST_CASE("[SmartMultiArray] SmartMultiArray::operator="){
    
    namespace ma = multi_array;

    SUBCASE("assign from scalars"){


        SUBCASE("1D"){

            SUBCASE("matching type"){
                auto a  = ma::arange(0,4);
                REQUIRE(a.contiguous());
                REQUIRE_EQ(a.size(),4);

                a = int(2);
                REQUIRE_EQ(a(0),2);
                REQUIRE_EQ(a(1),2);
                REQUIRE_EQ(a(2),2);
                REQUIRE_EQ(a(3),2);
            }
            SUBCASE("not matching type"){
                auto a  = ma::arange(0,4);
                a = float(2);
                REQUIRE_EQ(a(0),2);
                REQUIRE_EQ(a(1),2);
                REQUIRE_EQ(a(2),2);
                REQUIRE_EQ(a(3),2);
            }
        }
        SUBCASE("2D not dense"){

            SUBCASE("allmost matching type"){
                auto a  = ma::ones<float>(4,4)(ma::range(0,4,2),ma::range(0,4,2));
                REQUIRE_FALSE(a.contiguous());
                REQUIRE_EQ(a.size(),4);

                a = uint8_t(4);
                REQUIRE_EQ(a(0,0),4);
                REQUIRE_EQ(a(0,1),4);
                REQUIRE_EQ(a(1,0),4);
                REQUIRE_EQ(a(1,1),4);
            }
            SUBCASE("not matching type"){
                auto a  = ma::ones<float>(4,4)(ma::range(0,4,2),ma::range(0,4,2));
                REQUIRE_FALSE(a.contiguous());
                REQUIRE_EQ(a.size(),4);

                a = uint8_t(3);
                REQUIRE_EQ(a(0,0),3);
                REQUIRE_EQ(a(0,1),3);
                REQUIRE_EQ(a(1,0),3);
                REQUIRE_EQ(a(1,1),3);
            }
        }
    }
}

TEST_SUITE_END();