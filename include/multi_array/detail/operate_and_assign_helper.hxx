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


    // NEEDS_COORDINATE = FALSE, 
    // IS_SHAPELESS = FALSE
    template<class LHS, class RHS, bool CAN_OVERLAP>
    struct AssignHelperExpression<LHS, RHS, CAN_OVERLAP, ViewExpressionMeta<false, false> > 
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
                    const auto eStrides = rhs.makeStrides();
                    if(eStrides.relaxedEqual(lhs.strides())){

                        if(lhs.contiguous()){


                            // this is the most important loop
                            auto data = lhs.data();

                            #if 1

                            #define ROUND_DOWN(x, s) ((x) & ~((s)-1))
                            const static uint64_t stepsize = 8;
                            uint64_t i = 0;
                            for(; i < ROUND_DOWN(lhs.size(), stepsize); i+=stepsize){

                                data[i    ] = rhs.unsafeAccess(i    );
                                data[i + 1] = rhs.unsafeAccess(i + 1);
                                data[i + 2] = rhs.unsafeAccess(i + 2);
                                data[i + 3] = rhs.unsafeAccess(i + 3);
                                data[i + 4] = rhs.unsafeAccess(i + 4);
                                data[i + 5] = rhs.unsafeAccess(i + 5);
                                data[i + 6] = rhs.unsafeAccess(i + 6);
                                data[i + 7] = rhs.unsafeAccess(i + 7);
                                //data[i + 8] = rhs.unsafeAccess(i + 8);
                                //data[i + 9] = rhs.unsafeAccess(i + 9);
                                //data[i + 10] = rhs.unsafeAccess(i + 10);
                                //data[i + 11] = rhs.unsafeAccess(i + 11);
                                //data[i + 12] = rhs.unsafeAccess(i + 12);
                                //data[i + 13] = rhs.unsafeAccess(i + 13);
                                //data[i + 14] = rhs.unsafeAccess(i + 14);
                                //data[i + 15] = rhs.unsafeAccess(i + 15);
                            }
                            for(; i < lhs.size(); i++){
                                data[i] = rhs.unsafeAccess(i );
                            }
                            #undef ROUND_DOWN

                            #else
                            

                            
                            for(auto i=0;i<lhs.size(); ++i){
                                data[i] = rhs.unsafeAccess(i);
                            }
                            #endif



                        }
                        else{
                            auto data = lhs.data();
                            detail_for_each::forEachOffset(lhs,Order::ANY_ORDER,[&](const uint64_t offset){
                                data[offset] = rhs.unsafeAccess(offset); 
                            });
                        }
                    }
                    else{
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
        }
    };

    // NEEDS_COORDINATE = TRUE, 
    // IS_SHAPELESS = FALSE
    template<class LHS, class RHS, bool CAN_OVERLAP>
    struct AssignHelperExpression<LHS, RHS,CAN_OVERLAP, ViewExpressionMeta<true, false> > 
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
    // IS_SHAPELESS = TRUE
    template<class LHS, class RHS, bool CAN_OVERLAP>
    struct AssignHelperExpression<LHS, RHS,CAN_OVERLAP, ViewExpressionMeta<true, true> > 
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
    struct AssignHelper : public
        AssignHelperExpression<LHS, RHS,CAN_OVERLAP, typename RHS::ViewExpressionMetaType>
    {
        
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