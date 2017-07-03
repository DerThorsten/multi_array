#pragma once


#include <iostream>
#include <memory>
#include <type_traits>
#include <utility>


#include "multi_array/meta/promote_type.hxx"

#include "shared_handle.hxx"
#include "meta.hxx"
#include "misc.hxx"
#include "indexing_types.hxx"
#include "bracket_op_dispatcher.hxx"


namespace multi_array{



class ViewExpressionBase;

template<std::size_t DIM>
class ViewExpressionDimBase;

template<std::size_t DIM, class T>
class ViewExpressionDimTBase;

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


#if 1
namespace meta{

    template<class T>
    struct IsViewExpression 
    : public std::is_base_of<
        ViewExpressionBase, T
    >
    {
    };

    

    template<std::size_t DIM, class T>
    struct IsViewExpressionWithDim 
    : public std::is_base_of<
        ViewExpressionDimBase<DIM>,
        T
    >
    {
    };

    

    namespace detail_meta{

        template<
            class TA, class TB,
            bool IS_VIEW_EXPRESSION_A, 
            bool IS_VIEW_EXPRESSION_B
        >
        struct AreViewExpressionsOfSameDimAndTypeHelper
        : public FalseCase{
        };


        template<class TA, class TB>
        struct AreViewExpressionsOfSameDimAndTypeHelper<TA, TB, true, true>
        : public 
            std::integral_constant<
                bool,
                std::is_same<
                    typename TA::DimensionType, 
                    typename TB::DimensionType
                >::value &&
                std::is_same<
                    typename TA::value_type, 
                    typename TB::value_type
                >::value
            >
        {

        };



        template<
            class TA, class TB,
            bool IS_VIEW_EXPRESSION_A, 
            bool IS_VIEW_EXPRESSION_B
        >
        struct AreViewExpressionsOfSameDimHelper
        : public FalseCase{
        };


        template<class TA, class TB>
        struct AreViewExpressionsOfSameDimHelper<TA, TB, true, true>
        : public std::is_same<
                typename TA::DimensionType, 
                typename TB::DimensionType
            > 
        {

        };


        template<
            class TA, class TB,
            bool IS_VIEW_EXPRESSION_A 
        >
        struct IsViewExpressionAndHasValueTypeHelper
        : public FalseCase{
        };

        template<
            class TA, class TB
        >
        struct IsViewExpressionAndHasValueTypeHelper<TA, TB, true>
        : public std::is_same<typename TA::value_type, TB>
        {
        };

    }

    template<class TA, class TB>
    struct AreViewExpressionsOfSameDim
    : public 
        detail_meta::AreViewExpressionsOfSameDimHelper<
            TA,TB,
            IsViewExpression<TA>::value, 
            IsViewExpression<TB>::value
        >
    {
    };

    
    template<class TA, class TB>
    struct AreViewExpressionsOfSameDimAndType
    : public 
        detail_meta::AreViewExpressionsOfSameDimAndTypeHelper<
            TA,TB,
            IsViewExpression<TA>::value, 
            IsViewExpression<TB>::value
        >
    {
    };

     

    
    template<class TA, class TB>
    struct IsViewExpressionAndHasValueType
    : public 
        detail_meta::IsViewExpressionAndHasValueTypeHelper<
            TA, 
            TB,  
            IsViewExpression<TA>::value
        >
    {
    };
}
#endif



class ViewExpressionBase 
{
};


template<std::size_t DIM>
class ViewExpressionDimBase : public ViewExpressionBase
{
};

template<std::size_t DIM, class T>
class ViewExpressionDimTBase : public ViewExpressionDimBase<DIM>
{
};


template<std::size_t DIM, class E, class T>
class ViewExpression : public ViewExpressionDimTBase<DIM,T>{
public:

    typedef E ExpressionType;
    typedef std::integral_constant<std::size_t, DIM> DimensionType;
    typedef T ValueType;

    typedef E expression_type;
    typedef T value_type;

    
    size_t size() const{ 
        return static_cast<const E&>(*this).size(); 
    }

    Shape<DIM> shape()const{
        Shape<DIM> ret;
        for(auto d=0; d<DIM; ++d){
            ret[d] = static_cast<const E&>(*this).shape(d); 
        }
        return ret;
    }



    int64_t shape(const size_t j) const{ 
        return static_cast<const E&>(*this).shape(j); 
    }

    Shape<DIM> makeShape()const{
        Shape<DIM> ret;
        for(auto d=0; d<DIM; ++d){
            ret[d] = this->shape(d);
        }
        return ret;
    }
   

    int64_t strides(const std::size_t a)const{
        return static_cast<const E&>(*this).strides(a); 
    }

    bool matchingStrides(const Strides<DIM> & s)const{
        return static_cast<const E&>(*this).matchingStrides(s); 
    }
    bool matchingStrides()const{
        return static_cast<const E&>(*this).matchingStrides(); 
    }
    bool contiguous() const{
        return static_cast<const E&>(*this).contiguous(); 
    }
    bool contiguous(const Order & o) const{
        return static_cast<const E&>(*this).contiguous(o); 
    }





    template<class VIEW>
    bool overlaps(const VIEW & v) const{ 
        return static_cast<const E&>(*this).overlaps(v); 
    }
    const bool dense() const{
        return static_cast<const E&>(*this).dense(); 
    }

    // cast operator to convert to child expression ref
    operator E&(){
        return static_cast<E&>(*this); 
    }
    // cast operator to convert to child expression const ref
    operator E const&() const{
        return static_cast<const E&>(*this); 
    }


    T unsafeAccess(const uint64_t index)const{
        return static_cast<const E&>(*this).unsafeAccess(index); 
    }

    /*
    // \cond suppress doxygen
    class ExpressionPseudoIterator {
    public:
        ExpressionPseudoIterator(const ViewExpression<DIM, E, T>& expression)
        : expression_(expression), // cast!
          data_(&expression_(0)),
          offset_(0)
            {}

        template<std::size_t AXIS>
        void incrementCoordinate(){ 
            offset_ += expression_.strides(AXIS); 
        }

        template<std::size_t AXIS>
        void resetCoordinate(const size_t coordinateIndex){ 
            offset_ -= expression_.strides(coordinateIndex)
                        * (expression_.shape(coordinateIndex) - 1); 
        }
        const T& operator*() const { 
            return data_[offset_]; 
        }
    private:
        const E& expression_;
        const T* data_;
        uint64_t offset_;
    };
    */
    // \endcond suppress doxygen
};



template<std::size_t DIM,class E1, class T1, class E2, class T2, class BinaryFunctor>
class BinaryViewExpression
: public ViewExpression<DIM, BinaryViewExpression<DIM, E1, T1, E2, T2, BinaryFunctor>,
                        typename meta::PromoteType<T1, T2>::type>
{
public:
    typedef E1 expression_type_1;
    typedef E2 expression_type_2;
    typedef T1 value_type_1;
    typedef T2 value_type_2;
    typedef typename meta::PromoteType<T1, T2>::type value_type;
    typedef BinaryFunctor functor_type;
    typedef ViewExpression<DIM, BinaryViewExpression<DIM, E1, T1, E2, T2, BinaryFunctor>,
        value_type> base;

    BinaryViewExpression(
        const ViewExpression<DIM, E1, T1>& e1,
        const ViewExpression<DIM, E2, T2>& e2
    )
    :   base(),
        e1_(e1), // cast!
        e2_(e2), // cast!
        binaryFunctor_(BinaryFunctor())
    {

    }

    BinaryViewExpression(const BinaryViewExpression & other)
    :   base(),   
        e1_(other.e1_),
        e2_(other.e2_),
        binaryFunctor_(BinaryFunctor()){
    }


    const size_t size() const{ 
        return e1_.size() ;
    }
    const size_t shape(const size_t j) const{
        return e1_.shape(j); 
    }
    
    template<class VIEW>
    bool overlaps(const VIEW & v) const {
        return e1_.overlaps(v) || e2_.overlaps(v); 
    }


    int64_t strides(const std::size_t a)const{
        return e1_.strides(a);
    }

    bool matchingStrides(const Strides<DIM> & s)const{

        for(std::size_t a=0; a<DIM; ++a){
            const auto s1 = e1_.strides(a);
            const auto s2 = s[a];

            if(s1!=0 && s2!=0 && s1!=s2){
                return false;
            }
        }
        return true;
    }

    bool matchingStrides()const{

        for(std::size_t a=0; a<DIM; ++a){
            const auto s1 = e1_.strides(a);
            const auto s2 = e2_.strides(a);

            if(s1!=0 && s2!=0 && s1!=s2){
                return false;
            }
        }
        return true;
    }

    // only valid if matchin strides
    bool contiguous() const{
        return e1_.contiguous();
    }

    // only valid if matchin strides
    bool contiguous(const Order & o) const{
        return e1_.contiguous(o);
    }

   
    class ExpressionPseudoIterator {
    public:
        ExpressionPseudoIterator(
            const BinaryViewExpression<DIM, E1, T1, E2, T2, BinaryFunctor>& expression
        )
        :   binaryFunctor_(expression.binaryFunctor_),
            iterator1_(expression.e1_),
            iterator2_(expression.e2_){

        }

        void incrementCoordinate(const size_t axis){   
            iterator1_. template incrementCoordinate(axis);
            iterator2_. template incrementCoordinate(axis); 
        }

        void resetCoordinate(const size_t axis){   
            iterator1_. template resetCoordinate(axis);
            iterator2_. template resetCoordinate(axis); 
        }
        const value_type operator*() const{ 
            return binaryFunctor_(*iterator1_, *iterator2_); 
        }
    private:
        BinaryFunctor binaryFunctor_;
        typename E1::ExpressionPseudoIterator iterator1_;
        typename E2::ExpressionPseudoIterator iterator2_;
    };


    value_type unsafeAccess(const uint64_t index)const{

        return binaryFunctor_(
            e1_.unsafeAccess(index),
            e2_.unsafeAccess(index)
        ); 
    }
private:

    const expression_type_1  e1_;
    const expression_type_2  e2_;

    // having references is very
    // dangerous since these
    // expressions are usually temp. objects
    //const expression_type_1 & e1_;
    //const expression_type_2 & e2_;
    BinaryFunctor binaryFunctor_;
};


template<std::size_t DIM,class E1, class T1, class S,  class BinaryFunctor>
class BinaryViewExpressionScalarSecond
: public ViewExpression<DIM, BinaryViewExpressionScalarSecond<DIM, E1, T1, S, BinaryFunctor>,
                        typename meta::PromoteType<T1, S>::type>
{
public:
    typedef E1 expression_type_1;
    typedef typename meta::PromoteType<T1, S>::type value_type;
    typedef BinaryFunctor functor_type;
    typedef ViewExpression<DIM, BinaryViewExpressionScalarSecond<DIM, E1, T1, S, BinaryFunctor>,
        value_type> base;

    BinaryViewExpressionScalarSecond(
        const ViewExpression<DIM, E1, T1>& e1,
        const S & scalarLike
    )
    :   base(),
        e1_(e1), // cast!
        scalarLike_(scalarLike),
        binaryFunctor_(BinaryFunctor())
    {

    }

    BinaryViewExpressionScalarSecond(const BinaryViewExpressionScalarSecond & other)
    :   base(),   
        e1_(other.e1_),
        scalarLike_(other.scalarLike_),
        binaryFunctor_(BinaryFunctor()){
    }


    const size_t size() const{ 
        return e1_.size() ;
    }
    const size_t shape(const size_t j) const{
        return e1_.shape(j); 
    }
    
    template<class VIEW>
    bool overlaps(const VIEW & v) const {
        return e1_.overlaps(v);
    }


    int64_t strides(const std::size_t a)const{
        return e1_.strides(a);
    }

    bool matchingStrides(const Strides<DIM> & s)const{

        for(std::size_t a=0; a<DIM; ++a){
            const auto s1 = e1_.strides(a);
            const auto s2 = s[a];

            if(s1!=0 && s2!=0 && s1!=s2){
                return false;
            }
        }
        return true;
    }

    bool matchingStrides()const{
        return true;
    }

    // only valid if matchin strides
    bool contiguous() const{
        return e1_.contiguous();
    }

    // only valid if matchin strides
    bool contiguous(const Order & o) const{
        return e1_.contiguous(o);
    }


    const S & scalarLike()const{
        return scalarLike_;
    }

   
    class ExpressionPseudoIterator {
    public:
        ExpressionPseudoIterator(
            const BinaryViewExpressionScalarSecond<DIM, E1, T1, S, BinaryFunctor>& expression
        )
        :   binaryFunctor_(expression.binaryFunctor_),
            iterator1_(expression.e1_),
            scalarLike_(expression.scalarLike()){

        }

        void incrementCoordinate(const size_t axis){   
            iterator1_. template incrementCoordinate(axis);
        }

        void resetCoordinate(const size_t axis){   
            iterator1_. template resetCoordinate(axis);
        }
        const value_type operator*() const{ 
            return binaryFunctor_(*iterator1_, scalarLike_); 
        }
    private:
        BinaryFunctor binaryFunctor_;
        typename E1::ExpressionPseudoIterator iterator1_;
        const S & scalarLike_;
    };


    value_type unsafeAccess(const uint64_t index)const{

        return binaryFunctor_(
            e1_.unsafeAccess(index),
            scalarLike_
        ); 
    }

private:

    const expression_type_1  e1_;
    const S scalarLike_;

    // having references is very
    // dangerous since these
    // expressions are usually temp. objects
    //const expression_type_1 & e1_;
    //const expression_type_2 & e2_;
    BinaryFunctor binaryFunctor_;
};


template<std::size_t DIM,class E1, class T1, class S,  class BinaryFunctor>
class BinaryViewExpressionScalarFirst
: public ViewExpression<DIM, BinaryViewExpressionScalarFirst<DIM, E1, T1, S, BinaryFunctor>,
                        typename meta::PromoteType<T1, S>::type>
{
public:
    typedef E1 expression_type_1;
    typedef typename meta::PromoteType<T1, S>::type value_type;
    typedef BinaryFunctor functor_type;
    typedef ViewExpression<DIM, BinaryViewExpressionScalarFirst<DIM, E1, T1, S, BinaryFunctor>,
        value_type> base;

    BinaryViewExpressionScalarFirst(
        const S & scalarLike,
        const ViewExpression<DIM, E1, T1>& e1
    )
    :   base(),
        e1_(e1), // cast!
        scalarLike_(scalarLike),
        binaryFunctor_(BinaryFunctor())
    {

    }

    BinaryViewExpressionScalarFirst(const BinaryViewExpressionScalarFirst & other)
    :   base(),   
        e1_(other.e1_),
        scalarLike_(other.scalarLike_),
        binaryFunctor_(BinaryFunctor()){
    }


    const size_t size() const{ 
        return e1_.size() ;
    }
    const size_t shape(const size_t j) const{
        return e1_.shape(j); 
    }
    
    template<class VIEW>
    bool overlaps(const VIEW & v) const {
        return e1_.overlaps(v);
    }


    int64_t strides(const std::size_t a)const{
        return e1_.strides(a);
    }

    bool matchingStrides(const Strides<DIM> & s)const{

        for(std::size_t a=0; a<DIM; ++a){
            const auto s1 = e1_.strides(a);
            const auto s2 = s[a];

            if(s1!=0 && s2!=0 && s1!=s2){
                return false;
            }
        }
        return true;
    }

    bool matchingStrides()const{
        return true;
    }

    // only valid if matchin strides
    bool contiguous() const{
        return e1_.contiguous();
    }

    // only valid if matchin strides
    bool contiguous(const Order & o) const{
        return e1_.contiguous(o);
    }


    const S & scalarLike()const{
        return scalarLike_;
    }

   
    class ExpressionPseudoIterator {
    public:
        ExpressionPseudoIterator(
            const BinaryViewExpressionScalarFirst<DIM, E1, T1, S, BinaryFunctor>& expression
        )
        :   binaryFunctor_(expression.binaryFunctor_),
            iterator1_(expression.e1_),
            scalarLike_(expression.scalarLike()){

        }

        void incrementCoordinate(const size_t axis){   
            iterator1_. template incrementCoordinate(axis);
        }

        void resetCoordinate(const size_t axis){   
            iterator1_. template resetCoordinate(axis);
        }
        const value_type operator*() const{ 
            return binaryFunctor_(scalarLike_, *iterator1_); 
        }
    private:
        BinaryFunctor binaryFunctor_;
        typename E1::ExpressionPseudoIterator iterator1_;
        const S & scalarLike_;
    };


    value_type unsafeAccess(const uint64_t index)const{

        return binaryFunctor_(
            scalarLike_,
            e1_.unsafeAccess(index)
        ); 
    }

private:

    const expression_type_1  e1_;
    const S scalarLike_;

    // having references is very
    // dangerous since these
    // expressions are usually temp. objects
    //const expression_type_1 & e1_;
    //const expression_type_2 & e2_;
    BinaryFunctor binaryFunctor_;
};





}