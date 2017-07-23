#pragma once

#include <iostream>
#include <memory>
#include <type_traits>
#include <utility>
#include <exception>

#include "multi_array/meta/promote_type.hxx"
#include "multi_array/meta/promote_real.hxx"
#include "multi_array/functors.hxx"

#include "multi_array/runtime_check.hxx"
#include "multi_array/shared_handle.hxx"
#include "multi_array/meta.hxx"
#include "multi_array/misc.hxx"
#include "multi_array/indexing_types.hxx"


//#include "multi_array/expr/ternary_expression.hxx"
// #include "multi_array/expr/quaternary_expression.hxx"

namespace multi_array{




class CannotUseEmptyArrayInExpressionException: public std::runtime_error{
public:

    CannotUseEmptyArrayInExpressionException(const std::string & msg = std::string())
    :   std::runtime_error(msg.c_str()){
    }
};

class CannotAssignShapelessExpressionToEmptyArrayException: public std::runtime_error{
public:

    CannotAssignShapelessExpressionToEmptyArrayException(const std::string & msg = std::string())
    :   std::runtime_error(msg.c_str()){
    }
};



namespace detail_expr{


    struct ShapeInfo{
        bool isShapeless{false};
        bool needsBroadcasting{false};
    };

    template<std::size_t DIM>
    inline ShapeInfo broadcastShapesWithShapeless(
        const Shape<DIM> & shapeA,
        const Shape<DIM> & shapeB,
        Shape<DIM> & res
    ){
        auto shapeInfo = ShapeInfo();
        for(auto d=0; d<DIM; ++d){
            const auto sA = shapeA[d];
            const auto sB = shapeB[d];

            // shapeless result ?
            if(sA==HasNoShapeValue && sB==HasNoShapeValue){
                shapeInfo.isShapeless = true;
                res[d] = HasNoShapeValue;
            }
            else if(sA==HasNoShapeValue || sB==HasNoShapeValue){
                // HasNoShapeValue == -1
                res[d] = std::max(sA, sB);
            }
            else if(sA != sB){
                if(sA==1 || sB==1){
                    shapeInfo.needsBroadcasting = true;
                    res[d] = std::max(sA, sB);
                }
                else{
                    throw ShapesNotBroadcastableException(shapeA, shapeB);
                }
            }
            else{
                res[d] =sA;
            }
        }
        MULTI_ARRAY_CHECK(!shapeInfo.needsBroadcasting || !shapeInfo.isShapeless,"internal error");
        return shapeInfo;
    }

}



template<
    bool NEEDS_COORDINATE, 
    bool HAS_SHAPE,
    bool HAS_STRIDES,
    bool IS_COMPATILE_WITH_ANY_STRIDES
>
struct ViewExpressionMeta{

    typedef std::integral_constant<bool, NEEDS_COORDINATE>                  NeedsCoordinateType;
    typedef std::integral_constant<bool, HAS_SHAPE>                         HasShapeType;
    typedef std::integral_constant<bool, HAS_STRIDES>                       HasStridesType;
    typedef std::integral_constant<bool, IS_COMPATILE_WITH_ANY_STRIDES>     IsCompatibleWithAnyStridesType;
};



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

template< std::size_t DIM, class E1, class T1, class E2, class T2,  class E3, class T3,  class TernaryFunctor>
class TernaryViewExpression;

template<std::size_t DIM,class E1, class T1,class E2, class T2, class E3, class T3, class E4, class T4, class QuaquaternaryFunctor>
class QuaternaryViewExpression;



///\cond
class ViewExpressionBase {
};
template<std::size_t DIM>
class ViewExpressionDimBase : public ViewExpressionBase{
};
template<std::size_t DIM, class T>
class ViewExpressionDimTBase : public ViewExpressionDimBase<DIM>{
};
///\endcond

template<std::size_t DIM, class E, class T>
class ViewExpression : public ViewExpressionDimTBase<DIM,T>{
public:

    typedef E ExpressionType;
    typedef std::integral_constant<std::size_t, DIM> DimensionType;
    typedef T ValueType;

    typedef E expression_type;
    typedef T value_type;

   
    /*
    Shape<DIM> shape()const{
        Shape<DIM> ret;
        for(auto d=0; d<DIM; ++d){
            ret[d] = static_cast<const E&>(*this).shape(d); 
        }
        return ret;
    }
    */


    bool hasShape() const{ 
        return static_cast<const E&>(*this).hasShape(); 
    }
    bool hasStrides() const{ 
        return static_cast<const E&>(*this).hasStrides(); 
    }

    very_inline bool needsBroadcasting() const{
        return static_cast<const E&>(*this).needsBroadcasting(); 
    }


    bool needsBroadcastingWithShape(const Shape<DIM> & lhsShape) const{
        if(this->needsBroadcasting()){
            return true;
        }
        else{
            const auto & rhsShape = this->broadcastedShape();
            for(auto d=0; d<DIM; ++d){

                const auto sLhs = lhsShape[d];
                const auto sRhs = rhsShape[d];

                if(sLhs!=sRhs){

                    if(sLhs!=1 && sRhs!=1 ){
                        for(auto i=0; i<DIM; ++i){
                            std::cout<<"lhs "<<lhsShape[i]<<" rhs "<<rhsShape[i]<<"\n";
                        }
                        throw ShapesNotBroadcastableException(lhsShape, rhsShape);
                    }
                    else{
                        return true;
                    }
                }
            }
            return false;
        }
    }



    
    const Shape<DIM> & broadcastedShape()const{
        return static_cast<const E&>(*this).broadcastedShape(); 
    }

    const Strides<DIM> & strides()const{
        return static_cast<const E&>(*this).strides(); 
    }


    bool empty() const{ 
        return static_cast<const E&>(*this).empty(); 
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

    T unsafeAccess(const uint64_t index, const Coordinate<DIM>  & coordinate)const{
        return static_cast<const E&>(*this).unsafeAccess(index, coordinate); 
    }
};


template<std::size_t DIM, std::size_t AXIS, class T = int64_t>
class CoordExpr
:   public ViewExpression<DIM, CoordExpr<DIM, AXIS, T>,  T  >
{
public:
    typedef ViewExpressionMeta<true, false, false, true> ViewExpressionMetaType;
    typedef T value_type;
    typedef CoordExpr<DIM, AXIS, T> SelfType;
    typedef ViewExpression<DIM, CoordExpr<DIM, AXIS, T>, T> base;

    CoordExpr()
    :   base(),
        // HasNoShapeShapeValue == -1
        shape_(HasNoShapeValue),
        strides_(HasNoStridesValue)
    {
    }


    constexpr bool hasShape() const{ 
        return false;
    }

    constexpr bool hasStrides() const{ 
        return false;
    }

    constexpr bool empty() const{ 
        return false;
    }
    
    constexpr bool needsBroadcasting() const{
        return false;
    }

    const Shape<DIM> & broadcastedShape()const{
        return shape_;
    }
    const Strides<DIM> & strides()const{
        return strides_;
    }   
    
    template<class VIEW>
    bool overlaps(const VIEW & v) const {
        return false;
    }

    bool matchingStrides(const Strides<DIM> & s)const{
        return true;
    }

    constexpr bool matchingStrides()const{
        return true;
    }

    // only valid if matchin strides
    constexpr bool contiguous() const{
        return true;
    }
    // only valid if matchin strides
    bool contiguous(const Order & o) const{
        return true;
    }   


    very_inline value_type unsafeAccess(const uint64_t index)const{
        MULTI_ARRAY_CHECK(false,"cannot be called without coord");
        return value_type();
    }
    very_inline value_type unsafeAccess(const uint64_t index, const Coordinate<DIM>  & coordinate)const{
         return coordinate[AXIS];
    }

    class IterImpl{
    public:
       IterImpl(
           const SelfType & expression
       )
       : coord_(0){
       }
       void incrementCoordinate(const size_t axis){   
           ++coord_[axis];
       }
       void resetCoordinate(const size_t axis){   
           coord_[axis] = 0;
       }
       const value_type operator*() const{ 
           return coord_[AXIS];
       }
    private:
       TinyVector<value_type, DIM> coord_;
    }; 
    typedef IterImpl ExpressionPseudoIterator;
    typedef IterImpl ExpressionBroadcastPseudoIterator;
private:
    const Shape<DIM>   shape_;
    const Strides<DIM> strides_;
};




template<std::size_t DIM,class E, class T, class UNARY_FUNCTOR>
class UnaryViewExpression
: public ViewExpression<DIM, UnaryViewExpression<DIM, E, T, UNARY_FUNCTOR>, 
    typename UNARY_FUNCTOR::type >
    //typename meta::PromoteReal<T>::type >
{
public:
    typedef E ExpressionType;
    typedef typename ExpressionType::ViewExpressionMetaType ViewExpressionMetaType;
    typedef typename UNARY_FUNCTOR::type value_type;
    typedef UNARY_FUNCTOR UnaryFunctor;
    typedef ViewExpression<DIM, UnaryViewExpression<DIM, E, T, UNARY_FUNCTOR>,
        value_type> base;


    UnaryViewExpression(
        const ViewExpression<DIM, E, T>& e,
        const UNARY_FUNCTOR & functor = UNARY_FUNCTOR()
    )
    :   base(),
        e_(e), // cast!
        unaryFunctor_(functor),
        matchingStrides_(e.matchingStrides())
    {
        if(e_.empty()){
            throw CannotUseEmptyArrayInExpressionException();
        }
    }
    UnaryViewExpression(const UnaryViewExpression & other)
    :   base(),   
        e_(other.e_),
        unaryFunctor_(other.unaryFunctor_),
        matchingStrides_(other.matchingStrides_){
    }

    UnaryFunctor  unaryFunctor()const{
        return unaryFunctor_;
    }

    constexpr bool hasShape() const{ 
        return ViewExpressionMetaType::HasShapeType::value;
    }
    constexpr bool hasStrides() const{ 
        return ViewExpressionMetaType::HasStridesType::value;
    }

    bool needsBroadcasting() const{
        return e_.needsBroadcasting();
    }
    
    const Shape<DIM> & broadcastedShape()const{
        return e_.broadcastedShape();
    }
    const Strides<DIM> & strides()const{
        return e_.strides();
    }

    // we explicitly check in the constructor that
    // e_ is non empty
    constexpr bool empty() const{ 
        return false;
    }

    
    
    template<class VIEW>
    bool overlaps(const VIEW & v) const {
        return e_.overlaps(v);
    }

    const ExpressionType & inputExpression()const{
        return e_;
    }

    bool matchingStrides(const Strides<DIM> & s)const{
        if(this->hasStrides()){
            for(std::size_t a=0; a<DIM; ++a){
                const auto s1 = e_.strides(a);
                const auto s2 = s[a];
                if(s1!=0 && s2!=0 && s1!=s2){
                    return false;
                }
            }
            return true;
        }
        else{
            return true;
        }
    }
    bool matchingStrides()const{
        return matchingStrides_;
    }

    // only valid if matchin strides
    bool contiguous() const{
        return e_.contiguous();
    }

    // only valid if matchin strides
    bool contiguous(const Order & o) const{
        return e_.contiguous(o);
    }   


    template<class ITER>
    class IterImpl {
    public:
        IterImpl(
            const UnaryViewExpression<DIM, E, T, UNARY_FUNCTOR>& expression
        )
        :   unaryFunctor_(expression.unaryFunctor_),
            iterator_(expression.e_){
        }
        void incrementCoordinate(const size_t axis){   
            iterator_. incrementCoordinate(axis);
        }
        void resetCoordinate(const size_t axis){   
            iterator_. resetCoordinate(axis);
        }
        const value_type operator*() const{ 
            return unaryFunctor_(*iterator_); 
        }
    private:
        const UnaryFunctor & unaryFunctor_;
        ITER iterator_;
    };

    typedef IterImpl<typename E::ExpressionPseudoIterator>          ExpressionPseudoIterator;
    typedef IterImpl<typename E::ExpressionBroadcastPseudoIterator> ExpressionBroadcastPseudoIterator;
    



    very_inline value_type unsafeAccess(const uint64_t index)const{
        return unaryFunctor_(e_.unsafeAccess(index)); 
    }
    very_inline value_type unsafeAccess(const uint64_t index, const Coordinate<DIM> & coordinate)const{
        typedef typename  ViewExpressionMetaType::NeedsCoordinateType NeedsCoordinateType;
        if(NeedsCoordinateType::value){
            return unaryFunctor_(e_.unsafeAccess(index,coordinate)); 
        }
        else{
            return unaryFunctor_(e_.unsafeAccess(index)); 
        }
    }
private:

    const ExpressionType  e_;
    UnaryFunctor unaryFunctor_;
    bool matchingStrides_;
};







template<class T, std::size_t DIM, class E, class T_IN>
using CastExpr = 
    UnaryViewExpression<DIM, E, T_IN, functors::Cast<T> >;

template<class T, std::size_t DIM, class E, class T_IN>
inline CastExpr<T, DIM, E, T_IN>  castExpr(
    const ViewExpression<DIM, E, T_IN> & expression
){
    return CastExpr<T,DIM, E, T_IN>(expression);
}

template<class T, std::size_t DIM, class E, class T_IN>
inline CastExpr<T, DIM, E, T_IN>  castExpr(
    const ViewExpression<DIM, E, T_IN> & expression,
    const T dummy
){
    return CastExpr<T,DIM, E, T_IN>(expression);
}




template<std::size_t DIM,class E1, class T1, class E2, class T2, class BinaryFunctor>
class BinaryViewExpression
: public ViewExpression<DIM, BinaryViewExpression<DIM, E1, T1, E2, T2, BinaryFunctor>,
                        typename BinaryFunctor::type>
{
public:

    typedef E1 ExpressionType1;
    typedef E2 ExpressionType2;

    typedef typename ExpressionType1::ViewExpressionMetaType ViewExpressionMetaType1;
    typedef typename ExpressionType2::ViewExpressionMetaType ViewExpressionMetaType2;


    typedef ViewExpressionMeta<
        ViewExpressionMetaType1::NeedsCoordinateType::value                 || ViewExpressionMetaType2::NeedsCoordinateType::value, 
        ViewExpressionMetaType1::HasShapeType::value                        || ViewExpressionMetaType2::HasShapeType::value,
        ViewExpressionMetaType1::HasStridesType::value                      || ViewExpressionMetaType2::HasStridesType::value,
        ViewExpressionMetaType1::IsCompatibleWithAnyStridesType::value      && ViewExpressionMetaType2::IsCompatibleWithAnyStridesType::value
    > ViewExpressionMetaType;

    typedef typename BinaryFunctor::type value_type;


    typedef ViewExpression<DIM, BinaryViewExpression<DIM, E1, T1, E2, T2, BinaryFunctor>,
        value_type> base;

    BinaryViewExpression(
        const ViewExpression<DIM, E1, T1>& e1,
        const ViewExpression<DIM, E2, T2>& e2,
        const BinaryFunctor & binaryFunctor = BinaryFunctor()
    )
    :   base(),
        e1_(e1), // cast!
        e2_(e2), // cast!
        binaryFunctor_(binaryFunctor),
        matchingStrides_(),
        shapeInfo_(),
        broadcastedShape_(),
        strides_(HasNoStridesValue)
    {

        if(e1_.empty()){
            throw CannotUseEmptyArrayInExpressionException("first argument in binary expression is empty");
        }
        if(e2_.empty()){
            throw CannotUseEmptyArrayInExpressionException("second argument in binary expression is empty");
        }



        shapeInfo_ =  detail_expr::broadcastShapesWithShapeless(
                                        e1_.broadcastedShape(),
                                        e2_.broadcastedShape(), 
                                        this->broadcastedShape_);


        matchingStrides_ = this->matchingStridesImpl();
        if(matchingStrides_){
            this->initStrides();
        }
    }

    BinaryViewExpression(const BinaryViewExpression & other)
    :   base(),   
        e1_(other.e1_),
        e2_(other.e2_),
        binaryFunctor_(other.binaryFunctor_),
        matchingStrides_(other.matchingStrides_),
        shapeInfo_(other.shapeInfo_),
        broadcastedShape_(other.broadcastedShape_),
        strides_(other.strides_)
    {
        
    }


    bool needsBroadcasting() const{
        return shapeInfo_.needsBroadcasting;
    }

    constexpr bool hasShape() const{ 
        return ViewExpressionMetaType::HasShapeType::value;
    }
    constexpr bool hasStrides() const{ 
        return ViewExpressionMetaType::HasStridesType::value;
    }
    constexpr bool empty()const{
        return false;
    }
    
    const Shape<DIM> & broadcastedShape()const{
        return broadcastedShape_;
    }
    
    template<class VIEW>
    bool overlaps(const VIEW & v) const {
        return e1_.overlaps(v) || e2_.overlaps(v); 
    }   

    const Strides<DIM> & strides()const{
        return strides_;
    }


  

    bool matchingStrides(const Strides<DIM> & s)const{

        if(ViewExpressionMetaType::IsCompatibleWithAnyStridesType::value || !ViewExpressionMetaType::HasStridesType::value){
            return true;
        }
        else{
            for(std::size_t a=0; a<DIM; ++a){
                const auto s1 = strides_[a];
                const auto s2 = s[a];

                if(s1!=0 && s2!=0 && s1!=s2){
                    return false;
                }
            }
            return true;
        }
    }

    bool matchingStrides()const{
        return matchingStrides_;
    }
    

    // only valid if matchin strides
    bool contiguous() const{
        return e1_.contiguous();
    }

    // only valid if matchin strides
    bool contiguous(const Order & o) const{
        return e1_.contiguous(o);
    }

    very_inline value_type unsafeAccess(const uint64_t index)const{
        return binaryFunctor_(
            e1_.unsafeAccess(index),
            e2_.unsafeAccess(index)
        ); 
    }

    very_inline value_type unsafeAccess(const uint64_t index, const Coordinate<DIM>  & coordinate)const{

        typedef typename  ViewExpressionMetaType::NeedsCoordinateType NeedsCoordinateType;

        if(NeedsCoordinateType::value){
            return binaryFunctor_(
                e1_.unsafeAccess(index,coordinate),
                e2_.unsafeAccess(index,coordinate)
            ); 
        }
        else{
            return binaryFunctor_(
                e1_.unsafeAccess(index),
                e2_.unsafeAccess(index)
            );
        }
    }  


    const BinaryFunctor & binaryFunctor()const{
        return binaryFunctor_;
    }

    const ExpressionType1 inputExpression1()const{
       return e1_;
    }
    const ExpressionType2 inputExpression2()const{
       return e2_;
    }

    template<class ITER_A, class ITER_B>
    class IterImpl {
    public:
        IterImpl(
            const BinaryViewExpression<DIM, E1, T1, E2, T2, BinaryFunctor>& expression
        )
        :   binaryFunctor_(expression.binaryFunctor_),
            iterator1_(expression.e1_),
            iterator2_(expression.e2_){

        }

        void incrementCoordinate(const size_t axis){   
            iterator1_. incrementCoordinate(axis);
            iterator2_. incrementCoordinate(axis); 
        }

        void resetCoordinate(const size_t axis){   
            iterator1_. resetCoordinate(axis);
            iterator2_. resetCoordinate(axis); 
        }
        const value_type operator*() const{ 
            return binaryFunctor_(*iterator1_, *iterator2_); 
        }
    private:
        BinaryFunctor binaryFunctor_;
        typename E1::ExpressionPseudoIterator iterator1_;
        typename E2::ExpressionPseudoIterator iterator2_;
    };

    typedef IterImpl<typename E1::ExpressionPseudoIterator, 
                     typename E2::ExpressionPseudoIterator
    > ExpressionPseudoIterator;

    typedef IterImpl<typename E1::ExpressionBroadcastPseudoIterator, 
                     typename E2::ExpressionBroadcastPseudoIterator> 
    ExpressionBroadcastPseudoIterator;
private:
    void initStrides(){
        if(!ViewExpressionMetaType::HasStridesType::value){
            // do nothing
        }
        else if(ViewExpressionMetaType1::HasStridesType::value && !ViewExpressionMetaType2::HasStridesType::value){
            strides_ = e1_.strides();
        }
        else if(!ViewExpressionMetaType1::HasStridesType::value && ViewExpressionMetaType2::HasStridesType::value){
            strides_ = e1_.strides();
        }
        else{
            for(std::size_t a=0; a<DIM; ++a){
                const auto s1 = e1_.strides()[a];
                const auto s2 = e2_.strides()[a];
                strides_[a] = std::max(s1, s2);
            }
        }
    }
    very_inline bool matchingStridesImpl()const{

        if(e1_.matchingStrides() && e2_.matchingStrides()){
            if(
                ViewExpressionMetaType1::IsCompatibleWithAnyStridesType::value || 
                ViewExpressionMetaType2::IsCompatibleWithAnyStridesType::value || 
                !ViewExpressionMetaType1::HasStridesType::value || 
                !ViewExpressionMetaType2::HasStridesType::value 
            ){
                return true;
            }
            else{
                for(std::size_t a=0; a<DIM; ++a){
                    const auto s1 = e1_.strides()[a];
                    const auto s2 = e2_.strides()[a];
                    if(s1!=0 && s2!=0 && s1!=s2){
                        return false;
                    }
                }
                return true;
            }
        }
        else{
            return false;
        }
    }
    const ExpressionType1  e1_;
    const ExpressionType2  e2_;
    BinaryFunctor binaryFunctor_;
    bool matchingStrides_;
    detail_expr::ShapeInfo shapeInfo_;
    Shape<DIM> broadcastedShape_;
    Strides<DIM> strides_;
};



// this will optimize unary expressions
template<std::size_t DIM, class E, class T, class UNARY_FUNCTOR>
struct UnaryExprOpt
{
    typedef UnaryViewExpression<DIM, E, T, UNARY_FUNCTOR> type;
    inline static type op(
        const ViewExpression<DIM, E, T> & expression, 
        UNARY_FUNCTOR && unaryFunctor = UNARY_FUNCTOR()
    ){
        return type(
            expression, 
            std::forward<UNARY_FUNCTOR>(unaryFunctor)
        );
    }
};







template<
    std::size_t DIM,
    class E1, class T1,
    class E2, class T2, 
    class E3, class T3, 
    class E4, class T4, 
    class QuaquaternaryFunctor
>
class QuaternaryViewExpression
: public ViewExpression
<
    DIM, 
    QuaternaryViewExpression<DIM, 
        E1, T1, E2, T2, E3, T3, E4, T4,
        QuaquaternaryFunctor
    >,
    typename QuaquaternaryFunctor::type
>
{
public:

    typedef QuaternaryViewExpression<DIM, E1, T1, E2, T2, E3, T3,E4, T4, 
        QuaquaternaryFunctor> SelfType;
    typedef E1 ExpressionType1;
    typedef E2 ExpressionType2;
    typedef E3 ExpressionType3;
    typedef E4 ExpressionType4;

    typedef typename ExpressionType1::ViewExpressionMetaType ViewExpressionMetaType1;
    typedef typename ExpressionType2::ViewExpressionMetaType ViewExpressionMetaType2;
    typedef typename ExpressionType3::ViewExpressionMetaType ViewExpressionMetaType3;
    typedef typename ExpressionType4::ViewExpressionMetaType ViewExpressionMetaType4;

    typedef ViewExpressionMeta<
        ViewExpressionMetaType1::NeedsCoordinateType::value                 || ViewExpressionMetaType2::NeedsCoordinateType::value || 
        ViewExpressionMetaType3::NeedsCoordinateType::value                 || ViewExpressionMetaType4::NeedsCoordinateType::value 
        , 
        ViewExpressionMetaType1::HasShapeType::value                        || ViewExpressionMetaType2::HasShapeType::value ||
        ViewExpressionMetaType2::HasShapeType::value                        || ViewExpressionMetaType4::HasShapeType::value
        ,
        ViewExpressionMetaType1::HasStridesType::value                      || ViewExpressionMetaType2::HasStridesType::value ||
        ViewExpressionMetaType3::HasStridesType::value                      || ViewExpressionMetaType4::HasStridesType::value
        ,
        ViewExpressionMetaType1::IsCompatibleWithAnyStridesType::value      && ViewExpressionMetaType2::IsCompatibleWithAnyStridesType::value &&
        ViewExpressionMetaType4::IsCompatibleWithAnyStridesType::value      && ViewExpressionMetaType3::IsCompatibleWithAnyStridesType::value
    > ViewExpressionMetaType;

    typedef typename QuaquaternaryFunctor::type value_type;


    QuaternaryViewExpression(
        const ViewExpression<DIM, E1, T1>& e1,
        const ViewExpression<DIM, E2, T2>& e2,
        const ViewExpression<DIM, E3, T3>& e3,
        const ViewExpression<DIM, E4, T4>& e4,
        const QuaquaternaryFunctor & quaternaryFunctor = QuaquaternaryFunctor()
    )
    :   e1_(e1), // cast!
        e2_(e2), // cast!
        e3_(e3), // cast!
        e4_(e4), // cast!
        quaternaryFunctor_(quaternaryFunctor),
        broadcastedShape_(HasNoShapeValue),
        needsBroadcasting_(e1_.needsBroadcasting() || e2_.needsBroadcasting() || 
                           e3_.needsBroadcasting() || e4_.needsBroadcasting()),
        matchingStrides_(),
        strides_(HasNoStridesValue)
    {

        if(e1_.empty()){
            throw CannotUseEmptyArrayInExpressionException("first argument in quaternary expression is empty");
        }
        if(e2_.empty()){
            throw CannotUseEmptyArrayInExpressionException("second argument in quaternary expression is empty");
        }
        if(e3_.empty()){
            throw CannotUseEmptyArrayInExpressionException("third argument in quaternary expression is empty");
        }
        if(e4_.empty()){
            throw CannotUseEmptyArrayInExpressionException("forth argument in quaternary expression is empty");
        }

        matchingStrides_ = this->matchingStridesImpl();
        if(matchingStrides_){
            for(auto d=0; d<DIM;++d){
                strides_[d] = getStride(d);
            }
        }



        // broadcasted shape
        Shape<DIM> s12,s34;
        auto shapeInfoInfo12 = detail_expr::broadcastShapesWithShapeless(e1_.broadcastedShape(), e2_.broadcastedShape(), s12);
        auto shapeInfoInfo34 = detail_expr::broadcastShapesWithShapeless(e2_.broadcastedShape(), e3_.broadcastedShape(), s34);
        auto shapeInfo       = detail_expr::broadcastShapesWithShapeless(s12, s34, broadcastedShape_);
        // do we need broadcasting ?
        needsBroadcasting_ = needsBroadcasting_ || 
                             shapeInfoInfo12.needsBroadcasting || 
                             shapeInfoInfo34.needsBroadcasting ||
                             shapeInfo.needsBroadcasting;
    }

    QuaternaryViewExpression(const QuaternaryViewExpression & other)
    :   e1_(other.e1_),
        e2_(other.e2_),
        e3_(other.e3_),
        e4_(other.e4_),
        quaternaryFunctor_(other.quaternaryFunctor_),
        broadcastedShape_(other.broadcastedShape_),
        needsBroadcasting_(other.needsBroadcasting_),
        matchingStrides_(other.matchingStrides_),
        strides_(other.strides_){
    }


    constexpr bool empty() const{ 
        return false;
    }

    constexpr bool hasShape() const{ 
        return ViewExpressionMetaType::HasShapeType::value;
    }

    constexpr bool hasStrides() const{ 
        return ViewExpressionMetaType::HasStridesType::value;
    }

    const Shape<DIM> & broadcastedShape()const{
        return broadcastedShape_;
    }
    const Strides<DIM> & strides()const{
        return strides_;
    }
    template<class VIEW>
    bool overlaps(const VIEW & v) const {
        return e1_.overlaps(v) || e2_.overlaps(v) || e3_.overlaps(v) || e4_.overlaps(v); 
    }


    bool needsBroadcasting()const{
        return needsBroadcasting_;
    }

    // this function can only be called
    // iff and iff only this->matchingStrides()==true
    bool matchingStrides(const Strides<DIM> & s)const{

        if(ViewExpressionMetaType::IsCompatibleWithAnyStridesType::value){
            return true;
        }
        else{
            for(std::size_t a=0; a<DIM; ++a){
                const auto s1 = this->strides()[a];
                const auto s2 = s[a];

                if(s1!=0 && s2!=0 && s1!=s2){
                    return false;
                }
            }
            return true;
        }
    }

    bool matchingStrides()const{
        return matchingStrides_;
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
    very_inline value_type unsafeAccess(const uint64_t index)const{
        return quaternaryFunctor_(
            e1_.unsafeAccess(index),
            e2_.unsafeAccess(index),
            e3_.unsafeAccess(index),
            e4_.unsafeAccess(index)
        ); 
    }

    // a call to this function is valid 
    // if and if only this->matchingStrides()==true
    very_inline value_type unsafeAccess(const uint64_t index, const Coordinate<DIM>  & coordinate)const{

        typedef typename  ViewExpressionMetaType::NeedsCoordinateType NeedsCoordinateType;

        if(NeedsCoordinateType::value){
            return quaternaryFunctor_(
                e1_.unsafeAccess(index,coordinate),
                e2_.unsafeAccess(index,coordinate),
                e3_.unsafeAccess(index,coordinate),
                e4_.unsafeAccess(index,coordinate)
            ); 
        }
        else{
            return quaternaryFunctor_(
                e1_.unsafeAccess(index),
                e2_.unsafeAccess(index),
                e3_.unsafeAccess(index),
                e4_.unsafeAccess(index)
            );
        }
    }  


    const QuaquaternaryFunctor & quaternaryFunctor()const{
        return quaternaryFunctor_;
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
    const ExpressionType4 inputExpression4()const{
       return e4_;
    }

    template<class ITER_1, class ITER_2, class ITER_3, class ITER_4>
    class IterImpl {
    public:
        IterImpl(
            const SelfType & expression
        )
        :   self_(expression),
            iterator1_(expression.e1_),
            iterator2_(expression.e2_),
            iterator3_(expression.e3_),
            iterator4_(expression.e4_)
        {

        }

        void incrementCoordinate(const size_t axis){   
            iterator1_. incrementCoordinate(axis);
            iterator2_. incrementCoordinate(axis); 
            iterator3_. incrementCoordinate(axis); 
            iterator4_. incrementCoordinate(axis); 
        }

        void resetCoordinate(const size_t axis){   
            iterator1_. resetCoordinate(axis);
            iterator2_. resetCoordinate(axis); 
            iterator3_. resetCoordinate(axis); 
            iterator4_. resetCoordinate(axis); 
        }
        const value_type operator*() const{ 
            return self_.quaternaryFunctor()(*iterator1_, *iterator2_, *iterator3_, *iterator4_); 
        }
    private:
        const SelfType & self_;
        ITER_1 iterator1_;
        ITER_2 iterator2_;
        ITER_3 iterator3_;
        ITER_4 iterator4_;
    };


    typedef IterImpl<typename E1::ExpressionPseudoIterator, 
                     typename E2::ExpressionPseudoIterator,
                     typename E3::ExpressionPseudoIterator, 
                     typename E4::ExpressionPseudoIterator
    > ExpressionPseudoIterator;

    typedef IterImpl<typename E1::ExpressionBroadcastPseudoIterator, 
                     typename E2::ExpressionBroadcastPseudoIterator,
                     typename E3::ExpressionBroadcastPseudoIterator, 
                     typename E4::ExpressionBroadcastPseudoIterator
    > ExpressionBroadcastPseudoIterator;





private:
    bool matchingStridesImpl()const{

        if(e1_.matchingStrides() && e2_.matchingStrides() && e3_.matchingStrides() && e4_.matchingStrides()){
            auto match = [](const int64_t sa, const int64_t sb){
                if(sa == 0 || sb == 0)
                    return true;
                else
                    return sa == sb;
            };

            for(std::size_t a=0; a<DIM; ++a){
                const auto s1  = ViewExpressionMetaType1::HasStridesType::value ? e1_.strides()[a] : int64_t(0);
                const auto s2  = ViewExpressionMetaType2::HasStridesType::value ? e2_.strides()[a] : int64_t(0);
                const auto s3  = ViewExpressionMetaType3::HasStridesType::value ? e3_.strides()[a] : int64_t(0);
                const auto s4  = ViewExpressionMetaType3::HasStridesType::value ? e4_.strides()[a] : int64_t(0);
                if( !match(s1, s2) || !match(s1, s3) || !match(s1, s4))
                    return false;
            }
            return true;
        }
        else{
            return false;
        }
    }

    int64_t getStride(const std::size_t a)const{
        if(e1_.hasStrides()){
            return e1_.strides()[a];
        }
        else if(e2_.hasStrides()){
            return e2_.strides()[a];
        }
        else if(e3_.hasStrides()){
            return e3_.strides()[a];
        }
        else if(e4_.hasStrides()){
            return e4_.strides()[a];
        }
        else{
            MULTI_ARRAY_CHECK(false, "all 4 don't have a stride");
        }
    }

    const ExpressionType1  e1_;
    const ExpressionType2  e2_;
    const ExpressionType3  e3_;
    const ExpressionType4  e4_;
    QuaquaternaryFunctor quaternaryFunctor_;

    Shape<DIM> broadcastedShape_;
    bool needsBroadcasting_;
    bool matchingStrides_;
    Strides<DIM> strides_;
};









// this will optimize binary expressions
template<std::size_t DIM, class E1, class T1, class E2, class T2, class BINARY_FUNCTOR>
struct BinaryExprOpt
{
    typedef BinaryViewExpression<DIM, E1, T1, E2, T2, BINARY_FUNCTOR> type;
    very_inline static type op(
        const ViewExpression<DIM, E1, T1> & expression1, 
        const ViewExpression<DIM, E2, T2> & expression2, 
        const BINARY_FUNCTOR & binaryFunctor = BINARY_FUNCTOR()
    ){
        return type(
            expression1, 
            expression2,
            binaryFunctor
        );
    }
};




// this will optimize quaternary expressions
template<std::size_t DIM, class E1, class T1, class E2, class T2,class E3, class T3,class E4, class T4,class QUATERNARY_FUNCTOR>
struct QuaternaryExprOpt
{
    typedef QuaternaryViewExpression<DIM, E1, T1, E2, T2, E3, T3, E4, T4, QUATERNARY_FUNCTOR> type;
    very_inline static type op(
        const ViewExpression<DIM, E1, T1> & expression1, 
        const ViewExpression<DIM, E2, T2> & expression2, 
        const ViewExpression<DIM, E3, T3> & expression3,
        const ViewExpression<DIM, E4, T4> & expression4,
        const QUATERNARY_FUNCTOR & quaternaryFunctor = QUATERNARY_FUNCTOR()
    ){
        return type(
            expression1, 
            expression2,
            expression3,
            expression4,
            quaternaryFunctor
        );
    }
};






// optimize unary of binary
template<std::size_t DIM, class E1, class T1, class E2, class T2, class BINARY_FUNCTOR, class UNARY_FUNCTOR>
struct UnaryExprOpt<
    DIM,
    BinaryViewExpression<DIM, E1, T1, E2, T2, BINARY_FUNCTOR>,
    typename BINARY_FUNCTOR::type,
    UNARY_FUNCTOR
>
{
    // the input to the unary functor
    typedef BinaryViewExpression<DIM, E1, T1, E2, T2, BINARY_FUNCTOR> ExpressionType;
    // we make a new binary functor
    struct CombindedFunctor{
        typedef typename UNARY_FUNCTOR::type type;
        CombindedFunctor(
            const UNARY_FUNCTOR  & unaryFunctor = UNARY_FUNCTOR(),
            const BINARY_FUNCTOR & binaryFunctor = BINARY_FUNCTOR()
        )
        :   unaryFunctor_(unaryFunctor),
            binaryFunctor_(binaryFunctor){
        }    
        template<class A, class B>
        type operator()(const A & a, const B & b)const{
            return unaryFunctor_(binaryFunctor_(a, b));
        }
        const UNARY_FUNCTOR   unaryFunctor_;
        const BINARY_FUNCTOR  binaryFunctor_;
    };
    // the new binary functor
    typedef BinaryExprOpt<DIM, E1, T1, E2, T2, CombindedFunctor> BinaryExprOptType;
    typedef typename BinaryExprOptType::type type;

    // static factory function
    very_inline static type op(
        const ExpressionType & expression, 
        UNARY_FUNCTOR && unaryFunctor = UNARY_FUNCTOR()
    ){
        return BinaryExprOptType::op(
            expression.inputExpression1(),
            expression.inputExpression2(), 
            CombindedFunctor(unaryFunctor, expression.binaryFunctor())
        );
    }
};


// optimize unary of unary
template<std::size_t DIM, class E, class T,  class INNER_UNARY_FUNCTOR, class UNARY_FUNCTOR>
struct UnaryExprOpt<
    DIM,
    UnaryViewExpression<DIM, E, T, INNER_UNARY_FUNCTOR>,
    typename INNER_UNARY_FUNCTOR::type,
    UNARY_FUNCTOR
>
{
    // the input to the unary functor
    typedef UnaryViewExpression<DIM, E, T, INNER_UNARY_FUNCTOR> ExpressionType;
    // we make a new binary functor
    struct CombindedFunctor{
        typedef typename UNARY_FUNCTOR::type type;
        CombindedFunctor(
            const UNARY_FUNCTOR  & unaryFunctor = UNARY_FUNCTOR(),
            const INNER_UNARY_FUNCTOR & innerUnaryFunctor = INNER_UNARY_FUNCTOR()
        )
        :   unaryFunctor_(unaryFunctor),
            innerUnaryFunctor_(innerUnaryFunctor){
        }    
        template<class A>
        type operator()(const A & a)const{
            return unaryFunctor_(innerUnaryFunctor_(a));
        }
        const UNARY_FUNCTOR   unaryFunctor_;
        const INNER_UNARY_FUNCTOR  innerUnaryFunctor_;
    };

    // do it recursive
    typedef UnaryExprOpt<DIM, E, T, CombindedFunctor> UnaryExprOptType;
    typedef typename UnaryExprOptType::type type;

    very_inline static type op(
        const ExpressionType & expression, 
        UNARY_FUNCTOR && unaryFunctor = UNARY_FUNCTOR()
    ){
        return UnaryExprOptType::op(
            expression.inputExpression(),
            CombindedFunctor(unaryFunctor, expression.unaryFunctor())
        );
    }
};













// optimize binary of (unary, unary)
template<
    std::size_t DIM, 
    class E1, class T1, class UNARY_FUNCTOR_1,
    class E2, class T2, class UNARY_FUNCTOR_2,
    class BINARY_FUNCTOR
>
struct BinaryExprOpt<
    DIM,
    UnaryViewExpression<DIM, E1, T1, UNARY_FUNCTOR_1>,
    typename UNARY_FUNCTOR_1::type,
    UnaryViewExpression<DIM, E2, T2, UNARY_FUNCTOR_2>,
    typename UNARY_FUNCTOR_2::type,
    BINARY_FUNCTOR
>
{

    struct CombinedFunctor{
        CombinedFunctor(
            const BINARY_FUNCTOR  & binaryFunctor = BINARY_FUNCTOR(),
            const UNARY_FUNCTOR_1 & unaryFunctor1 = UNARY_FUNCTOR_1(),
            const UNARY_FUNCTOR_2 & unaryFunctor2 = UNARY_FUNCTOR_2()
        )
        :   binaryFunctor_(binaryFunctor),
            unaryFunctor1_(unaryFunctor1),
            unaryFunctor2_(unaryFunctor2){
        }
        typedef typename BINARY_FUNCTOR::type type;
        template<class A, class B>
        type operator()(const A & a, const B & b)const{
            return binaryFunctor_(unaryFunctor1_(a), unaryFunctor2_(b));
        }
        const BINARY_FUNCTOR  binaryFunctor_;
        const UNARY_FUNCTOR_1 unaryFunctor1_;
        const UNARY_FUNCTOR_2 unaryFunctor2_;
    };

    typedef BinaryExprOpt<DIM, E1, T1, E2, T2, CombinedFunctor> BinaryExprOptType;
    typedef typename BinaryExprOptType::type type;

    very_inline static type op(
        const UnaryViewExpression<DIM, E1, T1, UNARY_FUNCTOR_1> & e1,
        const UnaryViewExpression<DIM, E2, T2, UNARY_FUNCTOR_2> & e2,
        const BINARY_FUNCTOR & binaryFunctor = BINARY_FUNCTOR()
    ){
        return BinaryExprOptType::op(e1.inputExpression(), e2.inputExpression(), 
            CombinedFunctor(binaryFunctor, e1.unaryFunctor(), e2.unaryFunctor()));
    }

};



#if 0
// optimize binary of (binary, binary)
// will lead to a quaternary expression
template<
    std::size_t DIM, 

    class E1_A, class T1_A, class E1_B, class T1_B,  class BINARY_FUNCTOR_1,
    class E2_A, class T2_A, class E2_B, class T2_B,  class BINARY_FUNCTOR_2,
    class BINARY_FUNCTOR
>
struct BinaryExprOpt<
    DIM,
    BinaryViewExpression<DIM, E1_A, T1_A, E1_B, T1_B, BINARY_FUNCTOR_1>,
    typename BINARY_FUNCTOR_1::type,
    BinaryViewExpression<DIM, E2_A, T2_A, E2_B, T2_B, BINARY_FUNCTOR_2>,
    typename BINARY_FUNCTOR_2::type,
    BINARY_FUNCTOR
>
{

    // struct CombinedFunctor{
    //     CombinedFunctor(
    //         const BINARY_FUNCTOR  & binaryFunctor = BINARY_FUNCTOR(),
    //         const BINARY_FUNCTOR_1 & binaryFunctor1 = BINARY_FUNCTOR_1(),
    //         const BINARY_FUNCTOR_2 & binaryFunctor2 = BINARY_FUNCTOR_2()
    //     )
    //     :   binaryFunctor_(binaryFunctor),
    //         binaryFunctor1_(binaryFunctor1),
    //         binaryFunctor2_(binaryFunctor2){
    //     }
    //     typedef typename BINARY_FUNCTOR::type type;
    //     template<class A, class B, class C, class D>
    //     very_inline type operator()(const A & a, const B & b, const C & c , const D & d)const{
    //         return binaryFunctor_(binaryFunctor1_(a,b), binaryFunctor2_(c,d));
    //     }
    //     const BINARY_FUNCTOR  binaryFunctor_;
    //     const BINARY_FUNCTOR_1 binaryFunctor1_;
    //     const BINARY_FUNCTOR_2 binaryFunctor2_;
    // };


    typedef functors::BinaryFunctorOfBinaryFunctorAndBinaryFunctor<
        BINARY_FUNCTOR, BINARY_FUNCTOR_1, BINARY_FUNCTOR_2
    > CombinedFunctor;





    //typedef BinaryExprOpt<DIM, E1, T1, E2, T2, CombinedFunctor> BinaryExprOptType;
    //typedef typename BinaryExprOptType::type type;


    typedef QuaternaryExprOpt<DIM, E1_A, T1_A, E1_B, T1_B, E2_A, T2_A, E2_B, T2_B, CombinedFunctor> QuaternaryExprOptType;
    typedef typename QuaternaryExprOptType::type type;

    inline static type op(
        const BinaryViewExpression<DIM, E1_A, T1_A, E1_B, T1_B, BINARY_FUNCTOR_1> & e1,
        const BinaryViewExpression<DIM, E2_A, T2_A, E2_B, T2_B, BINARY_FUNCTOR_2> & e2,
        const BINARY_FUNCTOR & binaryFunctor = BINARY_FUNCTOR()
    ){
        return QuaternaryExprOptType::op(
            e1.inputExpression1(), e1.inputExpression2(), 
            e2.inputExpression1(), e2.inputExpression2(), 
            CombinedFunctor(binaryFunctor, e1.binaryFunctor(), e2.binaryFunctor())
        );
    }

};
#endif
























}

