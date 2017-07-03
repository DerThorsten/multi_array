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
#include "view_expression.hxx"
#include "operate.hxx"


namespace multi_array{

    namespace functors{

    // unary in-place functors
    template<class T>
    struct Negative { 
        void operator()(T& x) { x = -x; } 
    };

    template<class T>
    struct PrefixIncrement { 
        void operator()(T& x) { ++x; } 
    };

    template<class T>
    struct PostfixIncrement { 
        void operator()(T& x) { x++; } 
    };

    template<class T>
    struct PrefixDecrement { 
        void operator()(T& x) { --x; } 
    };

    template<class T>
    struct PostfixDecrement { 
        void operator()(T& x) { x--; } 
    };

    // binary in-place functors
    template<class T1, class T2>
    struct Assign { 
        void operator()(T1& x, const T2& y) { x = y; } 
    };

    template<class T1, class T2>
    struct PlusEqual { 
        static void op(T1& x, const T2& y) { x += y; } 
        void operator()(T1& x, const T2& y) { x += y; } 
    };

    template<class T1, class T2>
    struct MinusEqual { 
        static void op(T1& x, const T2& y) { x -= y; } 
        void operator()(T1& x, const T2& y) { x -= y; } 
    };

    template<class T1, class T2>
    struct TimesEqual { 
        static void op(T1& x, const T2& y) { x *= y; } 
        void operator()(T1& x, const T2& y) { x *= y; } 
    };

    template<class T1, class T2>
    struct DividedByEqual { 
        static void op(T1& x, const T2& y) { x /= y; } 
        void operator()(T1& x, const T2& y) { x /= y; } 
    };

    template<class T1, class T2>
    struct OrEqual { 
        static void op(T1& x, const T2& y) { x |= y; } 
        void operator()(T1& x, const T2& y) { x |= y; } 
    };


    template<class T1, class T2>
    struct AndEqual { 
        static void op(T1& x, const T2& y) { x &= y; } 
        void operator()(T1& x, const T2& y) { x &= y; } 
    };

    // binary functors
    template<class T1, class T2>
    struct Plus { 
        typedef typename meta::PromoteType<T1, T2>::type type;
        static type op(const T1& x, const T2& y){ return x + y; } 
        type operator()(const T1& x, const T2& y) const { return x + y; } 
    };

    template<class T1, class T2>
    struct Minus { 
        typedef typename meta::PromoteType<T1, T2>::type type;
        static type op(const T1& x, const T2& y){ return x - y; } 
        type operator()(const T1& x, const T2& y) const { return x - y; } 
    };

    template<class T1, class T2>
    struct Times { 
        typedef typename meta::PromoteType<T1, T2>::type type;
        static type op(const T1& x, const T2& y){ return x * y; } 
        type operator()(const T1& x, const T2& y) const { return x * y; } 
    };

    template<class T1, class T2>
    struct DividedBy {
        typedef typename meta::PromoteType<T1, T2>::type type;
        static type op(const T1& x, const T2& y){ return x / y; } 
        type operator()(const T1& x, const T2& y) const { return x / y; } 
    };

    template<class T1, class T2>
    struct Or { 
        typedef typename meta::PromoteType<T1, T2>::type type;
        static type op(const T1& x, const T2& y){ return x || y; } 
        type operator()(const T1& x, const T2& y) const { return x || y; } 
    };

    template<class T1, class T2>
    struct And { 
        typedef typename meta::PromoteType<T1, T2>::type type;
        static type op(const T1& x, const T2& y){ return x && y; } 
        type operator()(const T1& x, const T2& y) const { return x && y; } 
    };




    template< template<class, class> class FUNCTOR>
    struct FunctorGenerator{
        template<class T1, class T2>
        struct Generate{
            typedef FUNCTOR<T1, T2> type;
        };
    };










    }
}