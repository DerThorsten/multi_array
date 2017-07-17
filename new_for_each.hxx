#include "multi_array/navigator.hxx"
#include "multi_array/misc.hxx"
#include "multi_array/meta.hxx"


namespace multi_array{




template<std::size_t DIM>
struct ForEachImpl;



namespace detail_call_args{

    struct OffsetTag{};
    struct CoordinateTag{};
    struct OffsetAndCoordinateTag{};



    template<class TAG>
    struct FunctorCaller;

    template<>
    struct FunctorCaller<OffsetTag>{
        template<class NAVIGATOR, class F>
        static void op(const NAVIGATOR & navigator, F && f){
            f(navigator.offset());
        }
    }
    template<>
    struct FunctorCaller<CoordinateTag>{
        template<class NAVIGATOR, class F>
        static void op(const NAVIGATOR & navigator, F && f){
            f(navigator.coordinate());
        }
    }
    template<>
    struct FunctorCaller<CoordinateTag>{
        template<class NAVIGATOR, class F>
        static void op(const NAVIGATOR & navigator, F && f){
            f(navigator.offset(), navigator.coordinate());
        }
    }


}


template<>
struct ForEachImpl<2>{


    const static std::size_t 2 Dimension;

    typedef Shape<Dimension>        ShapeType;
    typedef Strides<Dimension>      StridesType;
    typedef Coordinate<Dimension>   CoordinateType;
    typedef Navigator<Dimension>    NavigatorType;
    typedef C int64_t;


    template<class F, class CALL_ARG_TAG>
    static void opCOrder(
        const ShapeType & shape,
        const StridesType & strides,
        F && f
    ){

        typedef FunctorCaller<CALL_ARG_TAG> FunctorCallerType;
        NavigatorType navigator(shape, strides);
        
        for(navigator.set(0,0); navigator.get(0)<shape[0]; navigator.increment(0))
        for(navigator.set(1,0); navigator.get(1)<shape[1]; navigator.increment(1))
        {
            FunctorCallerType::op(navigator, std::forward<F>(f));
        }
    }

}



}