#pragma once


#include <iostream>
#include <memory>
#include <type_traits>
#include <utility>


#include "meta.hxx"
#include "shared_handle.hxx"
#include "misc.hxx"
#include "indexing_types.hxx"
#include "bracket_op_dispatcher.hxx"


namespace multi_array{



template<class T, size_t DIM, class CHILD>
class MultiArrayBase{
};




template<class T, size_t DIM, bool IS_CONST = false>
class SmartMultiArray
: public MultiArrayBase<T, DIM, SmartMultiArray<T, DIM >>
{

    template<class MARRAY, bool CONST_INSTANCE, class ... Args>
    friend class detail_multi_array::BracketOpDispatcher;


    template<class MARRAY_IN, class MARRAY_OUT, size_t IN_AXIS_INDEX, size_t OUT_AXIS_INDEX,  class ARG, bool IS_INTEGRAL>
    friend struct detail_multi_array::ProcessArg;

    template<class _T, size_t _DIM, bool _IS_CONST>
    friend class SmartMultiArray;

    // private typedefs
    typedef SmartMultiArray<T, DIM> SelfType;
    typedef detail_multi_array::SharedHandle SharedHandleType;
    

public:

    // rebinding
    template<class REBIND_T, size_t REBIND_DIM, bool REBIND_IS_CONST>
    struct Rebind{
        typedef SmartMultiArray<REBIND_T, REBIND_DIM, REBIND_IS_CONST> type;
    };

    template<class REBIND_T>
    struct RebindType{
        typedef SmartMultiArray<REBIND_T, DIM, IS_CONST> type;
    };

    template<size_t REBIND_DIM>
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
    SmartMultiArray(const  SmartMultiArray & other);



    SmartMultiArray & operator= ( const SmartMultiArray & );

    

    constexpr std::size_t dimension() const ;
    const StridesType & strides()const;
    const ShapeType & shape()const;
    int64_t  strides(const std::size_t a)const;
    int64_t  shape(const std::size_t a)const;
    uint64_t size()const;
    bool empty()const;
    const SharedHandleType & smartHandle()const;
    bool isDense() const;
    T * data();
    ConstDataPtrType data()const;
    

    template<size_t SHAPE_DIM>
    SmartMultiArray<T, SHAPE_DIM, IS_CONST> reshapeView(const Shape<SHAPE_DIM> & shape)const;

    #if 0
    template<size_t SHAPE_DIM>
    void reshape(const Shape<SHAPE_DIM> & shape);
    void resize(const Shape<DIM> & shape)const;
    #endif

    reference front();
    reference back();
    const_reference front()const;
    const_reference back()const;

    template<class ... Args>
    typename detail_multi_array::BracketOpDispatcher<SelfType, false, Args ... >::type::type
    operator()(Args && ... args);

    template<class ... Args>
    typename detail_multi_array::BracketOpDispatcher<SelfType, true, Args ... >::type::type
    operator()(Args && ... args)const;

    inline SmartMultiArray<T, DIM-1> 
    bind(const uint64_t axis, const uint64_t value);



private:
    uint64_t lastValidMemOffset()const;




    
    ShapeType shape_;
    StridesType strides_;
    uint64_t size_;
    T * data_;
    // this is encapsulating the smart pointer
    // responsible for keeping track
    // of the ownership
    SharedHandleType smart_handle_;


};






    template<class T, size_t DIM, bool IS_CONST>
    SmartMultiArray<T, DIM, IS_CONST>::SmartMultiArray()

    :   shape_(),
        strides_(),
        size_(0),
        data_(nullptr),
        smart_handle_(){ 

    }


    template<class T, size_t DIM, bool IS_CONST>
    inline 
    SmartMultiArray<T, DIM, IS_CONST>::SmartMultiArray(
        const ShapeType & shape
    )
    :   shape_(shape),
        strides_(detail_multi_array::cOrderStrides(shape)),
        size_(detail_multi_array::shapeSize(shape)),
        data_(new T[size_]),
        smart_handle_(data_, typename SharedHandleType::AllocNewTag()){ 

    }


    template<class T, size_t DIM, bool IS_CONST>
    inline 
    SmartMultiArray<T, DIM, IS_CONST>::SmartMultiArray(
        const  SmartMultiArray & other
    )
    :   shape_(other.shape_),
        strides_(other.strides_),
        size_(other.size_),
        data_(other.data_),
        smart_handle_(other.smart_handle_,  typename SharedHandleType::CopyHandleTag())
    {

    }


    template<class T, size_t DIM, bool IS_CONST>
    inline auto 
    SmartMultiArray<T, DIM, IS_CONST>::operator= ( 
        const SmartMultiArray & other
    ) -> SmartMultiArray &{
        if(this!=&other){

        }
        return *this;
    }




    template<class T, size_t DIM, bool IS_CONST>
    inline constexpr std::size_t 
    SmartMultiArray<T, DIM, IS_CONST>::dimension() const { 
        return DimensionType::value; 
    }


    template<class T, size_t DIM, bool IS_CONST>
    inline auto  
    SmartMultiArray<T, DIM, IS_CONST>::strides()const -> const StridesType &  {
        return strides_;
    }


    template<class T, size_t DIM, bool IS_CONST>
    inline auto 
    SmartMultiArray<T, DIM, IS_CONST>::shape()const -> const ShapeType &{
        return shape_;
    }


    template<class T, size_t DIM, bool IS_CONST>
    inline int64_t 
    SmartMultiArray<T, DIM, IS_CONST>::strides(
        const std::size_t a
    )const{
        return strides_[a];
    }


    template<class T, size_t DIM, bool IS_CONST>
    inline int64_t 
    SmartMultiArray<T, DIM, IS_CONST>::shape(
        const std::size_t a
    )const{
        return shape_[a];
    }


    template<class T, size_t DIM, bool IS_CONST>
    inline uint64_t 
    SmartMultiArray<T, DIM, IS_CONST>::size()const{
        return size_;
    }

    template<class T, size_t DIM, bool IS_CONST>
    inline auto 
    SmartMultiArray<T, DIM, IS_CONST>::smartHandle()const -> const SharedHandleType &{
        return smart_handle_;
    }

    template<class T, size_t DIM, bool IS_CONST>
    inline bool
    SmartMultiArray<T, DIM, IS_CONST>::isDense()const {
        return this->lastValidMemOffset() + 1 == this->size();
    }


    template<class T, size_t DIM, bool IS_CONST>
    inline T * 
    SmartMultiArray<T, DIM, IS_CONST>::data() {
        return data_;
    }

    template<class T, size_t DIM, bool IS_CONST>
    inline auto 
    SmartMultiArray<T, DIM, IS_CONST>::data()const  -> ConstDataPtrType {
        return data_;
    }



    template<class T, size_t DIM, bool IS_CONST>
    inline bool 
    SmartMultiArray<T, DIM, IS_CONST>::empty()const{
        return data_ == nullptr;
    }

  

    template<class T, size_t DIM, bool IS_CONST>
    inline auto
    SmartMultiArray<T, DIM, IS_CONST>::front() ->reference {
        return *data_;
    }

    template<class T, size_t DIM, bool IS_CONST>
    inline auto
    SmartMultiArray<T, DIM, IS_CONST>::back() -> reference {
        return data_[this->lastValidMemOffset()];
    }

    template<class T, size_t DIM, bool IS_CONST>
    inline auto 
    SmartMultiArray<T, DIM, IS_CONST>::front()const -> const_reference {
        return *data_;
    }

    template<class T, size_t DIM, bool IS_CONST>
    inline auto 
    SmartMultiArray<T, DIM, IS_CONST>::back()const -> const_reference {
        return data_[this->lastValidMemOffset()];
    }


    #if 0 
    template<class T, size_t DIM, bool IS_CONST>
    inline void
    SmartMultiArray<T, DIM, IS_CONST>::resize(
        const Shape<DIM> & shape
    ) 
    {
        const auto shapeSize = detail_multi_array::shapeSize(shape);
        if(shapeSize == size){
            if(this->isDense()){
                this->reshapeView()
            }
        }
        else{

        }
    }

    template<class T, size_t DIM, bool IS_CONST>
    template<size_t SHAPE_DIM>
    inline auto 
    SmartMultiArray<T, DIM, IS_CONST>::reshapeView(
        const Shape<SHAPE_DIM> & shape
    ) const -> SmartMultiArray<T, SHAPE_DIM, IS_CONST> {

        const auto nNeg = shape.countNegativeEntries();

    }
    #endif

    template<class T, size_t DIM, bool IS_CONST>
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

    template<class T, size_t DIM, bool IS_CONST>
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


    template<class T, size_t DIM, bool IS_CONST>
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

    template<class T, size_t DIM, bool IS_CONST>
    inline uint64_t 
    SmartMultiArray<T, DIM, IS_CONST>::lastValidMemOffset()const{
        uint64_t offset = 0;
        for(auto d=0; d<DIM; ++d)
            offset += strides_[d]* (shape_[d] - 1);
        return offset;
    }






}
