#pragma once

#include <type_traits>
#include <cmath>

#include "multi_array/misc.hxx"
#include "multi_array/meta/variadic.hxx"

namespace multi_array{








    template<class T, std::size_t DIM, bool IS_CONST>
    class SmartMultiArray;



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


    template<class T, std::size_t DIM>
    inline SmartMultiArray<T, DIM, false> 
    ones(Shape<DIM> && shape, const DataType<T> & dt){
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


    ///\cond
    namespace detail_factories{

        // we assume integer like if not a float
        template<bool IS_FLOAT>
        struct ARangeImpl{

            template<class T>
            inline static  SmartMultiArray<T, 1, false>
            op(const T & start, const T & stop, const T & step){
                const auto size  = (stop-start)/step;
                auto array =  SmartMultiArray<T, 1, false>(makeShape(size));
                auto data = array.data();
                T current = start;
                for(auto i=0; i<size; ++i){
                    data[i] = current;
                    current += step;
                }
                return array;
            }
        };


        template<>
        struct ARangeImpl<true>{

            template<class T>
            inline 
            static  SmartMultiArray<T, 1, false>
            op(const T & start, const T & stop, const T & step){
                const auto size  = int64_t(std::ceil((stop-start)/step));
                auto array =  SmartMultiArray<T, 1, false>(makeShape(size));
                auto data = array.data();
                T current = start;
                for(auto i=0; i<size; ++i){
                    data[i] = current;
                    current += step;
                }
                return array;
            }
        };

    }   
    ///\endcond


    template<class T>
    inline SmartMultiArray<T, 1, false>
    arange(
        const T & start,
        const T & stop,
        const T & step = static_cast<T>(1)
    ){
        typedef detail_factories::ARangeImpl<std::is_floating_point<T>::value> ImplType;
        return ImplType::op(start, stop, step);
    }

    template<class T>
    inline SmartMultiArray<T, 1, false>
    arange(
        const T & stop
    ){
        return arange(static_cast<T>(0),stop);
    }

}