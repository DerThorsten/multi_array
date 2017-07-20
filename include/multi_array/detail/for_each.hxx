#pragma once


#include "multi_array/navigator.hxx"
#include "multi_array/misc.hxx"
#include "multi_array/meta.hxx"
#include "multi_array/order.hxx"

namespace multi_array{


namespace detail_for_each{




struct OffsetTag{};
struct CoordinateTag{};
struct OffsetAndCoordinateTag{};

struct OffsetAOffsetBTag{};
struct OffsetAOffsetBCoordinateTag{};


template<class TAG>
struct FunctorCaller;

template<>
struct FunctorCaller<OffsetTag>{
    template<class NAVIGATOR, class F>
    inline static void op(const NAVIGATOR & navigator, F && f){
        f(navigator.offset());
    }
};
template<>
struct FunctorCaller<CoordinateTag>{
    template<class NAVIGATOR, class F>
    inline static void op(const NAVIGATOR & navigator, F && f){
        f(navigator.coordinate());
    }
};
template<>
struct FunctorCaller<OffsetAndCoordinateTag>{
    template<class NAVIGATOR, class F>
    inline static void op(const NAVIGATOR & navigator, F && f){
        f(navigator.offset(), navigator.coordinate());
    }
};

// double navigator
template<>
struct FunctorCaller<OffsetAOffsetBTag>{
    template<class NAVIGATOR, class F>
    inline static void op(const NAVIGATOR & navigator, F && f){
        f(navigator.offsetA(), navigator.offsetB());
    }
};
template<>
struct FunctorCaller<OffsetAOffsetBCoordinateTag>{
    template<class NAVIGATOR, class F>
    inline static void op(const NAVIGATOR & navigator, F && f){
        f(navigator.offsetA(), navigator.offsetB(), navigator.coordinate());
    }
};



struct COrderTag{};
struct FOrderTag{};

template<class ORDER_TAG, std::size_t DIM>
struct AxisOrder;

template<std::size_t DIM>
struct AxisOrder<COrderTag, DIM>{
    inline static constexpr std::size_t op(const std::size_t i){
        return i;
    }
};
template<std::size_t DIM>
struct AxisOrder<FOrderTag, DIM>{
    inline static constexpr std::size_t op(const std::size_t i){
        return DIM-1-i;
    }
};



// fallback
template<std::size_t DIM>
struct ForEachDimImpl{
    typedef Shape<DIM>        ShapeType;
    typedef Strides<DIM>      StridesType;
    typedef Coordinate<DIM>   CoordinateType;
    typedef Navigator<DIM>    NavigatorType;
    

    template<class CALL_ARG_TAG, class ORDER_TAG, class NAVIGATOR, class F>
    inline static void op(
        NAVIGATOR & navigator,
        F && f
    ){
        ForEachDimImpl<DIM>:: template op<CALL_ARG_TAG>(
            navigator,ORDER_TAG(),std::forward<F>(f)); 
    }

    // c order
    template<class CALL_ARG_TAG, class NAVIGATOR, class F>
    inline static void op(
        NAVIGATOR & navigator,
        const COrderTag &,
        F && f
    ){
        typedef FunctorCaller<CALL_ARG_TAG> FunctorCallerType;
        const auto s = navigator.shape();
        for(;;){
            FunctorCallerType::op(navigator, std::forward<F>(f));
            for(int j=int(DIM-1); j>=0; --j) {

                if(navigator[j]+1 == s[j]) {
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
    // f-order
    template<class CALL_ARG_TAG, class NAVIGATOR, class F>
    inline static void op(
        NAVIGATOR & navigator,
        const FOrderTag &,
        F && f
    ){
        typedef FunctorCaller<CALL_ARG_TAG> FunctorCallerType;
        const auto s = navigator.shape();
        for(;;){
            FunctorCallerType::op(navigator, std::forward<F>(f));
            for(int j=0; j<DIM; ++j) {

                if(navigator[j]+1 == s[j]) {
                    if(j == int(DIM-1)) {
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
struct ForEachDimImpl<1>{
    const static std::size_t DIM = 1;
    typedef Shape<DIM>        ShapeType;
    typedef Strides<DIM>      StridesType;
    typedef Coordinate<DIM>   CoordinateType;
    typedef Navigator<DIM>    NavigatorType;
    

    template<class CALL_ARG_TAG, class ORDER_TAG, class NAVIGATOR, class F>
    inline static void op(
        NAVIGATOR & n,
        F && f
    ){
        typedef FunctorCaller<CALL_ARG_TAG> FunctorCallerType;
        const auto s = n.shape();
        typedef AxisOrder<ORDER_TAG, DIM> A;
        for(n.set(A::op(0),0); n[A::op(0)]<s[A::op(0)]; n.increment(A::op(0)))
        {
            FunctorCallerType::op(n, std::forward<F>(f));
        }
    }
};

template<>
struct ForEachDimImpl<2>{
    const static std::size_t DIM = 2;
    typedef Shape<DIM>        ShapeType;
    typedef Strides<DIM>      StridesType;
    typedef Coordinate<DIM>   CoordinateType;
    typedef Navigator<DIM>    NavigatorType;
    

    template<class CALL_ARG_TAG, class ORDER_TAG, class NAVIGATOR, class F>
    inline static void op(
        NAVIGATOR & n,
        F && f
    ){
        typedef FunctorCaller<CALL_ARG_TAG> FunctorCallerType;
        const auto s = n.shape();
        typedef AxisOrder<ORDER_TAG, DIM> A;
        for(n.set(A::op(0),0); n[A::op(0)]<s[A::op(0)]; n.increment(A::op(0)))
        for(n.set(A::op(1),0); n[A::op(1)]<s[A::op(1)]; n.increment(A::op(1)))
        {
            FunctorCallerType::op(n, std::forward<F>(f));
        }
    }
};

template<>
struct ForEachDimImpl<3>{
    const static std::size_t DIM = 3;
    typedef Shape<DIM>        ShapeType;
    typedef Strides<DIM>      StridesType;
    typedef Coordinate<DIM>   CoordinateType;
    typedef Navigator<DIM>    NavigatorType;
    

    template<class CALL_ARG_TAG, class ORDER_TAG, class NAVIGATOR, class F>
    inline static void op(
        NAVIGATOR & n,
        F && f
    ){
        typedef FunctorCaller<CALL_ARG_TAG> FunctorCallerType;
        const auto s = n.shape();
        typedef AxisOrder<ORDER_TAG, DIM> A;
        for(n.set(A::op(0),0); n[A::op(0)]<s[A::op(0)]; n.increment(A::op(0)))
        for(n.set(A::op(1),0); n[A::op(1)]<s[A::op(1)]; n.increment(A::op(1)))
        for(n.set(A::op(2),0); n[A::op(2)]<s[A::op(2)]; n.increment(A::op(2)))
        {
            FunctorCallerType::op(n, std::forward<F>(f));
        }
    }
};

template<>
struct ForEachDimImpl<4>{
    const static std::size_t DIM = 4;
    typedef Shape<DIM>        ShapeType;
    typedef Strides<DIM>      StridesType;
    typedef Coordinate<DIM>   CoordinateType;
    typedef Navigator<DIM>    NavigatorType;
    

    template<class CALL_ARG_TAG, class ORDER_TAG, class NAVIGATOR, class F>
    inline static void op(
        NAVIGATOR & n,
        F && f
    ){
        typedef FunctorCaller<CALL_ARG_TAG> FunctorCallerType;
        const auto s = n.shape();
        typedef AxisOrder<ORDER_TAG, DIM> A;
        for(n.set(A::op(0),0); n[A::op(0)]<s[A::op(0)]; n.increment(A::op(0)))
        for(n.set(A::op(1),0); n[A::op(1)]<s[A::op(1)]; n.increment(A::op(1)))
        for(n.set(A::op(2),0); n[A::op(2)]<s[A::op(2)]; n.increment(A::op(2)))
        for(n.set(A::op(3),0); n[A::op(3)]<s[A::op(3)]; n.increment(A::op(3)))
        {
            FunctorCallerType::op(n, std::forward<F>(f));
        }
    }
};


template<>
struct ForEachDimImpl<5>{
    const static std::size_t DIM = 5;
    typedef Shape<DIM>        ShapeType;
    typedef Strides<DIM>      StridesType;
    typedef Coordinate<DIM>   CoordinateType;
    typedef Navigator<DIM>    NavigatorType;
    

    template<class CALL_ARG_TAG, class ORDER_TAG, class NAVIGATOR, class F>
    inline static void op(
        NAVIGATOR & n,
        F && f
    ){
        typedef FunctorCaller<CALL_ARG_TAG> FunctorCallerType;
        const auto s = n.shape();
        typedef AxisOrder<ORDER_TAG, DIM> A;
        for(n.set(A::op(0),0); n[A::op(0)]<s[A::op(0)]; n.increment(A::op(0)))
        for(n.set(A::op(1),0); n[A::op(1)]<s[A::op(1)]; n.increment(A::op(1)))
        for(n.set(A::op(2),0); n[A::op(2)]<s[A::op(2)]; n.increment(A::op(2)))
        for(n.set(A::op(3),0); n[A::op(3)]<s[A::op(3)]; n.increment(A::op(3)))
        for(n.set(A::op(4),0); n[A::op(4)]<s[A::op(4)]; n.increment(A::op(4)))
        {
            FunctorCallerType::op(n, std::forward<F>(f));
        }
    }
};

template<>
struct ForEachDimImpl<6>{
    const static std::size_t DIM = 6;
    typedef Shape<DIM>        ShapeType;
    typedef Strides<DIM>      StridesType;
    typedef Coordinate<DIM>   CoordinateType;
    typedef Navigator<DIM>    NavigatorType;
    

    template<class CALL_ARG_TAG, class ORDER_TAG, class NAVIGATOR, class F>
    inline static void op(
        NAVIGATOR & n,
        F && f
    ){
        typedef FunctorCaller<CALL_ARG_TAG> FunctorCallerType;
        const auto s = n.shape();
        typedef AxisOrder<ORDER_TAG, DIM> A;
        for(n.set(A::op(0),0); n[A::op(0)]<s[A::op(0)]; n.increment(A::op(0)))
        for(n.set(A::op(1),0); n[A::op(1)]<s[A::op(1)]; n.increment(A::op(1)))
        for(n.set(A::op(2),0); n[A::op(2)]<s[A::op(2)]; n.increment(A::op(2)))
        for(n.set(A::op(3),0); n[A::op(3)]<s[A::op(3)]; n.increment(A::op(3)))
        for(n.set(A::op(4),0); n[A::op(4)]<s[A::op(4)]; n.increment(A::op(4)))
        for(n.set(A::op(5),0); n[A::op(5)]<s[A::op(4)]; n.increment(A::op(5)))
        {
            FunctorCallerType::op(n, std::forward<F>(f));
        }
    }
};




template<std::size_t DIM>
struct ForEachImpl{

    typedef Shape<DIM>        ShapeType;
    typedef Strides<DIM>      StridesType;

    // 1 arg
    template<class CALL_ARG_TAG, class F>
    inline static void op(
        const ShapeType & shape, 
        const StridesType & strides,
        const Order & order,
        F && f
    ){
        Navigator<DIM> navigator(shape, strides);
        if(order == Order::C_ORDER){
            ForEachDimImpl<DIM>:: template op<CALL_ARG_TAG, COrderTag> (navigator,std::forward<F>(f));
        }
        else if(order == Order::F_ORDER){
            ForEachDimImpl<DIM>:: template op<CALL_ARG_TAG, FOrderTag> (navigator,std::forward<F>(f));
        }
        else if(order == Order::ANY_ORDER){
            const auto fStrides = detail_multi_array::strides(shape, Order::F_ORDER);
            if(strides.relaxedEqual(fStrides)){
                ForEachDimImpl<DIM>:: template op<CALL_ARG_TAG, FOrderTag> (navigator,std::forward<F>(f));
            }
            else{
                ForEachDimImpl<DIM>:: template op<CALL_ARG_TAG, COrderTag> (navigator,std::forward<F>(f));
            }
        }
        else{
            MULTI_ARRAY_CHECK(false, "unrecognized order");
        }
    }

    // 2 arg
    template<class CALL_ARG_TAG, class F>
    inline static void op(
        const ShapeType & shape, 
        const StridesType & stridesA,
        const StridesType & stridesB,
        const Order & order,
        F && f
    ){
        DoubleNavigator<DIM> navigator(shape, stridesA, stridesB);
        if(order == Order::C_ORDER){
            ForEachDimImpl<DIM>:: template op<CALL_ARG_TAG, COrderTag>(navigator,std::forward<F>(f));
        }
        else if(order == Order::F_ORDER){
            ForEachDimImpl<DIM>:: template op<CALL_ARG_TAG, FOrderTag>(navigator,std::forward<F>(f));
        }
        else if(order == Order::ANY_ORDER){
            const auto fStrides = detail_multi_array::strides(shape, Order::F_ORDER);
            if( stridesA.relaxedEqual(fStrides) && stridesB.relaxedEqual(fStrides)){
                ForEachDimImpl<DIM>:: template op<CALL_ARG_TAG, FOrderTag>(navigator,std::forward<F>(f));
            }
            else{
                ForEachDimImpl<DIM>:: template op<CALL_ARG_TAG, COrderTag>(navigator,std::forward<F>(f));
            }
        }
        else{
            MULTI_ARRAY_CHECK(false, "unrecognized order");
        }
    }
};









// 1 arg optimized
template<
    class T,
    std::size_t DIM, 
    bool IS_CONST,
    class F
>
very_inline static void forEachOffset(
    const SmartMultiArray<T,DIM, IS_CONST> & array, // universal ref, can be const ref
    const Order & order,
    F && f
){
    const auto & shape = array.shape();
    const auto & strides = array.strides();

    // if we essentially do not rely on the order
    if(order == Order::ANY_ORDER){
        // optimal case
        if(array.contiguous()){
            for(auto i=0; i<array.size(); ++i){
                f(i);
            }
        }
        else{
            ForEachImpl<DIM>:: template op<OffsetTag>(shape, strides, order,  std::forward<F>(f));
        }
    }
    else{ // (order == Order::C_ORDER || order == Order::F_ORDER){
        if(array.contiguous(order)){
            for(auto i=0; i<array.size(); ++i){
                f(i);
            }
        }
        else{
            ForEachImpl<DIM>:: template op<OffsetTag>(shape, strides, order,  std::forward<F>(f));
        }
    }
}


// 1 arg
template<std::size_t DIM,  class F>
very_inline static void forEachOffset(
    const Shape<DIM> & shape, 
    const Strides<DIM> & strides,
    const Order & order,
    F && f
){
    if( (order == Order::ANY_ORDER && detail_multi_array::isContiguous(shape, strides)) ||
        detail_multi_array::isContiguous(shape, strides, order))
    {
        const auto size = detail_multi_array::shapeSize(shape);
        for(auto i=0; i<size; ++i){
            f(i);
        }
    }
    else{
        ForEachImpl<DIM>:: template op<OffsetTag>(shape, strides, order,  std::forward<F>(f));
    }
}


template<std::size_t DIM, class F>
inline static void forEachCoordinate(
    const Shape<DIM> & shape, 
    const Strides<DIM> & strides,
    const Order & order,
    F && f
){
    ForEachImpl<DIM>:: template op<CoordinateTag>(shape, strides, order,  std::forward<F>(f));
}

template<std::size_t DIM, class F>
static void forEachOffsetAndCoordinate(
    const Shape<DIM> & shape, 
    const Strides<DIM> & strides,
    const Order & order,
    F && f
){
    ForEachImpl<DIM>:: template op<OffsetAndCoordinateTag>(shape, strides, order,  std::forward<F>(f));
}





// 2 arg
template<std::size_t DIM, class F>
inline static void forEachOffset(
    const Shape<DIM> & shape, 
    const Strides<DIM> & stridesA,
    const Strides<DIM> & stridesB,
    const Order & order,
    F && f
){
    if(stridesA.relaxedEqual(stridesB)){
        ForEachImpl<DIM>:: template op<OffsetTag>(shape, stridesA, order, [&](const int64_t offset){
            f(offset, offset);
        });
    }
    else{
        ForEachImpl<DIM>:: template op<OffsetAOffsetBTag>(shape, stridesA, stridesB, order,  std::forward<F>(f));
    }
    
}





template<std::size_t DIM, class F>
inline static void forEachOffsetAndCoordinate(
    const Shape<DIM> & shape, 
    const Strides<DIM> & stridesA,
    const Strides<DIM> & stridesB,
    const Order & order,
    F && f
){
    ForEachImpl<DIM>:: template op<OffsetAOffsetBCoordinateTag>(shape, stridesA, stridesB, order,  std::forward<F>(f));
}



}
}