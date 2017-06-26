#include <iostream>
#include <memory>
#include <type_traits>

#include "multi_array/multi_array.hxx"
#include "multi_array/multi_array_factories.hxx"

#include "multi_array/meta.hxx"
#include "multi_array/runtime_check.hxx"


int main(){
    namespace ma = multi_array;

    auto a =  ma::ones<int>(2,3);
    auto b =  ma::ones<int>(2,3);

    //auto ab  = (a + b);
    auto aba  = (a + b) + a;


    

    std::cout<<"A\n\n\n\n";
    auto s0 = aba.strides(0);
    std::cout<<"B\n\n\n\n";
    auto s1 = aba.strides(1);
    std::cout<<"C\n\n\n\n";
    const auto m = aba.matchingStrides();
    std::cout<<"D\n";
    
}
