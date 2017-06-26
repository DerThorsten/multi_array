#pragma once


#include <array>

#include "multi_array/meta/variadic.hxx"
#include "multi_array/order.hxx"
namespace multi_array{




    template<std::size_t DIM>
    class Shape : public std::array<int64_t, DIM>{
    public:
        Shape(): std::array<int64_t, DIM>(){

        }

        std::size_t countNegativeEntries()const{
            std::size_t c = 0 ;
            for(std::size_t d=0; d<DIM; ++d){
                if(this->operator[](d)<0){
                    ++c;
                }
            }
            return c;
        }


        Shape<DIM> makeShape(
            const uint64_t size
        )const{
            Shape<DIM> res = *this;
            const auto nNeg = this->countNegativeEntries();
            if(nNeg > 1){
                throw std::runtime_error("Shape can only contain one negative element");
            }
            if(nNeg == 1){
                uint64_t accSize = 1;
                uint64_t whereNegative;
                for(std::size_t d=0; d<DIM; ++d){
                    if(res[d] >=0 ){
                        accSize *= res[d];
                    }
                    else{
                        whereNegative = d;
                    }
                }

                // calculate with
                // what we replace the -1
                auto estimatedShape =  size / accSize;

                // check if estimatedShape * accSize == size
                if(estimatedShape * accSize != size){
                    throw std::runtime_error("Cannot reshape: TODO ");
                }
                res[whereNegative] = estimatedShape;
            }
            return res;
        }

    };
        
    ///\cond
    namespace detail_multi_array{


        //void getShapeImpl( Shape<0> & result){
        //}  
        template<std::size_t CURRENT_INDEX, std::size_t DIM>
        void getShapeImpl(
            Shape<DIM> & result
        ){
        } 
        template<std::size_t CURRENT_INDEX, std::size_t DIM, class ARG>
        void getShapeImpl(
            Shape<DIM> & result,
            ARG && arg
        ){
            result[CURRENT_INDEX] = arg;
        } 
        template<std::size_t CURRENT_INDEX, std::size_t DIM, class ARG, class ... ARGS>
        void getShapeImpl(
            Shape<DIM> & result,
            ARG && arg,
            ARGS && ... args
        ){
            result[CURRENT_INDEX] = arg;
            getShapeImpl<CURRENT_INDEX+1, DIM, ARGS ...>(result, std::forward<ARGS>(args) ...);
        }

        
        
    }
    ///\endcond

    template<class  ... ARGS>
    Shape< meta::VariadicArgumentsSize<ARGS ... >::value >
    shape(ARGS && ... args){
        typedef meta::VariadicArgumentsSize<ARGS ... > VSizeType;
        typedef Shape< VSizeType::value > ResultType;
        ResultType result;
        detail_multi_array::getShapeImpl<0>(result, std::forward<ARGS>(args)...);
        return result;
    }



    template<std::size_t DIM>
    class Strides : public std::array<int64_t, DIM>{
    public:
        Strides(): std::array<int64_t, DIM>(){

        }
        bool operator== (const Strides & other)const{
            for(std::size_t d=0; d<DIM; ++d){
                if(this->operator[](d) != other[d])
                    return false;
            }
            return true;
        }
        bool operator != (const Strides & other)const{
            return !((*this) == other);
        }
        bool relaxedEqual(const Strides & other)const{
            for(std::size_t d=0; d<DIM; ++d){
                const auto val = this->operator[](d);
                const auto otherVal = other[d];
                if( (val != 0 && otherVal != 0) && val!=otherVal){
                    return false;
                }
            }
            return true;
        }
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
        inline void cOrderStrides(
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


        template<size_t DIM>
        inline void fOrderStrides(
            const Shape<DIM>   & shape,
            Strides<DIM> & strides
        ){
            strides.front() = 1;
            for(int d=1; d<DIM; ++d){
                strides[d] = shape[d-1] * strides[d-1];
            }

        }

        template<size_t DIM>
        Strides<DIM> fOrderStrides(
            const Shape<DIM>   & shape
            
        ){
            Strides<DIM>  strides;
            fOrderStrides(shape, strides);
            return strides;
        }

        template<size_t DIM>
        Strides<DIM> strides(
            const Shape<DIM>   & shape,
            const Order & order
        ){
            if(order == Order::C_ORDER){
                return cOrderStrides(shape);
            }
            else if(order == Order::F_ORDER){
                return fOrderStrides(shape);
            }
            else{
                throw std::runtime_error("wrong order");
            }
        }

    }
    ///\encdond


}