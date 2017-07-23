#include <iostream>
#include <memory>
#include <type_traits>

#include "multi_array/multi_array.hxx"




#include <iostream>       // std::cout
#include <typeinfo>       // operator typeid



int main(){


    namespace ma = multi_array;
    const static ma::NewAxis<1> NA;




    // some arrays with real data
    // auto a = ma::zeros<ValueType>(20,20,5,3);
    // auto b = ma::ones<ValueType>(5,3);
    // c = b(NA,NA, ALL, ALL);


    auto a = ma::arange(20);
    auto b = ma::arange(20);
    auto c = ma::ones<int>(20,20);

    auto exp = a(ma::all(), ma::newAxis()) *  b(ma::newAxis(), ma::All);

    c = exp;
}
