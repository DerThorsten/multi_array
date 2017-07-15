#pragma once

#include <iostream>
#include <memory>
#include <type_traits>
#include <utility>

#include "multi_array/misc.hxx"
#include "multi_array/expr/view_expression.hxx"

namespace multi_array{




template<std::size_t DIM, class E, class T>
class ViewExpression;

template<bool NEEDS_COORDINATE, bool IS_SHAPELESS>
struct ViewExpressionMeta;


template<
    std::size_t DIM,
    class E1, class T1,
    class E2, class T2, 
    class E3, class T3, 
    class TernaryFunctor
>
class TernaryViewExpression
: public ViewExpression<DIM, TernaryViewExpression<DIM, E1, T1, E2, T2, E3, T3, TernaryFunctor>,
                        typename TernaryFunctor::type>
{
public:

    typedef TernaryViewExpression<DIM, E1, T1, E2, T2, E3, T3, TernaryFunctor> SelfType;
    typedef E1 ExpressionType1;
    typedef E2 ExpressionType2;
    typedef E3 ExpressionType3;

    typedef typename ExpressionType1::ViewExpressionMetaType ViewExpressionMetaType1;
    typedef typename ExpressionType2::ViewExpressionMetaType ViewExpressionMetaType2;
    typedef typename ExpressionType3::ViewExpressionMetaType ViewExpressionMetaType3;

    typedef ViewExpressionMeta<
        ViewExpressionMetaType1::NeedsCoordinateType::value || 
        ViewExpressionMetaType2::NeedsCoordinateType::value || 
        ViewExpressionMetaType3::NeedsCoordinateType::value, 
        ViewExpressionMetaType1::IsShapelessType::value && 
        ViewExpressionMetaType2::IsShapelessType::value && 
        ViewExpressionMetaType3::IsShapelessType::value
    > ViewExpressionMetaType;

    typedef typename TernaryFunctor::type value_type;


    TernaryViewExpression(
        const ViewExpression<DIM, E1, T1>& e1,
        const ViewExpression<DIM, E2, T2>& e2,
        const ViewExpression<DIM, E3, T2>& e3,
        const TernaryFunctor & ternaryFunctor = TernaryFunctor()
    )
    :   e1_(e1), // cast!
        e2_(e2), // cast!
        e3_(e3), // cast!
        ternaryFunctor_(ternaryFunctor)
    {

    }

    TernaryViewExpression(const TernaryViewExpression & other)
    :   e1_(other.e1_),
        e2_(other.e2_),
        e3_(other.e3_),
        ternaryFunctor_(other.ternaryFunctor_){
    }


    bool hasShape() const{ 
        return !ViewExpressionMetaType::IsShapelessType::value;
    }

    bool hasStrides() const{ 
        return (e1_.hasStrides() || e2_.hasStrides() || e3_.hasStrides());
    }

    const size_t shape(const size_t j) const{
        if(!ViewExpressionMetaType1::IsShapelessType::value){
            return e1_.shape(j);
        }
        else if(!ViewExpressionMetaType2::IsShapelessType::value){
            return e2_.shape(j);
        }
        else if(!ViewExpressionMetaType3::IsShapelessType::value){
            return e3_.shape(j);
        }
        else{
             MULTI_ARRAY_CHECK(false, "all 3 don't have a shape");
        }
    }
    
    template<class VIEW>
    bool overlaps(const VIEW & v) const {
        return e1_.overlaps(v) || e2_.overlaps(v) || e3_.overlaps(v); 
    }


    int64_t strides(const std::size_t a)const{
        if(e1_.hasStrides()){
            return e1_.strides(a);
        }
        else if(e2_.hasStrides()){
            return e2_.strides(a);
        }
        else if(e3_.hasStrides()){
            return e3_.strides(a);
        }
        else{
            MULTI_ARRAY_CHECK(false, "all dont have a stride");
        }
    }

    // this function can only be called
    // iff and iff only this->matchingStrides()==true
    bool matchingStrides(const Strides<DIM> & s)const{

        if(!e1_.hasStrides() && !e2_.hasStrides() && !e3_.hasStrides()){
            return true;
        }
        else{
            for(std::size_t a=0; a<DIM; ++a){
                const auto s1 = this->strides(a);
                const auto s2 = s[a];

                if(s1!=0 && s2!=0 && s1!=s2){
                    return false;
                }
            }
            return true;
        }
    }

    bool matchingStrides()const{

        auto match = [](const int64_t sa, const int64_t sb){
            if(sa == 0 || sb == 0)
                return true;
            else
                return sa == sb;
        };

        for(std::size_t a=0; a<DIM; ++a){
            const auto s1  = e1_.hasStrides() ? e1_.strides(a) : int64_t(0);
            const auto s2  = e2_.hasStrides() ? e2_.strides(a) : int64_t(0);
            const auto s3  = e3_.hasStrides() ? e3_.strides(a) : int64_t(0);
            if( !match(s1, s2) || !match(s1, s3))
                return false;
        }
        return true;
    }

    // a call to this function is valid 
    // if and if only this->matchingStrides()==true
    bool contiguous() const{
        return e1_.contiguous();
    }

    // a call to this function is valid 
    // if and if only this->matchingStrides()==true
    bool contiguous(const Order & o) const{
        return e1_.contiguous(o);
    }

    // a call to this function is valid 
    // if and if only this->matchingStrides()==true
    value_type unsafeAccess(const uint64_t index)const{
        return ternaryFunctor_(
            e1_.unsafeAccess(index),
            e2_.unsafeAccess(index),
            e3_.unsafeAccess(index)
        ); 
    }

    // a call to this function is valid 
    // if and if only this->matchingStrides()==true
    value_type unsafeAccess(const uint64_t index, const Coordinate<DIM>  & coordinate)const{

        typedef typename  ViewExpressionMetaType::NeedsCoordinateType NeedsCoordinateType;

        if(NeedsCoordinateType::value){
            return ternaryFunctor_(
                e1_.unsafeAccess(index,coordinate),
                e2_.unsafeAccess(index,coordinate),
                e3_.unsafeAccess(index,coordinate)

            ); 
        }
        else{
            return ternaryFunctor_(
                e1_.unsafeAccess(index),
                e2_.unsafeAccess(index),
                e3_.unsafeAccess(index)
            );
        }
    }  


    const TernaryFunctor & ternaryFunctor()const{
        return ternaryFunctor_;
    }

    const ExpressionType1 inputExpression1()const{
       return e1_;
    }
    const ExpressionType2 inputExpression2()const{
       return e2_;
    }
    const ExpressionType3 inputExpression3()const{
       return e3_;
    }


    class ExpressionPseudoIterator {
    public:
        ExpressionPseudoIterator(
            const SelfType & expression
        )
        :   self_(expression),
            iterator1_(expression.e1_),
            iterator2_(expression.e2_),
            iterator3_(expression.e3_)
        {

        }

        void incrementCoordinate(const size_t axis){   
            iterator1_. incrementCoordinate(axis);
            iterator2_. incrementCoordinate(axis); 
            iterator3_. incrementCoordinate(axis); 
        }

        void resetCoordinate(const size_t axis){   
            iterator1_. resetCoordinate(axis);
            iterator2_. resetCoordinate(axis); 
            iterator3_. resetCoordinate(axis); 
        }
        const value_type operator*() const{ 
            return self_.ternaryFunctor()(*iterator1_, *iterator2_, *iterator3_); 
        }
    private:
        const SelfType & self_;
        typename E1::ExpressionPseudoIterator iterator1_;
        typename E2::ExpressionPseudoIterator iterator2_;
        typename E3::ExpressionPseudoIterator iterator3_;
    };
private:

    const ExpressionType1  e1_;
    const ExpressionType2  e2_;
    const ExpressionType3  e3_;
    TernaryFunctor ternaryFunctor_;
};




}