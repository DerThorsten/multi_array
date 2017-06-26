#pragma once


#include <iostream>
#include <memory>
#include <type_traits>
#include <utility>
#include <algorithm>


#include "meta.hxx"
#include "shared_handle.hxx"
#include "misc.hxx"
#include "indexing_types.hxx"
#include "bracket_op_dispatcher.hxx"
#include "view_expression.hxx"
#include "operate.hxx"
#include "navigator.hxx"
#include "for_each_impl.hxx"

namespace multi_array{



template<std::size_t DIM, class E, class T>
class ViewExpression;

template<std::size_t DIM, class E, class T, class UnaryFunctor>
class UnaryViewExpression;

template<std::size_t DIM, class E1, class T1, class E2, class T2, class BinaryFunctor>
class BinaryViewExpression;

template<std::size_t DIM, class E, class T, class S, class BinaryFunctor>
class BinaryViewExpressionScalarFirst;

template<std::size_t DIM, class E, class T, class S, class BinaryFunctor>
class BinaryViewExpressionScalarSecond;




template<class T, std::size_t DIM, bool IS_CONST = false>
class SmartMultiArray
: 
    //public MultiArrayBase<T, DIM, SmartMultiArray<T, DIM,IS_CONST >>
    public ViewExpression<DIM, SmartMultiArray<T, DIM, IS_CONST>, T>
{

    typedef ViewExpression<DIM, SmartMultiArray<T, DIM, IS_CONST>, T> BaseType;

    template<std::size_t _DIM, class _E, class _T>
    friend class ViewExpression;

    template<std::size_t _DIM, class _E, class _T, class _UnaryFunctor>
    friend class UnaryViewExpression;

    template<std::size_t _DIM, class _E1, class _T1, class _E2, class _T2, class _BinaryFunctor>
    friend class BinaryViewExpression;

    template<std::size_t _DIM, class _E, class _T, class _S, class _BinaryFunctor>
    friend class BinaryViewExpressionScalarFirst;

    template<std::size_t _DIM, class _E, class _T, class _S, class _BinaryFunctor>
    friend class BinaryViewExpressionScalarSecond;


    template<class MARRAY, bool CONST_INSTANCE, class ... Args>
    friend class detail_multi_array::BracketOpDispatcher;

    template<class MARRAY_IN, class MARRAY_OUT, std::size_t IN_AXIS_INDEX, std::size_t OUT_AXIS_INDEX,  class ARG, bool IS_INTEGRAL>
    friend struct detail_multi_array::ProcessArg;

    template<class _T, std::size_t _DIM, bool _IS_CONST>
    friend class SmartMultiArray;

    // private typedefs
    typedef SmartMultiArray<T, DIM> SelfType;
    typedef detail_multi_array::SharedHandle SharedHandleType;
    
public:

    // rebinding
    template<class REBIND_T, std::size_t REBIND_DIM, bool REBIND_IS_CONST>
    struct Rebind{
        typedef SmartMultiArray<REBIND_T, REBIND_DIM, REBIND_IS_CONST> type;
    };

    template<class REBIND_T>
    struct RebindType{
        typedef SmartMultiArray<REBIND_T, DIM, IS_CONST> type;
    };

    template<std::size_t REBIND_DIM>
    struct RebindDimension{
        typedef SmartMultiArray<T, REBIND_DIM, IS_CONST> type;
    };

    template<bool REBIND_IS_CONST>
    struct RebindIsConst{
        typedef SmartMultiArray<T, DIM, REBIND_IS_CONST> type;
    };


    typedef T value_type;
    typedef typename std::conditional<IS_CONST, const T & , T &>::type reference;
    typedef const T & const_reference;
    
    typedef std::integral_constant<std::size_t, DIM> DimensionType;
    typedef std::integral_constant<bool, IS_CONST> IsConstType;


    typedef Shape<DIM> ShapeType;
    typedef Strides<DIM> StridesType;

    typedef typename std::conditional<IS_CONST, const T * , T *>::type DataPtrType;
    typedef const T * ConstDataPtrType;







    SmartMultiArray();
    SmartMultiArray(const ShapeType & shape);
    SmartMultiArray(const ShapeType & shape, const T &);
    SmartMultiArray(const SmartMultiArray & other);

    //////////////////////////////////////////////////
    // assignment operators
    //////////////////////////////////////////////////
    SmartMultiArray & operator= ( const SmartMultiArray & );

    // assign from fancy expression =)
    template<class E, class U>
    SmartMultiArray & operator= ( const ViewExpression<DIM, E, U> & );

    //////////////////////////////////////////////////
    // simple queries
    //////////////////////////////////////////////////
    constexpr std::size_t dimension() const ;
    const StridesType & strides()const;
    const ShapeType & shape()const;
    int64_t  strides(const std::size_t a)const;
    int64_t  shape(const std::size_t a)const;
    uint64_t size()const;
    bool empty()const;
    const SharedHandleType & smartHandle()const;

    bool contiguous() const;
    bool contiguous(const Order & ) const;
    bool cOrderContiguous() const;
    bool fOrderContiguous() const;

    bool overlaps(void * ptr)const;
    template<class _T, std::size_t _DIM, bool _IS_CONST>
    bool overlaps(const SmartMultiArray<_T, _DIM, _IS_CONST> & other)const;



    //////////////////////////////////////////////////
    // data access and modification
    ////////////////////////////////////////////////// 

    reference front();
    reference back();
    const_reference front()const;
    const_reference back()const;

    T * data();
    ConstDataPtrType data()const;
        
    template<class ... Args>
    typename detail_multi_array::BracketOpDispatcher<SelfType, false, Args ... >::type::type
    operator()(Args && ... args);

    template<class ... Args>
    typename detail_multi_array::BracketOpDispatcher<SelfType, true, Args ... >::type::type
    operator()(Args && ... args)const;


    template<class F>
    void forEach(F && f, const Order & order = Order::C_ORDER)const;
    template<class F>
    void forEach(F && f, const Order & order = Order::C_ORDER);


    inline SmartMultiArray<T, DIM-1> 
    bind(const uint64_t axis, const uint64_t value);

    
    template<std::size_t SHAPE_DIM>
    SmartMultiArray<T, SHAPE_DIM, IS_CONST> 
    reshape(const Shape<SHAPE_DIM> & shape, const Order & order = Order::C_ORDER)const;
    


    void transpose();
    SmartMultiArray transposedView()const;



    bool matchingStrides()const;

private:


    const_reference unsafeAccess(const uint64_t index)const;
    uint64_t lastValidMemOffset()const;
    
    template<class E, class U>
    void assignFromNonOverlappingExpression(const ViewExpression<DIM, E, U> &);

    
    ShapeType shape_;
    StridesType strides_;
    uint64_t size_;
    T * data_;
    // this is encapsulating the smart pointer
    // responsible for keeping track
    // of the ownership
    SharedHandleType smart_handle_;

};

template<class T, std::size_t DIM, bool IS_CONST>
SmartMultiArray<T, DIM, IS_CONST>::SmartMultiArray()

:   BaseType(),
    shape_(),
    strides_(),
    size_(0),
    data_(nullptr),
    smart_handle_(){ 
}


template<class T, std::size_t DIM, bool IS_CONST>
inline 
SmartMultiArray<T, DIM, IS_CONST>::SmartMultiArray(
    const ShapeType & shape
)
:   BaseType(),
    shape_(shape),
    strides_(detail_multi_array::cOrderStrides(shape)),
    size_(detail_multi_array::shapeSize(shape)),
    data_(new T[size_]),
    smart_handle_(data_, typename SharedHandleType::AllocNewTag()){ 

}


template<class T, std::size_t DIM, bool IS_CONST>
inline 
SmartMultiArray<T, DIM, IS_CONST>::SmartMultiArray(
    const ShapeType & shape,
    const T &  initValue
)
:   BaseType(),
    shape_(shape),
    strides_(detail_multi_array::cOrderStrides(shape)),
    size_(detail_multi_array::shapeSize(shape)),
    data_(new T[size_]),
    smart_handle_(data_, typename SharedHandleType::AllocNewTag()){ 
    std::fill(data_, data_ + this->size(), initValue);
}


template<class T, std::size_t DIM, bool IS_CONST>
inline 
SmartMultiArray<T, DIM, IS_CONST>::SmartMultiArray(
    const  SmartMultiArray & other
)
:   BaseType(),
    shape_(other.shape_),
    strides_(other.strides_),
    size_(other.size_),
    data_(other.data_),
    smart_handle_(other.smart_handle_,  typename SharedHandleType::CopyHandleTag())
{

}


template<class T, std::size_t DIM, bool IS_CONST>
inline auto 
SmartMultiArray<T, DIM, IS_CONST>::operator= ( 
    const SmartMultiArray & other
) -> SmartMultiArray &{
    if(this!=&other){

    }
    return *this;
}



template<class T, std::size_t DIM, bool IS_CONST>
template<class E, class U>
inline auto 
SmartMultiArray<T, DIM, IS_CONST>::operator= ( 
    const ViewExpression<DIM, E, U> & e
) -> SmartMultiArray & {

    // TODO check if empty


    const E & expression = e;
    const auto isOverlapping =  e.overlaps(*this);
    if(isOverlapping){
        // alloc a new array (TODO think about axis order)
        SmartMultiArray<T,DIM,IS_CONST> tmp(this->shape());
        tmp.assignFromNonOverlappingExpression(e);
        // assign tmp to this
        // TODO
    }
    else{
        return this->assignFromNonOverlappingExpression(e);
    }

    return *this;
}


template<class T, std::size_t DIM, bool IS_CONST>
template<class E, class U>
inline void 
SmartMultiArray<T, DIM, IS_CONST>::assignFromNonOverlappingExpression ( 
    const ViewExpression<DIM, E, U> & e
){

    // check the expression
    // is homogeneous wrt the strides
    if(e.matchingStrides()){
        // now we can check 
        // if the array is also
        // matching the expressions strides
        if(e.matchingStrides(this->strides())){

            // optimal operation in case
            // of dense array
            if(this->dense){
                //the array is dense this means withe can use the 
                // ``unsafeAccess`` access  of the expression
                // which is just linear memory access
            }
            else{
                // we need to create te
            }
        }
        else{

        }

    }
}





template<class T, std::size_t DIM, bool IS_CONST>
inline constexpr std::size_t 
SmartMultiArray<T, DIM, IS_CONST>::dimension() const { 
    return DimensionType::value; 
}


template<class T, std::size_t DIM, bool IS_CONST>
inline auto  
SmartMultiArray<T, DIM, IS_CONST>::strides()const -> const StridesType &  {
    return strides_;
}


template<class T, std::size_t DIM, bool IS_CONST>
inline auto 
SmartMultiArray<T, DIM, IS_CONST>::shape()const -> const ShapeType &{
    return shape_;
}


template<class T, std::size_t DIM, bool IS_CONST>
inline int64_t 
SmartMultiArray<T, DIM, IS_CONST>::strides(
    const std::size_t a
)const{
    const auto s =  strides_[a];
    return s;
}


template<class T, std::size_t DIM, bool IS_CONST>
inline int64_t 
SmartMultiArray<T, DIM, IS_CONST>::shape(
    const std::size_t a
)const{
    return shape_[a];
}


template<class T, std::size_t DIM, bool IS_CONST>
inline uint64_t 
SmartMultiArray<T, DIM, IS_CONST>::size()const{
    return size_;
}

template<class T, std::size_t DIM, bool IS_CONST>
inline auto 
SmartMultiArray<T, DIM, IS_CONST>::smartHandle()const -> const SharedHandleType &{
    return smart_handle_;
}

template<class T, std::size_t DIM, bool IS_CONST>
inline bool
SmartMultiArray<T, DIM, IS_CONST>::contiguous()const {
    return this->lastValidMemOffset() + 1 == this->size();
}


template<class T, std::size_t DIM, bool IS_CONST>
inline bool
SmartMultiArray<T, DIM, IS_CONST>::contiguous(
    const Order & order
)const {
    if(order == Order::C_ORDER)
        return this->cOrderContiguous();
    else if(order == Order::F_ORDER)
        return this->fOrderContiguous();
    else{
        throw std::runtime_error("wrong order");
    }
}


template<class T, std::size_t DIM, bool IS_CONST>
inline bool
SmartMultiArray<T, DIM, IS_CONST>::cOrderContiguous()const {

    if(this->contiguous()){
        const auto cStrides = detail_multi_array::cOrderStrides(shape_);
        return strides_.relaxedEqual(cStrides);
    }
    return false;
}

template<class T, std::size_t DIM, bool IS_CONST>
inline bool
SmartMultiArray<T, DIM, IS_CONST>::fOrderContiguous()const {

    if(this->contiguous()){
        const auto cStrides = detail_multi_array::fOrderStrides(shape_);
        return strides_.relaxedEqual(cStrides);
    }
    return false;
}





template<class T, std::size_t DIM, bool IS_CONST>
inline T * 
SmartMultiArray<T, DIM, IS_CONST>::data() {
    return data_;
}

template<class T, std::size_t DIM, bool IS_CONST>
inline auto 
SmartMultiArray<T, DIM, IS_CONST>::data()const  -> ConstDataPtrType {
    return data_;
}


template<class T, std::size_t DIM, bool IS_CONST>
inline bool 
SmartMultiArray<T, DIM, IS_CONST>::overlaps(
    void * ptr
)const{

    void * aBegin  = this->data_;
    void * aEnd  = this->data_ + this->lastValidMemOffset() + 1;

    return ptr>=aBegin && ptr<aEnd;
}

template<class T, std::size_t DIM, bool IS_CONST>
template<class _T, std::size_t _DIM, bool _IS_CONST>
inline bool 
SmartMultiArray<T, DIM, IS_CONST>::overlaps(
    const SmartMultiArray<_T, _DIM, _IS_CONST> & other
)const{

    void * aBegin  = this->data_;
    void * bBegin  = other.data_;
    void * aEnd  = this->data_ + this->lastValidMemOffset() + 1;
    void * bEnd  = other.data_ + other.lastValidMemOffset() + 1;

    return this->overlaps(other.data_) ||
           other.overlaps(this->data_);
}


template<class T, std::size_t DIM, bool IS_CONST>
template<class F>
inline void 
SmartMultiArray<T, DIM, IS_CONST>::forEach(
    F && f,
    const Order & order
)const{
    if(this->contiguous()){
        for(auto i=0; i<size_; ++i)
            f(data_[i]);
    }
    else{
        detail_for_each::ForEachImpl<DIM, uint8_t(Order::C_ORDER) >::op(*this, std::forward<F>(f));
    }
}


template<class T, std::size_t DIM, bool IS_CONST>
template<class F>
inline void 
SmartMultiArray<T, DIM, IS_CONST>::forEach(
    F && f,
    const Order & order
){
    if(this->contiguous()){
        for(auto i=0; i<size_; ++i)
            f(data_[i]);
    }
    else{
        detail_for_each::ForEachImpl<DIM, uint8_t(Order::C_ORDER) >::op(*this, std::forward<F>(f));
    }
}


template<class T, std::size_t DIM, bool IS_CONST>
inline bool 
SmartMultiArray<T, DIM, IS_CONST>::empty()const{
    return data_ == nullptr;
}

template<class T, std::size_t DIM, bool IS_CONST>
inline auto
SmartMultiArray<T, DIM, IS_CONST>::front() ->reference {
    return *data_;
}

template<class T, std::size_t DIM, bool IS_CONST>
inline auto
SmartMultiArray<T, DIM, IS_CONST>::back() -> reference {
    return data_[this->lastValidMemOffset()];
}

template<class T, std::size_t DIM, bool IS_CONST>
inline auto 
SmartMultiArray<T, DIM, IS_CONST>::front()const -> const_reference {
    return *data_;
}

template<class T, std::size_t DIM, bool IS_CONST>
inline auto 
SmartMultiArray<T, DIM, IS_CONST>::back()const -> const_reference {
    return data_[this->lastValidMemOffset()];
}


#if 0 
template<class T, std::size_t DIM, bool IS_CONST>
inline void
SmartMultiArray<T, DIM, IS_CONST>::resize(
    const Shape<DIM> & shape
) 
{
    const auto shapeSize = detail_multi_array::shapeSize(shape);
    if(shapeSize == size){
        if(this->contiguous()){
            this->reshapeView()
        }
    }
    else{

    }
}
#endif

template<class T, std::size_t DIM, bool IS_CONST>
void 
SmartMultiArray<T, DIM, IS_CONST>::transpose(
){
    std::reverse(shape_.begin(), shape_.end());
    std::reverse(strides_.begin(), strides_.end());
}

template<class T, std::size_t DIM, bool IS_CONST>
inline auto   
SmartMultiArray<T, DIM, IS_CONST>::transposedView(

)const -> SmartMultiArray<T, DIM, IS_CONST>{
    SmartMultiArray<T, DIM, IS_CONST> ret(*this);
    ret.transpose();
    return ret;
}


template<class T, std::size_t DIM, bool IS_CONST>
template<std::size_t SHAPE_DIM>
inline auto 
SmartMultiArray<T, DIM, IS_CONST>::reshape(
    const Shape<SHAPE_DIM> & shape,
    const Order & order
) const -> SmartMultiArray<T, SHAPE_DIM, IS_CONST> {

   
    if(order != Order::C_ORDER && order != Order::F_ORDER){
        throw std::runtime_error("can only reshape in C or F Order currently");
    }

    // try to replace -1 with fitting values
    const auto actualShape = shape.makeShape(this->size());


    // is contiguous with respect 
    // to the desired order
    if( this->contiguous(order) ){

        SmartMultiArray<T, SHAPE_DIM, IS_CONST> retVal;
        retVal.shape_ = actualShape;
        retVal.strides_ = detail_multi_array::strides(actualShape, order);
        retVal.size_ = this->size_;
        retVal.data_ = this->data_;
        retVal.smart_handle_ = this->smart_handle_;

        return retVal;
    }
    else{

        SmartMultiArray<T, SHAPE_DIM, IS_CONST> retVal(actualShape);
        auto retPtr = retVal.data();
        uint64_t c = 0;
        this->forEach([&](const_reference value){
            retPtr[c] = value;
            ++c;
        }, order);

        return retVal;
    }
}





template<class T, std::size_t DIM, bool IS_CONST>
template<class ... Args>
inline auto 
SmartMultiArray<T, DIM, IS_CONST>::operator()(
    Args && ... args
) -> typename detail_multi_array::BracketOpDispatcher<SelfType, false, Args ... >::type::type
{

    typedef detail_multi_array::BracketOpDispatcher<SelfType, false, Args ... > DispatcherType;
    typedef typename DispatcherType::type ImplType;
    return ImplType::op(*this, std::forward<Args>(args)...);

}   

template<class T, std::size_t DIM, bool IS_CONST>
template<class ... Args>
inline auto 
SmartMultiArray<T, DIM, IS_CONST>::operator()(
    Args && ... args
) const -> typename detail_multi_array::BracketOpDispatcher<SelfType, true, Args ... >::type::type
{

    typedef detail_multi_array::BracketOpDispatcher<SelfType, true, Args ... > DispatcherType;
    typedef typename DispatcherType::type ImplType;
    return ImplType::op(*this, std::forward<Args>(args)...);
}


template<class T, std::size_t DIM, bool IS_CONST>
inline SmartMultiArray<T, DIM-1> 
SmartMultiArray<T, DIM, IS_CONST>::bind(
    const uint64_t axis, 
    const uint64_t value
){
    static_assert(DIM >= 1, "Can Only bind arrays with DIM >= 1");
    typedef SmartMultiArray<T, DIM-1> ResType;
    typedef typename ResType::ShapeType    ResShapeType;
    typedef typename ResType::StridesType  ResStridesType;


    ResType res;
    res.data_ = data_;
    res.size_ = size_;
    res.smart_handle_ = smart_handle_;
    uint64_t aRes = 0;
    for(uint64_t a=0; a<DIM; ++a){
        if(a == axis){
            res.data_ += shape_[a] * strides_[a];
            res.size_ /= shape_[a];
        }
        else{
            res.shape_[aRes] = shape_[a];
            res.strides_[aRes] = strides_[a];
            ++aRes;
        }
    }
    return res;
}


template<class T, std::size_t DIM, bool IS_CONST>
inline auto
SmartMultiArray<T, DIM, IS_CONST>::unsafeAccess(
    const uint64_t index
)const -> const_reference{
    return data_[index];
}

template<class T, std::size_t DIM, bool IS_CONST>
inline bool 
SmartMultiArray<T, DIM, IS_CONST>::matchingStrides()const{
    return true;
}



template<class T, std::size_t DIM, bool IS_CONST>
inline uint64_t 
SmartMultiArray<T, DIM, IS_CONST>::lastValidMemOffset()const{
    uint64_t offset = 0;
    for(auto d=0; d<DIM; ++d)
        offset += strides_[d]* (shape_[d] - 1);
    return offset;
}






}
