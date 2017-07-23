#pragma once









#include <array>
#include "multi_array/meta/variadic.hxx"
#include "multi_array/order.hxx"
#include "multi_array/misc.hxx"
#include "multi_array/detail/for_each.hxx"

namespace multi_array{


template<std::size_t DIM, class E, class T>
class ViewExpression;
template<std::size_t DIM, class E, class T, class UnaryFunctor>
class UnaryViewExpression;
template<std::size_t DIM, class E1, class T1, class E2, class T2, class BinaryFunctor>
class BinaryViewExpression;

template<class T, std::size_t DIM, bool IS_CONST>
class SmartMultiArray;


namespace detail_op{


    template<class LHS, class RHS, bool CAN_OVERLAP>
    struct AssignHelper;



    // assignment helper for view expressions
    template<class LHS, class RHS, bool CAN_OVERLAP, class VIEW_EXPRESSION_META_TYPE>
    struct AssignHelperExpression;




    template<class LHS, std::size_t DIM, class E, class U>
    inline void assignPseudoIterFallback(
        LHS & lhs, 
        const ViewExpression<DIM, E, U> & expr
    ){

        const E & e = static_cast<const E &>(expr);
        typename E::ExpressionPseudoIterator eIter(e);
        uint64_t thisOffset = 0;
        Coordinate<DIM> coordinate(0);
        auto data = lhs.data();

        const auto & shape = lhs.shape();
        const auto & strides = lhs.strides();
        for(;;){

            data[thisOffset] = *eIter;

            for(int j=int(DIM)-1; j>=0; --j) {
                if(coordinate[j]+1 == shape[j]) {
                    if(j == 0) {
                        return;
                    }
                    else {
                        thisOffset -= coordinate[j] * strides[j];
                        eIter.resetCoordinate(j);
                        coordinate[j] = 0;
                    }
                }
                else {
                    thisOffset += strides[j];
                    eIter.incrementCoordinate(j);
                    ++coordinate[j];
                    break;
                }      
            }
        }
    }



    template<class LHS, std::size_t DIM, class E, class U>
    inline void assignNeedsBroadcastingFallback(
        LHS & lhs, 
        const ViewExpression<DIM, E, U> & expr
    ){

        const E & e = static_cast<const E &>(expr);
        typename E::ExpressionBroadcastPseudoIterator eIter(e);
        uint64_t thisOffset = 0;
        Coordinate<DIM> coordinate(0);
        auto data = lhs.data();

        const auto & shape = lhs.shape();
        const auto & strides = lhs.strides();
        for(;;){

            data[thisOffset] = *eIter;

            for(int j=int(DIM)-1; j>=0; --j) {
                if(coordinate[j]+1 == shape[j]) {
                    if(j == 0) {
                        return;
                    }
                    else {
                        thisOffset -= coordinate[j] * strides[j];
                        eIter.resetCoordinate(j);
                        coordinate[j] = 0;
                    }
                }
                else {
                    thisOffset += strides[j];
                    eIter.incrementCoordinate(j);
                    ++coordinate[j];
                    break;
                }      
            }
        }
    }


    // NEEDS_COORDINATE = FALSE, 
    // HAS_SHAPE = TRUE
    template<class LHS, class RHS, bool CAN_OVERLAP, bool HAS_STRIDES, bool COMPATIBLE_WITH_ANY>
    struct AssignHelperExpression<LHS, RHS, CAN_OVERLAP, ViewExpressionMeta<false, true, HAS_STRIDES, COMPATIBLE_WITH_ANY> > 
    {
        const static std::size_t DIM = LHS::DimensionType::value;
        inline static void op(LHS & lhs, const RHS & rhs){

            

            //or(auto i=0; i<DIM; ++i){
            //   std::cout<<"lhs "<<lhs.shape()[i]<<" rhs "<<rhs.broadcastedShape()[i]<<"\n";
            //


            const auto eShape =rhs.broadcastedShape(); 
            const auto wasEmpty = lhs.empty();
            if(wasEmpty){
                lhs.assign(eShape);
            }
            if(CAN_OVERLAP && !wasEmpty && rhs.overlaps(lhs)){
                const LHS tmpRhs(lhs.shape());
                AssignHelper<LHS, LHS, false>::op(lhs, tmpRhs);
            }
            else{

                if(!rhs.needsBroadcastingWithShape(lhs.shape())){

                    if(rhs.matchingStrides()){
                        
                        if(rhs.matchingStrides(lhs.strides())){

                            if(lhs.contiguous()){


                                // this is the most important loop
                                auto data = lhs.data();
                                for(auto i=0;i<lhs.size(); ++i){
                                    data[i] = rhs.unsafeAccess(i);
                                }
                            }
                            else{
                                auto data = lhs.data();
                                detail_for_each::forEachOffset(lhs,Order::ANY_ORDER,[&](const uint64_t offset){
                                    data[offset] = rhs.unsafeAccess(offset); 
                                });
                            }
                        }
                        else{
                            const auto & eStrides = rhs.strides();
                            auto data = lhs.data();
                            detail_for_each::forEachOffset(lhs.shape(),lhs.strides(),eStrides,Order::ANY_ORDER,[&](const uint64_t offsetA, const uint64_t offsetB){
                                data[offsetA] =  rhs.unsafeAccess(offsetB); 
                            }); 
                        }
                    }
                    else{
                        assignPseudoIterFallback(lhs, rhs);
                    }
                }
                else{
                    std::cout<<"whauuuuut\n";
                    assignNeedsBroadcastingFallback(lhs, rhs);
                }
            }            
        }
    };

    // NEEDS_COORDINATE = TRUE, 
    // HAS_SHAPE = TRUE
    template<class LHS, class RHS, bool CAN_OVERLAP, bool HAS_STRIDES, bool COMPATIBLE_WITH_ANY>
    struct AssignHelperExpression<LHS, RHS,CAN_OVERLAP, ViewExpressionMeta<true, true, HAS_STRIDES, COMPATIBLE_WITH_ANY> > 
    {
        inline static void op(LHS & lhs, const RHS & rhs){
            const auto eShape =rhs.broadcastedShape(); 
            const auto wasEmpty = lhs.empty();
            if(wasEmpty){
                lhs.assign(eShape);
            }
            if(CAN_OVERLAP && !wasEmpty && rhs.overlaps(lhs)){
                const LHS tmpRhs(lhs.shape());
                AssignHelper<LHS, LHS, false>::op(lhs, tmpRhs);
            }
            else{
                if(rhs.matchingStrides()){

                    if(rhs.matchingStrides(lhs.strides())){

                        auto data = lhs.data();
                        detail_for_each::forEachOffsetAndCoordinate(lhs.shape(),lhs.strides(),Order::ANY_ORDER,[&](
                            const uint64_t offset, const Coordinate<LHS::DimensionType::value>  & coord
                        ){
                            data[offset] =  rhs.unsafeAccess(offset,coord); 
                        });  
                    }
                    else{
                        auto data = lhs.data();
                        detail_for_each::forEachOffsetAndCoordinate(lhs.shape(),lhs.strides(),rhs.makeStrides(),Order::ANY_ORDER,[&](
                            const uint64_t offsetA, const uint64_t offsetB, const Coordinate<LHS::DimensionType::value>  & coord
                        ){
                            data[offsetA] =  rhs.unsafeAccess(offsetB,coord); 
                        });
                    }
                }
                else{
                    assignPseudoIterFallback(lhs, rhs);
                }
            }
        }
    };

    // NEEDS_COORDINATE = TRUE, 
    // HAS_SHAPE = FALSE
    template<class LHS, class RHS, bool CAN_OVERLAP, bool HAS_STRIDES, bool COMPATIBLE_WITH_ANY>
    struct AssignHelperExpression<LHS, RHS,CAN_OVERLAP, ViewExpressionMeta<true, false, HAS_STRIDES, COMPATIBLE_WITH_ANY> > 
    {
        inline static void op(LHS & lhs, const RHS & rhs){
            const auto eShape =rhs.broadcastedShape(); 
            if(lhs.empty()){
                throw CannotAssignShapelessExpressionToEmptyArrayException();
            }
            if(CAN_OVERLAP && rhs.overlaps(lhs)){
                const LHS tmpRhs(lhs.shape());
                AssignHelper<LHS, LHS, false>::op(lhs, tmpRhs);
            }
            else{
                MULTI_ARRAY_CHECK(rhs.matchingStrides(), "internal error");
                MULTI_ARRAY_CHECK(rhs.matchingStrides(lhs.strides()), "internal error");

                auto data = lhs.data();
                detail_for_each::forEachOffsetAndCoordinate(lhs.shape(),lhs.strides(),Order::ANY_ORDER,[&](
                    const uint64_t offset, const Coordinate<LHS::DimensionType::value>  & coord
                ){
                    data[offset] =  rhs.unsafeAccess(offset,coord); 
                });  
            }
        }
    };
    


    // assignment helper for view expressions like
    template<class LHS, class RHS, bool CAN_OVERLAP>
    struct AssignHelper 
    {
        inline static void op(LHS & lhs, const RHS & rhs){
            // the rhs is the expression:
            // lets see if broadcastinn is needed
            if(true || !rhs.needsBroadcastingWithShape(lhs.shape())){
                AssignHelperExpression<LHS, RHS,CAN_OVERLAP, typename RHS::ViewExpressionMetaType>::op(lhs, rhs);
            }
            else{
                assignNeedsBroadcastingFallback(lhs, rhs);
            }
        }
    };


    // assignment helper for arrays
    template<class LHS,class T, std::size_t DIM, bool IS_CONST, bool CAN_OVERLAP>
    struct AssignHelper<LHS, SmartMultiArray<T, DIM, IS_CONST>, CAN_OVERLAP>
    {
        typedef SmartMultiArray<T, DIM, IS_CONST> RHS;

        inline static void op(LHS & lhs, const RHS & rhs){

            // check for empty
            MULTI_ARRAY_CHECK(rhs.empty(), "rhs cannot empty in assignment");
            if(lhs.empty()){
                lhs.assign(rhs.shape());
            }

            // check for overlapping
            if(CAN_OVERLAP && rhs.overlaps(lhs)){
                const LHS tmpRhs(lhs.shape());
                AssignHelper<LHS, LHS, false>::op(lhs, tmpRhs);
            }
            else{
                auto dataLhs = lhs.data();
                const auto dataRhs = rhs.data();
                if(lhs.strides().relaxedEqual(rhs.strides())){    
                    if(lhs.contiguous()){
                        for(auto i=0; i<lhs.size(); ++i){
                            dataLhs[i] = dataRhs[i];
                        }
                    }
                    else{
                        detail_for_each::forEachOffset(lhs, Order::ANY_ORDER,[&](const int64_t offset){
                            dataLhs[offset] = dataRhs[offset];
                        });
                    }
                }
                else{
                    detail_for_each::forEachOffset(lhs.shape(), lhs.strides(),rhs.strides(), Order::C_ORDER,
                        [&](const uint64_t offset, const uint64_t otherOffset){
                         dataLhs[offset] = dataRhs[otherOffset];
                    });
                }
            }
        }
    };



    




}
}