#include <iostream>
#include <memory>
#include <type_traits>

#include "multi_array/multi_array.hxx"
#include "multi_array/multi_array_factories.hxx"

#include "multi_array/meta.hxx"
#include "multi_array/runtime_check.hxx"



#include <iostream>       // std::cout
#include <typeinfo>       // operator typeid



int main(){
    namespace ma = multi_array;



    {
        auto a =  ma::ones<int>(2,3);
        auto b =  ma::ones<int>(2,3);
        auto c =  ma::ones<int>(2,3);
        auto d =  c.transposedView();
        std::cout<<"strides a "<<a.strides()<<"\n";
        std::cout<<"strides c "<<c.strides()<<"\n";
        std::cout<<"strides d "<<d.strides()<<"\n";
        auto ad = a + d ;   
        //std::cout<<typeid(ab).name()<<"\n";
        std::cout<<"ad.matchingStrides() ?\n"<<ad.matchingStrides()<<"\n";
      
    }

}
