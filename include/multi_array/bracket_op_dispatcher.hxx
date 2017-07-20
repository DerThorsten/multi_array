#pragma once

#include <typeinfo>  
#include <iostream>
#include <memory>
#include <type_traits>
#include <utility>


#include "multi_array/meta.hxx"
#include "multi_array/misc.hxx"




namespace multi_array{




///\cond
namespace detail_multi_array{


template<class T>
struct NewAxisCount
: public meta::SizeT<0>{
};

template<std::size_t N>
struct NewAxisCount<NewAxis<N>>
: public meta::SizeT<N>{
};


template<>
struct NewAxisCount<NoneTag>
: public meta::SizeT<1>{
};


template<class ARG,  class ... ARGS>
struct CountTotalNeededNewAxisHelper
{
    typedef typename meta::RemoveCvAndReference<ARG>::type ProcessedArg;
    static const std::size_t value = NewAxisCount<ProcessedArg>::value + CountTotalNeededNewAxisHelper<ARGS ... >::value;
};

template<class ARG>
struct CountTotalNeededNewAxisHelper<ARG>{
    typedef typename meta::RemoveCvAndReference<ARG>::type ProcessedArg;
    static const std::size_t value = NewAxisCount<ProcessedArg>::value;
};

template<class ... ARGS>
struct CountTotalNeededNewAxis
: public meta::SizeT<
    CountTotalNeededNewAxisHelper<ARGS ... >::value
>
{
};



template<class T>
struct IsNewAxisLike
: public meta::SizeT<0>{
};

template<std::size_t N>
struct IsNewAxisLike<NewAxis<N>>
: public meta::SizeT<1>{
};

template<>
struct IsNewAxisLike<NoneTag>
: public meta::SizeT<1>{
};


template<class ARG,  class ... ARGS>
struct CountNewAxisLikeArgsHelper
{
    typedef typename meta::RemoveCvAndReference<ARG>::type ProcessedArg;
    static const std::size_t value = IsNewAxisLike<ProcessedArg>::value + CountNewAxisLikeArgsHelper<ARGS ... >::value;
};

template<class ARG>
struct CountNewAxisLikeArgsHelper<ARG>{
    typedef typename meta::RemoveCvAndReference<ARG>::type ProcessedArg;
    static const std::size_t value = IsNewAxisLike<ProcessedArg>::value;
};

template<class ... ARGS>
struct CountNewAxisLikeArgs
: public meta::SizeT<
    CountNewAxisLikeArgsHelper<ARGS ... >::value
>
{
};












template<class MARRAY_IN, class MARRAY_OUT, size_t IN_AXIS_INDEX, size_t OUT_AXIS_INDEX, int N_ARG_TO_COVER_WITH_ELLIPSIS, class ARG, bool IS_INTEGRAL>
struct ProcessArg;


// integral arg
template<class MARRAY_IN, class MARRAY_OUT, size_t IN_AXIS_INDEX, size_t OUT_AXIS_INDEX, int N_ARG_TO_COVER_WITH_ELLIPSIS, class ARG>
struct ProcessArg<MARRAY_IN, MARRAY_OUT, IN_AXIS_INDEX, OUT_AXIS_INDEX, N_ARG_TO_COVER_WITH_ELLIPSIS, ARG, true>{

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
template<class MARRAY_IN, class MARRAY_OUT, size_t IN_AXIS_INDEX, size_t OUT_AXIS_INDEX, int N_ARG_TO_COVER_WITH_ELLIPSIS>
struct ProcessArg<MARRAY_IN, MARRAY_OUT, IN_AXIS_INDEX, OUT_AXIS_INDEX, N_ARG_TO_COVER_WITH_ELLIPSIS, Range, false>{
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
template<class MARRAY_IN, class MARRAY_OUT, size_t IN_AXIS_INDEX, size_t OUT_AXIS_INDEX,int N_ARG_TO_COVER_WITH_ELLIPSIS>
struct ProcessArg<MARRAY_IN, MARRAY_OUT, IN_AXIS_INDEX, OUT_AXIS_INDEX, N_ARG_TO_COVER_WITH_ELLIPSIS, StridedRange, false>{
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
template<class MARRAY_IN, class MARRAY_OUT, size_t IN_AXIS_INDEX, size_t OUT_AXIS_INDEX,int N_ARG_TO_COVER_WITH_ELLIPSIS>
struct ProcessArg<MARRAY_IN, MARRAY_OUT, IN_AXIS_INDEX, OUT_AXIS_INDEX,N_ARG_TO_COVER_WITH_ELLIPSIS, AllTag, false>{
    static void op(
        const MARRAY_IN & marrayIn,
        MARRAY_OUT & marrayOut,
        const AllTag & 
    ){
        marrayOut.shape_[OUT_AXIS_INDEX] = marrayIn.shape_[IN_AXIS_INDEX];
        marrayOut.strides_[OUT_AXIS_INDEX] = marrayIn.strides_[IN_AXIS_INDEX];
    }

};

// ellipsis
template<class MARRAY_IN, class MARRAY_OUT, size_t IN_AXIS_INDEX, size_t OUT_AXIS_INDEX,int N_ARG_TO_COVER_WITH_ELLIPSIS>
struct ProcessArg<MARRAY_IN, MARRAY_OUT, IN_AXIS_INDEX, OUT_AXIS_INDEX,N_ARG_TO_COVER_WITH_ELLIPSIS, EllipsisTag, false>{
    static void op(
        const MARRAY_IN & marrayIn,
        MARRAY_OUT & marrayOut,
        const EllipsisTag & 
    ){
        for(auto i=0; i<N_ARG_TO_COVER_WITH_ELLIPSIS; ++i){
            marrayOut.shape_[OUT_AXIS_INDEX+i] = marrayIn.shape_[IN_AXIS_INDEX+i];
            marrayOut.strides_[OUT_AXIS_INDEX+i] = marrayIn.strides_[IN_AXIS_INDEX+i];
        }
    }
};




// NoneTag
template<class MARRAY_IN, class MARRAY_OUT, size_t IN_AXIS_INDEX, size_t OUT_AXIS_INDEX,int N_ARG_TO_COVER_WITH_ELLIPSIS>
struct ProcessArg<MARRAY_IN, MARRAY_OUT, IN_AXIS_INDEX, OUT_AXIS_INDEX,N_ARG_TO_COVER_WITH_ELLIPSIS, NoneTag, false>{
    static void op(
        const MARRAY_IN & marrayIn,
        MARRAY_OUT & marrayOut,
        const NoneTag & 
    ){
        marrayOut.shape_[OUT_AXIS_INDEX] = 1;
        marrayOut.strides_[OUT_AXIS_INDEX] = 0;
    }
};

// newaxis
template<class MARRAY_IN, class MARRAY_OUT, size_t IN_AXIS_INDEX, size_t OUT_AXIS_INDEX,int N_ARG_TO_COVER_WITH_ELLIPSIS>
struct ProcessArg<MARRAY_IN, MARRAY_OUT, IN_AXIS_INDEX, OUT_AXIS_INDEX,N_ARG_TO_COVER_WITH_ELLIPSIS, NewAxis<1>, false>{
    static void op(
        const MARRAY_IN & marrayIn,
        MARRAY_OUT & marrayOut,
        const NewAxis<1> & 
    ){
        marrayOut.shape_[OUT_AXIS_INDEX] = 1;
        marrayOut.strides_[OUT_AXIS_INDEX] = 0;
    }
};

// n-newaxis
template<class MARRAY_IN, class MARRAY_OUT, size_t IN_AXIS_INDEX, size_t OUT_AXIS_INDEX,int N_ARG_TO_COVER_WITH_ELLIPSIS, std::size_t N_NEW_AXIS>
struct ProcessArg<MARRAY_IN, MARRAY_OUT, IN_AXIS_INDEX, OUT_AXIS_INDEX,N_ARG_TO_COVER_WITH_ELLIPSIS, NewAxis<N_NEW_AXIS>, false>{
    static void op(
        const MARRAY_IN & marrayIn,
        MARRAY_OUT & marrayOut,
        const NewAxis<N_NEW_AXIS> &
    ){
        for(auto i=0; i<N_NEW_AXIS; ++i){
            marrayOut.shape_[OUT_AXIS_INDEX + i] = 1;
            marrayOut.strides_[OUT_AXIS_INDEX + i] = 0;
        }
    }
};


template<class ARG, bool IS_INTEGRAL>
struct InOutAxisIncrement{
    typedef meta::Int<1> InAxis;
    typedef meta::Int<1> OutAxis;
};
template<class ARG>
struct InOutAxisIncrement<ARG, true>{
    typedef meta::Int<1> InAxis;
    typedef meta::Int<0> OutAxis;
};
template<std::size_t N>
struct InOutAxisIncrement<NewAxis<N>, false>{
    typedef meta::Int<0> InAxis;
    typedef meta::Int<N> OutAxis;
};
template<>
struct InOutAxisIncrement<NoneTag, false>{
    typedef meta::Int<0> InAxis;
    typedef meta::Int<1> OutAxis;
};


template<
    class MARRAY_IN, 
    class MARRAY_OUT, 
    size_t IN_AXIS_INDEX,
    size_t OUT_AXIS_INDEX,
    int N_ARG_TO_COVER_WITH_ELLIPSIS,
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

        typedef multi_array::meta::IsSameWithoutConstAndReferece<ARG, NewAxis<1>> IsNewAxisArgType;
        typedef multi_array::meta::IsSameWithoutConstAndReferece<ARG, EllipsisTag> IsEllipsisArgType;
        // handle the current argument

        typedef typename std::remove_reference<ARG>::type NoRefArgType;
        typedef typename std::remove_cv<NoRefArgType>::type NoConstNoRefArgType;

        ProcessArg<
            MARRAY_IN,
            MARRAY_OUT,
            IN_AXIS_INDEX,
            OUT_AXIS_INDEX,
            N_ARG_TO_COVER_WITH_ELLIPSIS,
            NoConstNoRefArgType, 
            IsIntgralArgType::value
        >::op(marrayIn, marrayOut, std::forward<ARG>(arg));
        

        typedef InOutAxisIncrement<NoConstNoRefArgType, IsIntgralArgType::value> InOutAxisIncrementType;

        const static int IncrementIn = IsEllipsisArgType::value ? 
            N_ARG_TO_COVER_WITH_ELLIPSIS : InOutAxisIncrementType::InAxis::value;

        const static int IncrementOut = IsEllipsisArgType::value ? 
            N_ARG_TO_COVER_WITH_ELLIPSIS : InOutAxisIncrementType::OutAxis::value;




        MixedArgsImpl<
            MARRAY_IN, MARRAY_OUT, 
            // if the argument is a newaxis argument
            // we do  not increment the in axis index
            // since the current in axis has not yet been handled
            IN_AXIS_INDEX + IncrementIn,
            OUT_AXIS_INDEX + IncrementOut,
            N_ARG_TO_COVER_WITH_ELLIPSIS,
            ARGS ... 
        >::op(marrayIn, marrayOut, std::forward<ARGS>(args) ...);
    
    } 
};


template<
    class MARRAY_IN, 
    class MARRAY_OUT, 
    size_t IN_AXIS_INDEX,
    size_t OUT_AXIS_INDEX,
    int N_ARG_TO_COVER_WITH_ELLIPSIS,
    class ARG
>
struct MixedArgsImpl<
    MARRAY_IN, 
    MARRAY_OUT,
    IN_AXIS_INDEX,
    OUT_AXIS_INDEX,
    N_ARG_TO_COVER_WITH_ELLIPSIS,
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
            N_ARG_TO_COVER_WITH_ELLIPSIS,
            NoConstNoRefArgType, 
            IsIntgralArgType::value
        >::op(marrayIn, marrayOut, std::forward<ARG>(arg));    
    } 
};
















template<class MARRAY, bool CONST_INSTANCE, class ... ARGS>
struct BracketOpDispatcher{

    typedef MARRAY MarrayType;
    typedef typename std::conditional<CONST_INSTANCE,
        const MarrayType &,
        MarrayType &
    >::type MarrayReferenceType;



    typedef typename MarrayType::DimensionType DimensionType;

    typedef std::tuple<ARGS ...> ArgTupleType;
    const static size_t RawArgSize = std::tuple_size<ArgTupleType>::value;
    const static size_t Dim =  DimensionType::value;
    

    // if there is only a single argument and the single argument
    // is a scalar we need to pay attention
    typedef typename std::tuple_element<0, ArgTupleType>::type FirstArgType;
    const static bool SingleIntegralArg  = meta::IsIntegralOrIntegralRefeference<FirstArgType>::value;


    // information needed to dispatch...
    typedef CountTotalNeededNewAxis<ARGS ... >                     NumNeeededNewAxisType;
    typedef CountNewAxisLikeArgs<ARGS ... >                        NumNewArgLikeArgsType;
    typedef meta::CountOccurrencesRelaxed<EllipsisTag, ARGS ... >  NumEllipsisArgsType;
    typedef meta::CountIntegralArgumetns<ARGS ... >                NumIntegralArgsType;
    typedef meta::Bool<NumIntegralArgsType::value==RawArgSize>     AllIntegralArgsType;

    // shortcuts
    const static size_t NumNeededNewAxisArgs  = NumNeeededNewAxisType::value;
    const static size_t NumNewArgLikeArgs  = NumNewArgLikeArgsType::value;
    const static size_t NumEllipsisArgs  = NumEllipsisArgsType::value;
    const static size_t NumIntegralArgs = NumIntegralArgsType::value;

    const static size_t RestArgSize = RawArgSize - NumEllipsisArgs - NumNewArgLikeArgs;

    // early sanity check / static asserts
    static_assert(NumEllipsisArgs<=1, "Only A single Ellipsis argument can be used");


    // how many arguments need to be covered
    // by a potential ellipsis arguments
    const static int NToConverWithEllipsis = (Dim  - RestArgSize);

    static_assert(NToConverWithEllipsis == 0 || NumEllipsisArgs == 1, "Not enough arguments are passed to operator()");


    // if there is NO ellipsis argument
    // the number of arguments must match exact bu we split this
    // up into 2 checks to indicate if we have to much or not enough args
    static_assert(
        NumEllipsisArgs == 1  || (Dim  - RestArgSize) == 0,
        "wrong number of arguments arguments are passed to operator()"
    );


    // if there IS an ellipsis argument
    // the number of arguments must not match exact.
    // fewer args are allowed but not to many
    static_assert(
        NumEllipsisArgs == 0  || (NToConverWithEllipsis>=1),
        "To many arguments are passed to operator()"
    );
    



    typedef meta::SizeT<
        NumNeededNewAxisArgs + 
        Dim - 
        NumIntegralArgs
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

        typedef typename MarrayType:: template RebindDimensionAndIsConst<ResultDimensionType::value, CONST_INSTANCE>:: type type;

        static type op(MarrayReferenceType marray, ARGS && ... args){


            // the result array
            type resultArray;

            // might be modified later on
            resultArray.data_ = marray.data_;
            resultArray.smart_handle_ = marray.smart_handle_;
            MixedArgsImpl<MarrayType, type, 0,0, NToConverWithEllipsis,ARGS ...>::op(
                marray, resultArray, std::forward<ARGS>(args) ... );
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

        meta::Bool<Dim==RawArgSize && AllIntegralArgsType::value>,
            OnlyScalarImplType,
        meta::Bool<true>,   
            MixedArgsImplType
    >::type type;



        

    
    //typedef typename ImplType::type type;
};


} ///\endcond


}
