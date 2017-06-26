#pragma once

#include "multi_array/multi_array.hxx"
#include "multi_array/misc.hxx"
#include "multi_array/meta/variadic.hxx"

namespace multi_array{

    template<class T, class ... ARGS>
    inline SmartMultiArray<
        T,
        meta::VariadicArgumentsSize<
            ARGS ... 
        >::value,
        false
    >
    ones(ARGS && ... args){
        typedef meta::VariadicArgumentsSize<ARGS ... >       VSizeType;
        typedef SmartMultiArray<T, VSizeType::value, false > ResultType; 
        return ResultType(shape(std::forward<ARGS>(args)...), static_cast<T>(1));
    }


    template<class T, std::size_t DIM>
    inline SmartMultiArray<T, DIM, false> 
    ones(Shape<DIM> && shape){
        typedef SmartMultiArray<T, DIM, false > ResultType; 
        return ResultType(shape);
    }


    template<class T, class ... ARGS>
    inline SmartMultiArray<
        T,
        meta::VariadicArgumentsSize<
            ARGS ... 
        >::value,
        false
    >
    zeros(ARGS && ... args){
        typedef meta::VariadicArgumentsSize<ARGS ... >       VSizeType;
        typedef SmartMultiArray<T, VSizeType::value, false > ResultType; 
        return ResultType(shape(std::forward<ARGS>(args)...), static_cast<T>(0));
    }

}