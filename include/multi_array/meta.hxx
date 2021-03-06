
#pragma once

#include <complex>



namespace multi_array{
namespace meta{




    
    template<uint64_t VAL>
    struct UInt64T : std::integral_constant<uint64_t, VAL>{
    };
    template<uint32_t VAL>
    struct UInt32T : std::integral_constant<uint32_t, VAL>{
    };
    template<uint8_t VAL>
    struct UInt8T : std::integral_constant<uint8_t, VAL>{
    };
    template<int64_t VAL>
    struct Int64T : std::integral_constant<int64_t, VAL>{
    };
    template<int32_t VAL>
    struct Int32T : std::integral_constant<int32_t, VAL>{
    };
    template<int8_t VAL>
    struct Int8T : std::integral_constant<int8_t, VAL>{
    };
    template<int8_t VAL>
    struct Int: std::integral_constant<int, VAL>{
    };

    template<bool VAL>
    struct Bool : std::integral_constant<bool, VAL>{
    };
    template<std::size_t VAL>
    struct SizeT : std::integral_constant<std::size_t, VAL>{
    };

    template<class U>
    struct Identity{
        typedef U type;
    };

    typedef std::true_type TrueCase;
    typedef std::false_type FalseCase;

    typedef Identity<TrueCase> TrueFunc;
    typedef Identity<FalseCase> FalseFunc;



    template<class CONDITION, class F1, class F2>
    struct EvalIf;

    template<class F1, class F2>
    struct EvalIf<FalseCase, F1, F2>
    : public F2{
    };

    template<class F1, class F2>
    struct EvalIf<TrueCase, F1, F2>
    : public F1{
    };

 
    template<class F_CONDITION, class F1, class F2>
    struct LazyEvalIf
    : public EvalIf<typename  F_CONDITION::type, F1, F2>{ 
    };

   

   



    template<class T>
    struct IsComplex : FalseCase {
    };
    template<class T>
    struct IsComplex<std::complex<T>> : TrueCase {
    };

    template<class T , class ... Args>
    class AllIntegral : public 
         std::integral_constant<
            bool, 

            std::conditional<
                std::is_integral<T>::value,
                typename AllIntegral<Args ... >::type,
                std::integral_constant<bool,false>
            >::type::value
        > 
    {
    };

  

    template<class COND, class ARG, class ... ARGS>
    struct  LazyEvalIfSwitch : public EvalIf<
        typename COND::type, 
        ARG,
        typename LazyEvalIfSwitch<ARGS ... >::type
    >
    {



    };

    template<class COND, class TYPE>
    struct LazyEvalIfSwitch<COND, TYPE> : public TYPE
    {
        static_assert(COND::type::value,"No case of the LazyEvalIfSwitch evaluated to true");
        //typedef TYPE type;
    };





    template<class COND, class TYPE, class ... Args>
    struct EvalIfSwitch :   public 
        EvalIf<
            COND,
            TYPE,
            typename EvalIfSwitch<Args ...>::type
        >
    {

    };
    

    template<class COND, class TYPE>
    struct EvalIfSwitch<COND, TYPE>{
        static_assert(COND::value,"No case of the EvalIfSwitch evaluated to true");
        typedef TYPE type;
    };    





    template<class T>
    struct AllIntegral<T> : public std::integral_constant<bool, std::is_integral<T>::value > {
    };


    template<class COND, class TYPE, class ... Args>
    struct Switch{
        typedef typename std::conditional<
            COND::value,
            TYPE,
            typename Switch<Args ...>::type
        >::type type;
    };
    

    template<class COND, class TYPE>
    struct Switch<COND, TYPE>{
        static_assert(COND::value,"No case of the Switch evaluated to true");
        typedef TYPE type;
    };    


    template<class TO_FIND, class FIRST , class ... Args>
    class CountOccurrences : public 

         std::integral_constant<
            std::size_t, 

            std::conditional<
                std::is_same<TO_FIND,FIRST>::value,
                std::integral_constant<std::size_t, 1>,
                std::integral_constant<std::size_t, 0>
            >::type::value + CountOccurrences<TO_FIND, Args ... >::value
        > 
    {
    };

    template<class TO_FIND, class FIRST>
    class CountOccurrences<TO_FIND, FIRST> : public 

         std::integral_constant<
            std::size_t, 

            std::conditional<
                std::is_same<TO_FIND,FIRST>::value,
                std::integral_constant<std::size_t, 1>,
                std::integral_constant<std::size_t, 0>
            >::type::value
        > 
    {
    };

    template<class T>
    struct RemoveCvAndReference
    : public std::remove_cv< typename std::remove_reference<T>::type >
    {

    };




    template<class A, class B>
    struct IsSameWithoutConstAndReferece
    : public std::is_same<
            typename RemoveCvAndReference<A>::type,
            typename RemoveCvAndReference<B>::type
        >
    {

    };




    template<class TO_FIND, class FIRST , class ... Args>
    class CountOccurrencesRelaxed : public 

         std::integral_constant<
            std::size_t, 

            std::conditional<
                IsSameWithoutConstAndReferece<TO_FIND,FIRST>::value,
                std::integral_constant<std::size_t, 1>,
                std::integral_constant<std::size_t, 0>
            >::type::value + CountOccurrencesRelaxed<TO_FIND, Args ... >::value
        > 
    {
    };

    template<class TO_FIND, class FIRST>
    class CountOccurrencesRelaxed<TO_FIND, FIRST> : public 

         std::integral_constant<
            std::size_t, 

            std::conditional<
                IsSameWithoutConstAndReferece<TO_FIND,FIRST>::value,
                std::integral_constant<std::size_t, 1>,
                std::integral_constant<std::size_t, 0>
            >::type::value
        > 
    {
    };


    template< template<typename> class F, class T, class FIRST , class ... Args>
    class AccumulateSum : public 

         std::integral_constant<
            std::size_t, 

            std::conditional<
                F<FIRST>::value,
                std::integral_constant<std::size_t, 1>,
                std::integral_constant<std::size_t, 0>
            >::type::value + AccumulateSum<F, T, Args ... >::value
        > 
    {
    };

    template<template<typename> class F, class T, class FIRST>
    class AccumulateSum<F, T, FIRST> : public 

         std::integral_constant<
            std::size_t, 

            std::conditional<
                F<FIRST>::value,
                std::integral_constant<std::size_t, 1>,
                std::integral_constant<std::size_t, 0>
            >::type::value
        > 
    {
    };



    template<class T>
    struct RemoveReferenceAndCv 
    :   std::remove_cv<
            typename std::remove_reference<
                T
            >::type
        >
    {
    };



    template<class T>
    struct IsIntegralOrIntegralRefeference
    :   std::is_integral<
            typename std::remove_cv<
                typename std::remove_reference<
                    T
                >::type
            >::type 
        >
    {

    };


    template<class ... Args>
    using CountIntegralArgumetns = 
        AccumulateSum<IsIntegralOrIntegralRefeference, std::size_t, Args ...>;




}
}