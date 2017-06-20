#include "doctest.h"

#include <iostream>
#include <memory>
#include <type_traits>

#include "multi_array/multi_array.hxx"
#include "multi_array/meta.hxx"
#include "multi_array/runtime_check.hxx"

TEST_SUITE_BEGIN("SmartMultiArray");


TEST_CASE("SmartMultiArray::operator="){

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

    REQUIRE_FALSE(array.empty());


    SUBCASE("assign to empty"){

        multi_array::SmartMultiArray<int, 2> empty;
        REQUIRE(empty.empty());
        empty = array;
        REQUIRE_FALSE(empty.empty());
        
    }
}




TEST_SUITE_END();