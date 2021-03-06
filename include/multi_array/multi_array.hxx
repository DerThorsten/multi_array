#pragma once



#include <iostream>
#include <memory>
#include <type_traits>
#include <utility>
#include <algorithm>
#include <functional>


#include "multi_array/meta.hxx"
#include "multi_array/shared_handle.hxx"
#include "multi_array/misc.hxx"
#include "multi_array/indexing_types.hxx"
#include "multi_array/bracket_op_dispatcher.hxx"

#include "multi_array/expr/view_expression.hxx"

#include "multi_array/operate.hxx"
#include "multi_array/accumulate.hxx"
#include "multi_array/accumulators.hxx"
#include "multi_array/navigator.hxx"
#include "multi_array/for_each_impl.hxx"
#include "multi_array/for_each_offset.hxx"
#include "multi_array/for_each_offset_and_coordiante.hxx"
#include "multi_array/runtime_check.hxx"
#include "multi_array/multi_array_factories.hxx"
#include "multi_array/pre_exisiting_memory_poliy.hxx"
#include "multi_array/math.hxx"

namespace multi_array{


// forward declarations to expression
// template classes:
// they are just for making operations
// as array = arrayA*2 + 2*(arrayB*3 + arrayD)
// very very efficient!
template<std::size_t DIM, class E, class T>
class ViewExpression;
template<std::size_t DIM, class E, class T, class UnaryFunctor>
class UnaryViewExpression;
template<std::size_t DIM, class E1, class T1, class E2, class T2, class BinaryFunctor>
class BinaryViewExpression;

template< std::size_t DIM, class E1, class T1, class E2, class T2,  class E3, class T3,  class TernaryFunctor>
class TernaryViewExpression;

template<std::size_t DIM,class E1, class T1,class E2, class T2, class E3, class T3, class E4, class T4, class QuaquaternaryFunctor>
class QuaternaryViewExpression;



template<std::size_t DIM, std::size_t AXIS, class T>
class CoordExpr;



template<class T, std::size_t DIM, bool IS_CONST = false>
class SmartMultiArray
: 
    // to make operations as array = arrayA*2 + 2*(arrayB*3 + arrayD)
    // very fast we need to make the array itself
    // an expression template
    public ViewExpression<DIM, SmartMultiArray<T, DIM, IS_CONST>, T>
{

    typedef ViewExpressionMeta<false, false> ViewExpressionMetaType;

    // cool Cats have many friends =)
    template<std::size_t _DIM, class _E, class _T>
    friend class ViewExpression;

    template<std::size_t _DIM, class _E, class _T, class _UnaryFunctor>
    friend class UnaryViewExpression;

    template<std::size_t _DIM, class _E1, class _T1, class _E2, class _T2, class _BinaryFunctor>
    friend class BinaryViewExpression;

    template< std::size_t _DIM, class _E1, class _T1, class _E2, class _T2,  class _E3, class _T3,  class _TernaryFunctor>
    friend class TernaryViewExpression;
    
    template<std::size_t _DIM,class _E1, class _T1,class _E2, class _T2, class _E3, class _T3, class _E4, class _T4, class _QuaquaternaryFunctor>
    friend class QuaternaryViewExpression;

    template<class MARRAY, bool CONST_INSTANCE, class ... Args>
    friend class detail_multi_array::BracketOpDispatcher;

    template<class MARRAY_IN, class MARRAY_OUT, std::size_t IN_AXIS_INDEX, std::size_t OUT_AXIS_INDEX, int N_ARG_TO_COVER_WITH_ELLIPSIS, class ARG, bool IS_INTEGRAL>
    friend struct detail_multi_array::ProcessArg;

    template<class _T, std::size_t _DIM, bool _IS_CONST>
    friend class SmartMultiArray;


    //template<class _T, std::size_t _DIM, bool _IS_CONST>
    //friend SmartMultiArray & operator+=(SmartMultiArray, const SmartMultiArray<_T, _DIM, _IS_CONST> &);


    // internal typedef 
    typedef SmartMultiArray<T, DIM> SelfType;
    typedef ViewExpression<DIM, SelfType, T> BaseType;
    typedef detail_multi_array::SharedHandle SharedHandleType;
    
public:

    // rebinding 
    // aka changing a some/one(/all) of the template
    // arguments but keep the rest of the template arguments
    template<class REBIND_T>
    struct RebindType{
        typedef SmartMultiArray<REBIND_T, DIM, IS_CONST> type;
    };
    template<std::size_t REBIND_DIM>
    struct RebindDimension{
        typedef SmartMultiArray<T, REBIND_DIM, IS_CONST> type;
    };

    template<std::size_t REBIND_DIM, bool REBIND_IS_CONST>
    struct RebindDimensionAndIsConst{
        typedef SmartMultiArray<T, REBIND_DIM, REBIND_IS_CONST> type;
    };


    template<bool REBIND_IS_CONST>
    struct RebindIsConst{
        typedef SmartMultiArray<T, DIM, REBIND_IS_CONST> type;
    };
    // rebind all
    template<class REBIND_T, std::size_t REBIND_DIM, bool REBIND_IS_CONST>
    struct Rebind{
        typedef SmartMultiArray<REBIND_T, REBIND_DIM, REBIND_IS_CONST> type;
    };

    // stl compatible lower_case_underscore typedefs
    typedef T value_type;
    typedef typename std::conditional<IS_CONST, const T & , T &>::type reference;
    typedef const T & const_reference;

    typedef typename std::conditional<IS_CONST, const T * , T *>::type pointer;
    typedef const T * const_pointer;
    
    // typedefs to intergral constants
    typedef std::integral_constant<std::size_t, DIM> DimensionType;
    typedef std::integral_constant<bool, IS_CONST> IsConstType;

    // shape and stride type
    typedef Shape<DIM> ShapeType;
    typedef Strides<DIM> StridesType;

    // the data ptr type
    typedef typename std::conditional<IS_CONST, const T * , T *>::type DataPtrType;
    typedef const T * ConstDataPtrType;



    // todo MAKE ME PRIVATE
    class ExpressionPseudoIterator {
    public:
        ExpressionPseudoIterator(const SelfType & array)
        : array_(array), // cast!
          data_(array.data()),
          offset_(0){

        }


        void incrementCoordinate(const std::size_t axis){ 
            offset_ += array_.strides(axis); 
        }

        void resetCoordinate(const std::size_t axis){ 
            offset_ -= array_.strides(axis)*
                 (array_.shape(axis) - 1); 
        }
        const_reference operator*() const { 
            return data_[offset_]; 
        }
    private:
        ConstDataPtrType data_;
        const SelfType & array_;
        uint64_t offset_;
    };

    class ExpressionBroadcastPseudoIterator {
    public:
        ExpressionBroadcastPseudoIterator(const SelfType & array)
        : array_(array), 
          data_(array.data()),
          offset_(0),
          coord_(0)
        {

        }


        void incrementCoordinate(const std::size_t axis){ 
            if(coord_[axis] + 1 < array_.shape(axis)){
                ++coord_[axis];
                offset_ += array_.strides(axis); 
            }
        }

        void resetCoordinate(const std::size_t axis){ 
            offset_ -= array_.strides(axis)*
                 (array_.shape(axis) - 1); 
        }
        const_reference operator*() const { 
            return data_[offset_]; 
        }
    private:
        
        const SelfType & array_;
        ConstDataPtrType data_;
        uint64_t offset_;
        Coordinate<DIM> coord_;
    };




    //////////////////////////////////////////////////
    // constructors
    //////////////////////////////////////////////////

    // empty, straight forward, isn't it?
    SmartMultiArray();              
    // from shape with c-order
    SmartMultiArray(const ShapeType & shape);
    // from shape with c-order and default value
    SmartMultiArray(const ShapeType & shape, const T &);
    // from other array <= NO new alloc, just a ref
    SmartMultiArray(const SmartMultiArray & other);
    // from other array with almost same type
    SmartMultiArray(const SmartMultiArray<T, DIM, !IS_CONST> & );
    // from other array with different type
    template<class _T, bool _IS_CONST>
    SmartMultiArray(const SmartMultiArray<_T,DIM, _IS_CONST> & );
    // from a view expression
    template<class E, class TE>
    SmartMultiArray(const ViewExpression<DIM, E, TE> & );

    // from existing mem
    SmartMultiArray(const ShapeType &, const StridesType &, pointer ,const PreExistingMemoryPolicy & preExistingMemoryPolicy);
    SmartMultiArray(const ShapeType &, const StridesType &, pointer ,const std::function<void(pointer)> &);




    //////////////////////////////////////////////////
    // assign
    //////////////////////////////////////////////////
    void assign(const ShapeType & shape);







    //////////////////////////////////////////////////
    // assignment operators
    //////////////////////////////////////////////////

    SmartMultiArray & operator= ( const SmartMultiArray & );
    SmartMultiArray & operator= ( const SmartMultiArray<T,DIM,!IS_CONST> &  );
    template<class _T, bool _IS_CONST>
    SmartMultiArray & operator= ( const SmartMultiArray<_T,DIM,_IS_CONST> &  );

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

    SmartMultiArray copy() const;
    SmartMultiArray view() const;
    typename RebindIsConst<true>::type constView() const;


    // special function to check if an array has the same
    // shape and values
    template<class _T, bool _IS_CONST>
    bool equal(const SmartMultiArray<_T, DIM, _IS_CONST> & )const;

    template<class ACCUMULATOR>
    void accumulate(ACCUMULATOR & )const;

    typename accumulators::Sum<T>::type  sum() const;
    typename accumulators::Mean<T>::type mean() const;
    typename accumulators::Min<T>::type min() const;
    typename accumulators::Max<T>::type max() const;
    typename accumulators::MinMax<T>::type minMax() const;

    template<class ACCUMULATOR>
    void accumulate(ACCUMULATOR & , const AccWithCoordEnum &)const;




    /////////////////////////////////////
    // sorting
    /////////////////////////////////////

    template<class COMPARE>
    SmartMultiArray sort(const uint16_t axis, COMPARE && compare) const;

    template<class COMPARE>
    void sortSelf(const uint16_t axis, COMPARE && compare)const;


    //////////////////////////////////////////////////
    // data access and modification
    ////////////////////////////////////////////////// 

    // stl compatible front and back
    reference front();
    reference back();
    const_reference front()const;
    const_reference back()const;

    // c++ 11 vector compatibility / analog
    // to give users (unsafe) access to data ptr
    T * data();
    ConstDataPtrType data()const;
        
    // numpy styled slicing synatx
    template<class ... Args>
    typename detail_multi_array::BracketOpDispatcher<SelfType, false, Args ... >::type::type
    operator()(Args && ... args);
    template<class ... Args>
    typename detail_multi_array::BracketOpDispatcher<SelfType, true, Args ... >::type::type
    operator()(Args && ... args)const;


    // to stuff for each element
    // and select the order in which the 
    // stuff is done
    template<class F>
    void forEach(const Order & order , F && f)const;
    template<class F>
    void forEach(F && f)const;
    template<class F>
    void forEach(const Order & order , F && f);
    template<class F>
    void forEach(F && f);


    // bind a axis (-> reduce DIM by one)
    inline SmartMultiArray<T, DIM-1, IS_CONST> 
    bind(const uint64_t axis, const uint64_t value);

    inline SmartMultiArray<T, DIM-1, true> 
    bind(const uint64_t axis, const uint64_t value)const;

    // reshape array 
    // -> size must match, tries
    // to return a view if possibe
    template<std::size_t SHAPE_DIM>
    SmartMultiArray<T, SHAPE_DIM, IS_CONST> 
    reshape(const Shape<SHAPE_DIM> & shape, const Order & order = Order::C_ORDER)const;
    

    // transpose the array (inplace)
    void transpose();
    void transposeSelf();


    // return transposed array
    SmartMultiArray transposedView()const;




    // helper to assign from another array
    template<class _T, bool _IS_CONST, class F>
    void operate(const SmartMultiArray<_T, DIM, _IS_CONST> &, F && f);


    template<class E, class U, class F>
    void operate(const ViewExpression<DIM, E, U> &, F && f);


private:
    template<class FUNCTOR>
    void forEach1DArrayAlongAxis(const uint16_t axis, FUNCTOR && functor);
    template<class FUNCTOR>
    void forEach1DArrayAlongAxis(const uint16_t axis, FUNCTOR && functor)const;


    // this is just for compatibility 
    // with the ViewExpression System

    bool hasShape() const;
    bool hasStrides() const;
    bool matchingStrides()const;
    bool matchingStrides(const StridesType &)const;



    // the same as data_[index] but will return the
    // correct type (in case of IS_CONST = true)
    const_reference unsafeAccess(const uint64_t index)const;
    reference unsafeAccess(const uint64_t index);

    const_reference unsafeAccess(const uint64_t index, const Coordinate<DIM>  & coordinate)const;
    reference unsafeAccess(const uint64_t index, const Coordinate<DIM>  & coordinate);



    // get the last element in the array (wrt. the strides)
    uint64_t lastValidMemOffset()const;
    
    // helper to assign from an expression as    ((a+b)*3 +d)
    template<class E, class U>
    void assignFromNonOverlappingExpression(const ViewExpression<DIM, E, U> &);

    // helper to assign from another array
    template<class _T, bool _IS_CONST>
    void assignFromOther(const SmartMultiArray<_T, DIM, _IS_CONST> &);



    template<bool _IS_CONST>
    void expressionCopyConstructorHelper(const SmartMultiArray<T, DIM,  _IS_CONST> & rhs);
    template<class _CATCH_THE_REST_TEMPLATE>
    void expressionCopyConstructorHelper(const _CATCH_THE_REST_TEMPLATE & rhs);


    /////////////////////////////////////////
    // the members
    /////////////////////////////////////////
    ShapeType shape_;       // the shape
    StridesType strides_;   // the strides  (can have zeros => singleton)
    uint64_t size_;         // the total number of elements in the array
    T * data_;              // the data ptr
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
inline 
SmartMultiArray<T, DIM, IS_CONST>::SmartMultiArray(
    const  SmartMultiArray<T,DIM, !IS_CONST> & other
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
template<class _T, bool _IS_CONST>
inline
SmartMultiArray<T, DIM, IS_CONST>::SmartMultiArray(
    const SmartMultiArray<_T,DIM, _IS_CONST> & other
)
:   BaseType(),
    shape_(other.shape_),
    strides_(detail_multi_array::cOrderStrides(other.shape_)),
    size_(other.size_),
    data_(new T[size_]),
    smart_handle_(data_, typename SharedHandleType::AllocNewTag())
{

    *this = other;
}


// from existing mem
template<class T, std::size_t DIM, bool IS_CONST>
inline SmartMultiArray<T, DIM, IS_CONST>::SmartMultiArray(
    const ShapeType & shape, 
    const StridesType & strides, 
    pointer data,
    const PreExistingMemoryPolicy & preExistingMemoryPolicy
)
:   BaseType(),
    shape_(shape),
    strides_(strides),
    size_(detail_multi_array::shapeSize(shape)),
    data_(data),
    smart_handle_(data, preExistingMemoryPolicy)
{

}

// from existing mem
template<class T, std::size_t DIM, bool IS_CONST>
inline SmartMultiArray<T, DIM, IS_CONST>::SmartMultiArray(
    const ShapeType & shape, 
    const StridesType & strides, 
    pointer data,
    const std::function<void(pointer)>  & f
)
:   BaseType(),
    shape_(shape),
    strides_(strides),
    size_(detail_multi_array::shapeSize(shape)),
    data_(data),
    smart_handle_(data, f)
{

}




template<class T, std::size_t DIM, bool IS_CONST>
template<class E, class TE>
inline
SmartMultiArray<T, DIM, IS_CONST>::SmartMultiArray(
    const ViewExpression<DIM, E, TE> & rhs
):  BaseType(),
    shape_(),
    strides_(),
    size_(0),
    data_(nullptr),
    smart_handle_()
{
    //std::cout<<"Copy Constructor from View Exp\n";
    this->expressionCopyConstructorHelper(static_cast<const E &>(rhs));
}


template<class T, std::size_t DIM, bool IS_CONST>
template<bool _IS_CONST>
void SmartMultiArray<T, DIM, IS_CONST>::expressionCopyConstructorHelper(
    const SmartMultiArray<T, DIM,  _IS_CONST> & rhs
){
    shape_ = rhs.shape_;
    strides_ = rhs.strides_;
    data_ = rhs.data_;
    size_ = rhs.size_;
    smart_handle_ = rhs.smart_handle_;
}


template<class T, std::size_t DIM, bool IS_CONST>
template<class _CATCH_THE_REST_TEMPLATE>
void SmartMultiArray<T, DIM, IS_CONST>::expressionCopyConstructorHelper(
    const _CATCH_THE_REST_TEMPLATE & rhs
){
    *this = rhs;
}








// same type
template<class T, std::size_t DIM, bool IS_CONST>
inline auto 
SmartMultiArray<T, DIM, IS_CONST>::operator= ( 
    const SmartMultiArray & other
) -> SmartMultiArray &{

    if(this!=&other){
        this->assignFromOther(other);
    }
    return *this;
}

// same type except const-ness
template<class T, std::size_t DIM, bool IS_CONST>
inline auto
SmartMultiArray<T, DIM, IS_CONST>::operator=(
 const SmartMultiArray<T,DIM, !IS_CONST > & other
)->SmartMultiArray & {

    this->assignFromOther(other);

}

// different type
template<class T, std::size_t DIM, bool IS_CONST>
template<class _T, bool _IS_CONST>
inline auto
SmartMultiArray<T, DIM, IS_CONST>::operator=(
 const SmartMultiArray<_T,DIM,_IS_CONST> &  other
)->SmartMultiArray & {
    this->assignFromOther(other);
}


template<class T, std::size_t DIM, bool IS_CONST>
template<class _T, bool _IS_CONST>
void 
SmartMultiArray<T, DIM, IS_CONST>::assignFromOther(
    const SmartMultiArray<_T, DIM, _IS_CONST> & other
){

    bool canOverlap = true;
    if(this->empty()){
        canOverlap = false;
        shape_ = other.shape_;
        strides_ =  detail_multi_array::cOrderStrides(shape_);
        size_ = other.size_;
        data_ = new T[size_];
        smart_handle_ = SharedHandleType(data_, typename SharedHandleType::AllocNewTag());
    }
    else{
        if(shape_ != other.shape_){
            // \TODO  make error display the shapes
            throw std::runtime_error("Shape mismatch");
        }
    }


    // check for overlap
    if(canOverlap && this->overlaps(other)){
        // copy into tmp
        // currently C-ORDER // TODO optimize order
        SmartMultiArray<T, DIM, IS_CONST> tmp(other.shape());
        // copy content of other into temp
        tmp = other;
        // copy content of temp into this
        (*this) = tmp;
        MULTI_ARRAY_CHECK(false, "TEST ME");
    }
    else{

        // matching strides
        if(this->strides().relaxedEqual(other.strides())){
            if(this->contiguous()){
                std::copy(other.data_, other.data_ + other.size(), data_);
            }
            else{
                // TODO optimize order 
                forEachOffset(shape_, strides_,Order::C_ORDER,
                    [&](const uint64_t offset){
                        data_[offset] = other.data_[offset];
                    }
                );
            }
        }
        // non matching strides
        else{
            forEachOffset(shape_, strides_, other.strides_, Order::C_ORDER,
                [&](const uint64_t offset, const uint64_t otherOffset){
                    data_[offset] = other.data_[otherOffset];
                }
            );
            MULTI_ARRAY_CHECK(false, "TEST ME");
        }
    }
}

template<class T, std::size_t DIM, bool IS_CONST>
template<class _T, bool _IS_CONST, class F>
void 
SmartMultiArray<T, DIM, IS_CONST>::operate(
    const SmartMultiArray<_T, DIM, _IS_CONST> & other,
    F && f
){

    bool canOverlap = true;
    if(this->empty()){
        canOverlap = false;
        shape_ = other.shape_;
        strides_ =  detail_multi_array::cOrderStrides(shape_);
        size_ = other.size_;
        data_ = new T[size_];
        smart_handle_ = SharedHandleType(data_, typename SharedHandleType::AllocNewTag());
    }
    else{
        if(shape_ != other.shape_){
            // \TODO  make error display the shapes
            throw std::runtime_error("Shape mismatch");
        }
    }


    // check for overlap
    if(canOverlap && this->overlaps(other)){
        // copy into tmp
        // currently C-ORDER // TODO optimize order
        SmartMultiArray<T, DIM, IS_CONST> tmp(other.shape());
        // copy content of other into temp
        tmp = other;
        // call this function with tmp as arg
        this->operate(tmp, f);
    }
    else{

        // matching strides
        if(this->strides().relaxedEqual(other.strides())){
            if(this->contiguous()){
                for(auto i=0; i<size_; ++i){
                    f(data_[i], other.data_[i]);
                }
            }
            else{
                // TODO optimize order 
                forEachOffset(shape_, strides_,Order::C_ORDER,
                    [&](const uint64_t offset){
                       f(data_[offset], other.data_[offset]);
                    }
                );
            }
        }
        // non matching strides
        else{
            forEachOffset(shape_, strides_, other.strides_, Order::C_ORDER,
                [&](const uint64_t offset, const uint64_t otherOffset){
                    f(data_[offset], other.data_[otherOffset]);
                }
            );
        }
    }
}


template<class T, std::size_t DIM, bool IS_CONST>
template<class E, class U, class F>
inline void 
SmartMultiArray<T, DIM, IS_CONST>::operate ( 
    const ViewExpression<DIM, E, U> & e,
    F && f
){

    typedef E ExpressionType;
    typedef typename ExpressionType::ExpressionPseudoIterator       ExpressionPseudoIterator;
    typedef typename ExpressionType::ViewExpressionMetaType         ViewExpressionMetaType;
    typedef typename ViewExpressionMetaType::NeedsCoordinateType    NeedsCoordinateType;
    typedef typename ViewExpressionMetaType::IsShapelessType        IsShapelessType;

    const static bool eHasShape = !IsShapelessType::value;
    const static bool eNeedsCoordinate = NeedsCoordinateType::value;

    bool canOverlap = true;
    if(this->empty()){
        MULTI_ARRAY_CHECK(eHasShape, "cannot assing shapeless expression to empty array");
        const auto eShape =e.shape(); 
        // just resize
        this->assign(e.shape());
        canOverlap = false;
    }

    if(canOverlap && e.overlaps(*this)){
        // 
        SmartMultiArray<T, DIM, IS_CONST> tmp(shape_);
        tmp = e;
        this->operate(tmp, std::forward<F>(f));
        return;
        
    }
    else{
        // check the expression
        // is homogeneous wrt the strides
        if(e.matchingStrides()){
            // now we can check 
            // if the array is also
            // matching the expressions strides
            if(e.matchingStrides(this->strides())){

                // optimal operation in case
                // of dense array
                if(this->contiguous() && !eNeedsCoordinate){
                    //the array is dense this means withe can use the 
                    // ``unsafeAccess`` access  of the expression
                    // which is just linear memory access
                    for(uint64_t i=0; i<this->size(); ++i){
                        f(this->unsafeAccess(i), e.unsafeAccess(i)); 
                    }
                    return;
                }
                else{
                    // we can loop over all memory
                    // offsets.
                    // since the expression and
                    // array have the same strides the offset
                    // is the same for e and the array
                    // currently we assume c order

                    if(!eNeedsCoordinate){
                        forEachOffset(shape_, strides_,
                            [&](const uint64_t offset){
                                f(this->unsafeAccess(offset), e.unsafeAccess(offset)); 
                            }
                        );
                    }
                    else{
                        forEachOffsetAndCoordinate(shape_, strides_,
                            [&](const uint64_t offset, const Coordinate<DIM> & coord){
                                f(this->unsafeAccess(offset,coord), e.unsafeAccess(offset,coord)); 
                            }
                        );
                    }
                    return;
                }
            }
            // fall trough
        }
        
        // fall trough

        // TODO there
        // is some speedup possible
        // if the array is dense

        ExpressionPseudoIterator eIter(e);

        // general case
        // c-order 
        uint64_t thisOffset = 0;
        Shape<DIM> coordinate;
        std::fill(coordinate.begin(), coordinate.end(), 0);

        for(;;){
            f(data_[thisOffset], *eIter);
            for(auto j=DIM-1; j>=0; --j) {
                if(coordinate[j]+1 == shape_[j]) {
                    if(j == 0) {
                        return;
                    }
                    else {
                        thisOffset -= coordinate[j] * strides_[j];
                        eIter.resetCoordinate(j);
                        coordinate[j] = 0;
                    }
                }
                else {
                    thisOffset += strides_[j];
                    eIter.incrementCoordinate(j);
                    ++coordinate[j];
                    break;
                }      
            }
        }
        return;
    }
    
}

template<class T, std::size_t DIM, bool IS_CONST>
template<class E, class U>
inline auto 
SmartMultiArray<T, DIM, IS_CONST>::operator= ( 
    const ViewExpression<DIM, E, U> & e
) -> SmartMultiArray & {

    typedef typename E::ViewExpressionMetaType                   ViewExpressionMetaType;
    typedef typename ViewExpressionMetaType::NeedsCoordinateType NeedsCoordinateType;
    typedef typename ViewExpressionMetaType::IsShapelessType     IsShapelessType;

    const static bool hasShape = !IsShapelessType::value;


    bool wasEmpty = false;
    // TODO check if empty
    if(this->empty()){

        //const auto hasShape = 
        MULTI_ARRAY_CHECK(hasShape, 
            "Cannot assign Shapeless expression to empty array");

        const auto eShape = e.makeShape();

        wasEmpty = true;
        // make new array
        shape_ = eShape;
        strides_ = detail_multi_array::cOrderStrides(shape_);
        size_ = detail_multi_array::shapeSize(shape_);
        data_ = new T[size_];
        smart_handle_ = SharedHandleType(data_, typename SharedHandleType::AllocNewTag());
    }
    const auto isOverlapping =  !wasEmpty && e.overlaps(*this);
    if(isOverlapping){
        // alloc a new array (TODO think about axis order)
        SmartMultiArray<T,DIM,IS_CONST> tmp(this->shape());
        tmp.assignFromNonOverlappingExpression(e);
        // assign tmp to this
        *this = tmp;
    }
    else{
        this->assignFromNonOverlappingExpression(e);
    }


    return *this;
}

template<class T, std::size_t DIM, bool IS_CONST>
template<class E, class U>
inline void 
SmartMultiArray<T, DIM, IS_CONST>::assignFromNonOverlappingExpression ( 
    const ViewExpression<DIM, E, U> & e
){

    typedef E ExpressionType;
    typedef typename ExpressionType::ViewExpressionMetaType ViewExpressionMetaType;
    typedef typename ViewExpressionMetaType::NeedsCoordinateType NeedsCoordinateType;
    const static bool needsCoordinate = NeedsCoordinateType::value;
    typedef typename ExpressionType::ExpressionPseudoIterator ExpressionPseudoIterator;

    // check the expression
    // is homogeneous wrt the strides
    if(e.matchingStrides()){
        // now we can check 
        // if the array is also
        // matching the expressions strides
        if(e.matchingStrides(this->strides())){

            // optimal operation in case
            // of dense array
            if(this->contiguous() && !needsCoordinate){
                //the array is dense this means withe can use the 
                // ``unsafeAccess`` access  of the expression
                // which is just linear memory access
                for(uint64_t i=0; i<this->size(); ++i){
                    this->unsafeAccess(i) = e.unsafeAccess(i); 
                }
                return;
            }
            else{
                // we can loop over all memory
                // offsets.
                // since the expression and
                // array have the same strides the offset
                // is the same for e and the array
                    
                // currently we assume c order

                if(!needsCoordinate){
                    forEachOffset(shape_, strides_,

                        [&](const uint64_t offset){
                            this->unsafeAccess(offset) = e.unsafeAccess(offset); 
                        }

                    );
                }
                else{
                    forEachOffsetAndCoordinate(shape_, strides_,

                        [&](
                            const uint64_t offset,
                            const Coordinate<DIM> & coordinate
                        ){
                            this->unsafeAccess(offset) = e.unsafeAccess(offset, coordinate); 
                        }

                    );
                }
                return;
            }
        }
        // fall trough
    }
    
    // fall trough

    // TODO there
    // is some speedup possible
    // if the array is dense

    ExpressionPseudoIterator eIter(e);

    // general case
    // c-order 
    uint64_t thisOffset = 0;
    Coordinate<DIM> coordinate(0);

    for(;;){
        data_[thisOffset] = *eIter;
        for(int j=DIM-1; j>=0; --j) {
            if(coordinate[j]+1 == shape_[j]) {
                if(j == 0) {
                    return;
                }
                else {
                    thisOffset -= coordinate[j] * strides_[j];
                    eIter.resetCoordinate(j);
                    coordinate[j] = 0;
                }
            }
            else {
                thisOffset += strides_[j];
                eIter.incrementCoordinate(j);
                ++coordinate[j];
                break;
            }      
        }
    }
    return;
    
}


template<class T, std::size_t DIM, bool IS_CONST>
inline void 
SmartMultiArray<T, DIM, IS_CONST>::assign(
    const ShapeType & shape
){

    shape_ = shape;
    strides_  = detail_multi_array::cOrderStrides(shape);
    size_ = detail_multi_array::shapeSize(shape);
    data_ = new T[size_];
    smart_handle_ = SharedHandleType(data_, typename SharedHandleType::AllocNewTag());
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
inline auto 
SmartMultiArray<T, DIM, IS_CONST>::copy() const ->SmartMultiArray{
    // todo think about order
    SmartMultiArray<T, DIM, IS_CONST> ret(shape_);
    ret = *this;
    MULTI_ARRAY_CHECK(false, "UNTESTED");
    return ret;
}

template<class T, std::size_t DIM, bool IS_CONST>
inline auto 
SmartMultiArray<T, DIM, IS_CONST>::view() const -> SmartMultiArray {
    SmartMultiArray ret;
    ret.shape_ = shape_;
    ret.strides_ = strides_;
    ret.data_ = data_;
    ret.smart_handle_ = smart_handle_;
    return ret;
}


template<class T, std::size_t DIM, bool IS_CONST>
inline auto 
SmartMultiArray<T, DIM, IS_CONST>::constView() const -> typename RebindIsConst<true>::type {

    SmartMultiArray<T, DIM, true> ret;
    ret.shape_ = shape_;
    ret.strides_ = strides_;
    ret.data_ = data_;
    ret.smart_handle_ = smart_handle_;
    MULTI_ARRAY_CHECK(false, "UNTESTED");
    return ret;
}

template<class T, std::size_t DIM, bool IS_CONST>
template<class _T, bool _IS_CONST>
inline bool 
SmartMultiArray<T, DIM, IS_CONST>::equal(
    const SmartMultiArray<_T, DIM, _IS_CONST> & other
)const{

    // TODO reimplement me

    typedef typename meta::PromoteType<T,_T>::type PromotedT;
    if(shape_ == other.shape_){


        uint8_t res = 1;
        forEachOffset(shape_, strides_, other.strides_,
            [&](
                const uint64_t offsetA,
                const uint64_t offsetB
            ){
                const auto & val = data_[offsetA];
                const auto & otherVal = other.data_[offsetB];
                const auto valA = static_cast<PromotedT>(val);
                const auto valB = static_cast<PromotedT>(otherVal);

                if(!detail_multi_array::areAlmostEqual(valA, valB)){
                    res = 0;
                }
            }
        );
        return bool(res);
    }
    else{
        return false;
    }
}

template<class T, std::size_t DIM, bool IS_CONST>
template<class ACCUMULATOR>
inline void 
SmartMultiArray<T, DIM, IS_CONST>::accumulate(
    ACCUMULATOR & accumulator
)const{
    this->forEach(
        [&](
            const_reference val
        ){
            accumulator(val);
        }
    );
}

#define __MULTI_ARRAY_ACC_IMPL__(FNAME, ACC_NAME)\
template<class T, std::size_t DIM, bool IS_CONST>\
inline typename ACC_NAME<T>::type  \
SmartMultiArray<T, DIM, IS_CONST>::FNAME() const{\
    ACC_NAME<T> acc;\
    this->accumulate(acc);\
    return acc.result();\
}
__MULTI_ARRAY_ACC_IMPL__(sum,       accumulators::Sum);
__MULTI_ARRAY_ACC_IMPL__(mean,      accumulators::Mean);
__MULTI_ARRAY_ACC_IMPL__(min,       accumulators::Min);
__MULTI_ARRAY_ACC_IMPL__(max,       accumulators::Max);
__MULTI_ARRAY_ACC_IMPL__(minMax,    accumulators::MinMax);
#undef __MULTI_ARRAY_ACC_IMPL__





template<class T, std::size_t DIM, bool IS_CONST>
template<class ACCUMULATOR>
inline void 
SmartMultiArray<T, DIM, IS_CONST>::accumulate(
    ACCUMULATOR & accumulator,
    const AccWithCoordEnum & 
)const{
    forEachOffsetAndCoordinate(shape_, strides_,
        [&](
            const uint64_t offset,
            const Coordinate<DIM> & coordinate
        ){
            accumulator(this->unsafeAccess(offset), coordinate);
        }
    );
}


template<class T, std::size_t DIM, bool IS_CONST>
template<class F>
inline void 
SmartMultiArray<T, DIM, IS_CONST>::forEach(
    F && f
)const{
    return this->forEach(Order::C_ORDER, f);
}
template<class T, std::size_t DIM, bool IS_CONST>
template<class F>
inline void 
SmartMultiArray<T, DIM, IS_CONST>::forEach(
    F && f
){
    return this->forEach(Order::C_ORDER, f);
}





template<class T, std::size_t DIM, bool IS_CONST>
template<class F>
inline void 
SmartMultiArray<T, DIM, IS_CONST>::forEach(
    const Order & order,
    F && f
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
    const Order & order,
    F && f
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
void 
SmartMultiArray<T, DIM, IS_CONST>::transposeSelf(
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
        this->forEach(order, [&](const_reference value){
            retPtr[c] = value;
            ++c;
        });

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
inline SmartMultiArray<T, DIM-1, IS_CONST> 
SmartMultiArray<T, DIM, IS_CONST>::bind(
    const uint64_t axis, 
    const uint64_t value
){
    static_assert(DIM >= 1, "Can Only bind arrays with DIM >= 1");
    typedef SmartMultiArray<T, DIM-1,IS_CONST> ResType;
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
inline SmartMultiArray<T, DIM-1, true> 
SmartMultiArray<T, DIM, IS_CONST>::bind(
    const uint64_t axis, 
    const uint64_t value
)const{
    static_assert(DIM >= 1, "Can Only bind arrays with DIM >= 1");
    typedef SmartMultiArray<T, DIM-1, true> ResType;
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
template<class FUNCTOR>
inline void 
SmartMultiArray<T, DIM, IS_CONST>::forEach1DArrayAlongAxis(
    const uint16_t axis, FUNCTOR && functor
){
    auto startArray = this->bind(axis,0);
    auto startArrayData = startArray.data_; 

    const auto axisShape = shape_[axis];
    const auto axisStrides = strides_[axis];

    SmartMultiArray<T,1, IS_CONST> array1d;
    array1d.shape_[0] = axisShape;
    array1d.strides_[0] = axisStrides;
    array1d.size_ = axisShape;
    array1d.smart_handle_ = this->smart_handle_;

    forEachOffset(startArray.shape(), startArray.strides(), [&](const uint64_t offset){
        const auto startPtr = startArrayData[offset];
        array1d.data_ = startPtr;
        functor(array1d);
    });
}
template<class T, std::size_t DIM, bool IS_CONST>
template<class FUNCTOR>
inline void 
SmartMultiArray<T, DIM, IS_CONST>::forEach1DArrayAlongAxis(
    const uint16_t axis, FUNCTOR && functor
)const{
    auto startArray = this->bind(axis,0);
    auto startArrayData = startArray.data_; 

    const auto axisShape = shape_[axis];
    const auto axisStrides = strides_[axis];

    SmartMultiArray<T,1, true> array1d;
    array1d.shape_[0] = axisShape;
    array1d.strides_[0] = axisStrides;
    array1d.size_ = axisShape;
    array1d.smart_handle_ = this->smart_handle_;

    forEachOffset(startArray.shape(), startArray.strides(), [&](const uint64_t offset){
        const auto startPtr = startArrayData[offset];
        array1d.data_ = startPtr;
        functor(array1d);
    });
}


template<class T, std::size_t DIM, bool IS_CONST>
inline auto
SmartMultiArray<T, DIM, IS_CONST>::unsafeAccess(
    const uint64_t index
)-> reference{
    return data_[index];
}
template<class T, std::size_t DIM, bool IS_CONST>
inline auto
SmartMultiArray<T, DIM, IS_CONST>::unsafeAccess(
    const uint64_t index
)const -> const_reference{
    return data_[index];
}

template<class T, std::size_t DIM, bool IS_CONST>
inline auto
SmartMultiArray<T, DIM, IS_CONST>::unsafeAccess(
    const uint64_t index,
    const Coordinate<DIM>  &
)-> reference{
    return data_[index];
}
template<class T, std::size_t DIM, bool IS_CONST>
inline auto
SmartMultiArray<T, DIM, IS_CONST>::unsafeAccess(
    const uint64_t index,
    const Coordinate<DIM>  & 
)const -> const_reference{
    return data_[index];
}




template<class T, std::size_t DIM, bool IS_CONST>
inline bool 
SmartMultiArray<T, DIM, IS_CONST>::matchingStrides()const{
    return true;
}

template<class T, std::size_t DIM, bool IS_CONST>
inline bool 
SmartMultiArray<T, DIM, IS_CONST>::matchingStrides(
    const Strides<DIM> & s
)const{
    return strides_.relaxedEqual(s);
}

template<class T, std::size_t DIM, bool IS_CONST>
inline bool 
SmartMultiArray<T, DIM, IS_CONST>::hasShape() const{ 
    return true;
}

template<class T, std::size_t DIM, bool IS_CONST>
inline bool 
SmartMultiArray<T, DIM, IS_CONST>::hasStrides() const{ 
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
