#pragma once


#include <iostream>
#include <memory>
#include <type_traits>
#include <utility>


#include "multi_array/meta/promote_type.hxx"

#include "shared_handle.hxx"
#include "misc.hxx"
#include "indexing_types.hxx"
#include "bracket_op_dispatcher.hxx"


namespace multi_array{






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







// expression templates
template<std::size_t DIM, class E, class T>
class ViewExpression {
public:
    typedef E expression_type;
    typedef T value_type;

    
    size_t size() const{ 
        return static_cast<const E&>(*this).size(); 
    }
    int64_t shape(const size_t j) const{ 
        return static_cast<const E&>(*this).shape(j); 
    }
   

    int64_t strides(const std::size_t a)const{
        std::cout<<"get da strides of view expr.\n";
        return static_cast<const E&>(*this).strides(a); 
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
        std::cout<<"da constructor\n";
        std::cout<<"e1 s0 :\n";
        std::cout<<e1_.strides(0)<<"\n";
        std::cout<<"e1 s1 :\n";
        std::cout<<e1_.strides(1)<<"\n";
        std::cout<<"e2 s0 :\n";
        std::cout<<e2_.strides(0)<<"\n";
        std::cout<<"e2 s1 :\n";
        std::cout<<e2_.strides(1)<<"\n";
        std::cout<<"da done\n";
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
        std::cout<<"get da strides of bin. view expr. at "<<a<<" \n";
        return e1_.strides(a);
    }

    bool matchingStrides()const{
        std::cout<<"bve matchingStrides\n";
        for(std::size_t a=0; a<DIM; ++a){
            std::cout<<"a="<<a<<"\n";
            std::cout<<"lea \n";
            const auto s1 = e1_.strides(a);
            std::cout<<"leb \n";
            const auto s2 = e2_.strides(a);
            std::cout<<"dunso \n";
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

   
    //const bool isSimple() const
    //    { return e1_.isSimple() && e2_.isSimple()
    //             && e1_.coordinateOrder() == e2_.coordinateOrder(); }

    class ExpressionPseudoIterator {
    public:
        ExpressionPseudoIterator(
            const BinaryViewExpression<DIM, E1, T1, E2, T2, BinaryFunctor>& expression
        )
        :   binaryFunctor_(expression.binaryFunctor_),
            iterator1_(expression.e1_),
            iterator2_(expression.e2_){

        }
        template<std::size_t AXIS>
        void incrementCoordinate(){   
            iterator1_. template incrementCoordinate<AXIS>();
            iterator2_. template incrementCoordinate<AXIS>(); 
        }
        template<std::size_t AXIS>
        void resetCoordinate(){   
            iterator1_. template resetCoordinate<AXIS>();
            iterator2_. template resetCoordinate<AXIS>(); 
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
    const expression_type_1& e1_;
    const expression_type_2& e2_;
    BinaryFunctor binaryFunctor_;
};





}