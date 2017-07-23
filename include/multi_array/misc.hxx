#pragma once





//#define very_inline inline  __attribute__((always_inline))
#define __forceinline __attribute__((always_inline)) inline
#define very_inline   __forceinline
//#define very_inline   inline









#include <array>
#include "multi_array/meta/variadic.hxx"
#include "multi_array/order.hxx"
#include "multi_array/meta.hxx"

namespace multi_array{

    const static int64_t HasNoShapeValue = -1;
    const static int64_t HasNoStridesValue = -1;
    enum AccWithCoordEnum{
        AccWithCoord
    };






    template<class T, std::size_t DIM>
    class TinyVector
    : public std::array<T, DIM>{
    public:
        TinyVector()
        : std::array<T, DIM>(){
        }
        TinyVector(const T & val)
        : std::array<T, DIM>(){
            std::fill(this->begin(), this->end(), val);
        }
    private:
    };


    namespace detail_multi_array{

        // TODO implement me with meta programming
        inline bool areAlmostEqual(const float x, const float y)
        {
            return std::fabs(x-y) < 1e-6f;
        }
        inline bool areAlmostEqual(const double x, const double y)
        {
            return std::fabs(x-y) < 1e-6f;
        }
        template<class T>
        inline bool areAlmostEqual(const T & x, const T & y){
            return x == y;
        }
    }




    template<class T>
    struct DataType{
        typedef T type;
    };

    template<class T>
    DataType<T> dtype(const T & val){
        return DataType<T>();
    }

    //template<class T>
    //DataType<T> dtype(){
    //    return DataType<T>();
    //}

    template<std::size_t DIM>
    class Coordinate : public TinyVector<int64_t, DIM>{
    public:
        typedef TinyVector<int64_t, DIM> BaseType;
        Coordinate(const int64_t defaultValue = 0 )
        : BaseType(defaultValue){
        }
        
    };

    template<std::size_t DIM>
    class Shape : public TinyVector<int64_t, DIM>{
    public:
        Shape(const int64_t value = 0): TinyVector<int64_t, DIM>(value){
        }
        bool operator== (const Shape & other)const{
            for(std::size_t d=0; d<DIM; ++d){
                if(this->operator[](d) != other[d])
                    return false;
            }
            return true;
        }
        bool operator != (const Shape & other)const{
            return !((*this) == other);
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

    class ShapesNotBroadcastableException: public std::runtime_error{
    public:
        template<std::size_t DIM>
        ShapesNotBroadcastableException(
            const Shape<DIM> & shapeA,
            const Shape<DIM> & shapeB
        )
        :   std::runtime_error(std::string("cannot broadcast shapes...")){
        }
        ShapesNotBroadcastableException(const std::string & msg = std::string())
        :   std::runtime_error(msg.c_str()){
        }
    };

    template<std::size_t DIM>
    inline bool canBroadcastShapes(
        const Shape<DIM> & shapeA,
        const Shape<DIM> & shapeB
    ){
        for(auto d=0; d<DIM; ++d){
            const auto sA = shapeA[d];
            const auto sB = shapeB[d];
            if(sA!=1 && sB!=1 && sA!=sB)
                return false;
        }
        return true;
    }


    template<std::size_t DIM>
    inline Shape<DIM> broadcastShapes(
        const Shape<DIM> & shapeA,
        const Shape<DIM> & shapeB
    ){
        Shape<DIM> res;

        for(auto d=0; d<DIM; ++d){
            const auto sA = shapeA[d];
            const auto sB = shapeB[d];


            // check for compatibility
            if(sA!=1 && sB!=1 && sA!=sB){
                throw ShapesNotBroadcastableException(shapeA, shapeB);
            }
            // since we checked for compatibility we can 
            // just take the max
            res[d] = std::max(sA, sB);
        }
        return res;
    }

        
    ///\cond
    namespace detail_multi_array{


        //void getShapeImpl( Shape<0> & result){
        //}  
        template<std::size_t CURRENT_INDEX, std::size_t DIM>
        inline void getShapeImpl(
            Shape<DIM> & result
        ){
        } 
        template<std::size_t CURRENT_INDEX, std::size_t DIM, class ARG>
        inline void getShapeImpl(
            Shape<DIM> & result,
            ARG && arg
        ){
            result[CURRENT_INDEX] = arg;
        } 
        template<std::size_t CURRENT_INDEX, std::size_t DIM, class ARG, class ... ARGS>
        inline void getShapeImpl(
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
    inline Shape< meta::VariadicArgumentsSize<ARGS ... >::value >
    shape(ARGS && ... args){
        typedef meta::VariadicArgumentsSize<ARGS ... > VSizeType;
        typedef Shape< VSizeType::value > ResultType;
        ResultType result;
        detail_multi_array::getShapeImpl<0>(result, std::forward<ARGS>(args)...);
        return result;
    }

    template<class  ... ARGS>
    inline Shape< meta::VariadicArgumentsSize<ARGS ... >::value >
    makeShape(ARGS && ... args){
        typedef meta::VariadicArgumentsSize<ARGS ... > VSizeType;
        typedef Shape< VSizeType::value > ResultType;
        ResultType result;
        detail_multi_array::getShapeImpl<0>(result, std::forward<ARGS>(args)...);
        return result;
    }


    template<std::size_t DIM>
    class Strides : public TinyVector<int64_t, DIM>{
    public:
        Strides(const int64_t value = 0): TinyVector<int64_t, DIM>(value){

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











      



    struct NoneTag{
    };
    constexpr NoneTag none(){
        return  NoneTag();
    }
    const static NoneTag None = NoneTag();

    struct AllTag{
    };
    constexpr AllTag all(){
        return  AllTag();
    }
    const static AllTag All = AllTag();
    
    struct EllipsisTag{
    };
    constexpr EllipsisTag ellipsis(){
        return  EllipsisTag();
    }
    const static EllipsisTag Ellipsis = EllipsisTag();

    

    template<std::size_t DIM>
    struct NewAxis
    : public meta::SizeT<DIM>{
    }; 
    constexpr NewAxis<1> newAxis(){
        return NewAxis<1>();
    }


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
        inline uint64_t shapeSize(const SHAPE & shape){
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


        template<std::size_t DIM>
        inline bool isContiguous(
            const Shape<DIM>    & shape,
            const Strides<DIM>  & strides
        ){
        
            uint64_t lastValidMemOffset = static_cast<uint64_t>((shape[0]-1) * strides[0]);
            uint64_t size = shape[0];

            for(auto d=1; d<DIM; ++d){
                const auto s = static_cast<uint64_t>(shape[d]);
                lastValidMemOffset += (s-1) * static_cast<uint64_t>(strides[d]);
                size *= s;
            }
            return (lastValidMemOffset + 1) == size +1;
        }


        template<std::size_t DIM>
        inline bool isContiguous(
            const Shape<DIM>    & shape,
            const Strides<DIM>  & _strides,
            const Order & order
        ){
            if(order == Order::ANY_ORDER){
                return isContiguous(shape, _strides);
            }
            const auto orderStrides = strides(shape, order);
            return _strides.relaxedEqual(orderStrides);
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
        inline Strides<DIM> cOrderStrides(
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
        inline Strides<DIM> fOrderStrides(
            const Shape<DIM>   & shape
            
        ){
            Strides<DIM>  strides;
            fOrderStrides(shape, strides);
            return strides;
        }

        template<size_t DIM>
        inline Strides<DIM> strides(
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


namespace std{

    template<std::size_t DIM>
    ostream& operator<<(
        ostream& stream, 
        const multi_array::Strides<DIM> &strides
    ) {
        stream<<"[";
        for(auto i=0; i<DIM; ++i){
            stream<<strides[i];
            if(i!=DIM-1){
                stream<<",";
            }
            else{
                stream<<"]";
            }
            
        }
        return stream;
    }

    template<std::size_t DIM>
    ostream& operator<<(
        ostream& stream, 
        const multi_array::Shape<DIM> &shape
    ) {
        stream<<"[";
        for(auto i=0; i<DIM; ++i){
            stream<<shape[i];
            if(i!=DIM-1){
                stream<<",";
            }
            else{
                stream<<"]";
            }
            
        }
        return stream;
    }
}