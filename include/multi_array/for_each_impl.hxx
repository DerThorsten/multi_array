#pragma once

#include "multi_array/navigator.hxx"
#include "multi_array/order.hxx"

namespace multi_array{
namespace detail_for_each{


template<std::size_t DIM, uint8_t ORDER >
struct ForEachImpl;


// c-order fallback
template<std::size_t DIM>
struct ForEachImpl<DIM, uint8_t(Order::C_ORDER)>{
    const static std::size_t MAX_DIM = DIM - 1;
    template<class A, class F>
    static void op(A && array, F && f){

        auto data = array.data();
        Navigator<DIM> navigator(array.shape(), array.strides());
        const auto s = array.shape();

        const auto & coordinate = navigator.coordinate();
        
        for(;;){
            const auto o = navigator.offset();
            f(data[o]);

            for(int j=int(MAX_DIM); j>=0; --j) {

                if(coordinate[j]+1 == s[j]) {
                    if(j == 0) {
                        return;
                    }
                    else {
                        navigator.set(j, 0);
                    }
                }
                else {
                    navigator.increment(j);
                    break;
                }
            }
        }
    }
};


// f-order fallback
template<std::size_t DIM>
struct ForEachImpl<DIM, uint8_t(Order::F_ORDER)>{
    const static std::size_t MAX_DIM = DIM - 1;
    template<class A, class F>
    static void op(A && array, F && f){

        auto data = array.data();
        Navigator<DIM> navigator(array.shape(), array.strides());
        const auto s = array.shape();

        const auto & coordinate = navigator.coordinate();
        
        for(;;){
            const auto o = navigator.offset();
            f(data[o]);

            for(int j=0; j<DIM; --j) {

                if(coordinate[j]+1 == s[j]) {
                    if(j == MAX_DIM) {
                        return;
                    }
                    else {
                        navigator.set(j, 0);
                    }
                }
                else {
                    navigator.increment(j);
                    break;
                }
            }
        }
    }
};

template<>
struct ForEachImpl<0, uint8_t(Order::C_ORDER)>{
template<class A, class F>
    static void op(A && array, F && f){
        auto data = array.data();
        f(*data);
    }
};
template<>
struct ForEachImpl<1, uint8_t(Order::C_ORDER) >{
    template<class A, class F>
    static void op(A && array, F && f){
        auto data = array.data();
        Navigator<1> navigator(array.shape(), array.strides());
        const auto s = array.shape();
        for(navigator.set(0, 0); navigator.get(0)<s[0]; navigator.increment(0)){
            f(data[navigator.offset()]);
        }
    }
};
template<>
struct ForEachImpl<2, uint8_t(Order::C_ORDER) >{
    template<class A, class F>
    static void op(A && array, F && f){
        auto data = array.data();
        Navigator<2> navigator(array.shape(), array.strides());
        const auto s = array.shape();
        for(navigator.set(0, 0); navigator.get(0)<s[0]; navigator.increment(0))
        for(navigator.set(1, 0); navigator.get(1)<s[1]; navigator.increment(1)){
            f(data[navigator.offset()]);
        }
    }
};

template<>
struct ForEachImpl<3, uint8_t(Order::C_ORDER) >{
    template<class A, class F>
    static void op(A && array, F && f){
        auto data = array.data();
        Navigator<3> navigator(array.shape(), array.strides());
        const auto s = array.shape();
        for(navigator.set(0, 0); navigator.get(0)<s[0]; navigator.increment(0))
        for(navigator.set(1, 0); navigator.get(1)<s[1]; navigator.increment(1))
        for(navigator.set(2, 0); navigator.get(2)<s[2]; navigator.increment(2)){
            f(data[navigator.offset()]);
        }
    }
};

template<>
struct ForEachImpl<4, uint8_t(Order::C_ORDER) >{
    template<class A, class F>
    static void op(A && array, F && f){
        auto data = array.data();
        Navigator<4> navigator(array.shape(), array.strides());
        const auto s = array.shape();
        for(navigator.set(0, 0); navigator.get(0)<s[0]; navigator.increment(0))
        for(navigator.set(1, 0); navigator.get(1)<s[1]; navigator.increment(1))
        for(navigator.set(2, 0); navigator.get(2)<s[2]; navigator.increment(2))
        for(navigator.set(3, 0); navigator.get(3)<s[3]; navigator.increment(3)){
            f(data[navigator.offset()]);
        }
    }
};

template<>
struct ForEachImpl<5, uint8_t(Order::C_ORDER) >{
    template<class A, class F>
    static void op(A && array, F && f){
        auto data = array.data();
        Navigator<5> navigator(array.shape(), array.strides());
        const auto s = array.shape();
        for(navigator.set(0, 0); navigator.get(0)<s[0]; navigator.increment(0))
        for(navigator.set(1, 0); navigator.get(1)<s[1]; navigator.increment(1))
        for(navigator.set(2, 0); navigator.get(2)<s[2]; navigator.increment(2))
        for(navigator.set(3, 0); navigator.get(3)<s[3]; navigator.increment(3))
        for(navigator.set(4, 0); navigator.get(4)<s[4]; navigator.increment(4))
        {
            f(data[navigator.offset()]);
        }
    }
};

template<>
struct ForEachImpl<6, uint8_t(Order::C_ORDER) >{
    template<class A, class F>
    static void op(A && array, F && f){
        auto data = array.data();
        Navigator<6> navigator(array.shape(), array.strides());
        const auto s = array.shape();
        for(navigator.set(0, 0); navigator.get(0)<s[0]; navigator.increment(0))
        for(navigator.set(1, 0); navigator.get(1)<s[1]; navigator.increment(1))
        for(navigator.set(2, 0); navigator.get(2)<s[2]; navigator.increment(2))
        for(navigator.set(3, 0); navigator.get(3)<s[3]; navigator.increment(3))
        for(navigator.set(4, 0); navigator.get(4)<s[4]; navigator.increment(4))
        for(navigator.set(5, 0); navigator.get(5)<s[5]; navigator.increment(5)){
            f(data[navigator.offset()]);
        }
    }
};

template<>
struct ForEachImpl<7, uint8_t(Order::C_ORDER) >{
    template<class A, class F>
    static void op(A && array, F && f){
        auto data = array.data();
        Navigator<7> navigator(array.shape(), array.strides());
        const auto s = array.shape();
        for(navigator.set(0, 0); navigator.get(0)<s[0]; navigator.increment(0))
        for(navigator.set(1, 0); navigator.get(1)<s[1]; navigator.increment(1))
        for(navigator.set(2, 0); navigator.get(2)<s[2]; navigator.increment(2))
        for(navigator.set(3, 0); navigator.get(3)<s[3]; navigator.increment(3))
        for(navigator.set(4, 0); navigator.get(4)<s[4]; navigator.increment(4))
        for(navigator.set(5, 0); navigator.get(5)<s[5]; navigator.increment(5))
        for(navigator.set(6, 0); navigator.get(6)<s[6]; navigator.increment(6)){
            f(data[navigator.offset()]);
        }
    }
};

template<>
struct ForEachImpl<8, uint8_t(Order::C_ORDER) >{
    template<class A, class F>
    static void op(A && array, F && f){
        auto data = array.data();
        Navigator<8> navigator(array.shape(), array.strides());
        const auto s = array.shape();
        for(navigator.set(0, 0); navigator.get(0)<s[0]; navigator.increment(0))
        for(navigator.set(1, 0); navigator.get(1)<s[1]; navigator.increment(1))
        for(navigator.set(2, 0); navigator.get(2)<s[2]; navigator.increment(2))
        for(navigator.set(3, 0); navigator.get(3)<s[3]; navigator.increment(3))
        for(navigator.set(4, 0); navigator.get(4)<s[4]; navigator.increment(4))
        for(navigator.set(5, 0); navigator.get(5)<s[5]; navigator.increment(5))
        for(navigator.set(6, 0); navigator.get(6)<s[6]; navigator.increment(6))
        for(navigator.set(7, 0); navigator.get(7)<s[7]; navigator.increment(7)){
            f(data[navigator.offset()]);
        }
    }
};

template<>
struct ForEachImpl<9, uint8_t(Order::C_ORDER) >{
    template<class A, class F>
    static void op(A && array, F && f){
        auto data = array.data();
        Navigator<9> navigator(array.shape(), array.strides());
        const auto s = array.shape();
        for(navigator.set(0, 0); navigator.get(0)<s[0]; navigator.increment(0))
        for(navigator.set(1, 0); navigator.get(1)<s[1]; navigator.increment(1))
        for(navigator.set(2, 0); navigator.get(2)<s[2]; navigator.increment(2))
        for(navigator.set(3, 0); navigator.get(3)<s[3]; navigator.increment(3))
        for(navigator.set(4, 0); navigator.get(4)<s[4]; navigator.increment(4))
        for(navigator.set(5, 0); navigator.get(5)<s[5]; navigator.increment(5))
        for(navigator.set(6, 0); navigator.get(6)<s[6]; navigator.increment(6))
        for(navigator.set(7, 0); navigator.get(7)<s[7]; navigator.increment(7))
        for(navigator.set(8, 0); navigator.get(8)<s[8]; navigator.increment(8)){
            f(data[navigator.offset()]);
        }
    }
};

template<>
struct ForEachImpl<10, uint8_t(Order::C_ORDER) >{
    template<class A, class F>
    static void op(A && array, F && f){
        auto data = array.data();
        Navigator<10> navigator(array.shape(), array.strides());
        const auto s = array.shape();
        for(navigator.set(0, 0); navigator.get(0)<s[0]; navigator.increment(0))
        for(navigator.set(1, 0); navigator.get(1)<s[1]; navigator.increment(1))
        for(navigator.set(2, 0); navigator.get(2)<s[2]; navigator.increment(2))
        for(navigator.set(3, 0); navigator.get(3)<s[3]; navigator.increment(3))
        for(navigator.set(4, 0); navigator.get(4)<s[4]; navigator.increment(4))
        for(navigator.set(5, 0); navigator.get(5)<s[5]; navigator.increment(5))
        for(navigator.set(6, 0); navigator.get(6)<s[6]; navigator.increment(6))
        for(navigator.set(7, 0); navigator.get(7)<s[7]; navigator.increment(7))
        for(navigator.set(8, 0); navigator.get(8)<s[8]; navigator.increment(8))
        for(navigator.set(9, 0); navigator.get(9)<s[9]; navigator.increment(9)){
            f(data[navigator.offset()]);
        }
    }
};

template<>
struct ForEachImpl<0, uint8_t(Order::F_ORDER)>{
template<class A, class F>
    static void op(A && array, F && f){
        auto data = array.data();
        f(*data);
    }
};
template<>
struct ForEachImpl<1, uint8_t(Order::F_ORDER)>{
    template<class A, class F>
    static void op(A && array, F && f){
        auto data = array.data();
        Navigator<1> navigator(array.shape(), array.strides());
        const auto s = array.shape();
        for(navigator.set(0, 0); navigator.get(0)<s[0]; navigator.increment(0))
        {
            f(data[navigator.offset()]);
        }
    }
};
template<>
struct ForEachImpl<2, uint8_t(Order::F_ORDER)>{
    template<class A, class F>
    static void op(A && array, F && f){
        auto data = array.data();
        Navigator<2> navigator(array.shape(), array.strides());
        const auto s = array.shape();
        for(navigator.set(1, 0); navigator.get(1)<s[1]; navigator.increment(1))
        for(navigator.set(0, 0); navigator.get(0)<s[0]; navigator.increment(0))
        {
            f(data[navigator.offset()]);
        }
    }
};

template<>
struct ForEachImpl<3, uint8_t(Order::F_ORDER)>{
    template<class A, class F>
    static void op(A && array, F && f){
        auto data = array.data();
        Navigator<3> navigator(array.shape(), array.strides());
        const auto s = array.shape();
        for(navigator.set(2, 0); navigator.get(2)<s[2]; navigator.increment(2))
        for(navigator.set(1, 0); navigator.get(1)<s[1]; navigator.increment(1))
        for(navigator.set(0, 0); navigator.get(0)<s[0]; navigator.increment(0))
        {
            f(data[navigator.offset()]);
        }
    }
};

template<>
struct ForEachImpl<4, uint8_t(Order::F_ORDER)>{
    template<class A, class F>
    static void op(A && array, F && f){
        auto data = array.data();
        Navigator<4> navigator(array.shape(), array.strides());
        const auto s = array.shape();
        for(navigator.set(3, 0); navigator.get(3)<s[3]; navigator.increment(3))
        for(navigator.set(2, 0); navigator.get(2)<s[2]; navigator.increment(2))
        for(navigator.set(1, 0); navigator.get(1)<s[1]; navigator.increment(1))
        for(navigator.set(0, 0); navigator.get(0)<s[0]; navigator.increment(0))
        {
            f(data[navigator.offset()]);
        }
    }
};

template<>
struct ForEachImpl<5, uint8_t(Order::F_ORDER)>{
    template<class A, class F>
    static void op(A && array, F && f){
        auto data = array.data();
        Navigator<5> navigator(array.shape(), array.strides());
        const auto s = array.shape();
        for(navigator.set(4, 0); navigator.get(4)<s[4]; navigator.increment(4))
        for(navigator.set(3, 0); navigator.get(3)<s[3]; navigator.increment(3))
        for(navigator.set(2, 0); navigator.get(2)<s[2]; navigator.increment(2))
        for(navigator.set(1, 0); navigator.get(1)<s[1]; navigator.increment(1))
        for(navigator.set(0, 0); navigator.get(0)<s[0]; navigator.increment(0))
        {
            f(data[navigator.offset()]);
        }
    }
};

template<>
struct ForEachImpl<6, uint8_t(Order::F_ORDER)>{
    template<class A, class F>
    static void op(A && array, F && f){
        auto data = array.data();
        Navigator<6> navigator(array.shape(), array.strides());
        const auto s = array.shape();
        for(navigator.set(5, 0); navigator.get(5)<s[5]; navigator.increment(5))
        for(navigator.set(4, 0); navigator.get(4)<s[4]; navigator.increment(4))
        for(navigator.set(3, 0); navigator.get(3)<s[3]; navigator.increment(3))
        for(navigator.set(2, 0); navigator.get(2)<s[2]; navigator.increment(2))
        for(navigator.set(1, 0); navigator.get(1)<s[1]; navigator.increment(1))
        for(navigator.set(0, 0); navigator.get(0)<s[0]; navigator.increment(0))
        {
            f(data[navigator.offset()]);
        }
    }
};

template<>
struct ForEachImpl<7, uint8_t(Order::F_ORDER)>{
    template<class A, class F>
    static void op(A && array, F && f){
        auto data = array.data();
        Navigator<7> navigator(array.shape(), array.strides());
        const auto s = array.shape();
        for(navigator.set(6, 0); navigator.get(6)<s[6]; navigator.increment(6))
        for(navigator.set(5, 0); navigator.get(5)<s[5]; navigator.increment(5))
        for(navigator.set(4, 0); navigator.get(4)<s[4]; navigator.increment(4))
        for(navigator.set(3, 0); navigator.get(3)<s[3]; navigator.increment(3))
        for(navigator.set(2, 0); navigator.get(2)<s[2]; navigator.increment(2))
        for(navigator.set(1, 0); navigator.get(1)<s[1]; navigator.increment(1))
        for(navigator.set(0, 0); navigator.get(0)<s[0]; navigator.increment(0))
        {
            f(data[navigator.offset()]);
        }
    }
};

template<>
struct ForEachImpl<8, uint8_t(Order::F_ORDER)>{
    template<class A, class F>
    static void op(A && array, F && f){
        auto data = array.data();
        Navigator<8> navigator(array.shape(), array.strides());
        const auto s = array.shape();
        for(navigator.set(7, 0); navigator.get(7)<s[7]; navigator.increment(7))
        for(navigator.set(6, 0); navigator.get(6)<s[6]; navigator.increment(6))
        for(navigator.set(5, 0); navigator.get(5)<s[5]; navigator.increment(5))
        for(navigator.set(4, 0); navigator.get(4)<s[4]; navigator.increment(4))
        for(navigator.set(3, 0); navigator.get(3)<s[3]; navigator.increment(3))
        for(navigator.set(2, 0); navigator.get(2)<s[2]; navigator.increment(2))
        for(navigator.set(1, 0); navigator.get(1)<s[1]; navigator.increment(1))
        for(navigator.set(0, 0); navigator.get(0)<s[0]; navigator.increment(0))
        {
            f(data[navigator.offset()]);
        }
    }
};

template<>
struct ForEachImpl<9, uint8_t(Order::F_ORDER)>{
    template<class A, class F>
    static void op(A && array, F && f){
        auto data = array.data();
        Navigator<9> navigator(array.shape(), array.strides());
        const auto s = array.shape();
        for(navigator.set(8, 0); navigator.get(8)<s[8]; navigator.increment(8))
        for(navigator.set(7, 0); navigator.get(7)<s[7]; navigator.increment(7))
        for(navigator.set(6, 0); navigator.get(6)<s[6]; navigator.increment(6))
        for(navigator.set(5, 0); navigator.get(5)<s[5]; navigator.increment(5))
        for(navigator.set(4, 0); navigator.get(4)<s[4]; navigator.increment(4))
        for(navigator.set(3, 0); navigator.get(3)<s[3]; navigator.increment(3))
        for(navigator.set(2, 0); navigator.get(2)<s[2]; navigator.increment(2))
        for(navigator.set(1, 0); navigator.get(1)<s[1]; navigator.increment(1))
        for(navigator.set(0, 0); navigator.get(0)<s[0]; navigator.increment(0))
        {
            f(data[navigator.offset()]);
        }
    }
};

template<>
struct ForEachImpl<10, uint8_t(Order::F_ORDER)>{
    template<class A, class F>
    static void op(A && array, F && f){
        auto data = array.data();
        Navigator<10> navigator(array.shape(), array.strides());
        const auto s = array.shape();
        for(navigator.set(9, 0); navigator.get(9)<s[9]; navigator.increment(9))
        for(navigator.set(8, 0); navigator.get(8)<s[8]; navigator.increment(8))
        for(navigator.set(7, 0); navigator.get(7)<s[7]; navigator.increment(7))
        for(navigator.set(6, 0); navigator.get(6)<s[6]; navigator.increment(6))
        for(navigator.set(5, 0); navigator.get(5)<s[5]; navigator.increment(5))
        for(navigator.set(4, 0); navigator.get(4)<s[4]; navigator.increment(4))
        for(navigator.set(3, 0); navigator.get(3)<s[3]; navigator.increment(3))
        for(navigator.set(2, 0); navigator.get(2)<s[2]; navigator.increment(2))
        for(navigator.set(1, 0); navigator.get(1)<s[1]; navigator.increment(1))
        for(navigator.set(0, 0); navigator.get(0)<s[0]; navigator.increment(0))
        {
            f(data[navigator.offset()]);
        }
    }
};



}
}