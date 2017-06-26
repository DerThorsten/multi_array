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
        int64_t offset()const{
            return offset_;
        }

        const Shape<DIM> & coordinate()const{
            return coordinate_;
        }
    private:
        const Shape<DIM> & shape_;
        const Strides<DIM> & strides_;
        Shape<DIM>  coordinate_;

        int64_t offset_;
    };  


    

    

}