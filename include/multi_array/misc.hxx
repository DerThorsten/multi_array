#pragma once


#include <array>

namespace multi_array{

    template<std::size_t DIM>
    class Shape : public std::array<int64_t, DIM>{
        std::size_t countNegativeEntries()const{
            std::size_t c = 0 ;
            for(std::size_t d=0; d<DIM; ++d){
                if(this->operator[](d)<0){
                    ++c;
                }
            }
            ++c;
            return c;
        }
    };
    




    template<std::size_t DIM>
    class Strides : public std::array<int64_t, DIM>{
    };




    struct NewAxis{};   
    struct All{};
    struct Range{
        Range(const int64_t begin, const int64_t end)
        :   begin_(begin),
            end_(end){
        }
        int64_t begin()const{
            return begin_;
        }
        int64_t end()const{
            return end_;
        }
        int64_t begin_,end_;
    };
    struct StridedRange{
        StridedRange(const int64_t begin, const int64_t end, const int64_t step)
        :   begin_(begin),
            end_(end),
            step_(step){
        }
        int64_t begin()const{
            return begin_;
        }
        int64_t end()const{
            return end_;
        }
        int64_t step()const{
            return step_;
        }
        int64_t begin_,end_, step_;
    };


    inline NewAxis newAxis(){
        return NewAxis();
    }
    inline All all(){
        return All();
    }
    inline Range range(const int64_t begin, const int64_t end){
        return Range(begin, end);
    }
    inline StridedRange range(const int64_t begin, const int64_t end, const int64_t step){
        return StridedRange(begin, end, step);
    }




    ///\cond
    namespace detail_multi_array{

        

        template<size_t I, size_t DIM, class ARG, class ... ARGS>
        struct DotImpl{
            static int64_t op(const Strides<DIM> & strides, ARG && arg, ARGS &&  ... args){
                return strides[I] * arg + DotImpl<I+1, DIM, ARGS ...>::op(
                    strides, std::forward<ARGS>(args) ... 
                );
            }   
        };
        template<size_t I, size_t DIM, class ARG>
        struct DotImpl<I, DIM, ARG>{
            static int64_t op(const Strides<DIM> & strides, ARG &&  arg){
                return strides[I] * arg;
            }  
        };
        
        template<size_t DIM, class ... ARGS>
        int64_t dot(const Strides<DIM> & strides, ARGS &&  ... args){
            return DotImpl<0,DIM, ARGS ...>::op(strides, std::forward<ARGS>(args) ... );
        }
        // not sure if this is ever needed but
        // does not hurt to be on the save side
        template<class ... ARGS>
        int64_t dot(const Strides<0> & strides, ARGS &&  ... args){
            return 0;
        }



        template<class SHAPE>
        uint64_t shapeSize(const SHAPE & shape){
            if(shape.empty()){
                return 0;
            }
            else{
                uint64_t size = 1;
                for(const auto s : shape){
                    size *= static_cast<uint64_t>(s);
                }
                return size;
            }
        }


        template<size_t DIM>
        inline uint64_t cOrderStrides(
            const Shape<DIM>   & shape,
            Strides<DIM> & strides
        ){
            strides.back() = 1;
            for(int d=int(DIM)-2; d>=0; --d){
                strides[d] = shape[d+1] * strides[d+1];
            }
        }

        template<size_t DIM>
        Strides<DIM> cOrderStrides(
            const Shape<DIM>   & shape
            
        ){
            Strides<DIM>  strides;
            cOrderStrides(shape, strides);
            return strides;
        }
    }
    ///\encdond


}