#include <iostream>
#include <memory>
#include <type_traits>

#include "multi_array/multi_array.hxx"




#include <iostream>       // std::cout
#include <typeinfo>       // operator typeid



int main(){
    namespace ma = multi_array;



     {
         typedef double ValueType;
         const static std::size_t DIM = 5;

        

        // some arrays with real data
        auto a = ma::zeros<ValueType>(10,10,10,3,5);
        auto b = ma::zeros<ValueType>(3,5);



        // ca and cb are the same

        // ca = b[None,None,None,:,:]
        auto ca = b(ma::newAxis(), ma::newAxis(),ma::newAxis(), ma::all(), ma::all());
        // cb = b[None,None,None,:,:]
        auto cb = b(ma::NewAxis<3>(), ma::all(), ma::all());
        


        /// cc = b[None,None,None,...]
        //uto cc = b(ma::newAxis(), ma::newAxis(),ma::newAxis(), ma::ellipsis());
        /// cc = b[None,None,None,...]
        //uto cd = b(ma::NewAxis<3>(),ma::ellipsis());
        ///auto cd = b(ma::ellipsis());

         
     }

}
