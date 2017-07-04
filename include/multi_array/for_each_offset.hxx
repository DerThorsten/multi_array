#pragma once

#include "runtime_check.hxx"
#include "navigator.hxx"
#include "order.hxx"

namespace multi_array{

///\cond
namespace detail_for_each_offset{

    template<std::size_t DIM, uint8_t ORDER >
    struct ForEachOffsetImpl;

    // c-order fallback
    template<std::size_t DIM>
    struct ForEachOffsetImpl<DIM, uint8_t(Order::C_ORDER)>{
        const static std::size_t MAX_DIM = DIM - 1;
        template<class F>
        static void op(const Shape<DIM> & shape, const Strides<DIM> & strides, F && f){

            
            Navigator<DIM> navigator(shape, strides);
            const auto & s = shape;

            const auto & coordinate = navigator.coordinate();
            
            for(;;){

                f(navigator.offset());

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
        template<class F>
        static void op(
            const Shape<DIM> & shape, 
            const Strides<DIM> & stridesA, 
            const Strides<DIM> & stridesB, 
            F && f
        ){
            
            DoubleNavigator<DIM> navigator(shape, stridesA, stridesB);
            const auto & s = shape;

            const auto & coordinate = navigator.coordinate();
            
            for(;;){

                f(navigator.offsetA(), navigator.offsetB());

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
    struct ForEachOffsetImpl<DIM, uint8_t(Order::F_ORDER)>{
        const static std::size_t MAX_DIM = DIM - 1;
        template<class F>
        static void op(const Shape<DIM> & shape, const Strides<DIM> & strides, F && f){

            
            Navigator<DIM> navigator(shape, strides);
            const auto & s = shape;

            const auto & coordinate = navigator.coordinate();
            
            for(;;){

                f(navigator.offset());

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
        template<class F>
        static void op(
            const Shape<DIM> & shape, 
            const Strides<DIM> & stridesA, 
            const Strides<DIM> & stridesB, 
            F && f
        ){

            
            DoubleNavigator<DIM> navigator(shape, stridesA, stridesB);
            const auto & s = shape;

            const auto & coordinate = navigator.coordinate();
            
            for(;;){

                f(navigator.offsetA(), navigator.offsetB());

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
    struct ForEachOffsetImpl<0, uint8_t(Order::C_ORDER)>{
        template<class F>
        static void op(const Shape<0> & shape, const Strides<0> & strides, F && f){
            f(0);
        }

        template<class F>
        static void op(
            const Shape<0> & shape,
            const Strides<0> & stridesA,
            const Strides<0> & stridesB,
            F && f
        ){
            f(0,0);
        }
    };


    template<>
    struct ForEachOffsetImpl<1, uint8_t(Order::C_ORDER) >{
        template<class F>
        static void op(const Shape<1> & shape, const Strides<1> & strides, F && f){
            
            Navigator<1> navigator(shape, strides);
            const auto & s = shape;
            for(navigator.set(0, 0); navigator.get(0)<s[0]; navigator.increment(0)){
                f(navigator.offset());
            }
        }
        template<class F>
        static void op(const Shape<1> & shape, const Strides<1> & stridesA,const Strides<1> & stridesB, F && f){
            
            DoubleNavigator<1> navigator(shape, stridesA, stridesB);
            const auto & s = shape;
            for(navigator.set(0, 0); navigator.get(0)<s[0]; navigator.increment(0)){
                f(navigator.offsetA(),navigator.offsetB());
            }
        }
    };

    template<>
    struct ForEachOffsetImpl<2, uint8_t(Order::C_ORDER) >{
        template<class F>
        static void op(const Shape<2> & shape, const Strides<2> & strides, F && f){
            
            Navigator<2> navigator(shape, strides);
            const auto & s = shape;
            for(navigator.set(0, 0); navigator.get(0)<s[0]; navigator.increment(0))
            for(navigator.set(1, 0); navigator.get(1)<s[1]; navigator.increment(1)){
                f(navigator.offset());
            }
        }
        template<class F>
        static void op(const Shape<2> & shape, const Strides<2> & stridesA, const Strides<2> & stridesB, F && f){
            
            DoubleNavigator<2> navigator(shape, stridesA, stridesB);
            const auto & s = shape;
            for(navigator.set(0, 0); navigator.get(0)<s[0]; navigator.increment(0))
            for(navigator.set(1, 0); navigator.get(1)<s[1]; navigator.increment(1)){
                f(navigator.offsetA(),navigator.offsetB());
            }
        }
    };

    template<>
    struct ForEachOffsetImpl<3, uint8_t(Order::C_ORDER) >{
        template<class F>
        static void op(const Shape<3> & shape, const Strides<3> & strides, F && f){
            
            Navigator<3> navigator(shape, strides);
            const auto & s = shape;
            for(navigator.set(0, 0); navigator.get(0)<s[0]; navigator.increment(0))
            for(navigator.set(1, 0); navigator.get(1)<s[1]; navigator.increment(1))
            for(navigator.set(2, 0); navigator.get(2)<s[2]; navigator.increment(2)){
                f(navigator.offset());
            }
        }
        template<class F>
        static void op(const Shape<3> & shape, const Strides<3> & stridesA, const Strides<3> & stridesB, F && f){
            
            DoubleNavigator<3> navigator(shape, stridesA, stridesB);
            const auto & s = shape;
            for(navigator.set(0, 0); navigator.get(0)<s[0]; navigator.increment(0))
            for(navigator.set(1, 0); navigator.get(1)<s[1]; navigator.increment(1))
            for(navigator.set(2, 0); navigator.get(2)<s[2]; navigator.increment(2))
            {
                f(navigator.offsetA(),navigator.offsetB());
            }
        }

    };

    template<>
    struct ForEachOffsetImpl<4, uint8_t(Order::C_ORDER) >{
        template<class F>
        static void op(const Shape<4> & shape, const Strides<4> & strides, F && f){
            
            Navigator<4> navigator(shape, strides);
            const auto & s = shape;
            for(navigator.set(0, 0); navigator.get(0)<s[0]; navigator.increment(0))
            for(navigator.set(1, 0); navigator.get(1)<s[1]; navigator.increment(1))
            for(navigator.set(2, 0); navigator.get(2)<s[2]; navigator.increment(2))
            for(navigator.set(3, 0); navigator.get(3)<s[3]; navigator.increment(3)){
                f(navigator.offset());
            }
        }
        template<class F>
        static void op(const Shape<4> & shape, const Strides<4> & stridesA, const Strides<4> & stridesB, F && f){
            
            DoubleNavigator<4> navigator(shape, stridesA, stridesB);
            const auto & s = shape;
            for(navigator.set(0, 0); navigator.get(0)<s[0]; navigator.increment(0))
            for(navigator.set(1, 0); navigator.get(1)<s[1]; navigator.increment(1))
            for(navigator.set(2, 0); navigator.get(2)<s[2]; navigator.increment(2))
            for(navigator.set(3, 0); navigator.get(3)<s[2]; navigator.increment(3))
            {
                f(navigator.offsetA(),navigator.offsetB());
            }
        }
    };

    template<>
    struct ForEachOffsetImpl<5, uint8_t(Order::C_ORDER) >{
        template<class F>
        static void op(const Shape<5> & shape, const Strides<5> & strides, F && f){
            
            Navigator<5> navigator(shape, strides);
            const auto & s = shape;
            for(navigator.set(0, 0); navigator.get(0)<s[0]; navigator.increment(0))
            for(navigator.set(1, 0); navigator.get(1)<s[1]; navigator.increment(1))
            for(navigator.set(2, 0); navigator.get(2)<s[2]; navigator.increment(2))
            for(navigator.set(3, 0); navigator.get(3)<s[3]; navigator.increment(3))
            for(navigator.set(4, 0); navigator.get(4)<s[4]; navigator.increment(4))
            {
                f(navigator.offset());
            }
        }
        template<class F>
        static void op(const Shape<5> & shape, const Strides<5> & stridesA, const Strides<5> & stridesB, F && f){
            
            DoubleNavigator<5> navigator(shape, stridesA, stridesB);
            const auto & s = shape;
            for(navigator.set(0, 0); navigator.get(0)<s[0]; navigator.increment(0))
            for(navigator.set(1, 0); navigator.get(1)<s[1]; navigator.increment(1))
            for(navigator.set(2, 0); navigator.get(2)<s[2]; navigator.increment(2))
            for(navigator.set(3, 0); navigator.get(3)<s[2]; navigator.increment(3))
            for(navigator.set(4, 0); navigator.get(4)<s[2]; navigator.increment(4))
            {
                f(navigator.offsetA(),navigator.offsetB());
            }
        }
    };

    template<>
    struct ForEachOffsetImpl<6, uint8_t(Order::C_ORDER) >{
        template<class F>
        static void op(const Shape<6> & shape, const Strides<6> & strides, F && f){
            
            Navigator<6> navigator(shape, strides);
            const auto & s = shape;
            for(navigator.set(0, 0); navigator.get(0)<s[0]; navigator.increment(0))
            for(navigator.set(1, 0); navigator.get(1)<s[1]; navigator.increment(1))
            for(navigator.set(2, 0); navigator.get(2)<s[2]; navigator.increment(2))
            for(navigator.set(3, 0); navigator.get(3)<s[3]; navigator.increment(3))
            for(navigator.set(4, 0); navigator.get(4)<s[4]; navigator.increment(4))
            for(navigator.set(5, 0); navigator.get(5)<s[5]; navigator.increment(5)){
                f(navigator.offset());
            }
        }
        template<class F>
        static void op(const Shape<6> & shape, const Strides<6> & stridesA, const Strides<6> & stridesB, F && f){
            
            DoubleNavigator<6> navigator(shape, stridesA, stridesB);
            const auto & s = shape;
            for(navigator.set(0, 0); navigator.get(0)<s[0]; navigator.increment(0))
            for(navigator.set(1, 0); navigator.get(1)<s[1]; navigator.increment(1))
            for(navigator.set(2, 0); navigator.get(2)<s[2]; navigator.increment(2))
            for(navigator.set(3, 0); navigator.get(3)<s[2]; navigator.increment(3))
            for(navigator.set(4, 0); navigator.get(4)<s[2]; navigator.increment(4))
            for(navigator.set(5, 0); navigator.get(5)<s[2]; navigator.increment(5))
            {
                f(navigator.offsetA(),navigator.offsetB());
            }
        }
    };

    template<>
    struct ForEachOffsetImpl<7, uint8_t(Order::C_ORDER) >{
        template<class F>
        static void op(const Shape<7> & shape, const Strides<7> & strides, F && f){
            
            Navigator<7> navigator(shape, strides);
            const auto & s = shape;
            for(navigator.set(0, 0); navigator.get(0)<s[0]; navigator.increment(0))
            for(navigator.set(1, 0); navigator.get(1)<s[1]; navigator.increment(1))
            for(navigator.set(2, 0); navigator.get(2)<s[2]; navigator.increment(2))
            for(navigator.set(3, 0); navigator.get(3)<s[3]; navigator.increment(3))
            for(navigator.set(4, 0); navigator.get(4)<s[4]; navigator.increment(4))
            for(navigator.set(5, 0); navigator.get(5)<s[5]; navigator.increment(5))
            for(navigator.set(6, 0); navigator.get(6)<s[6]; navigator.increment(6)){
                f(navigator.offset());
            }
        }
        template<class F>
        static void op(const Shape<7> & shape, const Strides<7> & stridesA, const Strides<7> & stridesB, F && f){
            
            DoubleNavigator<7> navigator(shape, stridesA, stridesB);
            const auto & s = shape;
            for(navigator.set(0, 0); navigator.get(0)<s[0]; navigator.increment(0))
            for(navigator.set(1, 0); navigator.get(1)<s[1]; navigator.increment(1))
            for(navigator.set(2, 0); navigator.get(2)<s[2]; navigator.increment(2))
            for(navigator.set(3, 0); navigator.get(3)<s[2]; navigator.increment(3))
            for(navigator.set(4, 0); navigator.get(4)<s[2]; navigator.increment(4))
            for(navigator.set(5, 0); navigator.get(5)<s[2]; navigator.increment(5))
            for(navigator.set(6, 0); navigator.get(6)<s[2]; navigator.increment(6))
            {
                f(navigator.offsetA(),navigator.offsetB());
            }
        }
    };

    template<>
    struct ForEachOffsetImpl<8, uint8_t(Order::C_ORDER) >{
        template<class F>
        static void op(const Shape<8> & shape, const Strides<8> & strides, F && f){
            
            Navigator<8> navigator(shape, strides);
            const auto & s = shape;
            for(navigator.set(0, 0); navigator.get(0)<s[0]; navigator.increment(0))
            for(navigator.set(1, 0); navigator.get(1)<s[1]; navigator.increment(1))
            for(navigator.set(2, 0); navigator.get(2)<s[2]; navigator.increment(2))
            for(navigator.set(3, 0); navigator.get(3)<s[3]; navigator.increment(3))
            for(navigator.set(4, 0); navigator.get(4)<s[4]; navigator.increment(4))
            for(navigator.set(5, 0); navigator.get(5)<s[5]; navigator.increment(5))
            for(navigator.set(6, 0); navigator.get(6)<s[6]; navigator.increment(6))
            for(navigator.set(7, 0); navigator.get(7)<s[7]; navigator.increment(7)){
                f(navigator.offset());
            }
        }
        template<class F>
        static void op(const Shape<8> & shape, const Strides<8> & stridesA, const Strides<8> & stridesB, F && f){
            
            DoubleNavigator<8> navigator(shape, stridesA, stridesB);
            const auto & s = shape;
            for(navigator.set(0, 0); navigator.get(0)<s[0]; navigator.increment(0))
            for(navigator.set(1, 0); navigator.get(1)<s[1]; navigator.increment(1))
            for(navigator.set(2, 0); navigator.get(2)<s[2]; navigator.increment(2))
            for(navigator.set(3, 0); navigator.get(3)<s[2]; navigator.increment(3))
            for(navigator.set(4, 0); navigator.get(4)<s[2]; navigator.increment(4))
            for(navigator.set(5, 0); navigator.get(5)<s[2]; navigator.increment(5))
            for(navigator.set(6, 0); navigator.get(6)<s[2]; navigator.increment(6))
            for(navigator.set(7, 0); navigator.get(7)<s[2]; navigator.increment(7))
            {
                f(navigator.offsetA(),navigator.offsetB());
            }
        }
    };

    template<>
    struct ForEachOffsetImpl<9, uint8_t(Order::C_ORDER) >{
        template<class F>
        static void op(const Shape<9> & shape, const Strides<9> & strides, F && f){
            
            Navigator<9> navigator(shape, strides);
            const auto & s = shape;
            for(navigator.set(0, 0); navigator.get(0)<s[0]; navigator.increment(0))
            for(navigator.set(1, 0); navigator.get(1)<s[1]; navigator.increment(1))
            for(navigator.set(2, 0); navigator.get(2)<s[2]; navigator.increment(2))
            for(navigator.set(3, 0); navigator.get(3)<s[3]; navigator.increment(3))
            for(navigator.set(4, 0); navigator.get(4)<s[4]; navigator.increment(4))
            for(navigator.set(5, 0); navigator.get(5)<s[5]; navigator.increment(5))
            for(navigator.set(6, 0); navigator.get(6)<s[6]; navigator.increment(6))
            for(navigator.set(7, 0); navigator.get(7)<s[7]; navigator.increment(7))
            for(navigator.set(8, 0); navigator.get(8)<s[8]; navigator.increment(8)){
                f(navigator.offset());
            }
        }
        template<class F>
        static void op(const Shape<9> & shape, const Strides<9> & stridesA, const Strides<9> & stridesB, F && f){
            
            DoubleNavigator<9> navigator(shape, stridesA, stridesB);
            const auto & s = shape;
            for(navigator.set(0, 0); navigator.get(0)<s[0]; navigator.increment(0))
            for(navigator.set(1, 0); navigator.get(1)<s[1]; navigator.increment(1))
            for(navigator.set(2, 0); navigator.get(2)<s[2]; navigator.increment(2))
            for(navigator.set(3, 0); navigator.get(3)<s[2]; navigator.increment(3))
            for(navigator.set(4, 0); navigator.get(4)<s[2]; navigator.increment(4))
            for(navigator.set(5, 0); navigator.get(5)<s[2]; navigator.increment(5))
            for(navigator.set(6, 0); navigator.get(6)<s[2]; navigator.increment(6))
            for(navigator.set(7, 0); navigator.get(7)<s[2]; navigator.increment(7))
            for(navigator.set(8, 0); navigator.get(8)<s[2]; navigator.increment(8))
            {
                f(navigator.offsetA(),navigator.offsetB());
            }
        }
    };

    template<>
    struct ForEachOffsetImpl<10, uint8_t(Order::C_ORDER) >{
        template<class F>
        static void op(const Shape<10> & shape, const Strides<10> & strides, F && f){
            
            Navigator<10> navigator(shape, strides);
            const auto & s  = shape;
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
                f(navigator.offset());
            }
        }
        template<class F>
        static void op(const Shape<10> & shape, const Strides<10> & stridesA, const Strides<10> & stridesB, F && f){
            
            DoubleNavigator<10> navigator(shape, stridesA, stridesB);
            const auto & s = shape;
            for(navigator.set(0, 0); navigator.get(0)<s[0]; navigator.increment(0))
            for(navigator.set(1, 0); navigator.get(1)<s[1]; navigator.increment(1))
            for(navigator.set(2, 0); navigator.get(2)<s[2]; navigator.increment(2))
            for(navigator.set(3, 0); navigator.get(3)<s[2]; navigator.increment(3))
            for(navigator.set(4, 0); navigator.get(4)<s[2]; navigator.increment(4))
            for(navigator.set(5, 0); navigator.get(5)<s[2]; navigator.increment(5))
            for(navigator.set(6, 0); navigator.get(6)<s[2]; navigator.increment(6))
            for(navigator.set(7, 0); navigator.get(7)<s[2]; navigator.increment(7))
            for(navigator.set(8, 0); navigator.get(8)<s[2]; navigator.increment(8))
            for(navigator.set(9, 0); navigator.get(9)<s[2]; navigator.increment(9))
            {
                f(navigator.offsetA(),navigator.offsetB());
            }
        }

    };

    template<>
    struct ForEachOffsetImpl<0, uint8_t(Order::F_ORDER)>{
        template<class F>
        static void op(const Shape<0> & shape, const Strides<0> & strides, F && f){
            f(0);
        }

        template<class F>
        static void op(
            const Shape<0> & shape, 
            const Strides<0> & stridesA,
            const Strides<0> & stridesB, 
            F && f
        ){
            f(0,0);
        }
    };

    template<>
    struct ForEachOffsetImpl<1, uint8_t(Order::F_ORDER)>{
        template<class F>
        static void op(const Shape<1> & shape, const Strides<1> & strides, F && f){
            
            Navigator<1> navigator(shape, strides);
            const auto & s = shape;
            for(navigator.set(0, 0); navigator.get(0)<s[0]; navigator.increment(0))
            {
                f(navigator.offset());
            }
        }
        template<class F>
        static void op(const Shape<1> & shape, const Strides<1> & stridesA,const Strides<1> & stridesB, F && f){
            
            DoubleNavigator<1> navigator(shape, stridesA, stridesB);
            const auto & s = shape;
            for(navigator.set(0, 0); navigator.get(0)<s[0]; navigator.increment(0))
            {
                f(navigator.offsetA(), navigator.offsetB());
            }
        }
    };

    template<>
    struct ForEachOffsetImpl<2, uint8_t(Order::F_ORDER)>{
        template<class F>
        static void op(const Shape<2> & shape, const Strides<2> & strides, F && f){
            
            Navigator<2> navigator(shape, strides);
            const auto & s = shape;
            for(navigator.set(1, 0); navigator.get(1)<s[1]; navigator.increment(1))
            for(navigator.set(0, 0); navigator.get(0)<s[0]; navigator.increment(0))
            {
                f(navigator.offset());
            }
        }

        template<class F>
        static void op(const Shape<2> & shape, const Strides<2> & stridesA,const Strides<2> & stridesB, F && f){
            
            DoubleNavigator<2> navigator(shape, stridesA, stridesB);
            const auto & s = shape;
            for(navigator.set(1, 0); navigator.get(1)<s[1]; navigator.increment(1))
            for(navigator.set(0, 0); navigator.get(0)<s[0]; navigator.increment(0))
            {
                f(navigator.offsetA(), navigator.offsetB());
            }
        }
    };

    template<>
    struct ForEachOffsetImpl<3, uint8_t(Order::F_ORDER)>{
        template<class F>
        static void op(const Shape<3> & shape, const Strides<3> & strides, F && f){
            
            Navigator<3> navigator(shape, strides);
            const auto & s = shape;
            for(navigator.set(2, 0); navigator.get(2)<s[2]; navigator.increment(2))
            for(navigator.set(1, 0); navigator.get(1)<s[1]; navigator.increment(1))
            for(navigator.set(0, 0); navigator.get(0)<s[0]; navigator.increment(0))
            {
                f(navigator.offset());
            }
        }

        template<class F>
        static void op(const Shape<3> & shape, const Strides<3> & stridesA,const Strides<3> & stridesB, F && f){
            
            DoubleNavigator<3> navigator(shape, stridesA, stridesB);
            const auto & s = shape;
            for(navigator.set(2, 0); navigator.get(2)<s[2]; navigator.increment(2))
            for(navigator.set(1, 0); navigator.get(1)<s[1]; navigator.increment(1))
            for(navigator.set(0, 0); navigator.get(0)<s[0]; navigator.increment(0))
            {
                f(navigator.offsetA(), navigator.offsetB());
            }
        }
    };

    template<>
    struct ForEachOffsetImpl<4, uint8_t(Order::F_ORDER)>{
        template<class F>
        static void op(const Shape<4> & shape, const Strides<4> & strides, F && f){
            
            Navigator<4> navigator(shape, strides);
            const auto & s = shape;
            for(navigator.set(3, 0); navigator.get(3)<s[3]; navigator.increment(3))
            for(navigator.set(2, 0); navigator.get(2)<s[2]; navigator.increment(2))
            for(navigator.set(1, 0); navigator.get(1)<s[1]; navigator.increment(1))
            for(navigator.set(0, 0); navigator.get(0)<s[0]; navigator.increment(0))
            {
                f(navigator.offset());
            }
        }

        template<class F>
        static void op(const Shape<4> & shape, const Strides<4> & stridesA,const Strides<4> & stridesB, F && f){
            
            DoubleNavigator<4> navigator(shape, stridesA, stridesB);
            const auto & s = shape;
            for(navigator.set(3, 0); navigator.get(3)<s[3]; navigator.increment(3))
            for(navigator.set(2, 0); navigator.get(2)<s[2]; navigator.increment(2))
            for(navigator.set(1, 0); navigator.get(1)<s[1]; navigator.increment(1))
            for(navigator.set(0, 0); navigator.get(0)<s[0]; navigator.increment(0))
            {
                f(navigator.offsetA(), navigator.offsetB());
            }
        }
    };

    template<>
    struct ForEachOffsetImpl<5, uint8_t(Order::F_ORDER)>{
        template<class F>
        static void op(const Shape<5> & shape, const Strides<5> & strides, F && f){
            
            Navigator<5> navigator(shape, strides);
            const auto & s = shape;
            for(navigator.set(4, 0); navigator.get(4)<s[4]; navigator.increment(4))
            for(navigator.set(3, 0); navigator.get(3)<s[3]; navigator.increment(3))
            for(navigator.set(2, 0); navigator.get(2)<s[2]; navigator.increment(2))
            for(navigator.set(1, 0); navigator.get(1)<s[1]; navigator.increment(1))
            for(navigator.set(0, 0); navigator.get(0)<s[0]; navigator.increment(0))
            {
                f(navigator.offset());
            }
        }

        template<class F>
        static void op(const Shape<5> & shape, const Strides<5> & stridesA,const Strides<5> & stridesB, F && f){
            
            DoubleNavigator<5> navigator(shape, stridesA, stridesB);
            const auto & s = shape;
            for(navigator.set(4, 0); navigator.get(4)<s[4]; navigator.increment(4))
            for(navigator.set(3, 0); navigator.get(3)<s[3]; navigator.increment(3))
            for(navigator.set(2, 0); navigator.get(2)<s[2]; navigator.increment(2))
            for(navigator.set(1, 0); navigator.get(1)<s[1]; navigator.increment(1))
            for(navigator.set(0, 0); navigator.get(0)<s[0]; navigator.increment(0))
            {
                f(navigator.offsetA(), navigator.offsetB());
            }
        }
    };

    template<>
    struct ForEachOffsetImpl<6, uint8_t(Order::F_ORDER)>{
        template<class F>
        static void op(const Shape<6> & shape, const Strides<6> & strides, F && f){
            
            Navigator<6> navigator(shape, strides);
            const auto & s = shape;
            for(navigator.set(5, 0); navigator.get(5)<s[5]; navigator.increment(5))
            for(navigator.set(4, 0); navigator.get(4)<s[4]; navigator.increment(4))
            for(navigator.set(3, 0); navigator.get(3)<s[3]; navigator.increment(3))
            for(navigator.set(2, 0); navigator.get(2)<s[2]; navigator.increment(2))
            for(navigator.set(1, 0); navigator.get(1)<s[1]; navigator.increment(1))
            for(navigator.set(0, 0); navigator.get(0)<s[0]; navigator.increment(0))
            {
                f(navigator.offset());
            }
        }

        template<class F>
        static void op(const Shape<6> & shape, const Strides<6> & stridesA,const Strides<6> & stridesB, F && f){
            
            DoubleNavigator<6> navigator(shape, stridesA, stridesB);
            const auto & s = shape;
            for(navigator.set(5, 0); navigator.get(5)<s[5]; navigator.increment(5))
            for(navigator.set(4, 0); navigator.get(4)<s[4]; navigator.increment(4))
            for(navigator.set(3, 0); navigator.get(3)<s[3]; navigator.increment(3))
            for(navigator.set(2, 0); navigator.get(2)<s[2]; navigator.increment(2))
            for(navigator.set(1, 0); navigator.get(1)<s[1]; navigator.increment(1))
            for(navigator.set(0, 0); navigator.get(0)<s[0]; navigator.increment(0))
            {
                f(navigator.offsetA(), navigator.offsetB());
            }
        }
    };

    template<>
    struct ForEachOffsetImpl<7, uint8_t(Order::F_ORDER)>{
        template<class F>
        static void op(const Shape<7> & shape, const Strides<7> & strides, F && f){
            
            Navigator<7> navigator(shape, strides);
            const auto & s = shape;
            for(navigator.set(6, 0); navigator.get(6)<s[6]; navigator.increment(6))
            for(navigator.set(5, 0); navigator.get(5)<s[5]; navigator.increment(5))
            for(navigator.set(4, 0); navigator.get(4)<s[4]; navigator.increment(4))
            for(navigator.set(3, 0); navigator.get(3)<s[3]; navigator.increment(3))
            for(navigator.set(2, 0); navigator.get(2)<s[2]; navigator.increment(2))
            for(navigator.set(1, 0); navigator.get(1)<s[1]; navigator.increment(1))
            for(navigator.set(0, 0); navigator.get(0)<s[0]; navigator.increment(0))
            {
                f(navigator.offset());
            }
        }

        template<class F>
        static void op(const Shape<7> & shape, const Strides<7> & stridesA,const Strides<7> & stridesB, F && f){
            
            DoubleNavigator<7> navigator(shape, stridesA, stridesB);
            const auto & s = shape;
            for(navigator.set(6, 0); navigator.get(6)<s[6]; navigator.increment(6))
            for(navigator.set(5, 0); navigator.get(5)<s[5]; navigator.increment(5))
            for(navigator.set(4, 0); navigator.get(4)<s[4]; navigator.increment(4))
            for(navigator.set(3, 0); navigator.get(3)<s[3]; navigator.increment(3))
            for(navigator.set(2, 0); navigator.get(2)<s[2]; navigator.increment(2))
            for(navigator.set(1, 0); navigator.get(1)<s[1]; navigator.increment(1))
            for(navigator.set(0, 0); navigator.get(0)<s[0]; navigator.increment(0))
            {
                f(navigator.offsetA(), navigator.offsetB());
            }
        }
    };

    template<>
    struct ForEachOffsetImpl<8, uint8_t(Order::F_ORDER)>{
        template<class F>
        static void op(const Shape<8> & shape, const Strides<8> & strides, F && f){
            
            Navigator<8> navigator(shape, strides);
            const auto & s = shape;
            for(navigator.set(7, 0); navigator.get(7)<s[7]; navigator.increment(7))
            for(navigator.set(6, 0); navigator.get(6)<s[6]; navigator.increment(6))
            for(navigator.set(5, 0); navigator.get(5)<s[5]; navigator.increment(5))
            for(navigator.set(4, 0); navigator.get(4)<s[4]; navigator.increment(4))
            for(navigator.set(3, 0); navigator.get(3)<s[3]; navigator.increment(3))
            for(navigator.set(2, 0); navigator.get(2)<s[2]; navigator.increment(2))
            for(navigator.set(1, 0); navigator.get(1)<s[1]; navigator.increment(1))
            for(navigator.set(0, 0); navigator.get(0)<s[0]; navigator.increment(0))
            {
                f(navigator.offset());
            }
        }

        template<class F>
        static void op(const Shape<8> & shape, const Strides<8> & stridesA,const Strides<8> & stridesB, F && f){
            
            DoubleNavigator<8> navigator(shape, stridesA, stridesB);
            const auto & s = shape;
            for(navigator.set(0, 0); navigator.get(0)<s[0]; navigator.increment(0))
            for(navigator.set(1, 0); navigator.get(1)<s[1]; navigator.increment(1))
            for(navigator.set(2, 0); navigator.get(2)<s[1]; navigator.increment(2))
            for(navigator.set(3, 0); navigator.get(3)<s[1]; navigator.increment(3))
            for(navigator.set(4, 0); navigator.get(4)<s[1]; navigator.increment(4))
            for(navigator.set(5, 0); navigator.get(5)<s[1]; navigator.increment(5))
            for(navigator.set(6, 0); navigator.get(6)<s[1]; navigator.increment(6))
            for(navigator.set(7, 0); navigator.get(7)<s[1]; navigator.increment(7))
            {
                f(navigator.offsetA(), navigator.offsetB());
            }
        }
    };

    template<>
    struct ForEachOffsetImpl<9, uint8_t(Order::F_ORDER)>{
        template<class F>
        static void op(const Shape<9> & shape, const Strides<9> & strides, F && f){
            
            Navigator<9> navigator(shape, strides);
            const auto & s = shape;
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
                f(navigator.offset());
            }
        }

        template<class F>
        static void op(const Shape<9> & shape, const Strides<9> & stridesA,const Strides<9> & stridesB, F && f){
            
            DoubleNavigator<9> navigator(shape, stridesA, stridesB);
            const auto & s = shape;
            for(navigator.set(0, 0); navigator.get(0)<s[0]; navigator.increment(0))
            for(navigator.set(1, 0); navigator.get(1)<s[1]; navigator.increment(1))
            for(navigator.set(2, 0); navigator.get(2)<s[1]; navigator.increment(2))
            for(navigator.set(3, 0); navigator.get(3)<s[1]; navigator.increment(3))
            for(navigator.set(4, 0); navigator.get(4)<s[1]; navigator.increment(4))
            for(navigator.set(5, 0); navigator.get(5)<s[1]; navigator.increment(5))
            for(navigator.set(6, 0); navigator.get(6)<s[1]; navigator.increment(6))
            for(navigator.set(7, 0); navigator.get(7)<s[1]; navigator.increment(7))
            for(navigator.set(8, 0); navigator.get(8)<s[1]; navigator.increment(8))
            {
                f(navigator.offsetA(), navigator.offsetB());
            }
        }
    };

    template<>
    struct ForEachOffsetImpl<10, uint8_t(Order::F_ORDER)>{
        template<class F>
        static void op(const Shape<10> & shape, const Strides<10> & strides, F && f){
            
            Navigator<10> navigator(shape, strides);
            const auto & s  = shape;
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
                f(navigator.offset());
            }
        }

        template<class F>
        static void op(const Shape<10> & shape, const Strides<10> & stridesA,const Strides<10> & stridesB, F && f){
            
            DoubleNavigator<10> navigator(shape, stridesA, stridesB);
            const auto & s = shape;
            for(navigator.set(0, 0); navigator.get(0)<s[0]; navigator.increment(0))
            for(navigator.set(1, 0); navigator.get(1)<s[1]; navigator.increment(1))
            for(navigator.set(2, 0); navigator.get(2)<s[1]; navigator.increment(2))
            for(navigator.set(3, 0); navigator.get(3)<s[1]; navigator.increment(3))
            for(navigator.set(4, 0); navigator.get(4)<s[1]; navigator.increment(4))
            for(navigator.set(5, 0); navigator.get(5)<s[1]; navigator.increment(5))
            for(navigator.set(6, 0); navigator.get(6)<s[1]; navigator.increment(6))
            for(navigator.set(7, 0); navigator.get(7)<s[1]; navigator.increment(7))
            for(navigator.set(8, 0); navigator.get(8)<s[1]; navigator.increment(8))
            for(navigator.set(9, 0); navigator.get(9)<s[1]; navigator.increment(9))
            {
                f(navigator.offsetA(), navigator.offsetB());
            }
        }
    };
}
///\endcond

template<size_t DIM, class F>
void forEachOffset(
    const Shape<DIM> & shape,
    const Strides<DIM> & strides,
    const Order & coordianteOrder,
    F && f
){
    if(coordianteOrder == Order::C_ORDER){
        detail_for_each_offset::ForEachOffsetImpl<DIM, uint8_t(Order::C_ORDER)>::op(
            shape, strides, std::forward<F>(f));
    }
    else if (coordianteOrder == Order::F_ORDER){
        detail_for_each_offset::ForEachOffsetImpl<DIM, uint8_t(Order::F_ORDER)>::op(
            shape, strides, std::forward<F>(f));
    }
    else{
        MULTI_ARRAY_CHECK(false, "Not yet implemented SORRY");
    }
}



template<size_t DIM, class F>
void forEachOffset(
    const Shape<DIM> & shape,
    const Strides<DIM> & strides,
    F && f
){
    detail_for_each_offset::ForEachOffsetImpl<DIM, uint8_t(Order::C_ORDER)>::op(
        shape, strides, std::forward<F>(f));
}


template<size_t DIM, class F>
void forEachOffset(
    const Shape<DIM> & shape,
    const Strides<DIM> & stridesA,
    const Strides<DIM> & stridesB,
    const Order & coordianteOrder,
    F && f
){
    if(coordianteOrder == Order::C_ORDER){
        detail_for_each_offset::ForEachOffsetImpl<DIM, uint8_t(Order::C_ORDER)>::op(
            shape, stridesA, stridesB, std::forward<F>(f));
    }
    else if (coordianteOrder == Order::F_ORDER){
        detail_for_each_offset::ForEachOffsetImpl<DIM, uint8_t(Order::F_ORDER)>::op(
            shape, stridesA, stridesB, std::forward<F>(f));
    }
    else{
        MULTI_ARRAY_CHECK(false, "Not yet implemented SORRY");
    }
}


template<size_t DIM, class F>
void forEachOffset(
    const Shape<DIM> & shape,
    const Strides<DIM> & stridesA,
    const Strides<DIM> & stridesB,
    F && f
){
    detail_for_each_offset::ForEachOffsetImpl<DIM, uint8_t(Order::C_ORDER)>::op(
        shape, stridesA,stridesB, std::forward<F>(f));
}


}