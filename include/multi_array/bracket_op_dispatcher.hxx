#pragma once

#include <typeinfo>  
#include <iostream>
#include <memory>
#include <type_traits>
#include <utility>


#include "meta.hxx"
#include "misc.hxx"
#include "indexing_types.hxx"



namespace multi_array{




///\cond
namespace detail_multi_array{


template<class MARRAY_IN, class MARRAY_OUT, size_t IN_AXIS_INDEX, size_t OUT_AXIS_INDEX,  class ARG, bool IS_INTEGRAL>
struct ProcessArg;


// integral arg
template<class MARRAY_IN, class MARRAY_OUT, size_t IN_AXIS_INDEX, size_t OUT_AXIS_INDEX, class ARG>
struct ProcessArg<MARRAY_IN, MARRAY_OUT, IN_AXIS_INDEX, OUT_AXIS_INDEX, ARG, true>{

    template<class _ARG>
    static void op(
        const MARRAY_IN & marrayIn,
        MARRAY_OUT & marrayOut,
        _ARG && arg
    ){
        marrayOut.data_ += marrayIn.strides()[IN_AXIS_INDEX] * arg;
    }
};

// range argument
template<class MARRAY_IN, class MARRAY_OUT, size_t IN_AXIS_INDEX, size_t OUT_AXIS_INDEX>
struct ProcessArg<MARRAY_IN, MARRAY_OUT, IN_AXIS_INDEX, OUT_AXIS_INDEX, Range, false>{
    //template<class RANGE>
    static void op(
        const MARRAY_IN & marrayIn,
        MARRAY_OUT & marrayOut,
         Range  range
    ){

        marrayOut.data_ += marrayIn.strides()[IN_AXIS_INDEX] * range.begin();
        marrayOut.shape_[OUT_AXIS_INDEX] = range.end() - range.begin();
        marrayOut.strides_[OUT_AXIS_INDEX] = marrayIn.strides_[IN_AXIS_INDEX];

    }

};


// strided range argument
template<class MARRAY_IN, class MARRAY_OUT, size_t IN_AXIS_INDEX, size_t OUT_AXIS_INDEX>
struct ProcessArg<MARRAY_IN, MARRAY_OUT, IN_AXIS_INDEX, OUT_AXIS_INDEX, StridedRange, false>{
    static void op(
        const MARRAY_IN & marrayIn,
        MARRAY_OUT & marrayOut,
        const StridedRange & range
    ){

        // CHECK RANGE?!?!?!
        // TODO
        marrayOut.data_ += marrayIn.strides()[IN_AXIS_INDEX] * range.begin();

        const auto step = range.step();
        const auto nElements = range.end() - range.begin();
        const auto tmp = nElements / step;
        const auto newShape = tmp +  ((step*tmp == nElements ) ? 0 : 1);

        marrayOut.shape_[OUT_AXIS_INDEX] = newShape;
        marrayOut.strides_[OUT_AXIS_INDEX] = marrayIn.strides_[IN_AXIS_INDEX]*step;
    }

};
// all
template<class MARRAY_IN, class MARRAY_OUT, size_t IN_AXIS_INDEX, size_t OUT_AXIS_INDEX>
struct ProcessArg<MARRAY_IN, MARRAY_OUT, IN_AXIS_INDEX, OUT_AXIS_INDEX, All, false>{
    static void op(
        const MARRAY_IN & marrayIn,
        MARRAY_OUT & marrayOut,
        const All & all
    ){
        marrayOut.shape_[OUT_AXIS_INDEX] = marrayIn.shape_[IN_AXIS_INDEX];
        marrayOut.strides_[OUT_AXIS_INDEX] = marrayIn.strides_[IN_AXIS_INDEX];
    }

};

// newaxis
template<class MARRAY_IN, class MARRAY_OUT, size_t IN_AXIS_INDEX, size_t OUT_AXIS_INDEX>
struct ProcessArg<MARRAY_IN, MARRAY_OUT, IN_AXIS_INDEX, OUT_AXIS_INDEX, NewAxis, false>{
    static void op(
        const MARRAY_IN & marrayIn,
        MARRAY_OUT & marrayOut,
        const NewAxis & range
    ){

        marrayOut.shape_[OUT_AXIS_INDEX] = 1;
        marrayOut.strides_[OUT_AXIS_INDEX] = 0;
    }

};


template<
    class MARRAY_IN, 
    class MARRAY_OUT, 
    size_t IN_AXIS_INDEX,
    size_t OUT_AXIS_INDEX,
    class ARG,
    class ... ARGS
>
struct MixedArgsImpl{
    static void op(
        const MARRAY_IN &   marrayIn,
        MARRAY_OUT &        marrayOut,
        ARG &&              arg,
        ARGS && ...         args
    ){
        
        // check if the argument is integral
        typedef multi_array::meta::IsIntegralOrIntegralRefeference<ARG> IsIntgralArgType;

        typedef multi_array::meta::IsSameWithoutConstAndReferece<ARG, NewAxis> IsNewAaxisArgType;
        // handle the current argument

        typedef typename std::remove_reference<ARG>::type NoRefArgType;
        typedef typename std::remove_cv<NoRefArgType>::type NoConstNoRefArgType;

        ProcessArg<
            MARRAY_IN,
            MARRAY_OUT,
            IN_AXIS_INDEX,
            OUT_AXIS_INDEX,
            NoConstNoRefArgType, 
            IsIntgralArgType::value
        >::op(marrayIn, marrayOut, std::forward<ARG>(arg));
        

        MixedArgsImpl<
            MARRAY_IN, MARRAY_OUT, 
            // if the argument is a newaxis argument
            // we do  not increment the in axis index
            // since the current in axis has not yet been handled
            IsNewAaxisArgType::value ?  IN_AXIS_INDEX : IN_AXIS_INDEX + 1, 
            // iff arg is NOT integer we increment
            IsIntgralArgType::value ?  OUT_AXIS_INDEX : OUT_AXIS_INDEX + 1, 
            ARGS ... 
        >::op(marrayIn, marrayOut, std::forward<ARGS>(args) ...);
    
    } 
};


template<
    class MARRAY_IN, 
    class MARRAY_OUT, 
    size_t IN_AXIS_INDEX,
    size_t OUT_AXIS_INDEX,
    class ARG
>
struct MixedArgsImpl<
    MARRAY_IN, 
    MARRAY_OUT,
    IN_AXIS_INDEX,
    OUT_AXIS_INDEX,
    ARG 
>{
    static void op(
        const MARRAY_IN &   marrayIn,
        MARRAY_OUT &        marrayOut,
        ARG &&              arg
    ){
        // check if the argument is integral
        typedef  multi_array::meta::IsIntegralOrIntegralRefeference<ARG> IsIntgralArgType;

        typedef typename std::remove_reference<ARG>::type NoRefArgType;
        typedef typename std::remove_cv<NoRefArgType>::type NoConstNoRefArgType;
        // handle the current argument
        ProcessArg<
            MARRAY_IN,
            MARRAY_OUT,
            IN_AXIS_INDEX,
            OUT_AXIS_INDEX,
            NoConstNoRefArgType, 
            IsIntgralArgType::value
        >::op(marrayIn, marrayOut, std::forward<ARG>(arg));    
    } 
};




/**
 * @brief Dispatch the implementation of SmartMultiArray::operator()
 * @details SmartMultiArray::operator() is implemented with variadic
 * template. Depending on the actual types the implementation
 * will differ drastically.
 * This struct is responsible to dispatch the SmartMultiArray::operator()
 * to the appropriate functions.    
 * 
 * @tparam MARRAY Always a SmartMultiArray 
 * @tparam CONST_INSTANCE If SmartMultiArray instance is const this is true.
 * @tparam class ... ARGS The argument types passed to SmartMultiArray::operator()
 */
template<class MARRAY, bool CONST_INSTANCE, class ... ARGS>
struct BracketOpDispatcher{

    typedef MARRAY MarrayType;
    typedef typename std::conditional<CONST_INSTANCE,
        const MarrayType &,
        MarrayType &
    >::type MarrayReferenceType;



    typedef typename MarrayType::DimensionType DimensionType;

    typedef std::tuple<ARGS ...> ArgTupleType;
    const static size_t ArgSize = std::tuple_size<ArgTupleType>::value;

    


    // information needed to dispatch...
    typedef typename meta::CountOccurrencesRelaxed<NewAxis,     ARGS ... >        NumNewAxisArgumentsType;
    typedef typename meta::CountIntegralArgumetns<ARGS ... >                      NumIntegralArgumentsType;
    typedef meta::Bool<NumIntegralArgumentsType::value==ArgSize>                  AllIntegralArgumentsType;

    static_assert(ArgSize == DimensionType::value  + NumNewAxisArgumentsType::value || ArgSize == 1, "wrong number of arguments in SmartMultiArray::operator()");

    typedef meta::SizeT<
        NumNewAxisArgumentsType::value + 
        DimensionType::value - 
        NumIntegralArgumentsType::value 
    >   ResultDimensionType;




    // the impls
    struct OnlyScalarImplType{

        typedef typename std::conditional<
            CONST_INSTANCE, 
            typename MarrayType::const_reference,
            typename MarrayType::reference
        >::type type;

        static type op(MarrayReferenceType marray, ARGS && ... args){
            const auto i = detail_multi_array::dot(marray.strides(), args ...);
            return (marray.data())[i];
        }
    };

    struct MixedArgsImplType{

        typedef typename MarrayType:: template RebindDimension<ResultDimensionType::value>:: type type;

        static type op(MarrayReferenceType marray, ARGS && ... args){


            // the result array
            type resultArray;

            // might be modified later on
            resultArray.data_ = marray.data_;
            resultArray.smart_handle_ = marray.smart_handle_;
            MixedArgsImpl<MarrayType, type, 0,0,ARGS ...>::op(marray, resultArray, std::forward<ARGS>(args) ... );
            resultArray.size_ = detail_multi_array::shapeSize(resultArray.shape());

            return resultArray;
        }
    };

    struct ImplC{
        struct type{
        };
        static type op(MarrayReferenceType marray, ARGS && ... args){

        }
    };



    // The dispatcher switch
    typedef typename meta::Switch<

        // `ordinary` bracket operator called with only integers
        meta::Bool<DimensionType::value==ArgSize && AllIntegralArgumentsType::value>,  OnlyScalarImplType,

        // single scalar as argument
        meta::Bool<ArgSize==1 && AllIntegralArgumentsType::value>,  ImplC,

        // mixed arguments
        // \TODO refine conditions
        meta::Bool<true>,  MixedArgsImplType,


        // fallback (error)
        meta::TrueCase, ImplC

    >::type type;



        

    
    //typedef typename ImplType::type type;
};


} ///\endcond


}
