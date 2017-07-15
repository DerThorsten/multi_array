#include <iostream>
#include <memory>
#include <type_traits>

#include "multi_array/multi_array.hxx"




#include <iostream>       // std::cout
#include <typeinfo>       // operator typeid



int main(){


    namespace ma = multi_array;
    const static ma::NewAxis NA;
    const static ma::All     ALL;


    


    // some arrays with real data
    auto a = ma::zeros<ValueType>(20,20,5,3);
    auto b = ma::ones<ValueType>(5,3);

    // in numpy python syntax c=b[None, None,...]
    c = b(NA,NA, ALL, ALL);


}
