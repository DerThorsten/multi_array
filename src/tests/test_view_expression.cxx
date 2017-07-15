#include "doctest.h"

#include <iostream>
#include <memory>
#include <type_traits>

#include "multi_array/multi_array.hxx"
#include "multi_array/multi_array_factories.hxx"

#include "multi_array/meta.hxx"
#include "multi_array/runtime_check.hxx"

TEST_SUITE_BEGIN("ViewExpression");

TEST_CASE("[BinaryViewExpression] BinaryViewExpression::matchingStrides"){

    namespace ma = multi_array;

    SUBCASE("2D"){

        auto a =  ma::ones<int>(2,3);
        auto b =  ma::ones<int>(2,3);
        auto c =  ma::ones<int>(3,2);
        auto d =  c.transposedView();
        auto e =  ma::ones<int>(2,3);
        
        SUBCASE("matching strides simple"){
            auto ab = a + b ;   
            REQUIRE_EQ(ab.matchingStrides(),true);
            REQUIRE_EQ(ab.strides(0) , a.strides(0));
            REQUIRE_EQ(ab.strides(1) , a.strides(1));
        }
        SUBCASE("matching strides advanced A"){
            auto ab  = a + b;
            auto aba  = ab + a;
            
            REQUIRE_EQ(ab.strides(0) , a.strides(0));
            REQUIRE_EQ(ab.strides(1) , a.strides(1));
            REQUIRE_EQ(ab.matchingStrides(),true);

            REQUIRE_EQ(aba.strides(0) , a.strides(0));
            REQUIRE_EQ(aba.strides(1) , a.strides(1));
            REQUIRE_EQ(aba.matchingStrides(),true);
            
        }
        SUBCASE("matching strides advanced B"){
            auto ab  = a + b;
            auto aba = a + ab;
            REQUIRE_EQ(ab.strides(0) , a.strides(0));
            REQUIRE_EQ(ab.strides(1) , a.strides(1));
            REQUIRE_EQ(ab.matchingStrides(),true);
            REQUIRE_EQ(aba.strides(0) , a.strides(0));
            REQUIRE_EQ(aba.strides(1) , a.strides(1));
            REQUIRE_EQ(aba.matchingStrides(),true);    
        }
        SUBCASE("matching strides advanced C"){
            //auto ab  = (a + b);
            auto aba  = ((a + b)) + e;

            //REQUIRE_EQ(a.smartHandle().useCount(),1);
            REQUIRE_EQ(aba.strides(0) , 3);
            REQUIRE_EQ(aba.strides(1) , 1);
            REQUIRE_EQ(aba.matchingStrides(),true);

        }
        SUBCASE("matching strides advanced D"){
            //auto ab  = (a + b);
            auto thesum  = e + ((a + b + (a+b+(a+b)))) + e;
            REQUIRE_EQ(thesum.strides(0) , 3);
            REQUIRE_EQ(thesum.strides(1) , 1);
            REQUIRE_EQ(thesum.matchingStrides(),true);

        }

        SUBCASE("not matching strides simple"){
            auto ad = a + d ;   
            REQUIRE_EQ(ad.matchingStrides(),false);
        }

        SUBCASE("matching strides advanced"){
            auto abd  = a + b + d;
            REQUIRE_EQ(abd.matchingStrides(),false);
        }
    }
}

TEST_CASE("[BinaryViewExpression] BinaryViewExpression::unsafeAccess"){

    namespace ma = multi_array;

    SUBCASE("2D"){

        auto a =  ma::ones<int>(2,3);
        auto b =  ma::ones<int>(2,3);
        auto c =  ma::ones<int>(2,3);
        
        SUBCASE("Simple"){
            auto ab = a + b ;   
            REQUIRE_EQ(ab.matchingStrides(),true);
            REQUIRE_EQ(ab.contiguous(),true);
            REQUIRE_EQ(ab.strides(0) , a.strides(0));
            REQUIRE_EQ(ab.strides(1) , a.strides(1));

            REQUIRE_EQ(ab.unsafeAccess(0),2);
            REQUIRE_EQ(ab.unsafeAccess(1),2);
            REQUIRE_EQ(ab.unsafeAccess(2),2);
            REQUIRE_EQ(ab.unsafeAccess(3),2);
            REQUIRE_EQ(ab.unsafeAccess(4),2);
            REQUIRE_EQ(ab.unsafeAccess(5),2);
        }
        SUBCASE("Advanced"){
            auto theSum = a + b + c + (a+b);   
            REQUIRE_EQ(theSum.matchingStrides(),true);
            REQUIRE_EQ(theSum.contiguous(),true);
            REQUIRE_EQ(theSum.strides(0) , a.strides(0));
            REQUIRE_EQ(theSum.strides(1) , a.strides(1));

            REQUIRE_EQ(theSum.unsafeAccess(0),5);
            REQUIRE_EQ(theSum.unsafeAccess(1),5);
            REQUIRE_EQ(theSum.unsafeAccess(2),5);
            REQUIRE_EQ(theSum.unsafeAccess(3),5);
            REQUIRE_EQ(theSum.unsafeAccess(4),5);
            REQUIRE_EQ(theSum.unsafeAccess(5),5);
        }
    }
}


TEST_CASE("[BinaryViewExpression] BinaryViewExpression::unsafeAccess"){

    namespace ma = multi_array;

    SUBCASE("2D"){

        auto a =  ma::ones<int>(2,3);
        auto b =  ma::ones<int>(2,3);
        auto c =  ma::ones<int>(2,3);
        
        SUBCASE("Simple"){
            auto ab = a + b ;   
            REQUIRE_EQ(ab.matchingStrides(),true);
            REQUIRE_EQ(ab.contiguous(),true);
            REQUIRE_EQ(ab.strides(0) , a.strides(0));
            REQUIRE_EQ(ab.strides(1) , a.strides(1));
            REQUIRE_EQ(ab.unsafeAccess(0),2);
            REQUIRE_EQ(ab.unsafeAccess(1),2);
            REQUIRE_EQ(ab.unsafeAccess(2),2);
            REQUIRE_EQ(ab.unsafeAccess(3),2);
            REQUIRE_EQ(ab.unsafeAccess(4),2);
            REQUIRE_EQ(ab.unsafeAccess(5),2);
        }
        SUBCASE("Advanced"){
            auto theSum = a + b + c + (a+b);   
            REQUIRE_EQ(theSum.matchingStrides(),true);
            REQUIRE_EQ(theSum.contiguous(),true);
            REQUIRE_EQ(theSum.strides(0) , a.strides(0));
            REQUIRE_EQ(theSum.strides(1) , a.strides(1));
            REQUIRE_EQ(theSum.unsafeAccess(0),5);
            REQUIRE_EQ(theSum.unsafeAccess(1),5);
            REQUIRE_EQ(theSum.unsafeAccess(2),5);
            REQUIRE_EQ(theSum.unsafeAccess(3),5);
            REQUIRE_EQ(theSum.unsafeAccess(4),5);
            REQUIRE_EQ(theSum.unsafeAccess(5),5);
        }
    }
}

TEST_CASE("[BinaryViewExpression] assign to array"){

    namespace ma = multi_array;

    SUBCASE("2D"){

        auto a =  ma::ones<int>(2,3);
        auto b =  ma::ones<int>(2,3);
        auto c =  ma::ones<int>(2,3);
        
        SUBCASE("Simple matching types"){
            auto ab = a + b ;   
            REQUIRE_EQ(ab.matchingStrides(),true);
            REQUIRE_EQ(ab.contiguous(),true);
            REQUIRE_FALSE(ab.overlaps(c));
            REQUIRE(a.overlaps(a));
            REQUIRE(b.overlaps(b));
            REQUIRE(ab.overlaps(a));
            REQUIRE(ab.overlaps(b));
            c = a + b;
        }
        SUBCASE("matching strides but not dense"){
            auto a = ma::zeros<int>(12);
            auto b = ma::ones<int>(12);
            auto c = ma::arange(12);
            auto d = ma::arange(12,24);

            auto aa = a(ma::range(0,12,2));
            auto bb = b(ma::range(0,12,2));
            auto cc = c(ma::range(0,12,2));
            auto dd = d(ma::range(0,12,2));

            auto e = bb + cc + dd;

            REQUIRE_EQ(e.matchingStrides(),true);
            REQUIRE_EQ(e.contiguous(),false);




            SUBCASE("assign to non empty"){
                aa = e;

                REQUIRE_EQ(aa(0),  bb(0) + cc(0) + dd(0));
                REQUIRE_EQ(aa(0),  1 +     0+    + 12);

                REQUIRE_EQ(aa(1),  bb(1) + cc(1) + dd(1));
                REQUIRE_EQ(aa(1),  1 +     2+    + 14);

            }
            SUBCASE("assign to  empty"){
                ma::SmartMultiArray<int,1,false> emptyArray;
                emptyArray = e;

                REQUIRE_EQ(emptyArray(0),  bb(0) + cc(0) + dd(0));
                REQUIRE_EQ(emptyArray(0),  1 +     0+    + 12);

                REQUIRE_EQ(emptyArray(1),  bb(1) + cc(1) + dd(1));
                REQUIRE_EQ(emptyArray(1),  1 +     2+    + 14);
                
            }
        }
    }
}




TEST_CASE("[ViewExpression] math view expressions"){

    namespace ma = multi_array;
    
    


    SUBCASE("NoCast "){
        typedef double ValueType;
        const static std::size_t DIM = 2;

        // the coordinates as lazy  implicit 
        // shapeless zero overhead expression
        const static ma::CoordExpr<DIM,0> x0;
        const static ma::CoordExpr<DIM,1> x1;



        // some arrays with real data
        auto a = ma::zeros<ValueType>(5,5);
        auto b = ma::ones<ValueType>(5,5);
        REQUIRE_FALSE(a.empty());
        

        a = 5.0 - (x0 + ma::exp(x1));

        // the same as
        for(auto x0=0; x0<5; ++x0)
        for(auto x1=0; x1<5; ++x1){
            a(x0,x1) = 5.0 - (static_cast<ValueType>(x0) + std::exp(x1));
        }
    }
    #if 0
    SUBCASE("Implicit Coordinates"){

        typedef double ValueType;
        const static std::size_t DIM = 2;

        // the coordinates as lazy  implicit 
        // shapeless zero overhead expression
        const static ma::CoordExpr<DIM,0> x0;
        const static ma::CoordExpr<DIM,1> x1;

        //// constant value shapeless arrays
        //// as zero overhead expression
        //const static ma::ValueExpr<DIM,ValueType> ones(1);
        //const static ma::ValueExpr<DIM,ValueType> twos(2);

        // some arrays with real data
        auto a = ma::zeros<ValueType>(5,5);
        auto b = ma::ones<ValueType>(5,5);

        
        auto expression = 5.0 - (x0 + ma::exp(x1));
        REQUIRE_FALSE(expression.hasShape());

        // the same as
        for(auto x0=0; x0<5; ++x0)
        for(auto x1=0; x1<5; ++x1){
            a(x0,x1) = 5.0 - (ValueType(x0) + std::exp(x1));
        }
    }
    
    SUBCASE("1D"){
        auto a = ma::zeros<int>(5);
        auto b = ma::ones<int>(5);
        auto c = ma::arange(5);
        auto d = ma::arange(5,10);
        


        auto expression = ma::fabs(ma::exp(b+c+2)*3.5);
        REQUIRE(expression.hasShape());
        a = expression;

    }
    SUBCASE("All "){

        typedef double ValueType;
        const static std::size_t DIM = 2;

        // the coordinates as lazy  implicit 
        // shapeless zero overhead expression
        const static ma::CoordExpr<DIM,0> x0;
        const static ma::CoordExpr<DIM,1> x1;



        // some arrays with real data
        auto a = ma::zeros<ValueType>(5,5);
        auto b = ma::ones<ValueType>(5,5);
        REQUIRE_FALSE(a.empty());
        

        a = 5.0 - (ma::castExpr<ValueType>(x0) + ma::exp(x1));

        // the same as
        for(auto x0=0; x0<5; ++x0)
        for(auto x1=0; x1<5; ++x1){
            a(x0,x1) = 5.0 - (static_cast<ValueType>(x0) + std::exp(x1));
        }
    }
    #endif

}


TEST_CASE("[ViewExpression] blackbox test"){

    namespace ma = multi_array;
    
    
    SUBCASE("2D"){

        typedef int ValueType;
        const static std::size_t DIM = 2;

        // the coordinates as lazy  implicit 
        // shapeless zero overhead expression
        const static ma::CoordExpr<DIM,0> x0;
        const static ma::CoordExpr<DIM,1> x1;

        // create arrays with same data but
        // in different variants (strides, transposed etc)

        // to write into non empty
        auto resultA = ma::zeros<ValueType>(2,3);
        auto resultB = ma::zeros<ValueType>(3,2).transposedView();
        auto resultC = ma::zeros<ValueType>(4,6)(ma::range(0,4,2),ma::range(0,6,2));

        auto resultMixedA = ma::zeros<ValueType>(2,3);
        auto resultMixedB = ma::zeros<ValueType>(3,2).transposedView();
        auto resultMixedC = ma::zeros<ValueType>(4,6)(ma::range(0,4,2),ma::range(0,6,2));

        REQUIRE_EQ(resultC.shape(0),2);
        REQUIRE_EQ(resultC.shape(1),3);

        auto resultD = resultA.view();
        REQUIRE(resultA.equal(resultA));
        REQUIRE(resultA.equal(resultB));
        REQUIRE(resultA.equal(resultC));
        REQUIRE(resultA.equal(resultD));

        auto zerosA = ma::zeros<ValueType>(2,3);
        auto zerosB = ma::zeros<ValueType>(3,2).transposedView();
        auto zerosC = ma::zeros<ValueType>(4,6)(ma::range(0,4,2),ma::range(0,6,2));
        auto zerosD = zerosA.view();
        REQUIRE(zerosA.equal(zerosA));
        REQUIRE(zerosA.equal(zerosB));
        REQUIRE(zerosA.equal(zerosC));
        REQUIRE(zerosA.equal(zerosD));

        auto onesA = ma::ones<ValueType>(2,3);
        auto onesB = ma::ones<ValueType>(3,2).transposedView();
        auto onesC = ma::ones<ValueType>(4,6)(ma::range(0,4,2),ma::range(0,6,2));
        auto onesD = onesA.view();
        REQUIRE(onesA.equal(onesA));
        REQUIRE(onesA.equal(onesB));
        REQUIRE(onesA.equal(onesC));
        REQUIRE(onesA.equal(onesD));

        auto coordSumA = ma::ones<ValueType>(2,3);
        auto coordSumB = ma::ones<ValueType>(3,2).transposedView();
        auto coordSumC = ma::ones<ValueType>(4,6)(ma::range(0,4,2),ma::range(0,6,2));
        auto coordSumD = coordSumA.view();

        for(auto x0=0; x0<2; ++x0)
        for(auto x1=0; x1<3; ++x1){
            coordSumA(x0, x1) = x0 + x1;
            coordSumB(x0, x1) = x0 + x1;
            coordSumC(x0, x1) = x0 + x1;
        }
        REQUIRE(coordSumA.equal(coordSumA));
        REQUIRE(coordSumA.equal(coordSumB));
        REQUIRE(coordSumA.equal(coordSumC));
        REQUIRE(coordSumA.equal(coordSumD));

        SUBCASE("expression which do not need coordinate"){

            auto expA = 10*zerosA + 2*onesA + coordSumA +  ma::castExpr<int>( ma::sqrt(zerosA + 4));
            auto expB = 10*zerosB + 2*onesB + coordSumB +  ma::castExpr<int>( ma::sqrt(zerosB + 4));
            auto expC = 10*zerosC + 2*onesC + coordSumC +  ma::castExpr<int>( ma::sqrt(zerosC + 4));
            auto expD = 10*zerosD + 2*onesD + coordSumD +  ma::castExpr<int>( ma::sqrt(zerosD + 4));

            auto expMixedA = 10*zerosA + 2*onesB + coordSumC +  ma::castExpr<int>( ma::sqrt(zerosD + 4));
            auto expMixedB = 10*zerosD + 2*onesC + coordSumA +  ma::castExpr<int>( ma::sqrt(zerosB + 4));
            auto expMixedC = 10*zerosD + 2*onesC + coordSumA +  ma::castExpr<int>( ma::sqrt(zerosB + 4));

            auto shouldRes = ma::zeros<ValueType>(2,3);
            for(auto x0=0; x0<2; ++x0)
            for(auto x1=0; x1<3; ++x1){
                shouldRes(x0, x1) = 10.0 * int(0) + 2*int(1) + int(x0+x1) + static_cast<int>(std::sqrt(int(0)+4));
            }


            // write into existing arrays with matching striding
            resultA =  expA;
            resultB =  expB;
            resultC =  expC;
            resultD =  expD;

            resultMixedA =  expMixedA;
            resultMixedB =  expMixedB;
            resultMixedC =  expMixedC;

            REQUIRE(shouldRes.equal(resultA));
            REQUIRE(shouldRes.equal(resultB));
            REQUIRE(shouldRes.equal(resultC));
            REQUIRE(shouldRes.equal(resultD));
            REQUIRE(shouldRes.equal(resultMixedA));
            REQUIRE(shouldRes.equal(resultMixedB));
            REQUIRE(shouldRes.equal(resultMixedC));

            // write  constructor
            ma::SmartMultiArray<ValueType, DIM> resultACtr = expA;
            ma::SmartMultiArray<ValueType, DIM> resultBCtr = expB;
            ma::SmartMultiArray<ValueType, DIM> resultCCtr = expC;
            ma::SmartMultiArray<ValueType, DIM> resultDCtr = expD;

            ma::SmartMultiArray<ValueType, DIM> resultMixedA = expMixedA;
            ma::SmartMultiArray<ValueType, DIM> resultMixedB = expMixedB;
            ma::SmartMultiArray<ValueType, DIM> resultMixedC = expMixedC;

            REQUIRE(shouldRes.equal(resultACtr));
            REQUIRE(shouldRes.equal(resultBCtr));
            REQUIRE(shouldRes.equal(resultCCtr));
            REQUIRE(shouldRes.equal(resultDCtr));
            REQUIRE(shouldRes.equal(resultMixedA));
            REQUIRE(shouldRes.equal(resultMixedB));
            REQUIRE(shouldRes.equal(resultMixedC));

            // write into empty
            ma::SmartMultiArray<ValueType, DIM> resultAEmpty;
            ma::SmartMultiArray<ValueType, DIM> resultBEmpty;
            ma::SmartMultiArray<ValueType, DIM> resultCEmpty;
            ma::SmartMultiArray<ValueType, DIM> resultDEmpty;

            ma::SmartMultiArray<ValueType, DIM> resultMixedAEmpty;
            ma::SmartMultiArray<ValueType, DIM> resultMixedBEmpty;
            ma::SmartMultiArray<ValueType, DIM> resultMixedCEmpty;

            resultAEmpty = expA;
            resultBEmpty = expB;
            resultCEmpty = expC;
            resultDEmpty = expD;

            resultMixedAEmpty = expMixedA;
            resultMixedBEmpty = expMixedB;
            resultMixedCEmpty = expMixedC;

            REQUIRE(shouldRes.equal(resultAEmpty));
            REQUIRE(shouldRes.equal(resultBEmpty));
            REQUIRE(shouldRes.equal(resultCEmpty));
            REQUIRE(shouldRes.equal(resultDEmpty));

            REQUIRE(shouldRes.equal(resultMixedAEmpty));
            REQUIRE(shouldRes.equal(resultMixedBEmpty));
            REQUIRE(shouldRes.equal(resultMixedCEmpty));


            



        }



            
        //auto expression = 5.0 - (x0 + ma::exp(x1));
        //REQUIRE_FALSE(expression.hasShape());
        //// the same as
        //for(auto x0=0; x0<5; ++x0)
        //for(auto x1=0; x1<5; ++x1){
        //    a(x0,x1) = 5.0 - (ValueType(x0) + std::exp(x1));
        //}

    }
    

}

TEST_SUITE_END();