#pragma once

#include <array> 

#include "multi_array/misc.hxx"

namespace multi_array{


    template<class T, std::size_t DIM, bool IS_CONST>
    class SmartMultiArray;


    template<std::size_t DIM>
    class Navigator{
    public:
        Navigator(const Shape<DIM> & shape, const Strides<DIM> & strides)
        :   shape_(shape),
            strides_(strides),
            coordinate_(),
            offset_(0){

            for(auto a=0; a<DIM; ++a){
                coordinate_[a] = 0;
            }

        }

        template<std::size_t AXIS>
        void increment(){
            ++coordinate_[AXIS];
            offset_ += strides_[AXIS];
        }

        void increment(const std::size_t axis){
            ++coordinate_[axis];
            offset_ += strides_[axis];
        }

        template<std::size_t AXIS>
        void set(const int64_t value){
            
            offset_ -= coordinate_[AXIS]*strides_[AXIS];
            coordinate_[AXIS] = value;
            offset_ += value * strides_[AXIS];
        }

       
        void set(const std::size_t axis, const int64_t value){
            
            offset_ -= coordinate_[axis]*strides_[axis];
            coordinate_[axis] = value;
            offset_ += value * strides_[axis];
        }

        template<std::size_t AXIS>
        int64_t get()const{
            return coordinate_[AXIS];
        }
        int64_t get(const std::size_t axis)const{
            return coordinate_[axis];
        }

        
        int64_t operator[](const std::size_t axis)const{
            return coordinate_[axis]; 
        }



        int64_t offset()const{
            return offset_;
        }

        const Coordinate<DIM> & coordinate()const{
            return coordinate_;
        }

        const Shape<DIM> & shape()const{
            return shape_;
        }
    private:
        const Shape<DIM> & shape_;
        const Strides<DIM> & strides_;
        Coordinate<DIM>  coordinate_;

        int64_t offset_;
    };  


        
    template<std::size_t DIM>
    class DoubleNavigator{
    public:
        DoubleNavigator(
            const Shape<DIM> & shape, 
            const Strides<DIM> & stridesA,
            const Strides<DIM> & stridesB
        )
        :   shape_(shape),
            stridesA_(stridesA),
            stridesB_(stridesB),
            coordinate_(),
            offsetA_(0),
            offsetB_(0)
        {

            for(auto a=0; a<DIM; ++a){
                coordinate_[a] = 0;
            }

        }

        template<std::size_t AXIS>
        void increment(){
            ++coordinate_[AXIS];
            offsetA_ += stridesA_[AXIS];
            offsetB_ += stridesB_[AXIS];
        }

        void increment(const std::size_t axis){
            ++coordinate_[axis];
            offsetA_ += stridesA_[axis];
            offsetB_ += stridesB_[axis];
        }

        template<std::size_t AXIS>
        void set(const int64_t value){
            
            offsetA_ -= coordinate_[AXIS]*stridesA_[AXIS];
            offsetB_ -= coordinate_[AXIS]*stridesB_[AXIS];
            coordinate_[AXIS] = value;
            offsetA_ += value * stridesA_[AXIS];
            offsetB_ += value * stridesB_[AXIS];
        }

       
        void set(const std::size_t axis, const int64_t value){
            
            offsetA_ -= coordinate_[axis]*stridesA_[axis];
            offsetB_ -= coordinate_[axis]*stridesB_[axis];
            coordinate_[axis] = value;
            offsetA_ += value * stridesA_[axis];
            offsetB_ += value * stridesB_[axis];
        }

        template<std::size_t AXIS>
        int64_t get()const{
            return coordinate_[AXIS];
        }
        int64_t get(const std::size_t axis)const{
            return coordinate_[axis];
        }
        int64_t operator[](const std::size_t axis)const{
            return coordinate_[axis]; 
        }

        int64_t offsetA()const{
            return offsetA_;
        }
        int64_t offsetB()const{
            return offsetB_;
        }

        const Coordinate<DIM> & coordinate()const{
            return coordinate_;
        }
        const Shape<DIM> & shape()const{
            return shape_;
        }
    private:

        const Shape<DIM> & shape_;
        const Strides<DIM> & stridesA_;
        const Strides<DIM> & stridesB_;
        Coordinate<DIM>  coordinate_;

        int64_t offsetA_;
        int64_t offsetB_;
    };  
    

}